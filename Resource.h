#pragma once

#include <iostream>

class Resource {
private: 
    int available;
    int total;
public:
    Resource(int available) : available(available), total(available) { }

    int getAvailable() {
        return this->available;
    }
    int getTotal() {
        return this->total;
    }

    void setAvailable(int value) {
        this->available = value;
    }
    void setTotal(int value) {
        this->total = value;
    }
};