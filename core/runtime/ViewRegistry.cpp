#include "ViewRegistry.h"

int ViewRegistry::nextId = 1;
std::unordered_map<int, View*> ViewRegistry::views;

View* ViewRegistry::create(const std::string& type){
    View* v = new View();
    v->id = nextId++;
    v->type = type;
    views[v->id] = v;
    return v;
}

View* ViewRegistry::get(int id){
    return views.count(id) ? views[id] : nullptr;
}

View* ViewRegistry::root = nullptr;

void ViewRegistry::setRoot(View* v){
    root = v;
}

View* ViewRegistry::getRoot(){
    return root;
}
