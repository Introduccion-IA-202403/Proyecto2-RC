
#include "LogicTransform.h"


vector<vector<Clause>> LogicTransform::transformClause(Clause *t) {


    deleteIFF(t, false);
    deleteImplies(t);
    transformNOT(t);

    vector<Quantifier> c;
    moveQuantifiers(t, c);

    /**
     * Skolemizacion funciona de la siguiente manera:
     * 1. Debemos encontrar las variables y los argumentos de la cláusula
     * 2. Debemos encontrar las funciones skolem
     * 3. Debemos reemplazar las variables con las funciones de skolem
     * 4. Debemos eliminar los cuantificadores
     */
    char skolemName = 'A';
    set<string> variables, argsVar;
    for (auto q: t->getQuantifiers()) {
        if (q.getType() == EXIST) variables.insert(string(1, q.getArg()));
        else argsVar.insert(string(1, q.getArg()));
    }

    /**
     * Esta parte es para referenciar las funciones skolem para asignarlas a las variables
     */
    unordered_map<string, PredicateArg> skolemFuncs;
    for (auto name: variables) {
        skolemFuncs[name] = PredicateArg(string(1, skolemName++), vector<string>(argsVar.begin(), argsVar.end()));
    }


    skolenize(t, skolemFuncs);

    /**
     * Borramos los cuantificadores de la cláusula, porque ya hemos reemplazado las variables
     * con las funciones de skolem y los cuantificadores FOR_ALL no son necesarios
     */
    t->setQuantifiers(vector<Quantifier>());


    deleteORlinks(t);

    vector< vector<Clause> > ans;
    vector<Clause> current;

    collectResults(t, ans, current);

    /**
     * Debe agregar la última cláusula
     */
    ans.push_back(current);
    return ans;
}

/**
 * Las siguientes funciones pueden ser vistas como funciones para recorrer la cláusula (como un árbol)
 */
void LogicTransform::collectResults(Clause *t, vector< vector<Clause>>&collect, vector<Clause>& curr) {

    /**
     * Si la cláusula es una hoja, debemos agregarla al vector actual
     */
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) {
        curr.emplace_back(*t);
        curr.back().setParent(nullptr);
        return;
    }

    /**
     * Si la cláusula es una hoja, pero no es la primera, debemos agregarla al vector actual
     */
    if (t->getFirst() == nullptr && t->getSecond() != nullptr) {
        curr.emplace_back();
        curr.back().setPredicate(t->getPredicate());
        curr.back().setNot(t->isNot());
    }


    if (t->getFirst() != nullptr) collectResults(t->getFirst(), collect, curr);

    /**
     * Si la cláusula tiene un conector AND, debemos agregar el vector actual al vector de recolección
     * porque debemos separar las cláusulas con el conector AND
     */
    if (t->getLink() == AND) {
        collect.push_back(curr);
        curr.clear();
    }
    if (t->getSecond() != nullptr) collectResults(t->getSecond(), collect, curr);
}

void LogicTransform::deleteIFF(Clause *t, bool first) {

    /**
     * Si la cláusula es una hoja, debemos detener el recorrido
     */
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    /**
     * Debemos recorrer la cláusula
     */
    if (t->getFirst() != nullptr) deleteIFF(t->getFirst(), true);
    if (t->getSecond() != nullptr) deleteIFF(t->getSecond(), false);

    /**
     * Si la cláusula es un conector iff, debemos transformarlo en un conector AND
     */
    if (t->getLink() == IFF) {

        /**
         * Deberíamos crear las nuevas cláusulas
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

        if (t->getFirst() == nullptr) { // Solo hay un predicado
            cloneL = new Clause(*t->getFirst());
        } else { // Esta clausula tiene un predicado y un conector
            cloneL = new Clause();
            cloneL->setPredicate(t->getPredicate());
            cloneL->setNot(t->isNot());
        }

        /**
         * Deberíamos establecer los padres de las nuevas cláusulas
         */
        l->setFirst(t->getFirst()); l->setSecond(t->getSecond());
        r->setFirst(cloneR); r->setSecond(cloneL);

        w->setFirst(l); w->setSecond(r);
        l->setParent(w); r->setParent(w);

        cloneL->setParent(r); cloneR->setParent(r);
        t->getFirst()->setParent(l); t->getSecond()->setParent(l);

        /**
         * Deberíamos eliminar la antigua cláusula para la gestión de memoria
         */
        delete t;

        /**
         * Deberíamos establecer la nueva cláusula como la cláusula actual
         */
        t = w;
    }
}

void LogicTransform::deleteImplies(Clause *t) {
    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    if (t->getFirst() != nullptr) deleteImplies(t->getFirst());
    if (t->getSecond() != nullptr) deleteImplies(t->getSecond());

    /**
     * Si la cláusula es un conector implica, debemos transformarlo en un conector OR
     */
    if (t->getLink() == IMPLIES) {
        t->setLink(OR);

        /**
         * Si la cláusula tiene solo un predicado, debemos crear una nueva cláusula
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
 * Esta función es un caso especial, porque debemos ir de izquierda a derecha
 * recorrer la cláusula hasta las hojas no es la mejor manera de cancelar los enlaces NOT
 * Podemos hacerlo en un solo paso y pasar la información a los hijos
 */
void LogicTransform::transformNOT(Clause *t) {

    if (t->getFirst() == nullptr && t->getSecond() == nullptr) return;

    /**
     * Si la cláusula tiene un enlace NOT, debemos cancelarlo
     */
    if (t->isNot()) {

        /**
         * Debemos cancelar el conector NOT porque vamos a pasar la información a los hijos
         */
        t->setNot(false);

        /**
         * Debemos cambiar el conector de la cláusula
         */
        if (t->getSecond() != nullptr) t->setLink((t->getLink() == AND ? OR : AND));

        /**
         * Debemos cambiar los cuantificadores de la cláusula
         * FOR_ALL -> EXIST
         * EXIST -> FOR_ALL
         */
        for (auto &g: t->getQuantifiers()) {
            if (g.getType() == FOR_ALL) g.setType(EXIST);
            else g.setType(FOR_ALL);
        }

        /**
         * Si la cláusula tiene un primer miembro, pasamos la información a él
         * si el miembro ya tiene un conector NOT, debemos cancelarlo
         */
        if (t->getFirst() != nullptr) {
            if (t->getFirst()->isNot()) t->getFirst()->setNot(false);
            else t->getFirst()->setNot(true);
        }

        /**
         * Lo mismo para el segundo miembro
         */
        if (t->getSecond() != nullptr) {
            if (t->getSecond()->isNot()) t->getSecond()->setNot(false);
            else t->getSecond()->setNot(true);
        }
    }

    /**
     * Debemos recorrer la cláusula
     */
    if (t->getFirst() != nullptr) transformNOT(t->getFirst());
    if (t->getSecond() != nullptr) transformNOT(t->getSecond());
}

void LogicTransform::moveQuantifiers(Clause *t, vector<Quantifier> &collect) {
    if (t->getFirst() != nullptr) moveQuantifiers(t->getFirst(), collect);
    if (t->getSecond() != nullptr) moveQuantifiers(t->getSecond(), collect);

    /**
     * Recolectamos los cuantificadores de la cláusula
     */
    for (auto g: t->getQuantifiers()) collect.push_back(g);

    /**
     * Si la clausula es la raiz, debemos establecer los cuantificadores
     * si no, debemos eliminar los cuantificadores de la clausula
     */
    if (t->getParent() == nullptr) t->setQuantifiers(collect);
    else t->setQuantifiers(vector<Quantifier>());
}

void LogicTransform::skolenize(Clause *t, unordered_map<string, PredicateArg> &skolemFuncs) {

    /**
     * Solo buscamos las variables y las reemplazamos con las funciones de skolem
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
     * Si la cláusula no es un conector OR o AND, debemos detener el recorrido
     */
    if (t->getLink() != OR || t->getFirst()->getLink() != AND) return;

    if (t->getFirst() != nullptr) deleteORlinks(t->getFirst());
    if (t->getSecond() != nullptr) deleteORlinks(t->getSecond());

    /**
     * Si la cláusula es un conector OR, debemos transformarlo en un conector AND
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

