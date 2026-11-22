#pragma once

#include <string>

struct ProductNode {
    std::string name;
    ProductNode* next;
    ProductNode* prev;
    int productId;
    int quantity;

    ProductNode(std::string xName, int xProductId, int quantity);
};
