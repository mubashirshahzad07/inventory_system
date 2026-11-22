#pragma once

#include <string>

#include "productManagement.h"
#include "transactionQueue.h"

class TransactionManager {
private:
    Product* inv;
    TransactionQueue log;

public:
    explicit TransactionManager(Product* xinv);

    int addProduct(int productId, std::string name, int quantity);
    int stockIn(int productId, int quantity);
    int stockOut(int productId, int quantity);

    void save(const std::string& productFile, const std::string& transactionFile);
    bool load(const std::string& productFile, const std::string& transactionFile);

    const TransactionQueue &getLog() const;

    void sortById(bool ascending = true);
    void sortByName(bool ascending = true);
    void sortByQuantity(bool ascending = true);
};
