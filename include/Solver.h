#ifndef PROYECTO2_RC_SOLVER_H
#define PROYECTO2_RC_SOLVER_H

#include <Clause.h>
#include <vector>

class Solver {
public:
    Solver(vector<Clause*>& base);  // Recibe la base de conocimiento

    bool resolve();  // Método de resolución por refutación

private:
    vector<Clause*> base;  // Base de conocimiento
    
    Clause* resolveClauses(Clause* c1, Clause* c2);  // Resuelve dos clausulas

    bool hasContradiction();  // Verifica si se ha encontrado una contradicción
};

#endif //PROYECTO2_RC_SOLVER_H
