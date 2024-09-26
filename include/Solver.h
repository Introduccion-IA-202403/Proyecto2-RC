#ifndef PROYECTO2_RC_SOLVER_H
#define PROYECTO2_RC_SOLVER_H

#include <Clause.h>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Solver {
public:

    /**
     * Constructor para el Solver con la base do conocimiento
     * @param clauses La base de conocimiento
     */
    Solver(vector<vector<Clause>>& clauses);

    /**
     * Resuelve un conjunto de clausulas por refutacion
     * @param resolve El conjunto de clausulas a resolver
     * @return Verdadero si el conjunto de clausulas es verdadero, falso si no
     */
    bool resolve(vector<Clause> resolve);

    /**
     * Devuelve el camino que se ha seguido para resolver el conjunto de clausulas
     * @return El camino que se ha seguido para resolver el conjunto de clausulas
     */
    vector<vector<Clause>> getPath();

private:
    vector<vector<Clause>> clauses;  // Base de conocimiento
    unordered_map<string, string> substitution;
    vector<vector<Clause>> path;

    /**
     * Revisa si el conjunto inicial de clausulas puede ser resuelto o unificado
     * @param resolve El conjunto de clausulas a resolver
     * @param uni La unificación a aplicar
     */
    void checkInitialResolve(vector<Clause>& resolve, unordered_map<string, string> &uni);

    /**
     * Hacer la unificación de un conjunto de clausulas
     * @param resolve El conjunto de clausulas a unificar
     * @param uni La unificación a aplicar
     */
    void makeUnification(vector<Clause> &resolve, unordered_map<string, string> &uni);

    /**
    * Retrocede en el conjunto de cláusulas a resolver
     * @param resolve El set de cláusulas a resolver
     * @param uni La unificación a aplicar
     * @return Verdadero si el conjunto de cláusulas es verdadero, falso si no
     */
    bool backtrack(vector<Clause> resolve, unordered_map<string, string> uni, vector<vector<Clause>> path);

    /**
     * Unificar dos predicados
     * @param p1 El primer predicado a unificar
     * @param p2 El segundo predicado a unificar
     * @param un La unificacion a aplicar
     * @param flag Revisar si la unificación es posible
     * @return La unificación de los dos predicados
     */
    unordered_map<string, string> unify(Predicate& p1, Predicate& p2, unordered_map<string, string> un, bool &flag);
    void printSubstitution();
};

#endif //PROYECTO2_RC_SOLVER_H
