#ifndef PROYECTO2_RC_PREDICATEARG_H
#define PROYECTO2_RC_PREDICATEARG_H

#include <iostream>
using namespace std;

enum PredicateArgType {
    CONSTANT,
    VARIABLE,
    SKOLEM
};

class PredicateArg {
public:

    PredicateArg(string name, char symbol, PredicateArgType type) : name(name), argSymbol(symbol), type(type) {}

    const string &getName() const;

    void setName(const string &name);

    char getArgSymbol() const;

    void setArgSymbol(char argSymbol);

    PredicateArgType getType() const;

    void setType(PredicateArgType type);

private:
    string name;
    char argSymbol = '-';
    PredicateArgType type;
};

#endif //PROYECTO2_RC_PREDICATEARG_H
