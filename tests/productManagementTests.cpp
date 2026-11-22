#include <vector>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include "productManagement.h"

void normalTests();
void boundaryTests();
void invalidInputTests();
void sortNormalTests();
void sortBoundaryTests();
void sortInvalidInputTests();
void searchNormalTests();
void searchBoundaryTests();
void searchInvalidInputTests();

int main() {
    normalTests();
    boundaryTests();
    invalidInputTests();

    sortNormalTests();
    sortBoundaryTests();
    sortInvalidInputTests();

    searchNormalTests();
    searchBoundaryTests();
    searchInvalidInputTests();

    std::cout << "\nAll tests passed successfully!" << std::endl;

    return 0;
}

void normalTests() {
    Product inventory;

    // N-01
    inventory.insert(101, "Keyboard", 10);

    assert(inventory.contains(101));
    assert(inventory.getSize() == 1);

    // N-02
    inventory.insert(102, "Mouse", 20);

    assert(inventory.getSize() == 2);

    // N-03
    assert(inventory.contains(101));

    // N-04
    inventory.update(101, "Gaming Keyboard", 5);

    assert(inventory.getName(101) == "Gaming Keyboard");
    assert(inventory.getQuantity(101) == 15);

    // N-05
    inventory.update(102, "Mouse", -5);

    assert(inventory.getQuantity(102) == 15);

    // N-06
    inventory.deleteProduct(101);

    assert(!inventory.contains(101));
    assert(inventory.getSize() == 1);

    // N-07
    assert(!inventory.contains(101));

    std::cout << "Normal tests passed.\n";
}


void boundaryTests() {
    Product inventory;

    inventory.insert(101, "Monitor", 1);

    // B-01: Reduce quantity to zero
    inventory.insert(102, "Keyboard", 10);
    inventory.update(102, "Keyboard", -10);

    assert(inventory.getQuantity(102) == 0);

    // B-02: Delete the only product
    inventory.deleteProduct(101);
    inventory.deleteProduct(102);

    assert(inventory.getSize() == 0);

    // B-03: Search empty list
    assert(!inventory.contains(101));

    // B-04: Insert into empty list
    inventory.insert(103, "Mouse", 5);

    assert(inventory.getSize() == 1);
    assert(inventory.contains(103));

    std::cout << "Boundary tests passed.\n";
}


void invalidInputTests() {
    Product inventory;

    inventory.insert(101, "Keyboard", 10);

    // I-01: Duplicate ID
    bool exceptionThrown = false;
    try {
        inventory.insert(101, "Mouse", 5);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    assert(inventory.getSize() == 1);
    assert(inventory.getName(101) == "Keyboard");

    // I-02: Negative ID
    exceptionThrown = false;

    try {
        inventory.insert(-1, "Monitor", 5);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    assert(inventory.getSize() == 1);
    assert(inventory.contains(101));

    // I-03: Empty name
    exceptionThrown = false;

    try {
        inventory.insert(102, "", 5);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    exceptionThrown = false;

    try {
        inventory.insert(102, " ", 5);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // I-04: Negative quantity
    exceptionThrown = false;

    try {
        inventory.insert(103, "Mouse", -5);
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // I-05: Update nonexistent product
    exceptionThrown = false;

    try {
        inventory.update(999, "Unknown", 5);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
    assert(inventory.getSize() == 1);

    // I-06 Update with blank new name
    exceptionThrown = false;

    try {
        inventory.update(999, " ", 5);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    // I-07: Delete nonexistent product
    exceptionThrown = false;

    try {
        inventory.deleteProduct(999);
    } catch (std::runtime_error&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    assert(inventory.getSize() == 1);

    // I-08: Insufficient quantity
    exceptionThrown = false;

    try {
        inventory.update(101, "Keyboard", -15);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    assert(inventory.getQuantity(101) == 10);

    // I-09: Search nonexistent product
    assert(!inventory.contains(999));

    // I-10: Zero quantity
    exceptionThrown = false;

    try {
        inventory.insert(192, "Monitor", 0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);

    std::cout << "Invalid-input tests passed.\n";
}

static void assertOrder(Product& inv, const std::vector<int>& expected) {
    assert(inv.getSize() == expected.size());

    ProductNode* curr = inv.getHead();
    ProductNode* last = nullptr;

    for (std::size_t i = 0; i < expected.size(); ++i) {
        assert(curr != nullptr);
        assert(curr->productId == expected[i]);
        assert(curr->prev == last);
        last = curr;
        curr = curr->next;
    }

    assert(curr == nullptr);
    assert(inv.getTail() == last);

    if (last) {
        assert(last->next == nullptr);
        assert(inv.getHead()->prev == nullptr);
    }
}

void sortNormalTests() {
    // Insertion order: 103, 101, 102 (every sort below gives a different order)
    Product inventory;
    inventory.insert(103, "apple", 20);
    inventory.insert(101, "banana", 30);
    inventory.insert(102, "Cherry", 10);

    // SN-01: Sort by id ascending
    inventory.sortById(true);
    assertOrder(inventory, {101, 102, 103});

    // SN-02: Sort by id descending
    inventory.sortById(false);
    assertOrder(inventory, {103, 102, 101});

    // SN-03: Sort by quantity ascending
    inventory.sortByQuantity(true);
    assertOrder(inventory, {102, 103, 101});

    // SN-04: Sort by quantity descending
    inventory.sortByQuantity(false);
    assertOrder(inventory, {101, 103, 102});

    // SN-05: Sort by name ascending (case-insensitive: apple, banana, Cherry)
    inventory.sortByName(true);
    assertOrder(inventory, {103, 101, 102});

    // SN-06: Sort by name descending
    inventory.sortByName(false);
    assertOrder(inventory, {102, 101, 103});

    // SN-07: Each product keeps its own name and quantity after sorting
    assert(inventory.getName(101) == "banana");
    assert(inventory.getQuantity(101) == 30);
    assert(inventory.getName(102) == "Cherry");
    assert(inventory.getQuantity(102) == 10);
    assert(inventory.getName(103) == "apple");
    assert(inventory.getQuantity(103) == 20);

    // SN-08: Default argument sorts ascending
    inventory.sortById();
    assertOrder(inventory, {101, 102, 103});

    std::cout << "Sorting normal tests passed.\n";
}

void sortBoundaryTests() {
    // SB-01: Sorting an empty list does nothing and does not crash
    {
        Product inventory;
        inventory.sortById();
        inventory.sortByName();
        inventory.sortByQuantity(false);
        assertOrder(inventory, {});
        assert(inventory.getHead() == nullptr);
        assert(inventory.getTail() == nullptr);
    }

    // SB-02: Single product (head and tail are the same node)
    {
        Product inventory;
        inventory.insert(1, "Solo", 5);
        inventory.sortById(false);
        inventory.sortByName(true);
        inventory.sortByQuantity(false);
        assertOrder(inventory, {1});
        assert(inventory.getHead() == inventory.getTail());
    }

    // SB-03: Two products in reverse order
    {
        Product inventory;
        inventory.insert(2, "B", 5);
        inventory.insert(1, "A", 5);
        inventory.sortById(true);
        assertOrder(inventory, {1, 2});
        assert(inventory.getHead() != inventory.getTail());
        assert(inventory.getHead()->productId == 1);
        assert(inventory.getTail()->productId == 2);
    }

    // SB-04: Already sorted list stays the same
    {
        Product inventory;
        inventory.insert(1, "A", 1);
        inventory.insert(2, "B", 2);
        inventory.insert(3, "C", 3);
        inventory.sortById(true);
        assertOrder(inventory, {1, 2, 3});
        inventory.sortByName(true);
        assertOrder(inventory, {1, 2, 3});
        inventory.sortByQuantity(true);
        assertOrder(inventory, {1, 2, 3});
    }

    // SB-05: Completely reversed list (worst case for bubble sort)
    {
        Product inventory;
        for (int id = 5; id >= 1; --id) {
            inventory.insert(id, "Item", id);
        }
        inventory.sortById(true);
        assertOrder(inventory, {1, 2, 3, 4, 5});
        inventory.sortById(false);
        assertOrder(inventory, {5, 4, 3, 2, 1});
    }

    // SB-06: Equal quantities keep their original relative order (stable)
    {
        Product inventory;
        inventory.insert(1, "A", 5);
        inventory.insert(2, "B", 5);
        inventory.insert(3, "C", 1);
        inventory.insert(4, "D", 5);
        inventory.sortByQuantity(false);
        assertOrder(inventory, {1, 2, 4, 3});
        inventory.sortByQuantity(true);
        assertOrder(inventory, {3, 1, 2, 4});
    }

    // SB-07: Names - prefix and case-only differences
    {
        Product inventory;
        inventory.insert(1, "Mouse Pad", 1);
        inventory.insert(2, "Mouse", 1);
        inventory.insert(3, "mouse", 1);
        inventory.sortByName(true);   // "Mouse" == "mouse", both before "Mouse Pad"
        assertOrder(inventory, {2, 3, 1});
        inventory.sortByName(false);
        assertOrder(inventory, {1, 2, 3});
    }

    // SB-08: A product whose quantity was reduced to zero sorts correctly
    {
        Product inventory;
        inventory.insert(1, "A", 3);
        inventory.insert(2, "B", 5);
        inventory.update(1, "A", -3);
        inventory.sortByQuantity(true);
        assertOrder(inventory, {1, 2});
        inventory.sortByQuantity(false);
        assertOrder(inventory, {2, 1});
    }

    // SB-09: Larger list, scrambled ids 0..49
    {
        Product inventory;
        for (int i = 0; i < 50; ++i) {
            int id = (i * 37) % 50;
            inventory.insert(id, "Item", (id * 7) % 13 + 1);
        }

        inventory.sortById(true);
        std::vector<int> expected;
        for (int id = 0; id < 50; ++id) {
            expected.push_back(id);
        }
        assertOrder(inventory, expected);

        inventory.sortByQuantity(true);
        assert(inventory.getSize() == 50);
        ProductNode* curr = inventory.getHead();
        while (curr->next) {
            assert(curr->quantity <= curr->next->quantity);
            curr = curr->next;
        }
    }

    std::cout << "Sorting boundary tests passed.\n";
}

// Sorting itself has no invalid arguments, so these tests check that sorting
// stays correct after failed operations and that the list is still usable
// (insert, delete, update, lookup) after it has been sorted.
void sortInvalidInputTests() {
    // SI-01: A failed insert does not break a later sort
    {
        Product inventory;
        inventory.insert(2, "B", 5);
        inventory.insert(1, "A", 5);

        bool exceptionThrown = false;
        try {
            inventory.insert(1, "Duplicate", 9);
        } catch (const std::runtime_error&) {
            exceptionThrown = true;
        }
        assert(exceptionThrown);

        exceptionThrown = false;
        try {
            inventory.insert(3, "C", -4);
        } catch (const std::invalid_argument&) {
            exceptionThrown = true;
        }
        assert(exceptionThrown);

        inventory.sortById(true);
        assertOrder(inventory, {1, 2});
    }

    // SI-02: A product inserted after sorting is added at the tail
    {
        Product inventory;
        inventory.insert(3, "C", 1);
        inventory.insert(1, "A", 1);
        inventory.sortById(false);
        assertOrder(inventory, {3, 1});

        inventory.insert(50, "New", 1);
        assertOrder(inventory, {3, 1, 50});
        assert(inventory.getTail()->productId == 50);
    }

    // SI-03: Delete head, tail and middle after sorting
    {
        Product inventory;
        for (int id = 5; id >= 1; --id) {
            inventory.insert(id, "Item", id);
        }
        inventory.sortById(true);          // 1 2 3 4 5

        inventory.deleteProduct(1);        // head
        assertOrder(inventory, {2, 3, 4, 5});

        inventory.deleteProduct(5);        // tail
        assertOrder(inventory, {2, 3, 4});

        inventory.deleteProduct(3);        // middle
        assertOrder(inventory, {2, 4});

        assert(!inventory.contains(3));
    }

    // SI-04: Deleted product is not found, and operations on it still throw
    {
        Product inventory;
        inventory.insert(1, "A", 1);
        inventory.insert(2, "B", 2);
        inventory.sortByQuantity(false);
        inventory.deleteProduct(2);

        bool exceptionThrown = false;
        try {
            inventory.update(2, "B", 1);
        } catch (const std::runtime_error&) {
            exceptionThrown = true;
        }
        assert(exceptionThrown);
        assertOrder(inventory, {1});
    }

    // SI-05: Updating after sorting, then sorting again
    {
        Product inventory;
        inventory.insert(1, "A", 10);
        inventory.insert(2, "B", 20);
        inventory.insert(3, "C", 30);

        inventory.sortByQuantity(false);
        assertOrder(inventory, {3, 2, 1});

        inventory.update(1, "A", 50);      // 1 now has 60
        inventory.sortByQuantity(false);
        assertOrder(inventory, {1, 3, 2});

        // Failed update (insufficient quantity) leaves the order intact
        bool exceptionThrown = false;
        try {
            inventory.update(2, "B", -999);
        } catch (const std::runtime_error&) {
            exceptionThrown = true;
        }
        assert(exceptionThrown);
        inventory.sortByQuantity(false);
        assertOrder(inventory, {1, 3, 2});
        assert(inventory.getQuantity(2) == 20);
    }

    // SI-06: Several sorts in a row never lose or change any data
    {
        Product inventory;
        inventory.insert(4, "delta", 40);
        inventory.insert(2, "Bravo", 15);
        inventory.insert(3, "charlie", 25);
        inventory.insert(1, "Alpha", 5);

        inventory.sortByName(false);
        inventory.sortById(true);
        inventory.sortByQuantity(false);
        inventory.sortByName(true);

        assert(inventory.getSize() == 4);
        assertOrder(inventory, {1, 2, 3, 4});

        int total = 0;
        for (ProductNode* c = inventory.getHead(); c; c = c->next) {
            total += c->quantity;
        }
        assert(total == 85);
        assert(inventory.getName(3) == "charlie");
        assert(inventory.getQuantity(4) == 40);
    }

    std::cout << "Sorting invalid-input tests passed.\n";
}

static std::vector<int> idsOf(const std::vector<ProductNode*>& result) {
    std::vector<int> ids;
    for (const ProductNode* p : result) {
        ids.push_back(p->productId);
    }
    return ids;
}
 
void searchNormalTests() {
    Product inventory;
    inventory.insert(1, "Keyboard", 10);
    inventory.insert(2, "Mouse", 20);
    inventory.insert(3, "Monitor", 20);
 
    // FN-01: Search by name finds the product
    std::vector<ProductNode*> result = inventory.searchByName("Mouse");
    assert(result.size() == 1);
    assert(result[0]->productId == 2);
    assert(result[0]->name == "Mouse");
 
    // FN-02: Name search ignores case
    assert(idsOf(inventory.searchByName("mOuSe")) == std::vector<int>({2}));
    assert(idsOf(inventory.searchByName("KEYBOARD")) == std::vector<int>({1}));
 
    // FN-03: Products sharing a name are all returned, in list order
    inventory.insert(4, "Mouse", 5);
    assert(idsOf(inventory.searchByName("Mouse")) == std::vector<int>({2, 4}));
 
    // FN-04: Search by quantity finds the product
    assert(idsOf(inventory.searchByQuantity(10)) == std::vector<int>({1}));
 
    // FN-05: Products sharing a quantity are all returned, in list order
    assert(idsOf(inventory.searchByQuantity(20)) == std::vector<int>({2, 3}));
 
    // FN-06: Results are the real nodes, not copies
    assert(inventory.searchByQuantity(10)[0] == inventory.searchByProductId(1));
 
    // FN-07: Results follow the current sort order
    inventory.sortById(false);
    assert(idsOf(inventory.searchByQuantity(20)) == std::vector<int>({3, 2}));
 
    std::cout << "Searching normal tests passed.\n";
}
 
void searchBoundaryTests() {
    // FB-01: Searching an empty list returns nothing
    {
        Product inventory;
        assert(inventory.searchByName("Mouse").empty());
        assert(inventory.searchByQuantity(5).empty());
    }
 
    // FB-02: No match returns an empty result (no exception)
    {
        Product inventory;
        inventory.insert(1, "Keyboard", 10);
        assert(inventory.searchByName("Printer").empty());
        assert(inventory.searchByQuantity(999).empty());
    }
 
    // FB-03: Quantity 0 (stock reduced to zero) can be found
    {
        Product inventory;
        inventory.insert(1, "Keyboard", 10);
        inventory.insert(2, "Mouse", 3);
        inventory.update(1, "Keyboard", -10);
        assert(idsOf(inventory.searchByQuantity(0)) == std::vector<int>({1}));
    }
 
    // FB-04: Smallest valid quantity (1)
    {
        Product inventory;
        inventory.insert(1, "Keyboard", 1);
        assert(idsOf(inventory.searchByQuantity(1)) == std::vector<int>({1}));
    }
 
    // FB-05: Name must match completely, not partially
    {
        Product inventory;
        inventory.insert(1, "Mouse", 1);
        inventory.insert(2, "Mouse Pad", 1);
        assert(inventory.searchByName("Mous").empty());
        assert(idsOf(inventory.searchByName("Mouse")) == std::vector<int>({1}));
        assert(idsOf(inventory.searchByName("mouse pad")) == std::vector<int>({2}));
    }
 
    // FB-06: Every product matches
    {
        Product inventory;
        inventory.insert(1, "A", 7);
        inventory.insert(2, "B", 7);
        inventory.insert(3, "C", 7);
        assert(idsOf(inventory.searchByQuantity(7)) == std::vector<int>({1, 2, 3}));
    }
 
    // FB-07: List with a single product (head is also tail)
    {
        Product inventory;
        inventory.insert(1, "Solo", 4);
        assert(idsOf(inventory.searchByName("solo")) == std::vector<int>({1}));
        assert(idsOf(inventory.searchByQuantity(4)) == std::vector<int>({1}));
    }
 
    // FB-08: Match on the last node of the list
    {
        Product inventory;
        inventory.insert(1, "A", 1);
        inventory.insert(2, "B", 2);
        inventory.insert(3, "C", 3);
        assert(idsOf(inventory.searchByName("C")) == std::vector<int>({3}));
        assert(idsOf(inventory.searchByQuantity(3)) == std::vector<int>({3}));
    }
 
    // FB-09: Search sees renamed products and changed quantities
    {
        Product inventory;
        inventory.insert(1, "Keyboard", 10);
        inventory.update(1, "Gaming Keyboard", 5);
        assert(inventory.searchByName("Keyboard").empty());
        assert(idsOf(inventory.searchByName("gaming keyboard")) == std::vector<int>({1}));
        assert(inventory.searchByQuantity(10).empty());
        assert(idsOf(inventory.searchByQuantity(15)) == std::vector<int>({1}));
    }
 
    std::cout << "Searching boundary tests passed.\n";
}
 
void searchInvalidInputTests() {
    Product inventory;
    inventory.insert(1, "Keyboard", 10);
    inventory.insert(2, "Mouse", 20);
 
    // FI-01: Empty name
    bool exceptionThrown = false;
    try {
        inventory.searchByName("");
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
 
    // FI-02: Whitespace-only name
    exceptionThrown = false;
    try {
        inventory.searchByName("   ");
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
 
    // FI-03: Negative quantity
    exceptionThrown = false;
    try {
        inventory.searchByQuantity(-1);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
 
    // FI-04: Failed searches change nothing
    assert(inventory.getSize() == 2);
    assert(inventory.getQuantity(1) == 10);
 
    // FI-05: Name with extra spaces is not trimmed, so it does not match
    assert(inventory.searchByName(" Mouse").empty());
    assert(inventory.searchByName("Mouse ").empty());
 
    // FI-06: Deleted product can no longer be found
    inventory.deleteProduct(2);
    assert(inventory.searchByName("Mouse").empty());
    assert(inventory.searchByQuantity(20).empty());
    assert(!inventory.contains(2));
 
    // FI-07: Search by id on a nonexistent product still throws
    exceptionThrown = false;
    try {
        inventory.searchByProductId(999);
    } catch (const std::runtime_error&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);
 
    std::cout << "Searching invalid-input tests passed.\n";
}