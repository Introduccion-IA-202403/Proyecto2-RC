//
// Created by talkysafe143 on 22/09/24.
//

#ifndef PROYECTO2_RC_CLAUSE_H
#define PROYECTO2_RC_CLAUSE_H

#include <Quantifier.h>
#include <Predicate.h>
#include <Link.h>

class Clause {
public:

    Clause() = default;

    Clause(string &raw, int &start);

    Clause *getFirst();
    void setFirst(Clause * n);

    Clause *getSecond();
    void setSecond(Clause * n);

    const vector<Quantifier> &getContext() const;

    vector<Quantifier> &getQuantifiers();
    void setQuantifiers(vector<Quantifier> n);
    void addQuantifier(Quantifier n);

    Predicate &getPredicate();
    void setPredicate(Predicate n);

    bool isNot() const;
    void setNot(bool n);

    Link getLink() const;
    void setLink(Link n);

    Clause* getParent();
    void setParent(Clause *n);

    Link evalLink(string str);

    ~Clause();

private:
    Clause *first = nullptr, *second = nullptr, *parent = nullptr;
    vector<Quantifier> context, quantifiers;
    Predicate predicate;
    bool NOT = false;
    Link link = EMPTY;
};

#endif //PROYECTO2_RC_CLAUSE_H
