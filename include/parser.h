// parser.h
#ifndef PARSER_H
#define PARSER_H

#include <ilcplex/ilocplex.h> // better to do a forward declaration?
ILOSTLBEGIN // macro to avoid incompatibility. Important to be before the other includes
#include <vector>
#include <tuple>
#include <map>

const double undefinedValue = std::numeric_limits<double>::quiet_NaN();

struct Arc {
    IloInt i;
    IloInt j;
    IloNum d;
    IloNum D;
};


class Index{
    public:
        int i;
        int j;
        Index(int i, int j): i(i), j(j) {};
        bool operator<(const Index& other) const {
            return (i < other.i) || (i == other.i && j < other.j);
        }
};



struct Instance {
    std::string name;
    IloInt n; // nombre de noeuds
    IloInt n_arc; // nombre d'arcs
    IloInt s; // ville d'entrée
    IloInt t; // ville d'entrée
    IloNum S; // contrainte poids de villes
    IloNum d1; // max incertitude poids arcs
    IloNum d2; // max incertitude poids villes
    IloNumArray p; // poids des villes
    IloNumArray ph; // incertitudes poids des villes
    IloNumArray d_vec; // durée de trajet des arcs
    IloNumArray D_vec; // incertitude durée de trajet des arcs
    // std::map<Index,double> d; // matrice des durées de trajet
    // std::map<Index,double> D; // matrice des incertitudes des durées de trajet
    std::vector<std::vector<double>> d; // matrice des durées de trajet
    std::vector<std::vector<double>> D; // matrice des incertitudes des durées de trajet
    std::vector<Arc> mat;
    std::vector<std::vector<int>>* neighbors_list;
    std::vector<std::vector<int>>* reverse_neighbors_list;



    std::vector<IloInt> sol; // liste des villes visitées dans l'ordre

    Instance(IloEnv env, char filename[]);
    ~Instance(){};
    void display() const;

    double compute_static_score() const { return compute_static_score(sol); }
    double compute_robust_score(IloEnv env, const unsigned int& time_limit=60, const int& verbose=0) const { return compute_robust_score(env, sol, time_limit, verbose); }
    double compute_static_constraint() const { return compute_static_constraint(sol); }
    double compute_robust_constraint(IloEnv env, const unsigned int& time_limit=60, const int& verbose=0) const { return compute_robust_constraint(env, sol, time_limit, verbose); }

    double compute_static_score(const std::vector<IloInt>& sol) const;
    double compute_robust_score(IloEnv env, const std::vector<IloInt>& sol, const unsigned int& time_limit =60, const int& verbose=0) const;
    double compute_static_constraint(const std::vector<IloInt>& sol) const;
    double compute_robust_constraint(IloEnv env, const std::vector<IloInt>& sol, const unsigned int& time_limit=60, const int& verbose=0) const;

};


#endif
