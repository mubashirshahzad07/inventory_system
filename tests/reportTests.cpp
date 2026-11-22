#include <iostream>
#include <cassert>
#include <sstream>
#include <string>

#include "productManagement.h"
#include "transactionManager.h"
#include "inventoryReports.h"

void reportNormalTests();
void reportBoundaryTests();

int main() {
    reportNormalTests();
    reportBoundaryTests();

    std::cout << "\nAll report tests passed successfully!" << std::endl;
    return 0;
}

void reportNormalTests() {
    Product inventory;
    TransactionManager manager(&inventory);
    manager.addProduct(101, "Keyboard", 15);
    manager.addProduct(102, "Mouse", 3);

    // RN-01: Inventory report lists products and flags low stock
    std::ostringstream captured;
    std::streambuf* oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printInventoryReport(inventory, 5);
    std::cout.rdbuf(oldBuffer);

    std::string text = captured.str();
    assert(text.find("Keyboard") != std::string::npos);
    assert(text.find("Mouse") != std::string::npos);
    assert(text.find("LOW STOCK") != std::string::npos);
    assert(text.find("Low-stock products (below 5): 1") != std::string::npos);

    // RN-02: Low-stock report shows only products below the threshold
    captured.str("");
    oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printLowStockReport(inventory, 5);
    std::cout.rdbuf(oldBuffer);

    text = captured.str();
    assert(text.find("Mouse") != std::string::npos);
    assert(text.find("Keyboard") == std::string::npos);

    // RN-03: Transaction history lists logged transactions
    captured.str("");
    oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printTransactionHistory(manager.getLog());
    std::cout.rdbuf(oldBuffer);

    text = captured.str();
    assert(text.find("STOCK_IN") != std::string::npos);
    assert(text.find("101") != std::string::npos);

    // RN-04: Newest-first order puts the latest transaction first
    captured.str("");
    oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printTransactionHistory(manager.getLog(), true);
    std::cout.rdbuf(oldBuffer);

    text = captured.str();
    assert(text.find("102") < text.find("101"));

    std::cout << "Report normal tests passed.\n";
}

void reportBoundaryTests() {
    Product emptyInventory;
    TransactionManager emptyManager(&emptyInventory);

    // RB-01: Empty inventory is reported clearly
    std::ostringstream captured;
    std::streambuf* oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printInventoryReport(emptyInventory, 5);
    std::cout.rdbuf(oldBuffer);
    assert(captured.str().find("No products in inventory.") != std::string::npos);

    // RB-02: Empty low-stock report
    captured.str("");
    oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printLowStockReport(emptyInventory, 5);
    std::cout.rdbuf(oldBuffer);
    assert(captured.str().find("No low-stock products.") != std::string::npos);

    // RB-03: Empty transaction history
    captured.str("");
    oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printTransactionHistory(emptyManager.getLog());
    std::cout.rdbuf(oldBuffer);
    assert(captured.str().find("No transactions recorded.") != std::string::npos);

    // RB-04: Product exactly at the threshold is NOT low stock
    Product inventory;
    TransactionManager manager(&inventory);
    manager.addProduct(101, "Keyboard", 5);

    captured.str("");
    oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printLowStockReport(inventory, 5);
    std::cout.rdbuf(oldBuffer);
    assert(captured.str().find("No low-stock products.") != std::string::npos);

    // RB-05: Product at zero stock is low stock
    manager.stockOut(101, 5);
    captured.str("");
    oldBuffer = std::cout.rdbuf(captured.rdbuf());
    printLowStockReport(inventory, 5);
    std::cout.rdbuf(oldBuffer);
    assert(captured.str().find("Keyboard") != std::string::npos);

    std::cout << "Report boundary tests passed.\n";
}
