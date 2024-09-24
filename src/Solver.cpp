#include "Solver.h"

Solver::Solver(vector<Clause*>& base) : base(base) {}

bool Solver::resolve() {
    vector<Clause*> newClauses;
    bool foundNew = true;

    while (foundNew) {
        foundNew = false;

        // Comparamos todas las cláusulas entre sí
        for (size_t i = 0; i < base.size(); i++) {
            for (size_t j = i + 1; j < base.size(); j++) {
                Clause* resolvent = resolveClauses(base[i], base[j]);
                if (resolvent != nullptr) {
                    newClauses.push_back(resolvent);
                    foundNew = true;
                    if (resolvent->getFirst() == nullptr && resolvent->getSecond() == nullptr) {
                        // Encontramos la cláusula vacía, lo que indica una contradicción
                        return true;
                    }
                }
            }
        }
        
        // Agregar las nuevas cláusulas a la base
        for (auto& clause : newClauses) {
            base.push_back(clause);
        }
    }

    // Si no se encontró la contradicción, la resolución falló
    return false;
}

Clause* Solver::resolveClauses(Clause* c1, Clause* c2) {
    // Aquí comparamos los predicados y eliminamos los complementarios
    if (c1->getPredicate().getName() == c2->getPredicate().getName() &&
        c1->isNot() != c2->isNot()) {

        // Crear una nueva cláusula sin los predicados complementarios
        Clause* resolvent = new Clause();
        
        // Aquí agregaríamos el resto de los que no sean complementarios
        return resolvent;
    }
    
    return nullptr;  // No se pudo resolver
}

bool Solver::hasContradiction() {
    // Recorremos la base para buscar la cláusula vacía
    for (Clause* clause : base) {
        if (clause->getFirst() == nullptr && clause->getSecond() == nullptr) {
            return true;
        }
    }
    return false;
}