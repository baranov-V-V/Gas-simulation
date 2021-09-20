#include "sphere.h"

int BasicWindow::get_size_x() const {
    return size.x;
};

int BasicWindow::get_size_y() const {
    return size.y;
};



Texture::Texture(int x_size, int y_size, COLORREF color, int coord_x, int coord_y) :
    BasicWindow(x_size, y_size), coord(coord_x, coord_y) {
    screen = txCreateDIBSection(x_size, y_size, &buf_screen);
    
    Texture::color = color;
    
    txSetFillColor(color, screen);
    txRectangle(0, 0, x_size, y_size, screen);
}

Texture::~Texture() {
    txDeleteDC(screen);
}

void Texture::show_on(const HDC& target) const {
    txBitBlt(target, coord.x, coord.y, 0, 0, screen);
}

int Texture::get_coord_x() const {
    return coord.x;
};

int Texture::get_coord_y() const {
    return coord.x;
};

COLORREF Texture::get_color() const {
    return color;
};

HDC Texture::get_hdc() const {
    return screen;
}

RGBQUAD* Texture::get_buf() const {
    return buf_screen;
};



Window::Window(int x_size, int y_size, COLORREF color) : BasicWindow(x_size, y_size), color(color) {
    txCreateWindow(x_size, y_size);
    screen = txDC();
    buf_screen = txVideoMemory();

    txSetFillColor(color, screen);
    txRectangle(0, 0, x_size, y_size, screen);
};

HDC Window::get_hdc() const {
    return screen;
};

RGBQUAD* Window::get_buf() const {
    return buf_screen;
};

COLORREF Window::get_color() const {
    return color;
}



Renderer::Renderer(Window* window, Coordinates* coord) :
    window(window), coordinates(coord) {
    scale.x = double(window->get_size_x()) / double(coord->get_max_x() - coord->get_min_x());
    scale.y = double(window->get_size_y()) / double(coord->get_max_y() - coord->get_min_y());    
};

void Renderer::set_window(Window* new_window) {
    window = new_window;
    Coordinates* coord = this->get_coordinates();
    scale.x = double(window->get_size_x()) / double(coord->get_max_x() - coord->get_min_x());
    scale.y = double(window->get_size_y()) / double(coord->get_max_y() - coord->get_min_y());    
};

void Renderer::draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txLine(this->to_pixel_x(x_begin), this->to_pixel_y(y_begin),
           this->to_pixel_x(x_end), this->to_pixel_y(y_end), window->get_hdc());
};

void Renderer::set_pixel(double x, double y, COLORREF color) const {
    txSetPixel(this->to_pixel_x(x), this->to_pixel_y(y), color, window->get_hdc());
};

void Renderer::draw_circle(double x, double y, double r, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txEllipse(this->to_pixel_x(x - r), this->to_pixel_y(y + r),
              this->to_pixel_x(x + r), this->to_pixel_y(y - r), window->get_hdc());
};

void Renderer::draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txRectangle(this->to_pixel_x(x1), this->to_pixel_y(y1),
                this->to_pixel_x(x2), this->to_pixel_y(y2), window->get_hdc());
};

inline int Renderer::to_pixel_x(double coord) const {
    return (coord - this->get_coordinates()->get_min_x()) * scale.x;
};

inline int Renderer::to_pixel_y(double coord) const {
    return (coord - this->get_coordinates()->get_min_y()) * scale.y;
};

double Renderer::to_coord_x(int x) const {
    return double(x) / scale.x + this->get_coordinates()->get_min_x();
};

double Renderer::to_coord_y(int y) const {
    return double(y) / scale.y + this->get_coordinates()->get_min_y();
};

void Renderer::clear() const {
    txClear(window->get_hdc());
}