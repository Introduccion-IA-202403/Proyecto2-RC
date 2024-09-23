#include <iostream>
#include <fstream>
#include <vector>
#include <Clause.h>

using namespace std;

int main() {
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

    // Limpiar la memoria
    for (auto & cl : base) delete cl;
};