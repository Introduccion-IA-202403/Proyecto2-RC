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

    Clause(string &raw, int &start);

    Clause *getFirst() const;

    Clause *getSecond() const;

    const vector<Quantifier> &getContext() const;

    const vector<Quantifier> &getQuantifiers() const;

    const Predicate &getPredicate() const;

    bool isNot() const;

    Link getLink() const;

    Link evalLink(string str);

    ~Clause();

private:
    Clause *first = nullptr, *second = nullptr, *parent = nullptr;
    vector<Quantifier> context, quantifiers;
    Predicate predicate;
    bool NOT;
    Link link = EMPTY;
};

#endif //PROYECTO2_RC_CLAUSE_H
