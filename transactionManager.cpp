#include "transactionManager.h"

#include <stdexcept>
#include <string>

#include "fileHandler.h"

TransactionManager::TransactionManager(Product* xinv) {
    if (xinv == nullptr) {
        throw std::invalid_argument("Inventory can not be null.");
    }
    inv = xinv;
}

// Adds a new product and logs its opening stock as a stock-in.
// Returns the transaction id, or 0 if nothing was logged (opening quantity 0).
int TransactionManager::addProduct(int productId, std::string name, int quantity) {
    inv->insert(productId, name, quantity);

    if (quantity == 0) {
        return 0;
    }

    return log.enqueue(productId, TransactionType::STOCK_IN, quantity);
}

int TransactionManager::stockIn(int productId, int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Stock-in quantity must be greater than zero.");
    }

    std::string currentName = inv->getName(productId);
    inv->update(productId, currentName, quantity);

    return log.enqueue(productId, TransactionType::STOCK_IN, quantity);
}

int TransactionManager::stockOut(int productId, int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Stock-out quantity must be greater than zero.");
    }

    std::string currentName = inv->getName(productId);
    inv->update(productId, currentName, -quantity);

    return log.enqueue(productId, TransactionType::STOCK_OUT, quantity);
}

void TransactionManager::save(const std::string& productFile,
                              const std::string& transactionFile) {
    saveProducts(*inv, productFile);
    saveTransactions(log, transactionFile);
}

// Meant to be called once at startup on an empty inventory and empty log.
// Returns true if the product file was found.
bool TransactionManager::load(const std::string& productFile,
                              const std::string& transactionFile) {
    bool productsFound = loadProducts(*inv, productFile);
    loadTransactions(log, transactionFile);
    return productsFound;
}

const TransactionQueue& TransactionManager::getLog() const {
    return log;
}

void TransactionManager::sortById(bool ascending) {
    inv->sortById(ascending);
}

void TransactionManager::sortByName(bool ascending) {
    inv->sortByName(ascending);
}

void TransactionManager::sortByQuantity(bool ascending) {
    inv->sortByQuantity(ascending);
}