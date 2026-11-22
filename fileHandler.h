#pragma once

#include <string>

#include "productManagement.h"
#include "transactionQueue.h"

void saveProducts(Product& inv, const std::string& filename);
bool loadProducts(Product& inv, const std::string& filename);

void saveTransactions(const TransactionQueue& log, const std::string& filename);
bool loadTransactions(TransactionQueue& log, const std::string& filename);
