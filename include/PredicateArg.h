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
     * Constructor para un PredicateArg
     * @param name El nombre del argumento
     * @param symbol El simbolo del argumento
     * @param type El tipo de argumento
     */
    PredicateArg(string name, string symbol, PredicateArgType type) : name(name), argSymbol(symbol), type(type) {};

    /**
     * Constructor por defecto
     */
    PredicateArg() = default;

    /**
     * Para funciones Skolem
     * @param n El nombre de la función
     * @param args Los argumentos de la función
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
     * Añade un simbolo al vector de argumentos
     * @param n El simbolo a añadir
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
