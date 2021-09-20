#include "sphere.h"

Manager::Manager(int max_count) : size(max_count), count(0) {
    Manager::figures = (Shape**) calloc(max_count, sizeof(Shape*));
};

Manager::~Manager() {
    free(figures);
};

void Manager::draw_all(const Renderer& renderer) {
    for (int i = 0; i < count; ++i) {
        figures[i]->draw(renderer);
    }
};

void Manager::add_figure(Shape* figure) {
    if (count < size) {
        figures[count++] = figure;
    }
};

void Manager::del_figure(const Shape* figure) {
    int pos = 0;
    for (int i = 0; i < count; ++i) {
        if (figures[i] == figure) {
            for (int j = i; j < count - 1; ++j) {
                figures[j] = figures[j + 1];   
            }
            count--;
            break;
        }
    }
};

void Manager::del_last() {
    figures[--count] = nullptr;
};

void Manager::move_all(double time) {
    for (int i = 0; i < count; ++i) {
        figures[i]->move(time);
    } 
}