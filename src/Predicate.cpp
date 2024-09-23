#include <Predicate.h>

const string &Predicate::getName() const {
    return name;
}

void Predicate::setName(const string &name) {
    Predicate::name = name;
}

const vector<PredicateArg> &Predicate::getArgs() const {
    return args;
}

void Predicate::setArgs(const vector<PredicateArg> &args) {
    Predicate::args = args;
}

bool Predicate::isEmpty() const {
    return empty;
}

void Predicate::setEmpty(bool empty) {
    Predicate::empty = empty;
}


Predicate::Predicate(string name, string &raw, int &i) {
    empty = false;
    this->name = name;
    string token = "";

    /*
     * Se supone que cuando se llama a este constructor, solo hay variables
     * o constantes, no hay funciones de Skolem, entonces por eso no se valida
     * si hay o no hay un parentesis en el token
     * */
    while (raw[i] != ')') {
        if (raw[i] == ' ') {
            i++;
            continue;
        }
        if (raw[i] == ',') {
            this->args.push_back(
                    (token.size() == 1 && islower(token[0])) ?
                    PredicateArg("", token[0], VARIABLE) :
                    PredicateArg(token, '-', CONSTANT)
                    ); // Agregar el argumento sea cual sea
            token = "";
        } else token += raw[i];
        i++;
    }
    this->args.push_back(
            (token.size() == 1 && islower(token[0])) ?
            PredicateArg("", token[0], VARIABLE) :
            PredicateArg(token, '-', CONSTANT)
    ); // Agregar el argumento sea cual sea
}
