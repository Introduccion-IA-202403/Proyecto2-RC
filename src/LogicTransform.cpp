//
// Created by talkysafe143 on 22/09/24.
//
#include "LogicTransform.h"

vector<Clause> LogicTransform::transformClause(Clause *t) {

    return {};
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

void LogicTransform::deleteImplies(Clause *t, bool first) {
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    if (t->getFirst() != nullptr) deleteImplies(t->getFirst(), true);
    if (t->getSecond() != nullptr) deleteImplies(t->getSecond(), false);

    if (t->getLink() == IMPLIES) {
        t->setLink(OR);
        if (t->getFirst() == nullptr) {
            auto *w = new Clause();
            w->setNot(true); w->setPredicate(t->getPredicate());
            t->setPredicate(Predicate()); t->setFirst(w);
        } else {
            if (t->getFirst()->isNot()) t->getFirst()->setNot(false);
            else t->getFirst()->setNot(true);
        }
    }
}

void LogicTransform::transformNOT(Clause *t, bool first) {
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) {
        if (t->getParent() == nullptr) return;
        if (t->getParent()->isNot() && t->isNot()) t->setNot(false);

        // Mirar los cuantificadores
        return;
    }
    if (t->getParent() == nullptr || !t->getParent()->isNot() || t->getLink() == EMPTY) return;
    t->setLink((t->getLink() == AND ? OR : AND));

    if (t->getSecond() == nullptr) {
        for (auto &g: t->getQuantifiers()) {
            if (g.getType() == FOR_ALL) g.setType(EXIST);
            else g.setType(FOR_ALL);
        }
        if (t->getFirst() != nullptr) {
            if (t->getFirst()->isNot()) t->getFirst()->setNot(false);
            else t->getFirst()->setNot(true);
        }
    } else {
        if (t->getParent()->isNot()) {
            if (t->getFirst()->isNot()) t->getFirst()->setNot(false);
            else t->getFirst()->setNot(true);

            if (t->getSecond()->isNot()) t->getSecond()->setNot(false);
            else t->getSecond()->setNot(true);
        }
    }
}

void LogicTransform::uniqueQuantifiers(Clause *t, bool first) {

}

void LogicTransform::moveQuantifiers(Clause *t, bool first, vector<Quantifier> &collect) {
    if (t->getFirst() != nullptr) moveQuantifiers(t->getFirst(), true, collect);
    if (t->getSecond() != nullptr) moveQuantifiers(t->getSecond(), false, collect);

    for (auto g: t->getQuantifiers()) collect.push_back(g);
    if (t->getParent() == nullptr) t->setQuantifiers(collect);
    else t->setQuantifiers(vector<Quantifier>());
}

void LogicTransform::skolenize(Clause *t, bool first, set<char>& skolem, char& skolemName) {
    set<char> predArgsSkolem;
    for (auto arg: t->getPredicate().getArgs()) {
        if (arg.getType() == VARIABLE && !skolem.count(arg.getArgSymbol())) predArgsSkolem.insert(arg.getArgSymbol());
    }

    for (auto &arg: t->getPredicate().getArgs()) {
        if (arg.getType() != VARIABLE) continue;
        if (skolem.count(arg.getArgSymbol())) {
            arg.setType(SKOLEM);
            arg.setName(string(1, skolemName++));
            arg.setArgsSymbols(vector<char>(predArgsSkolem.begin(), predArgsSkolem.end()));
        }
    }

    if (t->getFirst() != nullptr) skolenize(t->getFirst(), true, skolem, skolemName);
    if (t->getSecond() != nullptr) skolenize(t->getSecond(), false, skolem, skolemName);
}


void LogicTransform::deleteORlinks(Clause *t, bool first) {

}

vector<Clause *> LogicTransform::separateANDlinks(Clause *t, bool first) {
    return vector<Clause *>();
}

