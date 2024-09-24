#include <iostream>
#include <fstream>
#include <vector>
#include <Clause.h>
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


    // Limpiar la memoria
    for (auto & cl : base) delete cl;
};