#pragma once
#include <string>
#include <vector>
#include <map>

struct View {
    int id;
    std::string type;
    std::map<std::string, std::string> props;
    std::vector<View*> children;
};
