#pragma once

#include "productManagement.h"
#include "transactionQueue.h"

void printInventoryReport(Product& inv, int lowStockThreshold);
void printLowStockReport(Product& inv, int lowStockThreshold);
void printTransactionHistory(const TransactionQueue& log, bool newestFirst = false);
