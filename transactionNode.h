#pragma once

#include <ctime>

enum class TransactionType {
    STOCK_IN,
    STOCK_OUT
};

class TransactionNode {
public:
    TransactionNode* next;
    TransactionNode* prev;
    std::time_t timestamp;
    TransactionType type;
    int transactionId;
    int productId;
    int quantity;

    TransactionNode(
        int xTransactionId,
        int xProductId,
        TransactionType xType,
        int xQuantity,
        std::time_t xTimestamp = std::time(nullptr)
    );
};
