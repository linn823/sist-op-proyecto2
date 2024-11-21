#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <unordered_map>
#include <iostream>

class PageTable {
private:
    std::unordered_map<int, bool> table;

public:
    void insert(int numPag);
    bool contains(int numPag) const;
    void remove(int numPag);
};

#endif // PAGETABLE_H
