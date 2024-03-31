#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    // Open the input file
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;

    getline(inputFile, line);

    // Read the file line by line
    while (getline(inputFile, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Create a stringstream to parse the coordinates
        stringstream ss(line);
        string x_str, y_str, z_str;

        // Read the coordinates
        if (getline(ss, x_str, ',') && getline(ss, y_str, ',') && getline(ss, z_str, ',')) {
            try {
                // Convert the coordinates to integers
                int x = stoi(x_str);
                int y = stoi(y_str);
                int z = stoi(z_str);

                // Insert the sector into the LLRBT sector map
                insertSectorByCoordinates(x, y, z);

            } catch (const invalid_argument& ia) {
                cerr << "Invalid argument: " << ia.what() << std::endl;
            }
        }
    }

    // Close the input file
    inputFile.close();

}

// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.
    while (!sectors.empty()) {
        delete(sectors.top());
        sectors.pop();
    }

}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map
    // according to the coordinates-based comparison criteria.
    /*
    Sector* newSector = new Sector(x, y, z);
    sectors.push(newSector);
    newSector->left = nullptr;
    newSector->right = nullptr;
    newSector->parent = nullptr;
    newSector->color = true;
    temp = root;

    Sector* current = insertRecursive(newSector);
    SpaceSectorLLRBT::root->color=BLACK;

    current->distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    current->sector_code = std::to_string(static_cast<int>(current->distance_from_earth)); // Distance component

    current->sector_code += (x == 0) ? "S" : (x > 0) ? "R" : "L";
    current->sector_code += (y == 0) ? "S" : (y > 0) ? "U" : "D";
    current->sector_code += (z == 0) ? "S" : (z > 0) ? "F" : "B";
     */
    root = insertRecursive(root, nullptr, x, y, z);
    root->color = false;





}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector LLRBT map in-order and print the sectors
    // to STDOUT in the given format.
    cout << endl;
    cout << "Space sectors inorder traversal:" << endl;
    inOrderTraversal(root);

}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector LLRBT map in pre-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << endl;
    cout << "Space sectors preorder traversal:" << endl;
    preOrderTraversal(root);
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector LLRBT map in post-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << endl;
    cout << "Space sectors postorder traversal:" << endl;
    postOrderTraversal(root);
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    Sector* destinationSector = findSectorByCode(sector_code, root);

    // If the destination sector is found, trace the path
    if (destinationSector != nullptr) {
        tracePathToRoot(destinationSector, path);
        std::reverse(path.begin(), path.end()); // Reverse the path to start from Earth
        printStellarPath(path); // Optionally print the path
    } else {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
    }

    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.
    std::cout << "The stellar path to Dr. Elara: ";
    for (const auto& sector : path) {
        std::cout << sector->sector_code;
        if (sector != path.back()) {
            std::cout << "->";
        }
    }
    std::cout << std::endl;
}




Sector* SpaceSectorLLRBT::insertRecursive(Sector* node, Sector* parentNode, int x, int y, int z) {
    if (node == nullptr) {
        Sector* newSector = new Sector(x, y, z);
        sectors.push(newSector);
        newSector->parent = parentNode;
        newSector->distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

        newSector->sector_code = std::to_string(static_cast<int>(newSector->distance_from_earth)); // Distance component

        newSector->sector_code += (x == 0) ? "S" : (x > 0) ? "R" : "L";
        newSector->sector_code += (y == 0) ? "S" : (y > 0) ? "U" : "D";
        newSector->sector_code += (z == 0) ? "S" : (z > 0) ? "F" : "B";
        return newSector;
    }

    // compare coordinates
    if (x < node->x || (x == node->x && y < node->y) || (x == node->x && y == node->y && z < node->z)) {
        node->left = insertRecursive(node->left,node, x, y, z);
    } else {
        node->right = insertRecursive(node->right,node, x, y, z);
    }

    // fixing violations
    node = fixViolations(node);

    return node;
}


// Helper function to check if a node is red (assuming nullptr nodes are black)
bool SpaceSectorLLRBT::isRed(Sector* node) {
    if (node == NULL)
        return BLACK; // Black

    return (node -> color == RED);
}

// Helper function for left-leaning rotation
Sector* SpaceSectorLLRBT::rotateLeft(Sector* h) {
    Sector* x = h->right;
    Sector* y = x->left;
    x->left = h;
    h->right = y;
    if (root == h) {
        root = x;
        x->parent = nullptr;
    }
    x->parent = h->parent;
    h->parent = x;
    if (y != nullptr) {y->parent = h;}
    x->color = h->color;
    h->color = 1; // Red
    return x;
}

// Helper function for right rotation
Sector* SpaceSectorLLRBT::rotateRight(Sector* h) {
    Sector* x = h->left;
    Sector* y = x->right;
    x->right = h;
    h->left = y;

    if (root == h) {
        root = x;
        x->parent = nullptr;
    }

    x->parent = h->parent;
    if (y != nullptr) {y->parent = h;}
    h->parent = x;
    x->color = h->color;
    h->color = 1; // Red
    return x;
}

// Helper function for color flip
void SpaceSectorLLRBT::flipColors(Sector* h) {
    h->left->color = BLACK;
    h->right->color = BLACK;
    h->color = RED;
}

// Helper function for in-order traversal
void SpaceSectorLLRBT::inOrderTraversal(Sector* node) {
    if (node != nullptr) {
        inOrderTraversal(node->left);
        printSector(node);
        inOrderTraversal(node->right);
    }
}

// Helper function for pre-order traversal
void SpaceSectorLLRBT::preOrderTraversal(Sector* node) {
    if (node != nullptr) {
        printSector(node);
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
}

// Helper function for post-order traversal
void SpaceSectorLLRBT::postOrderTraversal(Sector* node) {
    if (node != nullptr) {
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        printSector(node);
    }
}

// Helper function to print the sector in the specified format
void SpaceSectorLLRBT::printSector(Sector* node) {
    std::cout << (isRed(node) ? "RED" : "BLACK") << " sector: " << node->sector_code << std::endl;
}

Sector* SpaceSectorLLRBT::findSectorByCode(const std::string& sector_code, Sector* node) {
    if (node != nullptr) {
        if (node->sector_code == sector_code) {
            return node;
        }
        Sector* leftResult = findSectorByCode(sector_code, node->left);
        if (leftResult != nullptr) {
            return leftResult;
        }
        return findSectorByCode(sector_code, node->right);
    }
    return nullptr;
}

// Helper function to trace the path from a sector to the root
void SpaceSectorLLRBT::tracePathToRoot(Sector* node, std::vector<Sector*>& path) {
    while (node != nullptr) {
        path.push_back(node);
        node = node->parent;
    }
}

Sector *SpaceSectorLLRBT::fixViolations(Sector* node) {
    if (node->right != nullptr && node->right->color == RED) {
        node = rotateLeft(node);
    }

    if (node->left != nullptr && node->left->left != nullptr && node->left->color == RED && node->left->left->color == RED) {
        node = rotateRight(node);
    }

    if (node->left != nullptr && node->left->color == RED && node->right != nullptr && node->right->color == RED) {
        node->color = true;
        node->left->color = false;
        node->right->color = false;
    }

    if (node->left != nullptr) {
        node->left->parent = node;
    }

    if (node->right != nullptr) {
        node->right->parent = node;
    }

    return node;
}
