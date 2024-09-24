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

    /*
    * It is assumed that when this constructor is called, there are only variables
    * or constants, there are no Skolem functions, so it is not validated
    * whether or not there is a parenthesis in the token
     * */
    while (raw[i] != ')') {
        if (raw[i] == ' ') {
            i++;
            continue;
        }

        /**
         * If the character is a comma, we must add the token to the arguments
         */
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

    /**
     * We must add the last argument
     */
    this->args.push_back(
            (token.size() == 1 && islower(token[0])) ?
            PredicateArg("", token[0], VARIABLE) :
            PredicateArg(token, '-', CONSTANT)
    );
}
