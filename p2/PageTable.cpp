#include "PageTable.h"
#include <unordered_map>

void PageTable::insert(int numPag) {
    if (!contains(numPag)) {
        table[numPag] = true; // Mark the page as present in memory
    }
}

bool PageTable::contains(int numPag) const {
    return table.find(numPag) != table.end();
}

void PageTable::remove(int numPag) {
    auto it = table.find(numPag);
    if (it != table.end()) {
        table.erase(it); // Remove the page from memory
    }
}