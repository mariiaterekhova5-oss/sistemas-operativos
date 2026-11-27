#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
using namespace std;
int const MEMORIA_FISICA = 2000;
int const MINIMA_ASIGN = 100;
struct Proceso
{
    string nombre;
    int instante;
    int memoria_requerida;
    int tiempo;
};
struct Particion
{
    int direccion;
    int tamanio;
    string estado;
    int tiempo_fn;
};

vector<Proceso> LeerFichero(string nombre_fichero)
{
    vector<Proceso> list;
    Proceso p;
    ifstream archivo(nombre_fichero);
    while (archivo >> p.nombre >> p.instante >> p.memoria_requerida >> p.tiempo)
    {
        p.memoria_requerida = ((p.memoria_requerida + 99) / 100) * 100;
        list.push_back(p);
    }
    archivo.close();
    return list;
}

int menu()
{
    int opcion;
    cout << "Elige un numero" << endl;
    cout << "1.Mejor Hueco" << endl;
    cout << "2.Peor Hueco" << endl;
    cin >> opcion;
    return opcion;
}

void escribirEstado(ofstream &salida, int instante, vector<Particion> &memoria)
{
    salida << instante;
    for (int i = 0; i < memoria.size(); i++)
    {
        salida << "[" << memoria[i].direccion << " " << memoria[i].estado << " " << memoria[i].tamanio << "]";
    }
    salida << endl;
}

int main()
{
    vector<Proceso> proceso = LeerFichero("SO.txt");
    vector<Particion> memoria;
    memoria.push_back({0, MEMORIA_FISICA, "hueco", -1});

    int opcion = menu();

    switch (opcion)
    {
    case 1:

        break;
    case 2:

        break;
    default:

        break;
    }
    return 0;
}