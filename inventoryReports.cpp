#include "inventoryReports.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

static std::string formatTime(std::time_t t) {
    std::tm timeInfo;
#ifdef _WIN32
    localtime_s(&timeInfo, &t);
#else
    localtime_r(&t, &timeInfo);
#endif
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
    return std::string(buffer);
}

void printInventoryReport(Product& inv, int lowStockThreshold) {
    std::cout << "\n=== Inventory Report ===\n";

    if (inv.getSize() == 0) {
        std::cout << "No products in inventory.\n";
        return;
    }

    std::cout << std::left << std::setw(8) << "ID"
              << std::setw(22) << "Name"
              << "Quantity\n";

    int lowStockCount = 0;
    ProductNode* curr = inv.getHead();

    while (curr) {
        std::cout << std::left << std::setw(8) << curr->productId
                  << std::setw(22) << curr->name
                  << curr->quantity;

        if (curr->quantity < lowStockThreshold) {
            std::cout << "   <- LOW STOCK";
            ++lowStockCount;
        }
        std::cout << "\n";
        curr = curr->next;
    }

    std::cout << "Total products: " << inv.getSize() << "\n";
    std::cout << "Low-stock products (below " << lowStockThreshold << "): "
              << lowStockCount << "\n";
}

void printLowStockReport(Product& inv, int lowStockThreshold) {
    std::cout << "\n=== Low-Stock Report (below " << lowStockThreshold << ") ===\n";

    int count = 0;
    ProductNode* curr = inv.getHead();

    while (curr) {
        if (curr->quantity < lowStockThreshold) {
            std::cout << std::left << std::setw(8) << curr->productId
                      << std::setw(22) << curr->name
                      << curr->quantity << "\n";
            ++count;
        }
        curr = curr->next;
    }

    if (count == 0) {
        std::cout << "No low-stock products.\n";
    }
}

void printTransactionHistory(const TransactionQueue& log, bool newestFirst) {
    std::cout << "\n=== Transaction History ===\n";

    if (log.getSize() == 0) {
        std::cout << "No transactions recorded.\n";
        return;
    }

    std::cout << std::left << std::setw(6) << "TID"
              << std::setw(10) << "Product"
              << std::setw(11) << "Type"
              << std::setw(10) << "Quantity"
              << "Time\n";

    const TransactionNode* curr = newestFirst ? log.getTail() : log.getHead();

    while (curr) {
        std::cout << std::left << std::setw(6) << curr->transactionId
                  << std::setw(10) << curr->productId
                  << std::setw(11) << (curr->type == TransactionType::STOCK_IN
                                           ? "STOCK_IN" : "STOCK_OUT")
                  << std::setw(10) << curr->quantity
                  << formatTime(curr->timestamp) << "\n";

        curr = newestFirst ? curr->prev : curr->next;
    }
}
