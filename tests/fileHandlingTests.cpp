#include <iostream>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <stdexcept>
#include <string>

#include "productManagement.h"
#include "transactionManager.h"

const std::string PRODUCT_FILE = "test_products.txt";
const std::string TRANSACTION_FILE = "test_transactions.txt";

void fileNormalTests();
void fileBoundaryTests();
void fileInvalidTests();

static void removeTestFiles() {
    std::remove(PRODUCT_FILE.c_str());
    std::remove(TRANSACTION_FILE.c_str());
}

static void writeFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    out << content;
}

int main() {
    removeTestFiles();

    fileNormalTests();
    fileBoundaryTests();
    fileInvalidTests();

    removeTestFiles();
    std::cout << "\nAll file handling tests passed successfully!" << std::endl;
    return 0;
}

void fileNormalTests() {
    Product inv1;
    TransactionManager manager1(&inv1);
    manager1.addProduct(101, "Gaming Keyboard", 10);
    manager1.addProduct(102, "Mouse", 20);
    manager1.stockIn(101, 5);
    manager1.stockOut(102, 8);
    manager1.save(PRODUCT_FILE, TRANSACTION_FILE);

    // FN-01: Products are restored after restart (names with spaces included)
    Product inv2;
    TransactionManager manager2(&inv2);
    assert(manager2.load(PRODUCT_FILE, TRANSACTION_FILE));
    assert(inv2.getSize() == 2);
    assert(inv2.getName(101) == "Gaming Keyboard");
    assert(inv2.getQuantity(101) == 15);
    assert(inv2.getName(102) == "Mouse");
    assert(inv2.getQuantity(102) == 12);

    // FN-02: Transaction log is restored with the same details and order
    assert(manager2.getLog().getSize() == 4);
    assert(manager2.getLog().getHead()->transactionId == 1);
    assert(manager2.getLog().getHead()->quantity == 10);
    assert(manager2.getLog().getTail()->transactionId == 4);
    assert(manager2.getLog().getTail()->type == TransactionType::STOCK_OUT);
    assert(manager2.getLog().getTail()->quantity == 8);
    assert(manager2.getLog().getTail()->timestamp ==
           manager1.getLog().getTail()->timestamp);

    // FN-03: New transactions continue the id sequence after loading
    int id = manager2.stockIn(101, 1);
    assert(id == 5);

    // FN-04: Saving again and reloading keeps everything
    manager2.save(PRODUCT_FILE, TRANSACTION_FILE);
    Product inv3;
    TransactionManager manager3(&inv3);
    assert(manager3.load(PRODUCT_FILE, TRANSACTION_FILE));
    assert(inv3.getQuantity(101) == 16);
    assert(manager3.getLog().getSize() == 5);

    std::cout << "File normal tests passed.\n";
}

void fileBoundaryTests() {
    removeTestFiles();

    // FB-01: No saved files (first run) is not an error
    Product inv1;
    TransactionManager manager1(&inv1);
    assert(!manager1.load(PRODUCT_FILE, TRANSACTION_FILE));
    assert(inv1.getSize() == 0);
    assert(manager1.getLog().getSize() == 0);

    // FB-02: Empty inventory and empty log save and load correctly
    manager1.save(PRODUCT_FILE, TRANSACTION_FILE);
    Product inv2;
    TransactionManager manager2(&inv2);
    assert(manager2.load(PRODUCT_FILE, TRANSACTION_FILE));
    assert(inv2.getSize() == 0);
    assert(manager2.getLog().getSize() == 0);

    // FB-03: A product that reached zero stock survives save and load
    Product inv3;
    TransactionManager manager3(&inv3);
    manager3.addProduct(103, "Monitor", 5);
    manager3.stockOut(103, 5);
    assert(inv3.getQuantity(103) == 0);
    manager3.save(PRODUCT_FILE, TRANSACTION_FILE);

    Product inv4;
    TransactionManager manager4(&inv4);
    assert(manager4.load(PRODUCT_FILE, TRANSACTION_FILE));
    assert(inv4.contains(103));
    assert(inv4.getQuantity(103) == 0);
    assert(manager4.getLog().getSize() == 2);

    std::cout << "File boundary tests passed.\n";
}

void fileInvalidTests() {
    // FI-01: Corrupted product file
    removeTestFiles();
    writeFile(PRODUCT_FILE, "abc\n");
    Product inv1;
    TransactionManager manager1(&inv1);
    bool exceptionThrown = false;
    try {
        manager1.load(PRODUCT_FILE, TRANSACTION_FILE);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // FI-02: Duplicate product ids in the product file
    removeTestFiles();
    writeFile(PRODUCT_FILE, "101|10|Keyboard\n101|5|Mouse\n");
    Product inv2;
    TransactionManager manager2(&inv2);
    exceptionThrown = false;
    try {
        manager2.load(PRODUCT_FILE, TRANSACTION_FILE);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // FI-03: Transaction ids not in sequence
    removeTestFiles();
    writeFile(PRODUCT_FILE, "101|10|Keyboard\n");
    writeFile(TRANSACTION_FILE, "2|101|IN|10|1700000000\n");
    Product inv3;
    TransactionManager manager3(&inv3);
    exceptionThrown = false;
    try {
        manager3.load(PRODUCT_FILE, TRANSACTION_FILE);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // FI-04: Unknown transaction type
    removeTestFiles();
    writeFile(PRODUCT_FILE, "101|10|Keyboard\n");
    writeFile(TRANSACTION_FILE, "1|101|XX|10|1700000000\n");
    Product inv4;
    TransactionManager manager4(&inv4);
    exceptionThrown = false;
    try {
        manager4.load(PRODUCT_FILE, TRANSACTION_FILE);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // FI-05: Transaction line with a missing field
    removeTestFiles();
    writeFile(PRODUCT_FILE, "101|10|Keyboard\n");
    writeFile(TRANSACTION_FILE, "1|101|IN\n");
    Product inv5;
    TransactionManager manager5(&inv5);
    exceptionThrown = false;
    try {
        manager5.load(PRODUCT_FILE, TRANSACTION_FILE);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    // FI-06: Negative quantity inside the product file
    removeTestFiles();
    writeFile(PRODUCT_FILE, "101|-10|Keyboard\n");
    Product inv6;
    TransactionManager manager6(&inv6);
    exceptionThrown = false;
    try {
        manager6.load(PRODUCT_FILE, TRANSACTION_FILE);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "File invalid-input tests passed.\n";
}
