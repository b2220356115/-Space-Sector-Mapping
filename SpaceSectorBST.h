#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include "Sector.h"
#include "cmath"
#include <algorithm>
#include <stack>

class SpaceSectorBST {
  
public:

    std::stack<Sector*> sectors;
    Sector *root;
    Sector *temp;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
    Sector* insertRecursive(Sector* sector, int x, int y, int z);
    Sector* deleteRecursive(Sector* node, const std::string& sector_code);
    Sector* findMin(Sector* node);
    Sector* findSectorByCode(const std::string& sector_code, Sector* node);
    void inOrderTraversal(Sector* node);
    void preOrderTraversal(Sector* node);
    void postOrderTraversal(Sector* node);

    void transplant(Sector *u, Sector *v);
};

#endif // SPACESECTORBST_H
