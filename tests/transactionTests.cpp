#include <iostream>
#include <cassert>
#include <stdexcept>

#include "productManagement.h"
#include "transactionManager.h"

void normalTests();
void boundaryTests();
void invalidInputTests();

int main() {
    normalTests();
    boundaryTests();
    invalidInputTests();

    std::cout << "\nAll transaction tests passed successfully!" << std::endl;
    return 0;
}

void normalTests() {
    Product inventory;
    inventory.insert(101, "Keyboard", 10);
    TransactionManager manager(&inventory);

    // TN-01: Stock-in increases quantity and is logged
    int id = manager.stockIn(101, 5);
    assert(id == 1);
    assert(inventory.getQuantity(101) == 15);
    assert(manager.getLog().getSize() == 1);

    // TN-02: Stock-out decreases quantity and is logged
    id = manager.stockOut(101, 5);
    assert(id == 2);
    assert(inventory.getQuantity(101) == 10);
    assert(manager.getLog().getSize() == 2);

    // TN-03: Transaction ids are sequential
    id = manager.stockIn(101, 1);
    assert(id == 3);
    assert(manager.getLog().getSize() == 3);

    // TN-04: Log stores correct details in order
    const TransactionNode* first = manager.getLog().getHead();
    assert(first->transactionId == 1);
    assert(first->productId == 101);
    assert(first->type == TransactionType::STOCK_IN);
    assert(first->quantity == 5);

    const TransactionNode* second = first->next;
    assert(second->transactionId == 2);
    assert(second->type == TransactionType::STOCK_OUT);
    assert(second->quantity == 5);

    // TN-05: Tail is the latest transaction and links back correctly
    const TransactionNode* last = manager.getLog().getTail();
    assert(last->transactionId == 3);
    assert(last->prev == second);
    assert(last->next == nullptr);

    std::cout << "Transaction normal tests passed.\n";
}

void boundaryTests() {
    Product inventory;
    inventory.insert(102, "Mouse", 10);
    TransactionManager manager(&inventory);

    // TB-01: Empty log
    assert(manager.getLog().getSize() == 0);
    assert(manager.getLog().getHead() == nullptr);
    assert(manager.getLog().getTail() == nullptr);

    // TB-02: Single transaction (head and tail are the same node)
    manager.stockIn(102, 1);
    assert(manager.getLog().getSize() == 1);
    assert(manager.getLog().getHead() == manager.getLog().getTail());

    // TB-03: Stock-out of the entire quantity (stock becomes zero)
    manager.stockOut(102, 11);
    assert(inventory.getQuantity(102) == 0);
    assert(manager.getLog().getSize() == 2);

    // TB-04: Stock-in on a product with zero stock
    manager.stockIn(102, 5);
    assert(inventory.getQuantity(102) == 5);
    assert(manager.getLog().getSize() == 3);

    // TB-05: Minimum allowed quantity (1)
    manager.stockOut(102, 1);
    assert(inventory.getQuantity(102) == 4);

    std::cout << "Transaction boundary tests passed.\n";
}

void invalidInputTests() {
    Product inventory;
    inventory.insert(101, "Keyboard", 10);
    TransactionManager manager(&inventory);

    // TI-01: Stock-in on a nonexistent product
    bool exceptionThrown = false;
    try {
        manager.stockIn(999, 5);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    assert(manager.getLog().getSize() == 0);

    // TI-02: Stock-out on a nonexistent product
    exceptionThrown = false;
    try {
        manager.stockOut(999, 5);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    assert(manager.getLog().getSize() == 0);

    // TI-03: Stock-in with zero quantity
    exceptionThrown = false;
    try {
        manager.stockIn(101, 0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    assert(inventory.getQuantity(101) == 10);
    assert(manager.getLog().getSize() == 0);

    // TI-04: Stock-in with negative quantity
    exceptionThrown = false;
    try {
        manager.stockIn(101, -5);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    assert(inventory.getQuantity(101) == 10);

    // TI-05: Stock-out with zero quantity
    exceptionThrown = false;
    try {
        manager.stockOut(101, 0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    assert(inventory.getQuantity(101) == 10);

    // TI-06: Stock-out with negative quantity
    exceptionThrown = false;
    try {
        manager.stockOut(101, -5);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    assert(inventory.getQuantity(101) == 10);

    // TI-07: Insufficient stock (stock unchanged, nothing logged)
    exceptionThrown = false;
    try {
        manager.stockOut(101, 15);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
    assert(inventory.getQuantity(101) == 10);
    assert(manager.getLog().getSize() == 0);

    // TI-08: Manager created with a null inventory
    exceptionThrown = false;
    try {
        TransactionManager badManager(nullptr);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

    std::cout << "Transaction invalid-input tests passed.\n";
}
