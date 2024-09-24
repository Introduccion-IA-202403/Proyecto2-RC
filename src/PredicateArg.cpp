
#include <string>
#include <PredicateArg.h>

const std::__cxx11::basic_string<char> &PredicateArg::getName() const {
    return name;
}

void PredicateArg::setName(const std::__cxx11::basic_string<char> &name) {
    PredicateArg::name = name;
}

string PredicateArg::getArgSymbol() const {
    return argSymbol;
}

void PredicateArg::setArgSymbol(string argSymbol) {
    PredicateArg::argSymbol = argSymbol;
}

PredicateArgType PredicateArg::getType() const {
    return type;
}

void PredicateArg::setType(PredicateArgType type) {
    PredicateArg::type = type;
}

vector<string> &PredicateArg::getArgsSymbols() {
    return argsSymbols;
}

void PredicateArg::setArgsSymbols(vector<string> argsSymbols) {
    PredicateArg::argsSymbols = argsSymbols;
}

void PredicateArg::addArgSymbolVector(char n) {
    this->argsSymbols.push_back(string(1,n));
}
