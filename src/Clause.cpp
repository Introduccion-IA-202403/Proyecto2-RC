#include <Clause.h>


Clause::Clause(string &raw, int &i) {

    /**
     * Este constructor se encarga de construir una cláusula a partir de una cadena
     */
    string token;
    bool buildFirst = true;
    for (; i < (int)raw.size(); i++) {

        /**
         * Rompe el ciclo si encuentra un paréntesis de cierre
         */
        if (raw[i] == ')') break;

        /**
         * Capturamos algunos espacios porque pueden ser tokens que nos interesan
         */
        if (raw[i] == ' ') {

            /**
             * Debe evaluar si el token es un cuantificador
             */
            auto quantifier = Quantifier::evalQuantifier(token);
            if (quantifier.getType() != INVALID) {
                /**
                 * Si el token es un cuantificador, debemos agregarlo a la lista de cuantificadores
                 */
                i++; quantifier.setArg(raw[i]);
                this->quantifiers.push_back(quantifier);
                token = "";
                continue;
            }

            /**
             * Debemos evaluar si el token es un conector lógico
             */
            auto possibleLink = evalLink(token);
            if (possibleLink != INVALID_LINK) {
                this->link = possibleLink;
                buildFirst = false;
                token = "";
                continue;
            }

            /**
             * Debemos evaluar si el token es un NOT
             */
            if (token == "NOT") {
                this->NOT = true;
                token = "";
            }
            continue;
        }

        /**
         * Si encontramos un paréntesis, debemos crear una nueva cláusula
         */
        if (raw[i] == '(') {

            i++;

            /**
             * Si el token está vacío, debemos crear una nueva cláusula
             */
            if (token == "") {
                if (buildFirst) {
                    this->first = new Clause(raw, i);
                    this->first->parent = this;
                    buildFirst = false;
                }
                else {
                    this->second = new Clause(raw, i);
                    this->second->parent = this;
                }
            } else {
                /**
                 * si no, debemos crear un nuevo predicado porque el token es el nombre del predicado
                 */
                this->predicate = Predicate(token, raw, i);
                token = "";
            }
        } else {
            /**
             * Si el caracter no es un paréntesis, debemos agregarlo al token
             */
            token += raw[i];
        }
    }
}

Clause *Clause::getFirst() {
    return first;
}

Clause *Clause::getSecond() {
    return second;
}

const vector<Quantifier> &Clause::getContext() const {
    return context;
}

vector<Quantifier> &Clause::getQuantifiers() {
    return quantifiers;
}

Predicate &Clause::getPredicate() {
    return predicate;
}

bool Clause::isNot() const {
    return NOT;
}

Link Clause::getLink() const {
    return link;
}

Link Clause::evalLink(string str) {
    if (str == "OR") return OR;
    if (str == "AND") return AND;
    if (str == "IMPLIES") return IMPLIES;
    if (str == "IFF") return IFF;
    return INVALID_LINK;
}

Clause::~Clause() {
    delete this->first;
    delete this->second;
}

void Clause::setLink(Link n) { link = n; }

void Clause::setFirst(Clause *n) {
    this->first = n;
}

void Clause::setSecond(Clause *n) {
    this->second = n;
}

void Clause::addQuantifier(Quantifier n) {
    this->quantifiers.push_back(n);
}

void Clause::setPredicate(Predicate n) {
    this->predicate = n;
}

void Clause::setNot(bool n) {
    this->NOT = n;
}

Clause *Clause::getParent() {
    return this->parent;
}

void Clause::setParent(Clause *n) {
    this->parent = n;
}

void Clause::setQuantifiers(vector<Quantifier> n) {
    this->quantifiers = n;
}

