#ifndef PROYECTO2_RC_LOGICTRANSFORM_H
#define PROYECTO2_RC_LOGICTRANSFORM_H

#include <Clause.h>
#include <set>

class LogicTransform {
private:
    void deleteIFF(Clause *t, bool first);
    void deleteImplies(Clause *t, bool first);
    void transformNOT(Clause *t, bool first);
    void uniqueQuantifiers(Clause *t, bool first);
    void moveQuantifiers(Clause *t, bool first, vector<Quantifier>& collect);
    void skolenize(Clause *t, bool first, set<char>& skolem, char& skolemName);
    void deleteQuantifiers(Clause *t, bool first);
    void deleteORlinks(Clause *t, bool first);
    vector<Clause*> separateANDlinks(Clause *t, bool first);
public:
    vector<Clause> transformClause(Clause* t);
};

#endif //PROYECTO2_RC_LOGICTRANSFORM_H
