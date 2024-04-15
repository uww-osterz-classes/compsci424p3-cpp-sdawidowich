#pragma once

#include <iostream>
#include <string>
#include <vector>

class Process {
private:
    std::vector<int> maxClaims;
    std::vector<int> currentAlloc;
    std::vector<int> currentReq;
public:

    std::vector<int> getMaxClaims() {
        return this->maxClaims;
    }
    std::vector<int> getCurrentAlloc() {
        return this->currentAlloc;
    }
    std::vector<int> getCurrentReq() {
        return this->currentReq;
    }
    int getPotentialRequests(int resource) {
        return this->getMaxClaims()[resource] - this->getCurrentAlloc()[resource];
    }

    void addResource(int maxClaims, int currentAlloc, int currentReq) {
        this->maxClaims.push_back(maxClaims);
        this->currentAlloc.push_back(currentAlloc);
        this->currentReq.push_back(currentReq);
    }

    void setMaxClaims(int resource, int value) {
        this->maxClaims[resource] = value;
    }
    void setCurrentAlloc(int resource, int value) {
        this->currentAlloc[resource] = value;
    }
    void setCurrentReq(int resource, int value) {
        this->currentReq[resource] = value;
    }
};