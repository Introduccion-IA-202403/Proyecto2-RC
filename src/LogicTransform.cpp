//
// Created by talkysafe143 on 22/09/24.
//
#include "LogicTransform.h"


vector<vector<Clause>> LogicTransform::transformClause(Clause *t) {


    deleteIFF(t, false);
    deleteImplies(t);
    transformNOT(t);

    vector<Quantifier> c;
    moveQuantifiers(t, c);


    /**
     * Skolemization works as follows:
     * 1. We must find the variables and the arguments of the clause
     * 2. We must find the skolem functions
     * 3. We must replace the variables with the skolem functions
     * 4. We must delete the quantifiers
     */
    char skolemName = 'A';
    set<string> variables, argsVar;
    for (auto q: t->getQuantifiers()) {
        if (q.getType() == EXIST) variables.insert(string(1, q.getArg()));
        else argsVar.insert(string(1, q.getArg()));
    }

    /**
     * This part is to reference the skolem functions for assigning them to the variables
     */
    unordered_map<string, PredicateArg> skolemFuncs;
    for (auto name: variables) {
        skolemFuncs[name] = PredicateArg(string(1, skolemName++), vector<string>(argsVar.begin(), argsVar.end()));
    }


    skolenize(t, skolemFuncs);

    /**
     * We delete the quantifiers of the clause, becuase we have already replaced the variables
     * with the skolem functions and FOR_ALL quantifiers are not needed
     */
    t->setQuantifiers(vector<Quantifier>());


    deleteORlinks(t);

    vector< vector<Clause> > ans;
    vector<Clause> current;

    collectResults(t, ans, current);

    /**
     * We must add the last clause
     */
    ans.push_back(current);
    return ans;
}

/**
 * The following functions can be seen like a functions to traverse the clause (As a tree)
 */
void LogicTransform::collectResults(Clause *t, vector< vector<Clause>>&collect, vector<Clause>& curr) {

    /**
     * If the clause is a leaf, we must add it to the current vector
     */
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) {
        curr.emplace_back(*t);
        curr.back().setParent(nullptr);
        return;
    }

    /**
     * If the clause is a leaf, but it is not the first, we must add it to the current vector
     */
    if (t->getFirst() == nullptr && t->getSecond() != nullptr) {
        curr.emplace_back();
        curr.back().setPredicate(t->getPredicate());
        curr.back().setNot(t->isNot());
    }


    if (t->getFirst() != nullptr) collectResults(t->getFirst(), collect, curr);

    /**
     * if the clause is an AND link, we must add the current vector to the collect vector
     * because we must separate the clauses with the AND link
     */
    if (t->getLink() == AND) {
        collect.push_back(curr);
        curr.clear();
    }
    if (t->getSecond() != nullptr) collectResults(t->getSecond(), collect, curr);
}

void LogicTransform::deleteIFF(Clause *t, bool first) {

    /**
     * If the clause is a leaf, we must stop traversing
     */
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    /**
     * We must traverse the clause
     */
    if (t->getFirst() != nullptr) deleteIFF(t->getFirst(), true);
    if (t->getSecond() != nullptr) deleteIFF(t->getSecond(), false);

    /**
     * If the clause is a iff link, we must transform it to a AND link
     */
    if (t->getLink() == IFF) {

        /**
         * We must create the new clauses
         */
        auto *w = new Clause(), *l = new Clause(), *r = new Clause();
        w->setLink(AND); l->setLink(IMPLIES); r->setLink(IMPLIES);
        for (auto g: t->getQuantifiers()) w->addQuantifier(g);


        w->setParent(t->getParent());
        if (t->getParent() != nullptr) {
            if (first) t->getParent()->setFirst(w);
            else t->getParent()->setSecond(w);
        }

        Clause *cloneL = nullptr, *cloneR = new Clause(*t->getSecond());

        if (t->getFirst() == nullptr) { // There is only one predicate
            cloneL = new Clause(*t->getFirst());
        } else { // This clause has a predicate and a link
            cloneL = new Clause();
            cloneL->setPredicate(t->getPredicate());
            cloneL->setNot(t->isNot());
        }

        /**
         * We must set the parents of the new clauses
         */
        l->setFirst(t->getFirst()); l->setSecond(t->getSecond());
        r->setFirst(cloneR); r->setSecond(cloneL);

        w->setFirst(l); w->setSecond(r);
        l->setParent(w); r->setParent(w);

        cloneL->setParent(r); cloneR->setParent(r);
        t->getFirst()->setParent(l); t->getSecond()->setParent(l);

        /**
         * We must delete the old clause for memory management
         */
        delete t;

        /**
         * We must set the new clause as the current clause
         */
        t = w;
    }
}

void LogicTransform::deleteImplies(Clause *t) {
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    if (t->getFirst() != nullptr) deleteImplies(t->getFirst());
    if (t->getSecond() != nullptr) deleteImplies(t->getSecond());

    /**
     * If the clause is an implies link, we must transform it to an OR link
     */
    if (t->getLink() == IMPLIES) {
        t->setLink(OR);

        /**
         * If the clause has only one predicate, we must create a new clause
         */
        if (t->getFirst() == nullptr) {
            auto *w = new Clause();
            w->setNot(true); w->setPredicate(t->getPredicate());
            t->setPredicate(Predicate()); t->setFirst(w);
            w->setParent(t);
        } else {
            if (t->getFirst()->isNot()) t->getFirst()->setNot(false);
            else t->getFirst()->setNot(true);
        }
    }
}

/**
 * This function is a special case, because we must go from left to right
 * traversing the clause to the leafs isn't the best way to cancel the NOT links
 * We can do it in a single pass and pass the information to the children
 */
void LogicTransform::transformNOT(Clause *t) {

    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    /**
     * If the clause is a NOT link, we must cancel it
     */
    if (t->isNot()) {

        /**
         * We must cancel the NOT link because we are going to pass the information to the children
         */
        t->setNot(false);

        /**
         * We must change the link of the clause
         */
        if (t->getSecond() != nullptr) t->setLink((t->getLink() == AND ? OR : AND));

        /**
         * We must change the quantifiers of the clause
         * FOR_ALL -> EXIST
         * EXIST -> FOR_ALL
         */
        for (auto &g: t->getQuantifiers()) {
            if (g.getType() == FOR_ALL) g.setType(EXIST);
            else g.setType(FOR_ALL);
        }

        /**
         * If the clause has a first member, we pass the information to it
         * if the member already has a NOT link, we must cancel it
         */
        if (t->getFirst() != nullptr) {
            if (t->getFirst()->isNot()) t->getFirst()->setNot(false);
            else t->getFirst()->setNot(true);
        }

        /**
         * same as the first member
         */
        if (t->getSecond() != nullptr) {
            if (t->getSecond()->isNot()) t->getSecond()->setNot(false);
            else t->getSecond()->setNot(true);
        }
    }

    /**
     * We must traverse the clause
     */
    if (t->getFirst() != nullptr) transformNOT(t->getFirst());
    if (t->getSecond() != nullptr) transformNOT(t->getSecond());
}

void LogicTransform::moveQuantifiers(Clause *t, vector<Quantifier> &collect) {
    if (t->getFirst() != nullptr) moveQuantifiers(t->getFirst(), collect);
    if (t->getSecond() != nullptr) moveQuantifiers(t->getSecond(), collect);

    /**
     * Collect the quantifiers of the clause
     */
    for (auto g: t->getQuantifiers()) collect.push_back(g);

    /**
     * if the clause is the root, we must set the quantifiers
     * if not, we delete the quantifiers from the clause
     */
    if (t->getParent() == nullptr) t->setQuantifiers(collect);
    else t->setQuantifiers(vector<Quantifier>());
}

void LogicTransform::skolenize(Clause *t, unordered_map<string, PredicateArg> &skolemFuncs) {

    /**
     * Just search for the variables and replace them with the skolem functions
     */
    for (auto &arg: t->getPredicate().getArgs()) {
        if (arg.getType() != VARIABLE) continue;
        if (skolemFuncs.count(arg.getArgSymbol())) {
            arg = skolemFuncs[arg.getArgSymbol()];
        }
    }

    if (t->getFirst() != nullptr) skolenize(t->getFirst(), skolemFuncs);
    if (t->getSecond() != nullptr) skolenize(t->getSecond(), skolemFuncs);
}


void LogicTransform::deleteORlinks(Clause *t) {

    if (t->getFirst() == nullptr || t->getSecond() == nullptr) return;
    if (t->getFirst()->getPredicate().isEmpty() && t->getFirst()->getFirst() == nullptr) return;

    /**
     * If the clause is not an OR or AND link, we must stop traversing
     */
    if (t->getLink() != OR || t->getFirst()->getLink() != AND) return;

    if (t->getFirst() != nullptr) deleteORlinks(t->getFirst());
    if (t->getSecond() != nullptr) deleteORlinks(t->getSecond());

    /**
     * If the clause is an OR link, we must transform it to an AND link
     */
    auto *copyY = new Clause(*t->getSecond()), *second = new Clause();

    // Segundo miembro
    second->setFirst(t->getFirst()->getSecond());
    t->getFirst()->getSecond()->setParent(second);
    second->setSecond(copyY); copyY->setParent(second);

    // Primer miembro
    t->getFirst()->setSecond(t->getSecond());
    t->getSecond()->setParent(t->getFirst());

    t->getFirst()->setLink(OR); second->setLink(OR);
    t->setLink(AND); t->setSecond(second);
}

