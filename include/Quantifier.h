#ifndef PROYECTO2_RC_QUANTIFIER_H
#define PROYECTO2_RC_QUANTIFIER_H

#include <string>

using namespace std;

/**
 * Enumeración de los tipos de cuantificadores
 */
enum QuantifierType {
    FOR_ALL,
    EXIST,
    INVALID
};

class Quantifier {
public:

    /**
     * Constructor por defecto
     * @param type El tipo de cuantificador
     * @param var La variable del cuantificador
     */
    Quantifier(QuantifierType type, char var) : type(type), arg(var) {}

    /**
     * Evalúa una cadena a un cuantificador
     * @param str El string a evaluar
     * @return El cuantificador que corresponde al string
     */
    static Quantifier evalQuantifier(string str);

    QuantifierType getType() const;

    void setType(QuantifierType type);

    const char &getArg() const;

    void setArg(const char &arg);

private:

    QuantifierType type;
    char arg;
};

#endif //PROYECTO2_RC_QUANTIFIER_H
