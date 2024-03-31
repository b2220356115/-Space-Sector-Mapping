#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>
#include "cmath"
#include <algorithm>
#include <stack>


class SpaceSectorLLRBT {
public:
    std::stack <Sector*> sectors;
    Sector* root;
    Sector* temp;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector *insertRecursive(Sector* node, Sector* parent, int x, int y, int z);

    bool isRed(Sector *node);

    Sector *rotateLeft(Sector *h);

    Sector *rotateRight(Sector *h);

    void flipColors(Sector* h);

    void printSector(Sector *node);

    void postOrderTraversal(Sector *node);

    void preOrderTraversal(Sector *node);

    void inOrderTraversal(Sector *node);

    Sector *findSectorByCode(const std::string &sector_code, Sector *node);

    void tracePathToRoot(Sector *node, std::vector<Sector *> &path);


    void deleteTree(Sector *pSector);


    Sector *fixViolations(Sector *pSector);
};

#endif // SPACESECTORLLRBT_H
