#include "transactionNode.h"

#include <stdexcept>

void validateTransaction(int transactionId, int productId, int quantity) {
    if (transactionId < 0) {
        throw std::invalid_argument("Transaction id can not be negative.");
    }

    if (productId < 0) {
        throw std::invalid_argument("Product id can not be negative.");
    }

    if (quantity <= 0) {
        throw std::invalid_argument("Transaction quantity must be greater than zero.");
    }
}

TransactionNode::TransactionNode(
    int xTransactionId,
    int xProductId,
    TransactionType xType,
    int xQuantity,
    std::time_t xTimestamp
) {
    transactionId = xTransactionId;
    productId = xProductId;
    type = xType;
    quantity = xQuantity;
    timestamp = xTimestamp;
    next = nullptr;
    prev = nullptr;

    validateTransaction(transactionId, productId, quantity);
}
