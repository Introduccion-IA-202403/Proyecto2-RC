#include <Quantifier.h>

QuantifierType Quantifier::getType() const {
    return type;
}

void Quantifier::setType(QuantifierType type) {
    Quantifier::type = type;
}

const char &Quantifier::getArg() const {
    return arg;
}

void Quantifier::setArg(const char &arg) {
    Quantifier::arg = arg;
}

Quantifier Quantifier::evalQuantifier(string str) {
    if (str == "FOR_ALL") return {FOR_ALL, '-'};
    if (str == "EXIST") return {EXIST, '-'};
    return {INVALID, '-'};
}
