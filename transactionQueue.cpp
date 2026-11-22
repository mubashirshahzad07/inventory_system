#include "transactionQueue.h"

TransactionQueue::TransactionQueue() {
    head = nullptr;
    tail = nullptr;
    size = 0;
    nextId = 1;
}

int TransactionQueue::enqueue(
    int productId,
    TransactionType type,
    int quantity,
    std::time_t timestamp
) {
    TransactionNode* node =
        new TransactionNode(nextId, productId, type, quantity, timestamp);

    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }

    ++size;
    return nextId++;
}

std::size_t TransactionQueue::getSize() const {
    return size;
}

const TransactionNode* TransactionQueue::getHead() const {
    return head;
}

const TransactionNode* TransactionQueue::getTail() const {
    return tail;
}

TransactionQueue::~TransactionQueue() {
    TransactionNode* curr = head;

    while (curr) {
        TransactionNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
}
