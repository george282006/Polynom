#ifndef POLYNOMIAL_INTERACTIVE_MENU_H
#define POLYNOMIAL_INTERACTIVE_MENU_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "polynomial.h"

enum StorageType {
    LIST_STORAGE = 0,
    AVL_TREE_STORAGE = 1,
    RED_BLACK_TREE_STORAGE = 2,
    HASH_OPEN_ADDRESS_STORAGE = 3,
    HASH_FORWARD_STORAGE = 4,
    UNORDERED_TAB_STORAGE = 5
};

class PolynomialInteractiveMenu {
private:
    Polynomial currentPolynomial;
    StorageType currentStorageType;

    void displayHeader();
    void displayStorageMenu();
    void displayMainMenu();
    void displayStorageInfo(StorageType type);
    
    std::string getStorageTypeName(StorageType type);
    std::string getStorageTypeDescription(StorageType type);
    
    void selectStorage();
    void addMonom();
    void displayPolynomial();
    void clearPolynomial();
    void getStorageInfo();

public:
    PolynomialInteractiveMenu();
    void run();
};

#endif // POLYNOMIAL_INTERACTIVE_MENU_H
