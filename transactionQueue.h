#pragma once

#include <ctime>
#include <cstddef>

#include "transactionNode.h"

class TransactionQueue {
private:
    TransactionNode* head;
    TransactionNode* tail;
    std::size_t size;
    int nextId;

public:
    TransactionQueue();
    ~TransactionQueue();

    int enqueue(
        int productId,
        TransactionType type,
        int quantity,
        std::time_t timestamp = std::time(nullptr)
    );
    std::size_t getSize() const;
    const TransactionNode* getHead() const;
    const TransactionNode* getTail() const;
};
