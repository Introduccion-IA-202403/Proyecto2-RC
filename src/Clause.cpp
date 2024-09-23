#include <Clause.h>

Clause::Clause(string &raw, int &i) {
    string token;
    bool buildFirst = true;
    for (; i < (int)raw.size(); i++) {
        if (raw[i] == ')') break;
        if (raw[i] == ' ') { // Puede ser un token que nos interese
            auto quantifier = Quantifier::evalQuantifier(token);
            if (quantifier.getType() != INVALID) {
                i++; quantifier.setArg(raw[i]);
                this->quantifiers.push_back(quantifier);
                token = "";
                continue;
            }
            auto possibleLink = evalLink(token);
            if (possibleLink != INVALID_LINK) {
                this->link = possibleLink;
                buildFirst = false;
                token = "";
                continue;
            }

            if (token == "NOT") {
                this->NOT = true;
                token = "";
            }
            continue;
        }
        if (raw[i] == '(') { // Va a comenzar una nueva clausula o argumentos
            i++;
            if (token == "") { // Comienza una nueva clausula
                if (buildFirst) {
                    this->first = new Clause(raw, i);
                    this->first->parent = this;
                    buildFirst = false;
                }
                else {
                    this->second = new Clause(raw, i);
                    this->second->parent = this;
                }
            } else { // Comienzan los argumentos de un predicado.
                this->predicate = Predicate(token, raw, i);
                token = "";
            }
        } else {
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

}

void Clause::setSecond(Clause *n) {

}

void Clause::addQuantifier(Quantifier n) {

}

void Clause::setPredicate(Predicate n) {

}

void Clause::setNot(bool n) {

}

Clause *Clause::getParent() {
    return nullptr;
}

void Clause::setParent(Clause *n) {

}

void Clause::setQuantifiers(vector<Quantifier> n) {
    this->quantifiers = n;
}

