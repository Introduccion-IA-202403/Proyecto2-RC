#ifndef PROYECTO2_RC_PREDICATE_H
#define PROYECTO2_RC_PREDICATE_H

#include <string>
#include <PredicateArg.h>
#include <vector>

using namespace std;

class Predicate {
public:

    Predicate() = default;

    Predicate(string name, string &raw, int &start);

    const string &getName() const;

    void setName(const string &name);

    const vector<PredicateArg>& getArgs() const;

    void setArgs(const vector<PredicateArg> &args);

    bool isEmpty() const;

    void setEmpty(bool empty);

private:
    string name;
    vector<PredicateArg> args;
    bool empty = true;
};


#endif //PROYECTO2_RC_PREDICATE_H
