#pragma once

#include <string>
#include <unordered_set>
#include <functional>
#include <vector>
#include <cstddef>

#include "productNode.h"

class Product {
private:
    std::unordered_set<int> registeredProductIds;
    ProductNode* head;
    ProductNode* tail;
    size_t size;

    void invalidProductId(int productId);
    using Comparator = std::function<bool(const ProductNode *, const ProductNode *)>;
    static void swapData(ProductNode* a, ProductNode* b);
    void sortBy(const Comparator& before);

public:
    Product();
    ~Product();
    void insert(int productId, std::string name, int quantity);
    void update(int productId, std::string newName, int quantityChange);
    void deleteProduct(int productId);
    ProductNode* searchByProductId(int productId);
    std::vector<ProductNode *> searchByName(const std::string &name);
    std::vector<ProductNode*> searchByQuantity(int quantity);
    size_t getSize();
    ProductNode* getHead();
    ProductNode* getTail();

    bool contains(int productId) const;
    std::string getName(int productId);
    int getQuantity(int productId);

    void sortById(bool ascending = true);
    void sortByName(bool ascending = true);
    void sortByQuantity(bool ascending = true);
};
