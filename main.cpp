#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <cstdlib>
#include <exception>

#include "productManagement.h"
#include "transactionManager.h"
#include "inventoryReports.h"

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        if (std::cin.eof()) {
            std::exit(0);
        }
        std::cout << "Invalid number.\n";
        clearInput();
    }
}

std::string readLine(const std::string& prompt) {
    std::string line;
    std::cout << prompt;
    std::getline(std::cin, line);
    return line;
}

void printProduct(const ProductNode* p) {
    std::cout << "ID: " << p->productId
              << " | Name: " << p->name
              << " | Quantity: " << p->quantity << "\n";
}

int main() {
    Product inventory;
    TransactionManager manager(&inventory);

    const std::string productFile = "products.txt";
    const std::string transactionFile = "transactions.txt";

    if (manager.load(productFile, transactionFile)) {
        std::cout << "Loaded existing data.\n";
    } else {
        std::cout << "Starting with empty inventory.\n";
    }

    int choice;
    do {
        std::cout << "\n=== Inventory Menu ===\n"
                  << "1. Add product\n"
                  << "2. Stock in\n"
                  << "3. Stock out\n"
                  << "4. Update product name\n"
                  << "5. Delete product\n"
                  << "6. Search product\n"
                  << "7. Inventory report\n"
                  << "8. Low-stock report\n"
                  << "9. Transaction history\n"
                  << "10. Sort by ID / Name / Quantity\n"
                  << "11. Save\n"
                  << "0. Exit\n";

        choice = readInt("Choice: ");

        try {
            switch (choice) {
                case 1: {
                    int id = readInt("Product ID: ");
                    std::string name = readLine("Name: ");
                    int qty = readInt("Opening quantity: ");
                    int tid = manager.addProduct(id, name, qty);
                    std::cout << "Added. Transaction ID: " << tid << "\n";
                    break;
                }
                case 2: {
                    int id = readInt("Product ID: ");
                    int qty = readInt("Quantity: ");
                    int tid = manager.stockIn(id, qty);
                    std::cout << "Stocked in. Transaction ID: " << tid << "\n";
                    break;
                }
                case 3: {
                    int id = readInt("Product ID: ");
                    int qty = readInt("Quantity: ");
                    int tid = manager.stockOut(id, qty);
                    std::cout << "Stocked out. Transaction ID: " << tid << "\n";
                    break;
                }
                case 4: {
                    int id = readInt("Product ID: ");
                    std::string name = readLine("New name: ");
                    inventory.update(id, name, 0);
                    std::cout << "Name updated.\n";
                    break;
                }
                case 5: {
                    int id = readInt("Product ID: ");
                    inventory.deleteProduct(id);
                    std::cout << "Product deleted.\n";
                    break;
                }
                case 6: {
                    int by = readInt("Search by: 1 = ID, 2 = Name, 3 = Quantity: ");
                    if (by == 1) {
                        int id = readInt("Product ID: ");
                        printProduct(inventory.searchByProductId(id));
                    } else if (by == 2) {
                        std::string name = readLine("Name: ");
                        std::vector<ProductNode*> found = inventory.searchByName(name);
                        if (found.empty()) std::cout << "No product found.\n";
                        for (ProductNode* p : found) printProduct(p);
                    } else if (by == 3) {
                        int qty = readInt("Quantity: ");
                        std::vector<ProductNode*> found = inventory.searchByQuantity(qty);
                        if (found.empty()) std::cout << "No product found.\n";
                        for (ProductNode* p : found) printProduct(p);
                    } else {
                        std::cout << "Unknown option.\n";
                    }
                    break;
                }
                case 7:
                    printInventoryReport(inventory, 5);
                    break;
                case 8:
                    printLowStockReport(inventory, 5);
                    break;
                case 9: {
                    int order = readInt("1 = oldest first, 2 = newest first: ");
                    printTransactionHistory(manager.getLog(), order == 2);
                    break;
                }
                case 10: {
                    int by = readInt("1 = ID, 2 = Name, 3 = Quantity: ");
                    int dir = readInt("1 = ascending, 2 = descending: ");
                    bool asc = (dir == 1);
                    if (by == 1) manager.sortById(asc);
                    else if (by == 2) manager.sortByName(asc);
                    else manager.sortByQuantity(asc);
                    std::cout << "Sorted.\n";
                    break;
                }
                case 11:
                    manager.save(productFile, transactionFile);
                    std::cout << "Saved.\n";
                    break;
                case 0:
                    manager.save(productFile, transactionFile);
                    std::cout << "Saved and exiting.\n";
                    break;
                default:
                    std::cout << "Unknown option.\n";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    } while (choice != 0);

    return 0;
}