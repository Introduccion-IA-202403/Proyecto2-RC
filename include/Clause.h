
#ifndef PROYECTO2_RC_CLAUSE_H
#define PROYECTO2_RC_CLAUSE_H

#include <Quantifier.h>
#include <Predicate.h>
#include <Link.h>

class Clause {
public:

    /**
    *  Constructor por defecto
     */
    Clause() = default;

    /**
    * Constructor para una cláusula con una cadena y un índice de inicio para construirla
    * @param raw La cadena a construir
    * @param start El índice inicial para comenzar la construcción
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
    * Evalúa una cadena a un conector
     * @param str El string a evaluar
     * @return  El conector que corresponde al string
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
