#include "productManagement.h"

#include <string>
#include <stdexcept>
#include <vector>
#include <cctype>
#include <cstdlib>
#include <utility>

void validateProduct(int productId, int quantity);

bool isBlank(const std::string& name) {
    return (name.find_first_not_of(" \t\n\r\f\v") == std::string::npos);
}

std::string toLower(std::string text) {
    for (char& ch : text) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    return text;
}


void Product::invalidProductId(int productId) {
    if (!registeredProductIds.count(productId)) {
        throw std::runtime_error("Product with provided product id does not exist.");
    }
}

Product::Product():
    head(nullptr), tail(nullptr), size(0)
{}

void Product::insert(int productId, std::string name, int quantity) {
    if (registeredProductIds.count(productId)) {
        throw std::runtime_error("Product id is already taken.");
    }

    if (isBlank(name)) {
        throw std::invalid_argument("Name can not be empty");
    }

    validateProduct(productId, quantity);

    ProductNode* newProduct = new ProductNode(name, productId, quantity);

    ++size;
    registeredProductIds.insert(productId);
    if (!head) {
        head = tail = newProduct;
        return;
    }

    tail->next = newProduct;
    newProduct->prev = tail;
    tail = newProduct;
}

void Product::update(int productId, std::string newName, int quantityChange) {
    invalidProductId(productId);

    if (isBlank(newName)) {
        throw std::invalid_argument("Name can not be empty");
    }

    ProductNode* product = searchByProductId(productId);

    if (quantityChange < 0) {
        if (abs(quantityChange) > product->quantity) {
            throw std::runtime_error("Insufficient quantity.");
        }
    }

    product->quantity += quantityChange;
    product->name = newName;
}

void Product::deleteProduct(int productId) {
    invalidProductId(productId);

    ProductNode* product = searchByProductId(productId);

    --size;
    registeredProductIds.erase(productId);
    if (head == tail) {
        ProductNode* temp = head;
        head = tail = nullptr;
        delete temp;

        return;
    }

    if (product == head) {
        ProductNode* temp = head;
        head = head->next;
        head->prev = nullptr;
        delete temp;

        return;
    }

    if (product == tail) {
        ProductNode* temp = tail;
        tail = tail->prev;
        tail->next = nullptr;
        delete temp;

        return;
    }

    ProductNode* temp = product;
    product->prev->next = product->next;
    product->next->prev = product->prev;

    delete temp;
}

ProductNode* Product::searchByProductId(int productId) {
    invalidProductId(productId);

    ProductNode* curr = head;
    while (curr) {
        if (curr->productId == productId) {
            break;
        }

        curr = curr->next;
    }

    return curr;
}

std::vector<ProductNode*> Product::searchByName(const std::string& name) {
    if (isBlank(name)) {
        throw std::invalid_argument("Name can not be empty");
    }

    std::string target = toLower(name);
    std::vector<ProductNode*> matches;

    ProductNode* curr = head;
    while (curr) {
        if (toLower(curr->name) == target) {
            matches.push_back(curr);
        }
        curr = curr->next;
    }

    return matches;
}

std::vector<ProductNode*> Product::searchByQuantity(int quantity) {
    if (quantity < 0) {
        throw std::invalid_argument("Quantity can not be negative.");
    }

    std::vector<ProductNode*> matches;

    ProductNode* curr = head;
    while (curr) {
        if (curr->quantity == quantity) {
            matches.push_back(curr);
        }
        curr = curr->next;
    }

    return matches;
}

size_t Product::getSize() {
    return size;
}

ProductNode* Product::getHead() {
    return head;
}

ProductNode* Product::getTail() {
    return tail;
}

bool Product::contains(int productId) const {
    return registeredProductIds.count(productId);
}

std::string Product::getName(int productId) {
    ProductNode* product = searchByProductId(productId);
    return product->name;
}

int Product::getQuantity(int productId) {
    ProductNode* product = searchByProductId(productId);
    return product->quantity;
}

Product::~Product() {
    ProductNode* curr = head;
    while (curr) {
        ProductNode* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

void Product::swapData(ProductNode* a, ProductNode* b) {
    std::swap(a->name, b->name);
    std::swap(a->productId, b->productId);
    std::swap(a->quantity, b->quantity);
}

void Product::sortBy(const Comparator& before) {
    if (size < 2) {
        return;
    }

    ProductNode* end = nullptr;
    bool swapped;

    do {
        swapped = false;
        ProductNode* curr = head;

        while (curr->next != end) {
            if (before(curr->next, curr)) {
                swapData(curr, curr->next);
                swapped = true;
            }
            curr = curr->next;
        }

        end = curr;
    } while (swapped);
}

void Product::sortById(bool ascending) {
    sortBy([ascending](const ProductNode* a, const ProductNode* b) {
        return ascending ? a->productId < b->productId
                         : a->productId > b->productId;
    });
}

void Product::sortByName(bool ascending) {
    sortBy([ascending](const ProductNode* a, const ProductNode* b) {
        const std::string& x = a->name;
        const std::string& y = b->name;
        std::size_t n = x.size() < y.size() ? x.size() : y.size();

        for (std::size_t i = 0; i < n; ++i) {
            int cx = std::tolower(static_cast<unsigned char>(x[i]));
            int cy = std::tolower(static_cast<unsigned char>(y[i]));
            if (cx != cy) {
                return ascending ? cx < cy : cx > cy;
            }
        }

        if (x.size() == y.size()) {
            return false;
        }
        return ascending ? x.size() < y.size() : x.size() > y.size();
    });
}

void Product::sortByQuantity(bool ascending) {
    sortBy([ascending](const ProductNode* a, const ProductNode* b) {
        return ascending ? a->quantity < b->quantity
                         : a->quantity > b->quantity;
    });
}