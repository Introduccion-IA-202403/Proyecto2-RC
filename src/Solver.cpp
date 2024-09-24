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
     * En esta función utilizamos una bandera pasada por referencia para verificar si la unificación es posible
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
             * If we have a Skolem function and a variable or a constant, we can unify them
             */
            if (
                    (arg1.getType() == SKOLEM && (arg2.getType() == VARIABLE || arg2.getType() == CONSTANT)) ||
                    ((arg1.getType() == VARIABLE || arg1.getType() == CONSTANT) && arg2.getType() == SKOLEM)
                    ) { // Si se puede unificar

                    continue;
                if (arg1.getType() == SKOLEM) {
                    /**
                     * If the Skolem function is in the unification, we can not unify
                     */
                    if (ans.count(arg1.getName())) {
                        flag = false;
                        return ans;
                    }
                    ans[arg1.getName()] = (arg2.getType() == VARIABLE ? arg2.getArgSymbol() : arg2.getName());
                }
                else {

                    /**
                     * Same as the Skolem function
                     */
                    if (ans.count(arg2.getName())) {
                        flag = false;
                        return ans;
                    }

                    /**
                     * We store the unification of the Skolem function
                     */
                    ans[arg2.getName()] = (arg1.getType() == VARIABLE ? arg1.getArgSymbol() : arg1.getName());
                }
                continue;
            }

            /**
             * If the second argument is a constant, we swap the arguments
             * for the next verification
             */
            if (arg2.getType() == CONSTANT) swap(arg2, arg1);

            /**
             * If the second argument is a variable and it is in the unification, we can not unify
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
     * Check for all the clauses and their predicates arguments
     */
    for (auto &clause: resolve) {
        for (auto &predArg: clause.getPredicate().getArgs()) {

            /**
             * If the argument is a Skolem function and it is in the unification, we must apply the unification
             */
            if (predArg.getType() == SKOLEM && uni.count(predArg.getName())) {

                /**
                 * The variables has a number at the end, so we can identify them
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
                 * Clear the arguments of the Skolem function
                 */
                predArg.getArgsSymbols().clear();
                continue;
            }

            /**
             * If the argument is a variable and it is in the unification, we must apply the unification
             */
            if (uni.count(predArg.getArgSymbol())) {

                /**
                 * Same as the Skolem function
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
                 * If the Skolem Function isn't in the unification, we must apply the unification to its arguments
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
     * Check for all pairs of clauses if they can be unified
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
     * Apply the unification to the pairs that can be unified, identified in the previous loop
     */
    makeUnification(resolve, uni);


    /**
     * Vector to store the clauses that can be canceled
     */
    vector<bool> deleted(resolve.size(), false);

    /**
     * Check if exist a pair of clauses that can be canceled with NOT links
     */
    for (int i = 0; i < (int)resolve.size(); i++) {
        if (deleted[i]) continue;
        for (int j = i+1; j < (int)resolve.size(); j++) {
            if (deleted[j]) continue;
            Predicate from = resolve[i].getPredicate();
            Predicate to = resolve[j].getPredicate();

            /**
             * If the clauses have the same name and the same arguments, they can be canceled
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
                     * It does not matter if the arguments are variables or constants, they must be the same
                     */
                    if (argFrom.getType() != argTo.getType()) {
                        same = false;
                        break;
                    }

                    /**
                     * If the arguments are variables, they must be the same
                     */
                    if (argFrom.getType() == VARIABLE && argFrom.getArgSymbol() != argTo.getArgSymbol()) {
                         same = false;
                         break;
                    }

                    /**
                     * If the arguments are constants or Skolem functions, they must be the same
                     */
                    if ((argFrom.getType() == CONSTANT || argFrom.getType() == SKOLEM) && argFrom.getName() == argTo.getName()) {
                        same = false;
                        break;
                    }
                }

                /**
                 * If the clauses can be canceled, we mark them as deleted
                 */
                if (same) {
                    deleted[i] = true; deleted[j] = true;
                }
            }
        }
    }

    /**
     * We remove the clauses that can be canceled
     */
    vector<Clause> temp;
    for (int i = 0; i < (int)resolve.size(); i++) {
        if (!deleted[i]) temp.push_back(resolve[i]);
    }

    /**
     * Update the set of clauses with the clauses that can not be canceled
     */
    resolve = vector<Clause>(temp.begin(), temp.end());
}

bool Solver::backtrack(vector<Clause> resolve, unordered_map<string, string> uni) {

    // cout << "Antes Resolviendo: ";
    // for (auto &clause: resolve) {
    //     if (clause.isNot()) cout << "NOT ";
    //     cout << clause.getPredicate() << " ";
    // }
    // cout << endl;

    /**
     * Check if the initial set of clauses can be resolved or unified
     */
    checkInitialResolve(resolve, uni);

    cout << "Despues Resolviendo: ";
    for (auto &clause: resolve) {
        if (clause.isNot()) cout << "NOT ";
        cout << clause.getPredicate() << " ";
    }
    cout << endl;

    /**
     * If the set of clauses is empty, we return true because we have found a solution
     */
    if (resolve.empty()) return true;

    int i = 0;
    for (auto clause: resolve) {
        Predicate pred = clause.getPredicate();

        for (auto baseSet: clauses) {
            int j = 0;
            for (auto baseClause: baseSet) {

                /**
                 * This clause maybe can be canceled
                 */
                if (!clause.isNot() == baseClause.isNot() &&
                pred.getName() == baseClause.getPredicate().getName()
                ) {


                    bool good = true;
                    /**
                     * We try to unify the predicates to verify if they can be canceled
                     */
                    unordered_map<string, string> newUn = unify(pred, baseClause.getPredicate(), uni, good);

                    if (!good) {
                        j++;
                        continue;
                    }

                    /**
                     * Merge the two sets of clauses
                     */
                    auto resolveCpy = vector<Clause>(resolve.begin(), resolve.end());
                    resolveCpy.erase(resolveCpy.begin()+i);
                    auto baseCpy = vector<Clause>(baseSet.begin(), baseSet.end());
                    baseCpy.erase(baseCpy.begin()+j);

                    resolveCpy.insert(resolveCpy.end(), baseCpy.begin(), baseCpy.end());

                    /**
                     * Apply the unification for the resulting set of clauses
                     */
                    makeUnification(resolveCpy, newUn);

                    /**
                     * If the resulting set of clauses can be resolved, we return true
                     */
                    if (backtrack(resolveCpy, newUn)) return true;
                }

                j++;
            }
        }
        i++;
    }

    /**
     * If we have traversed all the clauses and we have not found a solution, we return false
     * because we have not found a solution
     */
    return false;
}

bool Solver::resolve(vector<Clause> resolve) {
    unordered_map<string,string> unifications;
    return backtrack(resolve, unifications);
}

void Solver::printSubstitution() {
    cout << "Sustitución actual: ";
    for (const auto& pair : substitution) {
        cout << pair.first << " -> " << pair.second << ", ";
    }
    cout << endl;
}
