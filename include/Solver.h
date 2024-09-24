#ifndef PROYECTO2_RC_SOLVER_H
#define PROYECTO2_RC_SOLVER_H

#include <Clause.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solver {
public:
    Solver(vector<vector<Clause>>& clauses);  // Recibe la base de conocimiento

    bool resolve(vector<Clause> resolve);  // Método de resolución por refutación

private:
    vector<vector<Clause>> clauses;  // Base de conocimiento
    unordered_map<string, string> substitution;

    void checkInitialResolve(vector<Clause>& resolve, unordered_map<string, string> &uni);
    void makeUnification(vector<Clause> &resolve, unordered_map<string, string> &uni);
    bool backtrack(vector<Clause> resolve, unordered_map<string, string> uni);
    unordered_map<string, string> unify(Predicate& p1, Predicate& p2, unordered_map<string, string> un, bool &flag);
    void printSubstitution();
};

#endif //PROYECTO2_RC_SOLVER_H
