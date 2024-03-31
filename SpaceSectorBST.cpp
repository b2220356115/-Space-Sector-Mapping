#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    while (!sectors.empty()){
        delete sectors.top();
        sectors.pop();
    }
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.

    // Open the input file
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    string line;

    getline(inputFile, line);

    // Read the file line by line
    while (getline(inputFile, line)) {
        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        //Create a stringstream to parse the coordinates
        stringstream ss(line);
        string x_str, y_str, z_str;

        // Read the coordinates
        if (getline(ss, x_str, ',') && getline(ss, y_str, ',') && getline(ss, z_str, ',')) {
            try{
                // Convert the coordinates to integers
                int x = stoi(x_str);
                int y = stoi(y_str);
                int z = stoi(z_str);

                // Insert the sector into the BST sector map
                insertSectorByCoordinates(x, y, z);

            } catch (const std::invalid_argument& ia) {
                cerr << "Invalid argument: " << ia.what() << endl;
            }



        }


    }

    // Close the input file
    inputFile.close();
}
void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the
    // coordinates-based comparison criteria.

    Sector* newSector = new Sector(x, y, z);

    sectors.push(newSector);
    temp = root;
    Sector* current = insertRecursive(newSector, x, y, z);
    current->distance_from_earth = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    current->sector_code = std::to_string(static_cast<int>(current->distance_from_earth)); // Distance component

// Coordinate components
    current->sector_code += (x == 0) ? "S" : (x > 0) ? "R" : "L";
    current->sector_code += (y == 0) ? "S" : (y > 0) ? "U" : "D";
    current->sector_code += (z == 0) ? "S" : (z > 0) ? "F" : "B";


}


void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors
    // to STDOUT in the given format.
    cout << "Space sectors inorder traversal: " << endl;
    inOrderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors preorder traversal: " << endl;
    preOrderTraversal(root);
    std::cout << std::endl;
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print
    // the sectors to STDOUT in the given format.
    cout << "Space sectors postorder traversal: " << endl;
    postOrderTraversal(root);
    std::cout << std::endl;
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    Sector* destinationSector = findSectorByCode(sector_code, root);

    // Check if the destination sector exists in the BST
    if (destinationSector == nullptr) {
        return path;  // Return an empty path if the destination sector is not found
    }


    while (destinationSector != nullptr){
        path.push_back(destinationSector);
        destinationSector = destinationSector->parent;
    }
    reverse(path.begin(), path.end());
    return path;



    /*
    // Start from the Earth's sector and find the path to the destination sector
    Sector* currentSector = root;
    while (currentSector != nullptr) {
        path.push_back(currentSector);

        // Check if the current sector is the destination sector
        if (currentSector->sector_code == sector_code) {
            path.push_back(currentSector);
            break;  // Break the loop if the destination sector is reached
        }

        // Move to the left or right child based on the comparison of sector codes
        if (destinationSector->total < currentSector->total) {
            currentSector = currentSector->left;
        } else {
            currentSector = currentSector->right;
        }
    }

    return path;
    */
}

void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {

    if (path.empty()) {
        std::cout << "A path to Dr. Elara could not be found." << std::endl;
        return;
    }
    std::cout << "The stellar path to Dr. Elara: ";

    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i]->sector_code;

        // Add "->" if it's not the last sector in the path
        if (i < path.size() - 1) {
            std::cout << "->";
        }
    }

    std::cout << std::endl;
}

Sector* SpaceSectorBST::insertRecursive(Sector* newSector, int x, int y, int z) {
    if (root == nullptr) {
        root = newSector;
        return newSector;
    }


    if ((newSector->x < temp->x) ||
    (newSector->x == temp->x && newSector->y < temp->y) ||
    (newSector->x == temp->x && newSector->y == temp->y && newSector->z < temp->z)
    )
    {
        if (temp->left == nullptr){
            temp->left = newSector;
            newSector->parent = temp;
            temp = root;
            return newSector;
        }
        temp = temp->left;
        insertRecursive(newSector, x, y, z);

    }
    else if ((newSector->x > temp->x) ||
        (newSector->x == temp->x && newSector->y > temp->y) ||
        (newSector->x == temp->x && newSector->y == temp->y && newSector->z > temp->z)
            )
    {
        if (temp->right == nullptr){
            temp->right = newSector;
            newSector->parent = temp;
            temp = root;
            return newSector;
        }
        temp = temp->right;
        insertRecursive(newSector, x, y, z);
    }
    else{
        return newSector;
    }
    return newSector;
}



Sector* SpaceSectorBST::findSectorByCode(const string &sector_code, Sector* node) {
/*
    if (node != nullptr) {
        if (node->sector_code == sector_code) {
            return node;
        }
        findSectorByCode(sector_code, node->left);
        findSectorByCode(sector_code, node->right);
    }
    */

///////

    if (node != nullptr) {
        if (node->sector_code == sector_code) {
            return node;
        }

        // Recursively search in the left subtree
        Sector* leftResult = findSectorByCode(sector_code, node->left);
        if (leftResult != nullptr) {
            return leftResult;  // If found in the left subtree, return immediately
        }

        // Recursively search in the right subtree
        Sector* rightResult = findSectorByCode(sector_code, node->right);
        return rightResult;  // Whether found or not, return the result from the right subtree
    }

    return nullptr;  // Return nullptr if the node is nullptr (not found)

}



Sector* SpaceSectorBST::findMin(Sector* node) {
    if (node->left != nullptr) {
        return findMin(node->left);
    }
    return node;
}


void SpaceSectorBST::inOrderTraversal(Sector* node) {
    if (node != nullptr) {
        inOrderTraversal(node->left);
        std::cout << node->sector_code << " " << endl;
        inOrderTraversal(node->right);
    }
}

void SpaceSectorBST::preOrderTraversal(Sector* node) {
    if (node != nullptr) {
        std::cout << node->sector_code << " " << endl;
        preOrderTraversal(node->left);
        preOrderTraversal(node->right);
    }
}

void SpaceSectorBST::postOrderTraversal(Sector* node) {
    if (node != nullptr) {
        postOrderTraversal(node->left);
        postOrderTraversal(node->right);
        std::cout << node->sector_code << " " << endl;
    }
}

// Helper function to recursively delete a node with a specific sector_code
/*Sector* SpaceSectorBST::deleteRecursive(Sector *node, const std::string &sector_code) {}(Sector* root, const std::string& sector_code) {
    // Base case: If the tree is empty or the node is not found
    if (root == nullptr) {
        return root;
    }

    // Recur down the tree
    if (sector_code < root->sector_code) {
        root->left = deleteRecursive(root->left, sector_code);
    } else if (sector_code > root->sector_code) {
        root->right = deleteRecursive(root->right, sector_code);
    } else {
        // Node with only one child or no child
        if (root->left == nullptr) {
            Sector* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Sector* temp = root->left;
            delete root;
            return temp;
        }

        // Node with two children: Get the in-order successor (smallest in the right subtree)
        Sector* temp = findMin(root->right);

        // Copy the in-order successor's content to this node
        root->sector_code = temp->sector_code;

        // Delete the in-order successor
        root->right = deleteRecursive(root->right, temp->sector_code);
    }

    return root;
}

*/

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.

    Sector* node = findSectorByCode(sector_code, root);
    if (node == nullptr) {
        return;
    }

    if (node->left == nullptr && node->right == nullptr) {
        if (node->parent != nullptr) {
            if (node->parent->left == node) {
                node->parent->left = nullptr;
            } else {
                node->parent->right = nullptr;
            }
        } else {
            root = nullptr; // Update the root if the deleted node is the root
        }
      //  delete node;
    } else if (node->left == nullptr) {
        transplant(node, node->right);
    } else if (node->right == nullptr) {
        transplant(node, node->left);
    } else {
        Sector* min = findMin(node->right);
        if (min->parent != node) {
            transplant(min, min->right);
            min->right = node->right;
            min->right->parent = min;
        }
        transplant(node, min);
        min->left = node->left;
        min->left->parent = min;
       // delete node;
    }
}

// Helper function to replace one subtree with another
void SpaceSectorBST::transplant(Sector* u, Sector* v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}


