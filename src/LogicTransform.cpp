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

    char skolemName = 'A';
    set<char> variables, argsVar;
    for (auto q: t->getQuantifiers()) {
        if (q.getType() == EXIST) variables.insert(q.getArg());
        else argsVar.insert(q.getArg());
    }
    unordered_map<char, PredicateArg> skolemFuncs;
    for (auto name: variables) {
        skolemFuncs[name] = PredicateArg(string(1, skolemName++), vector<char>(argsVar.begin(), argsVar.end()));
    }

    skolenize(t, skolemFuncs);

    t->setQuantifiers(vector<Quantifier>());
    deleteORlinks(t);

    vector< vector<Clause> > ans;
    vector<Clause> current;

    collectResults(t, ans, current);

    ans.push_back(current);
    return ans;
}

void LogicTransform::collectResults(Clause *t, vector< vector<Clause>>&collect, vector<Clause>& curr) {
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) {
        curr.emplace_back(*t);
        curr.back().setParent(nullptr);
        return;
    }

    if (t->getFirst() == nullptr && t->getSecond() != nullptr) {
        curr.emplace_back();
        curr.back().setPredicate(t->getPredicate());
        curr.back().setNot(t->isNot());
    }

    if (t->getFirst() != nullptr) collectResults(t->getFirst(), collect, curr);
    if (t->getLink() == AND) {
        collect.push_back(curr);
        curr.clear();
    }
    if (t->getSecond() != nullptr) collectResults(t->getSecond(), collect, curr);
}

void LogicTransform::deleteIFF(Clause *t, bool first) {
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    if (t->getFirst() != nullptr) deleteIFF(t->getFirst(), true);
    if (t->getSecond() != nullptr) deleteIFF(t->getSecond(), false);

    if (t->getLink() == IFF) {
        auto *w = new Clause(), *l = new Clause(), *r = new Clause();
        w->setLink(AND); l->setLink(IMPLIES); r->setLink(IMPLIES);
        for (auto g: t->getQuantifiers()) w->addQuantifier(g);

        w->setParent(t->getParent());
        if (t->getParent() != nullptr) {
            if (first) t->getParent()->setFirst(w);
            else t->getParent()->setSecond(w);
        }

        Clause *cloneL = nullptr, *cloneR = new Clause(*t->getSecond());

        if (t->getFirst() == nullptr) { // Solo hay un predicado
            cloneL = new Clause(*t->getFirst());
        } else { // Esta compuesta los dos
            cloneL = new Clause();
            cloneL->setPredicate(t->getPredicate());
            cloneL->setNot(t->isNot());
        }

        l->setFirst(t->getFirst()); l->setSecond(t->getSecond());
        r->setFirst(cloneR); r->setSecond(cloneL);

        w->setFirst(l); w->setSecond(r);
        l->setParent(w); r->setParent(w);

        cloneL->setParent(r); cloneR->setParent(r);
        t->getFirst()->setParent(l); t->getSecond()->setParent(l);
        delete t;
        t = w;
    }
}

void LogicTransform::deleteImplies(Clause *t) {
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    if (t->getFirst() != nullptr) deleteImplies(t->getFirst());
    if (t->getSecond() != nullptr) deleteImplies(t->getSecond());

    if (t->getLink() == IMPLIES) {
        t->setLink(OR);
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

void LogicTransform::transformNOT(Clause *t) {

    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    if (t->isNot()) {
        t->setNot(false);

        if (t->getSecond() != nullptr) t->setLink((t->getLink() == AND ? OR : AND));

        for (auto &g: t->getQuantifiers()) {
            if (g.getType() == FOR_ALL) g.setType(EXIST);
            else g.setType(FOR_ALL);
        }

        if (t->getFirst() != nullptr) {
            if (t->getFirst()->isNot()) t->getFirst()->setNot(false);
            else t->getFirst()->setNot(true);
        }

        if (t->getSecond() != nullptr) {
            if (t->getSecond()->isNot()) t->getSecond()->setNot(false);
            else t->getSecond()->setNot(true);
        }
    }

    if (t->getFirst() != nullptr) transformNOT(t->getFirst());
    if (t->getSecond() != nullptr) transformNOT(t->getSecond());
}

void LogicTransform::moveQuantifiers(Clause *t, vector<Quantifier> &collect) {
    if (t->getFirst() != nullptr) moveQuantifiers(t->getFirst(), collect);
    if (t->getSecond() != nullptr) moveQuantifiers(t->getSecond(), collect);

    for (auto g: t->getQuantifiers()) collect.push_back(g);
    if (t->getParent() == nullptr) t->setQuantifiers(collect);
    else t->setQuantifiers(vector<Quantifier>());
}

void LogicTransform::skolenize(Clause *t, unordered_map<char, PredicateArg> &skolemFuncs) {

    for (auto &arg: t->getPredicate().getArgs()) {
        if (arg.getType() != VARIABLE) continue;
        if (skolemFuncs.count(arg.getArgSymbol())) { arg = skolemFuncs[arg.getArgSymbol()]; }
    }

    if (t->getFirst() != nullptr) skolenize(t->getFirst(), skolemFuncs);
    if (t->getSecond() != nullptr) skolenize(t->getSecond(), skolemFuncs);
}


void LogicTransform::deleteORlinks(Clause *t) {
    if (t->getFirst() == nullptr || t->getSecond() == nullptr) return;
    if (t->getFirst()->getPredicate().isEmpty() && t->getFirst()->getFirst() == nullptr) return;
    if (t->getLink() != OR || t->getFirst()->getLink() != AND) return;

    if (t->getFirst() != nullptr) deleteORlinks(t->getFirst());
    if (t->getSecond() != nullptr) deleteORlinks(t->getSecond());

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

