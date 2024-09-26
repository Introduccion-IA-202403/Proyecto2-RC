#include "Solver.h"

using namespace std;

Solver::Solver(vector<vector<Clause>>& cl)  {
    clauses = vector<vector<Clause>>(cl.begin(), cl.end());

    /**
    * En este bucle agregamos un número al final de las variables y constantes
    * para identificarlas en la unificación, ya que no podemos tener el mismo nombre
     */
    for (int i = 1; i <= (int)clauses.size(); i++) {
        for (auto &clause: clauses[i-1]) {
            for (auto &predArg: clause.getPredicate().getArgs()) {
                if (predArg.getType() != SKOLEM && predArg.getType() != CONSTANT) {
                    predArg.setArgSymbol(predArg.getArgSymbol()+ to_string(i));
                }
                else {
                    for (auto &skolemArg: predArg.getArgsSymbols()) {
                        skolemArg += to_string(i);
                    }
                }
            }
        }
    }

    for (int i = 0; i < (int)clauses.size(); i++) {
        for (int j = 0; j < (int)clauses[i].size(); j++) {
            if (clauses[i][j].isNot()) cout << "NOT ";
            cout << clauses[i][j].getPredicate() << " ";
        }
        cout << endl;
    }
}

unordered_map<string, string> Solver::unify(Predicate& from, Predicate& to, unordered_map<string, string> un, bool &flag) {

    /**
     * En esta función utilizamos una "bandera" pasada por referencia para verificar si la unificación es posible
     * Esa bandera indicará si la unificación es posible o no
     */


    /**
     * Debemos devolver una nueva copia de la unificación para realizar el retroceso
     */
    unordered_map<string, string> ans = un;
    /**
     * Verificar si los nombres y el número de argumentos son iguales
     */
    if (from.getArgs().size() != to.getArgs().size()) {
        flag = false;
        return ans;
    }


    /**
     * Verificar todos los argumentos de los predicados
     */
    for (size_t i = 0; i < from.getArgs().size(); ++i) {
        PredicateArg arg1 = PredicateArg(from.getArgs()[i]);
        PredicateArg arg2 = PredicateArg(to.getArgs()[i]);

        /**
         * Si los argumentos son iguales, continuamos con el siguiente argumento
         * de lo contrario, debemos verificar si los argumentos se pueden unificar
         */
        if (arg1.getName() != arg2.getName()) {

            /**
             * If the arguments are constants, we can not unify them
             */
            if (arg1.getType() == CONSTANT && arg2.getType() == CONSTANT) {
                flag = false;
                return ans;
            }

            /**
             * Si tenemos una función Skolem y una variable o una constante, podemos unificarlos
             */
            if (
                    (arg1.getType() == SKOLEM && (arg2.getType() == VARIABLE || arg2.getType() == CONSTANT)) ||
                    ((arg1.getType() == VARIABLE || arg1.getType() == CONSTANT) && arg2.getType() == SKOLEM)
                    ) { // Si se puede unificar

                //continue;
                if (arg1.getType() == SKOLEM) {

                    if (arg1.getArgsSymbols().size() == 0) continue;

                    /**
                     * Si la función Skolem está en la unificación, no podemos unificar
                     */
                    if (ans.count(arg1.getName())) {
                        flag = false;
                        return ans;
                    }
                    ans[arg1.getName()] = (arg2.getType() == VARIABLE ? arg2.getArgSymbol() : arg2.getName());
                }
                else {
                    
                    if (arg2.getArgsSymbols().size() == 0) continue;

                    /**
                     * Igual que la función Skolem
                     */
                    if (ans.count(arg2.getName())) {
                        flag = false;
                        return ans;
                    }

                    /**
                     * Guardamos la unificación de la función Skolem
                     */
                    ans[arg2.getName()] = (arg1.getType() == VARIABLE ? arg1.getArgSymbol() : arg1.getName());
                }
                continue;
            }

            /**
             * Si el segundo argumento es una constante, intercambiamos los argumentos
             * para la siguiente verificación
             */
            if (arg2.getType() == CONSTANT) swap(arg2, arg1);

            /**
             * Si el segundo argumento es una variable y está en la unificación, no podemos unificar
             */
            if (ans.count(arg2.getArgSymbol())) {
                flag = false;
                return ans;
            }
            ans[arg2.getArgSymbol()] = (arg1.getType() == VARIABLE ? arg1.getArgSymbol() : arg1.getName());
        }
    }
    flag = true;
    return ans;
}

void Solver::makeUnification(vector<Clause> &resolve, unordered_map<string, string> &uni) {

    /**
     * Revisar todas las cláusulas y sus argumentos de los predicados
     */
    for (auto &clause: resolve) {
        for (auto &predArg: clause.getPredicate().getArgs()) {

            /**
             * Si el argumento es una función Skolem y está en la unificación, debemos aplicar la unificación
             */
            if (predArg.getType() == SKOLEM && uni.count(predArg.getName())) {

                /**
                 * Las variables tienen un número al final, por lo que podemos identificarlas
                 */
                if (uni[predArg.getName()].back() >= 48 && uni[predArg.getName()].back() <= 57) {
                    predArg.setArgSymbol(uni[predArg.getName()]);
                    predArg.setName("");
                    predArg.setType(VARIABLE);
                }
                else {
                    predArg.setName(uni[predArg.getName()]);
                    predArg.setType(CONSTANT);
                    predArg.setArgSymbol("-");
                }

                /**
                 * "Limpiar" los argumentos de la función Skolem
                 * 
                 */
                predArg.getArgsSymbols().clear();
                continue;
            }

            /**
             * Si el argumento es una variable y está en la unificación, debemos aplicar la unificación
             */
            if (uni.count(predArg.getArgSymbol())) {

                /**
                 * Igual que la función Skolem
                 */
                if (uni[predArg.getArgSymbol()].back() >= 48 && uni[predArg.getArgSymbol()].back() <= 57) {
                    predArg.setArgSymbol(uni[predArg.getArgSymbol()]);
                    predArg.setType(VARIABLE);
                }
                else {
                    predArg.setName(uni[predArg.getArgSymbol()]);
                    predArg.setType(CONSTANT);
                    predArg.setArgSymbol("-");
                }
            } else if (predArg.getType() == SKOLEM) {
                /**
                 * Si la función Skolem no está en la unificación, debemos aplicar la unificación a sus argumentos
                 */
                for (auto &skolemArgs: predArg.getArgsSymbols()) {
                    if (uni.count(skolemArgs)) skolemArgs = uni[skolemArgs];
                }
            }
        }
    }
}

void Solver::checkInitialResolve(vector<Clause>& resolve, unordered_map<string, string> &uni) {

    /**
     * Revisa si existen pares de cláusulas que puedan ser unificadas
     */
    for (int i = 0; i < (int)resolve.size(); i++) {
        for (int j = i+1; j < (int)resolve.size(); j++) {

            if (resolve[i].getPredicate().getName() != resolve[j].getPredicate().getName()) continue;

            bool good;
            auto possibleUnify = unify(resolve[i].getPredicate(), resolve[j].getPredicate(), uni, good);
            if (!good) continue;
            uni = possibleUnify;
        }
    }

    /**
     * Aplica la unificación a los pares que pueden ser unificados, identificados en el bucle anterior
     */
    makeUnification(resolve, uni);


    /**
     * Vector para almacenar las cláusulas que pueden ser canceladas
     */
    vector<bool> deleted(resolve.size(), false);

    /**
     * Revisar si existen pares de cláusulas que puedan ser canceladas con enlaces NOT
     */
    for (int i = 0; i < (int)resolve.size(); i++) {
        if (deleted[i]) continue;
        for (int j = i+1; j < (int)resolve.size(); j++) {
            if (deleted[j]) continue;
            Predicate from = resolve[i].getPredicate();
            Predicate to = resolve[j].getPredicate();

            /**
             * Si las cláusulas tienen el mismo nombre y los mismos argumentos, pueden ser canceladas
             */
            if (resolve[i].isNot() == !resolve[j].isNot() && from.getName() == to.getName()) {
                bool same = true;
                for (int k = 0; k < (int)from.getArgs().size(); k++) {
                    auto argFrom = from.getArgs()[k];
                    auto argTo = to.getArgs()[k];

                    if (argFrom.getType() == SKOLEM || argTo.getType() == SKOLEM) {
                        continue;
                    }
                    /**
                     * No importa si los argumentos son variables o constantes, deben ser iguales
                     */
                    if (argFrom.getType() != argTo.getType()) {
                        same = false;
                        break;
                    }

                    /**
                     * Si los argumentos son variables, deben ser iguales
                     */
                    if (argFrom.getType() == VARIABLE && argFrom.getArgSymbol() != argTo.getArgSymbol()) {
                         same = false;
                         break;
                    }

                    /**
                     * Si los argumentos son constantes o funciones Skolem, deben ser iguales
                     */
                    if ((argFrom.getType() == CONSTANT || argFrom.getType() == SKOLEM) && argFrom.getName() == argTo.getName()) {
                        same = false;
                        break;
                    }
                }

                /**
                 * Si las cláusulas pueden ser canceladas, las marcamos como eliminadas
                 */
                if (same) {
                    deleted[i] = true; deleted[j] = true;
                }
            }
        }
    }

    /**
     * Borramos las cláusulas que pueden ser canceladas
     */
    vector<Clause> temp;
    for (int i = 0; i < (int)resolve.size(); i++) {
        if (!deleted[i]) temp.push_back(resolve[i]);
    }

    /**
     * Actualizamos el conjunto de cláusulas con las cláusulas que no pueden ser canceladas
     */
    resolve = vector<Clause>(temp.begin(), temp.end());
}

bool Solver::backtrack(vector<Clause> resolve, unordered_map<string, string> uni, vector<vector<Clause>> path) {

    // cout << "Antes Resolviendo: ";
    // for (auto &clause: resolve) {
    //     if (clause.isNot()) cout << "NOT ";
    //     cout << clause.getPredicate() << " ";
    // }
    // cout << endl;

    /**
     * Revisa si el conjunto inicial de cláusulas puede ser resuelto o unificado
     */
    checkInitialResolve(resolve, uni);

    cout << "Despues Resolviendo: ";
    for (auto &clause: resolve) {
        if (clause.isNot()) cout << "NOT ";
        cout << clause.getPredicate() << " ";
    }
    cout << endl;

    /**
     * Si el conjunto de cláusulas está vacío, devolvemos verdadero porque hemos encontrado una solución
     */
    if (resolve.empty()) {
        this->path = path;
        return true;
    }

    int i = 0;
    for (auto clause: resolve) {
        Predicate pred = clause.getPredicate();

        for (auto baseSet: clauses) {
            int j = 0;
            for (auto baseClause: baseSet) {

                /**
                 * Esta cláusula puede ser cancelada
                 */
                if (!clause.isNot() == baseClause.isNot() &&
                pred.getName() == baseClause.getPredicate().getName()
                ) {


                    bool good = true;
                    /**
                     * Tratamos de unificar los predicados para verificar si pueden ser cancelados
                     */
                    unordered_map<string, string> newUn = unify(pred, baseClause.getPredicate(), uni, good);

                    if (!good) {
                        j++;
                        continue;
                    }

                    /**
                     * Combinar los dos conjuntos de cláusulas
                     */
                    auto resolveCpy = vector<Clause>(resolve.begin(), resolve.end());
                    resolveCpy.erase(resolveCpy.begin()+i);
                    auto baseCpy = vector<Clause>(baseSet.begin(), baseSet.end());
                    baseCpy.erase(baseCpy.begin()+j);

                    resolveCpy.insert(resolveCpy.end(), baseCpy.begin(), baseCpy.end());

                    /**
                     * Aplicar la unificación para el conjunto resultante de cláusulas
                     */
                    makeUnification(resolveCpy, newUn);

                    path.push_back(baseSet);
                    /**
                     * Si el conjunto resultante de cláusulas puede ser resuelto, devolvemos verdadero
                     */
                    if (backtrack(resolveCpy, newUn, path)) {
                        return true;
                    }

                    path.pop_back();
                }

                j++;
            }
        }
        i++;
    }

    /**
     * Si hemos recorrido todas las cláusulas y no hemos encontrado una solución, devolvemos falso
     * porque no hemos encontrado una solución
     */
    return false;
}

bool Solver::resolve(vector<Clause> resolve) {
    unordered_map<string,string> unifications;
    vector<vector<Clause>> path;
    return backtrack(resolve, unifications, path);
}

void Solver::printSubstitution() {
    cout << "Sustitución actual: ";
    for (const auto& pair : substitution) {
        cout << pair.first << " -> " << pair.second << ", ";
    }
    cout << endl;
}

vector<vector<Clause>> Solver::getPath() {
    return path;
}
