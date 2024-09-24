#ifndef PROYECTO2_RC_LOGICTRANSFORM_H
#define PROYECTO2_RC_LOGICTRANSFORM_H

#include <Clause.h>
#include <set>
#include <unordered_map>

class LogicTransform {
private:
    void deleteIFF(Clause *t, bool first);
    void deleteImplies(Clause *t);
    void transformNOT(Clause *t);
    void moveQuantifiers(Clause *t, vector<Quantifier>& collect);
    void skolenize(Clause *t, unordered_map<char, PredicateArg>& skolemFuncs);
    void deleteORlinks(Clause *t);
    void collectResults(Clause *t, vector< vector<Clause>>&collect, vector<Clause>& curr);

public:
    vector<vector<Clause>> transformClause(Clause* t);
};

#endif //PROYECTO2_RC_LOGICTRANSFORM_H
