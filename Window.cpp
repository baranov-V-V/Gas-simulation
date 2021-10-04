#include "sphere.h"

int Frame::get_size_x() const {
    return size.x;
};

int Frame::get_size_y() const {
    return size.y;
};

int Texture::get_coord_x() const {
    return coord.x;
};

int Texture::get_coord_y() const {
    return coord.y;
};


VirtualWindow::VirtualWindow(int x_size, int y_size, COLORREF color) : Frame(x_size, y_size), color(color) {
    screen = nullptr;
    buf_screen = nullptr;
    type = 0;
};

HDC VirtualWindow::get_hdc() const {
    return screen;
};

RGBQUAD* VirtualWindow::get_buf() const {
    return buf_screen;
};

COLORREF VirtualWindow::get_color() const {
    return color;
}

Renderer::Renderer(VirtualWindow* window, Coordinates* coord) :
    window(window), coordinates(coord) {
    scale.x = double(window->get_size_x()) / double(coord->get_max_x() - coord->get_min_x());
    scale.y = double(window->get_size_y()) / double(coord->get_max_y() - coord->get_min_y());    
};

void Renderer::set_window(VirtualWindow* new_window) {
    window = new_window;
    Coordinates* coord = this->get_coordinates();
    scale.x = double(window->get_size_x()) / double(coord->get_max_x() - coord->get_min_x());
    scale.y = double(window->get_size_y()) / double(coord->get_max_y() - coord->get_min_y());    
};

int Renderer::to_pixel_x(double coord) const {
    return (coord - this->get_coordinates()->get_min_x()) * scale.x;
};

int Renderer::to_pixel_y(double coord) const {
    return (coord - this->get_coordinates()->get_min_y()) * scale.y;
};

double Renderer::to_coord_x(int x) const {
    return double(x) / scale.x + this->get_coordinates()->get_min_x();
};

double Renderer::to_coord_y(int y) const {
    return double(y) / scale.y + this->get_coordinates()->get_min_y();
};