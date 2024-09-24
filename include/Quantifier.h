#ifndef PROYECTO2_RC_QUANTIFIER_H
#define PROYECTO2_RC_QUANTIFIER_H

#include <string>

using namespace std;

/**
 * Enum for the type of quantifier
 */
enum QuantifierType {
    FOR_ALL,
    EXIST,
    INVALID
};

class Quantifier {
public:

    /**
     * Default constructor
     * @param type The type of the quantifier
     * @param var The variable of the quantifier
     */
    Quantifier(QuantifierType type, char var) : type(type), arg(var) {}

    /**
     * Evaluates a string to a Quantifier
     * @param str The string to evaluate
     * @return The Quantifier that corresponds to the string
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
