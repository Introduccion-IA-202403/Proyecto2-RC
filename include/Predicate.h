#ifndef PROYECTO2_RC_PREDICATE_H
#define PROYECTO2_RC_PREDICATE_H

#include <string>
#include <iostream>
#include <PredicateArg.h>
#include <vector>

using namespace std;

class Predicate {
public:

    Predicate() = default;

    /**
     * Constructor for a predicate with a string and a start index to parse it
     * @param name The name of the predicate
     * @param raw The string to parse
     * @param start The initial index to start parsing
     */
    Predicate(string name, string &raw, int &start);

    const string &getName() const;

    void setName(const string &name);

    vector<PredicateArg>& getArgs();

    void setArgs(const vector<PredicateArg> &args);

    bool isEmpty() const;

    void setEmpty(bool empty);

    friend ostream& operator << (ostream &os, const Predicate &p) {
        os << p.name << "(";
        for (int i = 0; i < p.args.size(); i++) {
            os << p.args[i];
            if (i != p.args.size() - 1) os << ", ";
        }
        os << ")";
        return os;
    }

private:
    string name;
    vector<PredicateArg> args;
    bool empty = true;
};


#endif //PROYECTO2_RC_PREDICATE_H
