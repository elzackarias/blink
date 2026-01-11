#include "Renderer.h"
#include <iostream>

namespace Renderer {

void indent(int depth) {
    for (int i = 0; i < depth; i++)
        std::cout << "  ";
}

void render(View* view, int depth) {
    if (!view) return;

    indent(depth);
    std::cout << view->type << "#" << view->id << std::endl;

    for (auto* child : view->children) {
        render(child, depth + 1);
    }
}

}

