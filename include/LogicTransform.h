#ifndef PROYECTO2_RC_LOGICTRANSFORM_H
#define PROYECTO2_RC_LOGICTRANSFORM_H

#include <Clause.h>
#include <set>
#include <unordered_map>

class LogicTransform {
private:
    /**
     * Deletes the iff links in the clause
     * @param t The clause to delete the iff links
     * @param first If the clause is the first in its parent
     */
    void deleteIFF(Clause *t, bool first);

    /**
     * Deletes the implies links in the clause
     * @param t The clause to delete the implies links
     */
    void deleteImplies(Clause *t);

    /**
     * Transforms the NOT links in the clause
     * @param t The clause to transform the NOT links
     */
    void transformNOT(Clause *t);

    /**
     * Moves the quantifiers to the front of the clause
     * @param t The clause to move the quantifiers
     * @param collect The vector to collect the quantifiers
     */
    void moveQuantifiers(Clause *t, vector<Quantifier>& collect);

    /**
     * Skolenizes the clause
     * @param t The clause to skolenize
     * @param skolemFuncs The map of skolem functions
     */
    void skolenize(Clause *t, unordered_map<string, PredicateArg>& skolemFuncs);

    /**
     * Deletes the OR links in the clause
     * @param t The clause to delete the OR links
     */
    void deleteORlinks(Clause *t);

    /**
     * Collects the results of the transformation
     * @param t The clause to collect the results
     * @param collect The vector to collect the results
     * @param curr The current vector of clauses
     */
    void collectResults(Clause *t, vector< vector<Clause>>&collect, vector<Clause>& curr);

public:
    /**
     * Transforms a clause to a vector of clauses
     * @param t The clause to transform
     * @return The vector of clauses
     */
    vector<vector<Clause>> transformClause(Clause* t);
};

#endif //PROYECTO2_RC_LOGICTRANSFORM_H
