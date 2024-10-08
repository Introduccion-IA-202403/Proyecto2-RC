#include <iostream>
#include <fstream>
#include <vector>
#include <Clause.h>
#include <Solver.h>
#include <LogicTransform.h>

using namespace std;

int main() {
    LogicTransform transform;
    vector<Clause*> base;
    cout << "Ingrese el nombre del archivo con la base: ";
    string filename; cin >> filename;
    ifstream file(filename);
    if (!file) {
        cout << "No se encontro el archivo" << endl;
        return 0;
    }
    string clause;
    while (getline(file, clause)) {
        int i = 0;
        base.push_back(new Clause(clause, i));
        cout << "Clausula " << clause << " añadida" << endl;
    }

    vector< vector< Clause > > normalBase;

    for (auto cl: base) {
        auto trns = transform.transformClause(cl);
        for (auto cls: trns) normalBase.push_back(cls);
    }

    // Crear el solver
    Solver solver(normalBase);

    cout << "Ingrese el predicado que desea probar: ";
    string parse;
    cin.ignore();
    getline(cin, parse);
    int st = 0;


    Clause proof(parse, st);
    proof.setNot(true);
    // Ejecutar la resolución por refutación
    if (solver.resolve({ proof })) {
        cout << "Se ha encontrado una contradicción. El teorema es válido." << endl;
        cout << "El camino usado para encontrar la contradicción es: " << endl;
        for (auto & path : solver.getPath()) {
            for (auto & cl : path) {
                if (cl.isNot()) cout << "NOT ";
                cout << cl.getPredicate() << " ";
            }
            cout << endl;
        }
    } else {
        cout << "No se ha encontrado ninguna contradicción. El teorema no se puede probar." << endl;
    }

    // Limpiar la memoria
    for (auto & cl : base) delete cl;
};