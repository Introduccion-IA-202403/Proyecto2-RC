#include "Solver.h"

using namespace std;

Solver::Solver(vector<vector<Clause>>& cl)  {
    clauses = vector<vector<Clause>>(cl.begin(), cl.end());
    for (int i = 1; i <= clauses.size(); i++) {
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
}

unordered_map<string, string> Solver::unify(Predicate& from, Predicate& to, unordered_map<string, string> un, bool &flag) {

    unordered_map<string, string> ans = un;
    // Verifica si los nombres y la cantidad de argumentos son iguales
    if (from.getArgs().size() != to.getArgs().size()) {
        flag = false;
        return ans;
    }


    for (size_t i = 0; i < from.getArgs().size(); ++i) {
        PredicateArg arg1 = PredicateArg(from.getArgs()[i]);
        PredicateArg arg2 = PredicateArg(to.getArgs()[i]);

        if (arg1.getName() != arg2.getName()) {
            if (arg1.getType() == CONSTANT && arg2.getType() == CONSTANT) {
                flag = false;
                return ans;
            }
            if (
                    (arg1.getType() == SKOLEM && (arg2.getType() == VARIABLE || arg2.getType() == CONSTANT)) ||
                    ((arg1.getType() == VARIABLE || arg1.getType() == CONSTANT) && arg2.getType() == SKOLEM)
                    ) { // Si se puede unificar


                if (arg1.getType() == SKOLEM) {
                    if (ans.count(arg1.getName())) {
                        flag = false;
                        return ans;
                    }
                    ans[arg1.getName()] = (arg2.getType() == VARIABLE ? arg2.getArgSymbol() : arg2.getName());
                }
                else {
                    if (ans.count(arg2.getName())) {
                        flag = false;
                        return ans;
                    }
                    ans[arg2.getName()] = (arg1.getType() == VARIABLE ? arg1.getArgSymbol() : arg1.getName());
                }
                continue;
            }

            if (arg2.getType() == CONSTANT) swap(arg2, arg1);

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
    for (auto &clause: resolve) {
        for (auto &predArg: clause.getPredicate().getArgs()) {

            if (predArg.getType() == SKOLEM && uni.count(predArg.getName())) {
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
                predArg.getArgsSymbols().clear();
                continue;
            }

            if (uni.count(predArg.getArgSymbol())) {

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
                for (auto &skolemArgs: predArg.getArgsSymbols()) {
                    if (uni.count(skolemArgs)) skolemArgs = uni[skolemArgs];
                }
            }
        }
    }
}

void Solver::checkInitialResolve(vector<Clause>& resolve, unordered_map<string, string> &uni) {
    for (int i = 0; i < resolve.size(); i++) {
        for (int j = i+1; j < resolve.size(); j++) {
            bool good;
            auto possibleUnify = unify(resolve[i].getPredicate(), resolve[j].getPredicate(), uni, good);
            if (!good) continue;
            uni = possibleUnify;
        }
    }

    makeUnification(resolve, uni);

    vector<bool> deleted(resolve.size(), false);
    for (int i = 0; i < resolve.size(); i++) {
        if (deleted[i]) continue;
        for (int j = i+1; j < resolve.size(); j++) {
            if (deleted[j]) continue;
            Predicate from = resolve[i].getPredicate();
            Predicate to = resolve[j].getPredicate();

            if (resolve[i].isNot() == !resolve[j].isNot() && from.getName() == to.getName()) {
                bool same = true;
                for (int k = 0; k < from.getArgs().size(); k++) {
                    auto argFrom = from.getArgs()[k];
                    auto argTo = to.getArgs()[k];

                    if (argFrom.getType() != argTo.getType()) {
                        same = false;
                        break;
                    }

                    if (argFrom.getType() == VARIABLE && argFrom.getArgSymbol() != argTo.getArgSymbol()) {
                         same = false;
                         break;
                    }

                    if ((argFrom.getType() == CONSTANT || argFrom.getType() == SKOLEM) && argFrom.getName() == argTo.getName()) {
                        same = false;
                        break;
                    }
                }

                if (same) {
                    deleted[i] = true; deleted[j] = true;
                }
            }
        }
    }

    vector<Clause> temp;
    for (int i = 0; i < resolve.size(); i++) {
        if (!deleted[i]) temp.push_back(resolve[i]);
    }

    resolve = vector<Clause>(temp.begin(), temp.end());
}

bool Solver::backtrack(vector<Clause> resolve, unordered_map<string, string> uni) {

    checkInitialResolve(resolve, uni);

    if (resolve.empty()) return true;

    int i = 0;
    for (auto clause: resolve) {
        Predicate pred = clause.getPredicate();

        for (auto baseSet: clauses) {
            int j = 0;
            for (auto baseClause: baseSet) {

                if (!clause.isNot() == baseClause.isNot() &&
                pred.getName() == baseClause.getPredicate().getName()
                ) { // Se puede cancelar

                    // Hay que verificar si se puede unificar
                    bool good = true;
                    unordered_map<string, string> newUn = unify(pred, baseClause.getPredicate(), uni, good);

                    if (!good) {
                        j++;
                        continue;
                    }

                    auto resolveCpy = vector<Clause>(resolve.begin(), resolve.end());
                    resolveCpy.erase(resolveCpy.begin()+i);
                    auto baseCpy = vector<Clause>(baseSet.begin(), baseSet.end());
                    baseCpy.erase(baseCpy.begin()+j);

                    resolveCpy.insert(resolveCpy.end(), baseCpy.begin(), baseCpy.end());

                    makeUnification(resolveCpy, newUn);

                    if (backtrack(resolveCpy, newUn)) return true;
                }

                j++;
            }
        }
        i++;
    }

    // Si hemos recorrido todas las cláusulas y no encontramos solución, regresamos false
    return false; // No se encontró solución
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
