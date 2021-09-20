#include "sphere.h"

Window::Window() : size(800, 600), coord(0, 0) {
    screen = txCreateDIBSection(800, 600, &buf_screen);
    Window::color = TX_BLACK;

    txSetFillColor(color, screen);
    txRectangle(0, 0, size.x, size.y, screen);
}

Window::Window(int x_size, int y_size, COLORREF color, int coord_x, int coord_y) :
    size(x_size, y_size), coord(coord_x, coord_y) {
    screen = txCreateDIBSection(x_size, y_size, &buf_screen);
    
    Window::color = color;

    txSetFillColor(color, screen);
    txRectangle(0, 0, size.x, size.y, screen);
}

Window::~Window() {
    txDeleteDC(screen);
}

void Window::show_on(const HDC& target) const {
    txBitBlt(target, coord.x, coord.y, 0, 0, screen);
}

int Window::get_size_x() const {
    return size.x;
};

int Window::get_size_y() const {
    return size.y;
};

int Window::get_coord_x() const {
    return coord.x;
};

int Window::get_coord_y() const {
    return coord.x;
};

COLORREF Window::get_color() const {
    return color;
};

HDC Window::get_hdc() const {
    return screen;
}

RGBQUAD* Window::get_buf() const {
    return buf_screen;
};



MainWindow::MainWindow(int x_size, int y_size) : size(x_size, y_size) {
    txCreateWindow(x_size, y_size);
    screen = txDC();
    buf_screen = txVideoMemory();
};

HDC MainWindow::get_hdc() const {
    return screen;
};

RGBQUAD* MainWindow::get_buf() const {
    return buf_screen;
};

int MainWindow::get_size_x() const {
    return size.x;
};

int MainWindow::get_size_y() const {
    return size.y;
};


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

void Renderer::draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color, int thickness) {
    txSetColor(color, thickness, window->get_hdc());
    txLine(this->to_pixel_x(x_begin), this->to_pixel_y(y_begin),
           this->to_pixel_x(x_end), this->to_pixel_y(y_end), window->get_hdc());
};

void Renderer::set_pixel(double x, double y, COLORREF color) {
    txSetPixel(this->to_pixel_x(x), this->to_pixel_y(y), color, window->get_hdc());
};

void Renderer::draw_circle(double x, double y, double r, COLORREF color, int thickness) {
    txSetColor(color, thickness, window->get_hdc());
    txEllipse(this->to_pixel_x(x - r), this->to_pixel_y(y + r),
              this->to_pixel_x(x + r), this->to_pixel_y(y - r), window->get_hdc());
};

void Renderer::draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color, int thickness) {
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

void Renderer::show_on(const MainWindow& target) const {
    window->show_on(target.get_hdc());
};

void Renderer::clear() const {
    txClear(window->get_hdc());
}