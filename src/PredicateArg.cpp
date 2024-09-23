
#include <string>
#include <PredicateArg.h>

const std::__cxx11::basic_string<char> &PredicateArg::getName() const {
    return name;
}

void PredicateArg::setName(const std::__cxx11::basic_string<char> &name) {
    PredicateArg::name = name;
}

char PredicateArg::getArgSymbol() const {
    return argSymbol;
}

void PredicateArg::setArgSymbol(char argSymbol) {
    PredicateArg::argSymbol = argSymbol;
}

PredicateArgType PredicateArg::getType() const {
    return type;
}

void PredicateArg::setType(PredicateArgType type) {
    PredicateArg::type = type;
}
