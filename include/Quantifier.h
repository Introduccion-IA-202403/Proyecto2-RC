#ifndef PROYECTO2_RC_QUANTIFIER_H
#define PROYECTO2_RC_QUANTIFIER_H

#include <string>

using namespace std;

enum QuantifierType {
    FOR_ALL,
    EXIST,
    INVALID
};

class Quantifier {
public:

    Quantifier(QuantifierType type, char var) : type(type), arg(var) {}

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
