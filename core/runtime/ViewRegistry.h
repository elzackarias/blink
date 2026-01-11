#pragma once
#include "View.h"
#include <unordered_map>

class ViewRegistry {
public:
    static View* create(const std::string& type);
    static View* get(int id);
    static View* root;
    static void setRoot(View* v);
    static View* getRoot();

private:
    static int nextId;
    static std::unordered_map<int, View*> views;
};


