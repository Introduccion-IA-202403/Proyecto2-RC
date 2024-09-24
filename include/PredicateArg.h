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

    /**
     * Constructor for a PredicateArg
     * @param name The name of the argument
     * @param symbol The symbol of the argument
     * @param type The type of the argument
     */
    PredicateArg(string name, string symbol, PredicateArgType type) : name(name), argSymbol(symbol), type(type) {};

    /**
     * Default constructor
     */
    PredicateArg() = default;

    /**
     * For Skolem Functions
     * @param n The name of the function
     * @param args The arguments of the function
     */
    PredicateArg(string n, vector<string> args) : name(n), argsSymbols(args), type(SKOLEM) {};

    const string &getName() const;

    void setName(const string &name);

    string getArgSymbol() const;

    void setArgSymbol(string argSymbol);

    PredicateArgType getType() const;

    void setType(PredicateArgType type);

    vector<string> &getArgsSymbols();

    void setArgsSymbols(vector<string> argsSymbols);

    /**
     * Adds a symbol to the vector of arguments
     * @param n The symbol to add
     */
    void addArgSymbolVector(char n);

    ~PredicateArg() = default;

    friend ostream& operator << (ostream &os, const PredicateArg &p) {
        if (p.type == SKOLEM) {
            os << p.name << "(";
            for (int i = 0; i < p.argsSymbols.size(); i++) {
                os << p.argsSymbols[i];
                if (i != p.argsSymbols.size() - 1) os << ", ";
            }
            os << ")";
        } else if (p.type == CONSTANT) os << p.name;
        else os << p.argSymbol;
        return os;
    }
private:
    string name;
    string argSymbol = "-";
    // Solo aplica si es Skolem
    vector<string> argsSymbols;
    PredicateArgType type;
};

#endif //PROYECTO2_RC_PREDICATEARG_H
