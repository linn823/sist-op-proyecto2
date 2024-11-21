#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <deque>
#include <string>
#include <unordered_map>
#include <algorithm>
#include "PageTable.h"

using namespace std;

void parseArguments(int argc, char *argv[], int &numMarcos, string &algo, string &filename) {
    int i = 1;
    while (i < argc) {
        string arg = argv[i];
        if (arg == "-m" && i + 1 < argc) {
            numMarcos = stoi(argv[++i]);
        } else if (arg == "-a" && i + 1 < argc) {
            algo = argv[++i];
        } else if (arg == "-f" && i + 1 < argc) {
            filename = argv[++i];
        }else{
            printf("Error en los argumentos\n");
            exit(1);
        }
        i++;
    }
}

vector<int> cargarReferencias(const string &filename) {
    vector<int> references;
    ifstream file(filename);
    if (!file.is_open()) {
        printf("No se pudo abrir archivo references\n");
        exit(1);
    }
    int pag;
    while (file >> pag) {
        references.push_back(pag);
    }
    return references;
}

// Algoritmo optimal
int optimalPageReplacement(PageTable &pageTable, vector<int> &references, int numMarcos) {
    int fallos = 0;
    vector<int> marcos;
    for (int i = 0; i < references.size(); i++) {
        int pag = references[i];

        if (!pageTable.contains(pag)) {
            if (marcos.size() < numMarcos) {
                marcos.push_back(pag);
            } else {
                int lejano = -1;
                int replacePage = -1;
                for (int j = 0; j < marcos.size(); j++) {
                    int ultimoUsado = -1;
                    for (int k = i + 1; k < references.size(); k++) {
                        if (references[k] == marcos[j]) {
                            ultimoUsado = k;
                            break;
                        }
                    }
                    if (ultimoUsado == -1) {
                        replacePage = marcos[j];
                        break;
                    }
                    if (ultimoUsado > lejano) {
                        lejano = ultimoUsado;
                        replacePage = marcos[j];
                    }
                }
                pageTable.remove(replacePage);
                auto it = find(marcos.begin(), marcos.end(), replacePage);
                if (it != marcos.end()) marcos.erase(it);
                marcos.push_back(pag);
            }
            pageTable.insert(pag);
            fallos++;
        }
    }
    return fallos;
}

// Algoritmo FIFO
int fifoPageReplacement(PageTable &pageTable, vector<int> &references, int numMarcos) {
    int fallos = 0;
    deque<int> marcos;
    for (int i = 0; i < references.size(); i++) {
        int pag = references[i];

        if (!pageTable.contains(pag)) {
            if (marcos.size() < numMarcos) {
                marcos.push_back(pag);
            } else {
                int toRemove = marcos.front();
                marcos.pop_front();
                pageTable.remove(toRemove);
                marcos.push_back(pag);
            }
            pageTable.insert(pag);
            fallos++;
        }
    }
    return fallos;
}

int lruPageReplacement(PageTable &pageTable, vector<int> &references, int numMarcos) {
    int fallos = 0;
    list<int> marcos;
    for (int i = 0; i < references.size(); i++) {
        int pag = references[i];

        auto it = find(marcos.begin(), marcos.end(), pag);
        if (!pageTable.contains(pag)) {
            if (marcos.size() < numMarcos) {
                marcos.push_front(pag);
            } else {
                int toRemove = marcos.back();
                marcos.pop_back();
                pageTable.remove(toRemove);
                marcos.push_front(pag);
            }
            pageTable.insert(pag);
            fallos++;
        } else {
            marcos.erase(it);
            marcos.push_front(pag);
        }
    }
    return fallos;
}

int clockPageReplacement(PageTable &pageTable, vector<int> &references, int numMarcos) {
    int fallos = 0;
    vector<int> marcos(numMarcos, -1);
    vector<bool> referenced(numMarcos, false);
    int clockHand = 0;

    for (int pag : references) {
        if (!pageTable.contains(pag)) {
            while (referenced[clockHand]) {
                referenced[clockHand] = false;
                clockHand = (clockHand + 1) % numMarcos;
            }
            if (marcos[clockHand] != -1) {
                pageTable.remove(marcos[clockHand]);
            }
            marcos[clockHand] = pag;
            referenced[clockHand] = true;
            pageTable.insert(pag);
            clockHand = (clockHand + 1) % numMarcos;
            fallos++;
        } else {
            for (int j = 0; j < marcos.size(); j++) {
                if (marcos[j] == pag) {
                    referenced[j] = true;
                    break;
                }
            }
        }
    }
    return fallos;
}

int main(int argc, char *argv[]) {
    int numMarcos = 0;
    string algo, filename;
    parseArguments(argc, argv, numMarcos, algo, filename);

    vector<int> references = cargarReferencias(filename);
    PageTable pageTable;

    int fallos = 0;
    if (algo == "FIFO") {
        fallos = fifoPageReplacement(pageTable, references, numMarcos);
    } else if (algo == "Optimal") {
        fallos = optimalPageReplacement(pageTable, references, numMarcos);
    } else if (algo == "LRU") {
        fallos = lruPageReplacement(pageTable, references, numMarcos);
    } else if (algo == "LRUClock") {
        fallos = clockPageReplacement(pageTable, references, numMarcos);
    } else {
        printf("Error: algoritmo invalido");
        exit(1);
    }

    cout << "Fallos de pagina: " << fallos << endl;
    return 0;
}