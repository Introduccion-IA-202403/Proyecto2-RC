#include "Solver.h"

using namespace std;

Solver::Solver(const vector<vector<Clause>>& clauses) : clauses(clauses) {}

bool Solver::unify(const Predicate& p1, const Predicate& p2) {
    // Verifica si los nombres y la cantidad de argumentos son iguales
    if (p1.getName() != p2.getName() || p1.getArgs().size() != p2.getArgs().size()) {
        return false;
    }

    for (size_t i = 0; i < p1.getArgs().size(); ++i) {
        const string& arg1 = p1.getArgs()[i];
        const string& arg2 = p2.getArgs()[i];

        if (arg1 != arg2) {
            // Realiza la unificación
            if (substitution.find(arg1) == substitution.end()) {
                substitution[arg1] = arg2;
            } else if (substitution[arg1] != arg2) {
                return false; // No se puede unificar
            }
        }
    }
    return true;
}

bool Solver::backtrack() {
    // Recorre todas las cláusulas
    for (const auto& clauseSet : clauses) {
        // Almacena el estado inicial de las sustituciones para backtracking
        auto originalSubstitution = substitution;

        // Verifica si alguna cláusula es satisfacible
        bool clauseSatisfiable = false;

        // Recorre cada cláusula en el conjunto de cláusulas
        for (const auto& clause : clauseSet) {
            // Recorre los predicados en la cláusula
            for (const auto& predicate : clause.getPredicate()) {
                // Intenta unificar el predicado con el primero de la cláusula
                if (unify(predicate, clause.getPredicate()[0])) {
                    clauseSatisfiable = true; // Al menos un predicado ha sido unificado
                    printSubstitution(); // Mostrar la sustitución actual
                    break; // Sale del bucle de predicados si se unificó
                }
            }

            // Si encontramos una cláusula satisfactoria, pasamos a la siguiente cláusula
            if (clauseSatisfiable) {
                break; 
            }
        }

        // Si no se encontró ninguna cláusula satisfactoria, restauramos el estado original
        if (!clauseSatisfiable) {
            substitution = originalSubstitution; // Revertimos a la sustitución anterior
        } else {
            // Si se encontró una solución, llamamos recursivamente a backtrack para resolver el resto
            if (backtrack()) {
                return true; // Si el resto es satisfacible, se encontró una solución
            }
        }
    }

    // Si hemos recorrido todas las cláusulas y no encontramos solución, regresamos false
    return false; // No se encontró solución
}

bool Solver::resolve() {
    return backtrack();
}

void Solver::printSubstitution() {
    cout << "Sustitución actual: ";
    for (const auto& pair : substitution) {
        cout << pair.first << " -> " << pair.second << ", ";
    }
    cout << endl;
}
