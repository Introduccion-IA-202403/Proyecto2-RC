#ifndef PROYECTO2_RC_PREDICATEARG_H
#define PROYECTO2_RC_PREDICATEARG_H

#include <iostream>
#include <vector>
using namespace std;

enum PredicateArgType {
    CONSTANT,
    VARIABLE,
    SKOLEM
};

class PredicateArg {
public:

    PredicateArg(string name, char symbol, PredicateArgType type) : name(name), argSymbol(symbol), type(type) {};

    PredicateArg() = default;

    /**
     * For Skolem Functions
     */
    PredicateArg(string n, vector<char> args) : name(n), argsSymbols(args), type(SKOLEM) {};

    const string &getName() const;

    void setName(const string &name);

    char getArgSymbol() const;

    void setArgSymbol(char argSymbol);

    PredicateArgType getType() const;

    void setType(PredicateArgType type);

    const vector<char> &getArgsSymbols() const;

    void setArgsSymbols(const vector<char> argsSymbols);
    void addArgSymbolVector(char n);

private:
    string name;
    char argSymbol = '-';
    // Solo aplica si es Skolem
    vector<char> argsSymbols;
    PredicateArgType type;
};

#endif //PROYECTO2_RC_PREDICATEARG_H
