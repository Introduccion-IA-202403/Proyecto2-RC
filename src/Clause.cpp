#include <Clause.h>


Clause::Clause(string &raw, int &i) {

    /**
     * This constructor is used to parse a string into a Clause object
     */
    string token;
    bool buildFirst = true;
    for (; i < (int)raw.size(); i++) {

        /**
         * We break the loop if we find a closing parenthesis because that means we have finished parsing the clause
         */
        if (raw[i] == ')') break;

        /**
         * We catch some spaces because they can be tokens that we are interested in
         */
        if (raw[i] == ' ') {

            /**
             * We must eval if the token is a quantifier
             */
            auto quantifier = Quantifier::evalQuantifier(token);
            if (quantifier.getType() != INVALID) {
                /**
                 * If the token is a quantifier, we must set the variable of the quantifier
                 */
                i++; quantifier.setArg(raw[i]);
                this->quantifiers.push_back(quantifier);
                token = "";
                continue;
            }

            /**
             * We must eval if the token is a link
             */
            auto possibleLink = evalLink(token);
            if (possibleLink != INVALID_LINK) {
                this->link = possibleLink;
                buildFirst = false;
                token = "";
                continue;
            }

            /**
             * We must eval if the token is a NOT
             */
            if (token == "NOT") {
                this->NOT = true;
                token = "";
            }
            continue;
        }

        /**
         * If we find an opening parenthesis, we must start a new clause or arguments
         */
        if (raw[i] == '(') {

            i++;

            /**
             * If the token is empty, we must start a new clause
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
                 * If not, we must create a new predicate because the token is the name of the predicate
                 */
                this->predicate = Predicate(token, raw, i);
                token = "";
            }
        } else {
            /**
             * If the character is not a parenthesis, we must add it to the token
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

