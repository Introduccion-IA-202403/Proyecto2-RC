#include <Predicate.h>

const string &Predicate::getName() const {
    return name;
}

void Predicate::setName(const string &name) {
    Predicate::name = name;
}

vector<PredicateArg> &Predicate::getArgs() {
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

    /**
    * Se asume que cuando se llama a este constructor, solo hay variables
    * o constantes, no hay funciones Skolem, por lo que no se valida
    * si hay o no un paréntesis en el token
     * */
    while (raw[i] != ')') {
        if (raw[i] == ' ') {
            i++;
            continue;
        }

        /**
         * Si el caracter es una coma, debemos agregar el token a los argumentos
         */
        if (raw[i] == ',') {
            this->args.push_back(
                    (token.size() == 1 && islower(token[0])) ?
                    PredicateArg("", string(1, token[0]), VARIABLE) :
                    PredicateArg(token, "-", CONSTANT)
                    ); // Agregar el argumento sea cual sea
            token = "";
        } else token += raw[i];
        i++;
    }

    /**
     * Debe agregar el último argumento
     */
    this->args.push_back(
            (token.size() == 1 && islower(token[0])) ?
            PredicateArg("", string(1, token[0]), VARIABLE) :
            PredicateArg(token, "-", CONSTANT)
    );
}
