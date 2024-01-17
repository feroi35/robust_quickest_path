// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <cassert>

// Magic tricks to have CPLEX behave well:
#ifndef IL_STD
#define IL_STD
#endif
#include <cstring>
#include <ilcplex/ilocplex.h>
ILOSTLBEGIN
// End magic tricks

struct Arc {
    IloInt i;
    IloInt j;
    IloNum d;
    IloNum D;
};

const double undefinedValue = std::numeric_limits<double>::quiet_NaN();


struct Instance {
    char* name;
    IloInt n;
    IloInt n_arc;
    IloInt s;
    IloInt t;
    IloNum S;
    IloNum d1;
    IloNum d2;
    IloNumArray p;
    IloNumArray ph;
    std::vector<Arc> mat;
    std::vector<std::vector<double>> d;
    std::vector<std::vector<double>> D;
    IloNumArray d_vec;
    IloNumArray D_vec;

    std::vector<IloInt> sol; // liste des villes visitées dans l'ordre

    Instance(IloEnv env, char filename[]);
    ~Instance(){};
    void display() const;

    double compute_static_score(const int& verbose=0) const { return compute_static_score(sol,verbose); }
    double compute_robust_score(IloEnv env, const unsigned int& time_limit, const int& verbose=0) const { return compute_robust_score(env, sol, time_limit, verbose); }
    double compute_static_constraint(const int& verbose=0) const { return compute_static_constraint(sol, verbose); }
    double compute_robust_constraint(IloEnv env, const unsigned int& time_limit, const int& verbose=0) const { return compute_robust_constraint(env, sol, time_limit, verbose); }

    double compute_static_score(const std::vector<IloInt>& sol, const int& verbose=0) const;
    double compute_robust_score(IloEnv env, const std::vector<IloInt>& sol, const unsigned int& time_limit =60, const int& verbose=0) const;
    double compute_static_constraint(const std::vector<IloInt>& sol, const int& verbose=0) const;
    double compute_robust_constraint(IloEnv env, const std::vector<IloInt>& sol, const unsigned int& time_limit=60, const int& verbose=0) const;


    void exportSol(char filename[]) const;
};


#endif
