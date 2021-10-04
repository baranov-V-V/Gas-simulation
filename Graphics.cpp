//graphics engine

#include "sphere.h"

Window::Window(int x_size, int y_size, COLORREF color) : VirtualWindow(x_size, y_size, color) {    
    txCreateWindow(x_size, y_size);
    screen = txDC();
    buf_screen = txVideoMemory();
    type = TYPE_WINDOW;
    
    txSetFillColor(color, screen);
    txRectangle(0, 0, x_size, y_size, screen);
};

Texture::Texture(int x_size, int y_size, COLORREF color, int coord_x, int coord_y) :
    VirtualWindow(x_size, y_size, color), coord(coord_x, coord_y) {
    type = TYPE_TEXTURE;
    screen = txCreateDIBSection(x_size, y_size, &buf_screen);
    
    txSetFillColor(color, screen);
    txRectangle(0, 0, x_size, y_size, screen);
}

Texture::~Texture() {
    txDeleteDC(screen);
}

void Texture::show_on(const HDC& target) const {
    txBitBlt(target, coord.x, coord.y, 0, 0, screen);
}

void Renderer::draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txLine(this->to_pixel_x(x_begin), window->get_size_y() - this->to_pixel_y(y_begin),
           this->to_pixel_x(x_end), window->get_size_y() -  this->to_pixel_y(y_end), window->get_hdc());
};

void Renderer::set_pixel(double x, double y, COLORREF color) const {
    txSetPixel(this->to_pixel_x(x), window->get_size_y() - this->to_pixel_y(y), color, window->get_hdc());
};

void Renderer::draw_circle(double x, double y, double r, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txEllipse(this->to_pixel_x(x - r), window->get_size_y() - this->to_pixel_y(y + r),
              this->to_pixel_x(x + r), window->get_size_y() - this->to_pixel_y(y - r), window->get_hdc());
};

void Renderer::draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color, int thickness) const {
    txSetColor(color, thickness, window->get_hdc());
    txRectangle(this->to_pixel_x(x1), window->get_size_y() - this->to_pixel_y(y1),
                this->to_pixel_x(x2), window->get_size_y() - this->to_pixel_y(y2), window->get_hdc());
};

void Renderer::clear() const {
    txClear(window->get_hdc());
}

void PrintMousePos() {
    fprintf(stderr, "x:%lg, y:%lg\n", txMouseX(), txMouseY());
}

void TextButton::draw(const Renderer& render) const {
    double x = 0, y = 0;
    double c_x = coord.x + size.x / 2, c_y = coord.y + size.y / 2;
    GetMouse(x, y, render);    
    
    int red = std::min(double(GetRValue(color)) * 1.5 + (255 - double(GetRValue(color)) * 1.5) / 200 * sqrt((c_x - x) * (c_x - x) + (c_y - y) * (c_y - y)), double(255));
    int green = std::min(double(GetGValue(color)) * 1.5 + (255 - double(GetGValue(color)) * 1.5) / 300 * sqrt((c_x - x) * (c_x - x) + (c_y - y) * (c_y - y)), double(255));
    int blue = std::min(double(GetBValue(color)) * 1.5 + (255 - double(GetBValue(color)) * 1.5) / 400 * sqrt((c_x - x) * (c_x - x) + (c_y - y) * (c_y - y)), double(255));

    txSetFillColor(RGB(red, green, blue), render.get_window()->get_hdc());
    render.draw_rectangle(coord.x, coord.y, coord.x + size.x, coord.y + size.y, color, 4);

    txSelectFont("Comic Sans MS", size.y / 2, size.x / len * 2 / 3, FW_DONTCARE, false, false, false, 0, render.get_window()->get_hdc());
    txTextOut(coord.x + size.x / 6, render.get_window()->get_size_y() - (coord.y + size.y * 3 / 4), text, render.get_window()->get_hdc());
};

void GetMouse(double& x, double& y, const Renderer& render) {
    x = txMouseX();
    y = render.get_window()->get_size_y() - txMouseY();  
    if (render.get_window()->get_type() == TYPE_TEXTURE) {
        x -= ((Texture*)render.get_window())->get_coord_x();
        y += ((Texture*)render.get_window())->get_coord_y();
    }
}

bool Button::check_boundary(const Renderer& render) const {
    double x = 0, y = 0;
    GetMouse(x, y, render);
    return (coord.x < x && (coord.x + size.x) > x) && (coord.y < y && (coord.y + size.y) > y);
};

bool Button::check_press(const Renderer& render) const {
    unsigned int press = txMouseButtons();  
    return this->check_boundary(render) && (press & 1);
}

void PhysCircle::draw(const Renderer& renderer) const {
    VirtualWindow* window = renderer.get_window();
    Coordinates* coordinates = renderer.get_coordinates(); 
    RGBQUAD* win_buf = window->get_buf();
    RGBQUAD place_color = ToRGBQUAD(color);

    int x1 = renderer.to_pixel_x(coord.x - radius);
    int x2 = renderer.to_pixel_x(coord.x + radius);
    int y1 = renderer.to_pixel_y(coord.y + radius);
    int y2 = renderer.to_pixel_y(coord.y - radius);
    
    for(int iy = y2; iy < y1; ++iy) {
        for(int ix = x1; ix < x2; ++ix) {
            int buf_pos = ix + iy * window->get_size_x();
            if (this->check_bound(renderer.to_coord_x(ix), renderer.to_coord_y(iy)) &&
                (buf_pos < window->get_size_x() * window->get_size_y()) && buf_pos >= 0) {
                win_buf[buf_pos] = place_color;
            }
        }
    }

};

void PhysRect::draw(const Renderer& renderer) const {
    VirtualWindow* window = renderer.get_window();
    Coordinates* coordinates = renderer.get_coordinates(); 
    RGBQUAD* win_buf = window->get_buf();
    RGBQUAD place_color = ToRGBQUAD(color);

    int x1 = renderer.to_pixel_x(std::max(coord1.x, coordinates->get_min_x()));
    int y1 = renderer.to_pixel_y(std::max(coord1.y, coordinates->get_min_y()));
    int x2 = renderer.to_pixel_x(std::min(coord2.x, coordinates->get_max_x()));
    int y2 = renderer.to_pixel_y(std::min(coord2.y, coordinates->get_max_y()));
    
    //fprintf(stderr, "started print %p\n", this);

    for(int iy = y1; iy < y2; ++iy) {
        for(int ix = x1; ix < x2; ++ix) {
            int buf_pos = ix + iy * window->get_size_x();
            if ((buf_pos < (window->get_size_x() * window->get_size_y())) && buf_pos >= 0) {
                win_buf[buf_pos] = place_color;
            }
        }
    }

    //fprintf(stderr, "ended print %p\n", this);
};

void Graph::draw(const Renderer& render) const {
    for(int i = 0; i < dots_count - 1; ++i) {
        render.draw_line(dots[i].x, dots[i].y, dots[i + 1].x, dots[i + 1].y);
    }
}

void Graph::draw_axis(const Renderer& render) {
    VirtualWindow* target = render.get_window();
    Coordinates* coord = render.get_coordinates();

    double size_x = target->get_size_x();
    double size_y = target->get_size_y();
    HDC hdc = target->get_hdc();

    const int edge_thickness = 2;

    txSetColor(TX_BLACK, edge_thickness, hdc);
    txSetFillColor(target->get_color(), hdc);
    txRectangle(0, 0, target->get_size_x(), target->get_size_y(), hdc);


    double right_x = size_x - size_x / 50;
    double upper_y = size_y / 50;
    double arrow_x = size_x / 30;
    double arrow_y = size_y / 30;

    double center_x = double(size_x) / 50 + fabs(coord->get_min_x()) * render.get_scale_x();
    double center_y = double(size_y) / 50 + fabs(coord->get_max_y()) * render.get_scale_y();

    txLine(size_x / 50, center_y, right_x, center_y, hdc);          //x-axis
    txLine(center_x, upper_y, center_x, size_y /*+ size_y / 50*/, hdc); //y-axis

    //arrows
    txLine(right_x, center_y, right_x - arrow_x , center_y - arrow_y, hdc);
    txLine(right_x, center_y, right_x - arrow_x , center_y + arrow_y, hdc);

    txLine(center_x, upper_y, center_x + arrow_x, upper_y + arrow_y, hdc);
    txLine(center_x, upper_y, center_x - arrow_x, upper_y + arrow_y, hdc);

    double font_x = size_x / 60;
    double font_y = size_y / 15;

    const int axis_thickness = 2;

    txSetColor(TX_BLACK, axis_thickness, hdc);
    txSelectFont("Comic Sans MS", font_y, font_x, FW_DONTCARE, false, false, false, 0, hdc);

    txTextOut(center_x - axis_thickness - font_x * 2, edge_thickness * 4 + arrow_y, "Y", hdc);
    txTextOut(size_x - edge_thickness * 4 - font_x * 1.5, center_y - axis_thickness - font_y * 1.5, "X", hdc);

    //!numbers
    char num_buff[4] = {};
    txTextOut(size_x - edge_thickness * 4 - font_x * (0.5 + NumOfDigits(coord->get_max_x())),
              center_y + axis_thickness + font_y * 0.25 + arrow_y, itoa(coord->get_max_x(), num_buff, 10), hdc);             //max_x
    txTextOut(edge_thickness * 4, center_y + axis_thickness + font_y * 0.25, itoa(coord->get_min_x(), num_buff, 10), hdc);   //min_x
    txTextOut(center_x + axis_thickness * 3 + arrow_x, edge_thickness * 2, itoa(coord->get_max_y(), num_buff, 10), hdc);     //max_y    
    txTextOut(center_x + axis_thickness * 3, size_y - edge_thickness - font_y, itoa(coord->get_min_y(), num_buff, 10), hdc); //min_y    
    txTextOut(center_x + axis_thickness * 3, center_y + axis_thickness + font_y * 0.25, "0", hdc);                           //draw "0"
};

