#include "productNode.h"

#include <utility>
#include <stdexcept>
#include <string>

void validateProduct(int productId, int quantity) {
    if (quantity <= 0) {
        throw std::invalid_argument("Product quantity must be greater than zero.");
    }

    if (productId < 0) {
        throw std::invalid_argument("Product id can not be negative.");
    }
}

ProductNode::ProductNode(std::string xName, int xProductId, int xQuantity):
    name(std::move(xName)),
    productId(xProductId),
    quantity(xQuantity),
    next(nullptr),
    prev(nullptr)
{
    validateProduct(productId, quantity);
}

