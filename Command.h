#pragma once

#include <iostream>

enum class CommandType {
    REQUEST, RELEASE
};

class Command {
private:
    CommandType type;
    int amount;
    int resource;
    int process;
public:
    Command(CommandType type, int amount, int resource, int process) : type(type), amount(amount), resource(resource), process(process) { }

    CommandType getType() {
        return this->type;
    }
    int getAmount() {
        return this->amount;
    }
    int getResource() {
        return this->resource;
    }
    int getProcess() {
        return this->process;
    }
};
