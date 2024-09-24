#ifndef PROYECTO2_RC_SOLVER_H
#define PROYECTO2_RC_SOLVER_H

#include <Clause.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solver {
public:

    /**
     * Constructor for the Solver with a base of knowledge
     * @param clauses The base of knowledge
     */
    Solver(vector<vector<Clause>>& clauses);

    /**
     * Resolve a set of clauses by refutation
     * @param resolve The set of clauses to resolve
     * @return True if the set of clauses is true, false otherwise
     */
    bool resolve(vector<Clause> resolve);

private:
    vector<vector<Clause>> clauses;  // Base de conocimiento
    unordered_map<string, string> substitution;

    /**
     * Check if the initial set of clauses can be resolved or unified
     * @param resolve The set of clauses to resolve
     * @param uni The unification to apply
     */
    void checkInitialResolve(vector<Clause>& resolve, unordered_map<string, string> &uni);

    /**
     * Make the unification of the set of clauses
     * @param resolve The set of clauses to unify
     * @param uni The unification to apply
     */
    void makeUnification(vector<Clause> &resolve, unordered_map<string, string> &uni);

    /**
     * Backtrack the set of clauses to resolve
     * @param resolve The set of clauses to resolve
     * @param uni The unification to apply
     * @return True if the set of clauses is true, false otherwise
     */
    bool backtrack(vector<Clause> resolve, unordered_map<string, string> uni);

    /**
     * Unify two predicates
     * @param p1 The first predicate to unify
     * @param p2 The second predicate to unify
     * @param un The unification to apply
     * @param flag The flag to check if the unification is possible
     * @return The unification of the two predicates
     */
    unordered_map<string, string> unify(Predicate& p1, Predicate& p2, unordered_map<string, string> un, bool &flag);
    void printSubstitution();
};

#endif //PROYECTO2_RC_SOLVER_H
