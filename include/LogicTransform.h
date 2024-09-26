#ifndef PROYECTO2_RC_LOGICTRANSFORM_H
#define PROYECTO2_RC_LOGICTRANSFORM_H

#include <Clause.h>
#include <set>
#include <unordered_map>

class LogicTransform {
private:
    /**
     * Borra los conectores si y solo si en la clausula
    * @param t La cláusula en la que se eliminarán los conectores iff
     * @param first Si la cláusula es la primera o la segunda
     */
    void deleteIFF(Clause *t, bool first);

    /**
     * Borra los conectores implica en la clausula
     * @param t La clausula en la que se eliminarán los conectores implica
     */
    void deleteImplies(Clause *t);

    /**
     * Transforma los conectores NOT en la clausula
     * @param t La claúsula en la que se transformarán los conectores NOT
     */
    void transformNOT(Clause *t);

    /**
     * Mover los cuantificadores al inicio de la clausula
     * @param t La clausula en la que se moverán los cuantificadores
     * @param collect El vector de cuantificadores
     */
    void moveQuantifiers(Clause *t, vector<Quantifier>& collect);

    /**
     * Skolenizar la clausula
     * @param t La clausula a skolenizar
     * @param skolemFuncs El mapa de funciones de skolem
     */
    void skolenize(Clause *t, unordered_map<string, PredicateArg>& skolemFuncs);

    /**
     * Elimina los conectores OR en la clausula
     * @param t La clausula en la que se eliminarán los conectores OR
     */
    void deleteORlinks(Clause *t);

    /**
     * Recopila los resultados de la transformación
     * @param t LA clausula a recopilar
     * @param collect El vector de clausulas
     * @param curr El vector de clausulas actual
     */
    void collectResults(Clause *t, vector< vector<Clause>>&collect, vector<Clause>& curr);

public:
    /**
     * Transforma una clausula en un vector de clausulas
     * @param t La claúsula a transformar
     * @return El vector de clausulas
     */
    vector<vector<Clause>> transformClause(Clause* t);
};

#endif //PROYECTO2_RC_LOGICTRANSFORM_H
