#ifndef PROYECTO2_RC_SOLVER_H
#define PROYECTO2_RC_SOLVER_H

#include <Clause.h>
#include <vector>
#include <unordered_map>

using namespace std;

class Solver {
public:
    Solver(const vector<vector<Clause>>& clauses);  // Recibe la base de conocimiento

    bool resolve();  // Método de resolución por refutación

private:
    vector<vector<Clause>> clauses;  // Base de conocimiento
    unordered_map<string, string> substitution;
    
    bool backtrack();
    bool unify(const Predicate& p1, const Predicate& p2);
    void printSubstitution();
};

#endif //PROYECTO2_RC_SOLVER_H
