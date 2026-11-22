#include "fileHandler.h"

#include <ctime>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

// Splits a line into exactly fieldCount fields separated by '|'.
// The last field keeps everything that remains (so a name may contain '|').
static std::vector<std::string> splitFields(const std::string& line,
                                            std::size_t fieldCount) {
    std::vector<std::string> fields;
    std::size_t start = 0;

    while (fields.size() + 1 < fieldCount) {
        std::size_t pos = line.find('|', start);
        if (pos == std::string::npos) {
            throw std::runtime_error("Missing field.");
        }
        fields.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }

    fields.push_back(line.substr(start));
    return fields;
}

static void removeTrailingCarriageReturn(std::string& line) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back();
    }
}

// Product file line format:  productId|quantity|name
void saveProducts(Product& inv, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    ProductNode* curr = inv.getHead();
    while (curr) {
        out << curr->productId << '|' << curr->quantity << '|' << curr->name << '\n';
        curr = curr->next;
    }

    if (!out) {
        throw std::runtime_error("Error while writing file: " + filename);
    }
}

bool loadProducts(Product& inv, const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(in, line)) {
        ++lineNumber;
        removeTrailingCarriageReturn(line);
        if (line.empty()) {
            continue;
        }

        try {
            std::vector<std::string> fields = splitFields(line, 3);
            int productId = std::stoi(fields[0]);
            int quantity = std::stoi(fields[1]);
            if (quantity < 0) {
                throw std::runtime_error("Negative quantity.");
            }

            // Product::insert rejects quantity 0, but a product can legitimately
            // reach 0 through stock-out, so restore it as 1 and then remove that 1.
            inv.insert(productId, fields[2], quantity == 0 ? 1 : quantity);
            if (quantity == 0) {
                inv.update(productId, fields[2], -1);
            }
        } catch (const std::exception&) {
            throw std::runtime_error("Corrupted product file at line " +
                                     std::to_string(lineNumber));
        }
    }

    return true;
}

// Transaction file line format:  transactionId|productId|IN or OUT|quantity|timestamp
void saveTransactions(const TransactionQueue& log, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    const TransactionNode* curr = log.getHead();
    while (curr) {
        out << curr->transactionId << '|'
            << curr->productId << '|'
            << (curr->type == TransactionType::STOCK_IN ? "IN" : "OUT") << '|'
            << curr->quantity << '|'
            << static_cast<long long>(curr->timestamp) << '\n';
        curr = curr->next;
    }

    if (!out) {
        throw std::runtime_error("Error while writing file: " + filename);
    }
}

bool loadTransactions(TransactionQueue& log, const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(in, line)) {
        ++lineNumber;
        removeTrailingCarriageReturn(line);
        if (line.empty()) {
            continue;
        }

        try {
            std::vector<std::string> fields = splitFields(line, 5);
            int savedId = std::stoi(fields[0]);
            int productId = std::stoi(fields[1]);
            int quantity = std::stoi(fields[3]);
            std::time_t timestamp = static_cast<std::time_t>(std::stoll(fields[4]));

            TransactionType type;
            if (fields[2] == "IN") {
                type = TransactionType::STOCK_IN;
            } else if (fields[2] == "OUT") {
                type = TransactionType::STOCK_OUT;
            } else {
                throw std::runtime_error("Unknown transaction type.");
            }

            // The queue gives out ids 1, 2, 3... so a valid file must match exactly.
            int newId = log.enqueue(productId, type, quantity, timestamp);
            if (newId != savedId) {
                throw std::runtime_error("Transaction ids are not sequential.");
            }
        } catch (const std::exception&) {
            throw std::runtime_error("Corrupted transaction file at line " +
                                     std::to_string(lineNumber));
        }
    }

    return true;
}
