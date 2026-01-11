#pragma once
#include "View.h"
#include <unordered_map>

class ViewRegistry {
public:
    static View* create(const std::string& type);
    static View* get(int id);

private:
    static int nextId;
    static std::unordered_map<int, View*> views;
};


