#include "static_solve.h"
#include "parser.h"


void static_solve(IloEnv env, Instance& inst, const unsigned int& time_limit, const int& verbose) {
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    IloModel model(env);

    // Variables
    IloBoolVarArray x(env, inst.n_arc);
    IloBoolVarArray y(env, inst.n);

    // Objective
    IloObjective obj(env, IloScalProd(x, inst.d_vec), IloObjective::Minimize);
    model.add(obj);

    // Constraints
    model.add(IloScalProd(y, inst.p) <= inst.S);
    // Flow conservation
    for (unsigned int i=0; i<inst.n; i++) {
        IloExpr out_arcs_i(env);
        IloExpr in_arcs_i(env);
        for (unsigned int a=0; a<inst.n_arc; a++) {
            // It may appear inefficient to do as so O(n^3)
            // But it is faster in practice than to make a vector of expression
            if (inst.mat[a].tail-1 == i)
                out_arcs_i += x[a];
            if (inst.mat[a].head-1 == i)
                in_arcs_i += x[a];
        }
        if (i != inst.t-1) {
            model.add(out_arcs_i == y[i]);
        } else {
            // you can't get out of t
            model.add(out_arcs_i == 0);
        }
        if (i != inst.s-1) {
            model.add(in_arcs_i == y[i]);
        } else {
            // you can't get into s
            model.add(in_arcs_i == 0);
        }
        out_arcs_i.end();
        in_arcs_i.end();
    }
    model.add(y[inst.s-1] == 1);
    model.add(y[inst.t-1] == 1);

    IloCplex cplex(model);
    cplex.setParam(IloCplex::Param::TimeLimit, time_limit);
    if (verbose < 2) cplex.setOut(env.getNullStream());

    cplex.solve();
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if (cplex.getStatus() == IloAlgorithm::Infeasible) {
        std::cout << inst.name << "," << "static,,,,,,,,," << std::endl;
        throw std::domain_error("Infeasible static model for instance " + inst.name);
    } else if (cplex.getStatus() == IloAlgorithm::Unknown) {
        std::cout << inst.name << "," << "static,,,,,,,,," << std::endl;
        throw std::domain_error("No solution found for instance " + inst.name + ". Maybe not enough time");
    }

    if (!inst.sol.empty()) {
        std::cerr << "Warning: solution vector not empty for instance " << inst.name << std::endl;
        inst.sol.clear();
    }
    std::string path_str = "[";
    unsigned int current_node = inst.s-1;
    while (current_node != inst.t-1) {
        path_str += std::to_string(current_node+1) + ";";
        inst.sol.push_back(current_node+1);
        for (unsigned int a=0; a<inst.n_arc; a++) {
            if (inst.mat[a].tail == current_node+1 && cplex.getValue(x[a]) == 1) {
                current_node = inst.mat[a].head-1;
                break;
            }
        }
        if (current_node == inst.sol[inst.sol.size()-1]-1) {
            std::cout << inst.name << "," << "static,,,,,,,,," << std::endl;
            throw std::domain_error("Using arc that does not exist for instance " + inst.name);
        }
    }
    inst.sol.push_back(inst.t);
    path_str += std::to_string(inst.t) + "]";

    double static_cost = inst.compute_static_score();
    if (abs(static_cost - cplex.getObjValue()) > 1e-3) {
        std::cout << inst.name << "," << "static,,,,,,,,," << std::endl;
        throw std::domain_error("Not the same objective value for instance " + inst.name);
    }

    float robust_constraint = inst.compute_robust_constraint_milp(env);
    float robust_score = (robust_constraint < inst.S + 1e-3) ? inst.compute_robust_score_milp(env) : 1e9;

    std::cout << inst.name << ","
        << "static,"
        << robust_score << ","
        << cplex.getBestObjValue() << ","
        << static_cast<double>(duration.count()) / 1e6 << ","
        << cplex.getNnodes() << ","
        << robust_constraint << ","
        << static_cost << ","
        << inst.compute_static_constraint() << ","
        << inst.S << ","
        << path_str << std::endl;
}
