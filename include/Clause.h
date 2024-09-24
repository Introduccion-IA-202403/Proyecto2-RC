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

    /**
     *  Default constructor
     */
    Clause() = default;

    /**
     * Constructor for a clause with a string and a start index to parse it
     * @param raw The string to parse
     * @param start The initial index to start parsing
     */
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

    /**
     * Evaluates a string to a Link
     * @param str The string to evaluate
     * @return  The Link that corresponds to the string
     */
    Link evalLink(string str);

    /**
     * Destructor
     */
    ~Clause();

private:
    Clause *first = nullptr, *second = nullptr, *parent = nullptr;
    vector<Quantifier> context, quantifiers;
    Predicate predicate;
    bool NOT = false;
    Link link = EMPTY;
};

#endif //PROYECTO2_RC_CLAUSE_H
