#include "TXLib.h"
#include <cmath>
#include "MathVector2D.h"

const double dt = 0.03;

enum WALLS {
    LEFT    = 1,
    RIGHT   = 2,
    UP      = 3,
    DOWN    = 4,
    NO_WALL = -1
};

class Window;
class Renderer;
class MainWindow;
class BasicWindow;

template <typename T>
class Pair {
public:
    Pair() = default;
    Pair(T x, T y) : x(x), y(y) {};
    T x;
    T y;
};



class Coordinates {
public:
    Coordinates(double min_x, double max_x, double min_y, double max_y) : max(max_x, max_y), min(min_x, min_y) {};

    int check_x(double x) const;
    int check_y(double y) const;
    
    double get_max_x() const {return max.x;};    
    double get_min_x() const {return min.x;};
    double get_max_y() const {return max.y;};    
    double get_min_y() const {return min.y;};

private:
    Pair<double> max;
    Pair<double> min;  
};

class Renderer {
public:
    Renderer(Window* window, Coordinates* coord);

    void set_window(Window* new_window);
    Window* get_window() const { return window; };
    
    void set_coordinates(Coordinates* new_coordinates) { coordinates = new_coordinates; };
    Coordinates* get_coordinates() const {return coordinates; };
    
    void clear() const;
    void draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color = TX_BLACK, int thickness = 1) const;
    void set_pixel(double x, double y, COLORREF color) const;
    void draw_circle(double x, double y, double r, COLORREF color = TX_BLACK, int thickness = 1) const;
    void draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color = TX_BLACK, int thinkness = 1) const;

    inline int to_pixel_x(double coord) const;
    inline int to_pixel_y(double coord) const;
    
    double to_coord_x(int x) const;
    double to_coord_y(int y) const;

private:
    Window* window;
    Coordinates* coordinates;
    Pair<double> scale;
};



class Shape {
public:
    Shape(int x, int y) : coord(x, y) {};
    virtual void draw(const Renderer& renderer) const = 0;

    double get_x() const {return coord.x;};
    double get_y() const {return coord.y;};

protected:
    Pair<double> coord;
};

class Circle : public Shape {
public:
    Circle(double x, double y, double raduis, COLORREF color);
    virtual void draw(const Renderer& renderer) const override;
    
    double get_radius() const { return radius; };  

private:
    inline bool check_bound(double x, double y) const;

    double radius;
    COLORREF color;
};

class Bubble : public Circle {
public:
    Bubble(double x, double y, double radius, double v_x, double v_y, double mass, COLORREF color);

    void move(double time);
    MathVector2D<double> get_speed();
    
    void set_speed(double v_x, double v_y);
    void set_speed(MathVector2D<double> new_speed);

private:    
    MathVector2D<double> speed;
    double mass;
};



class Manager {
public:
    Manager(int max_count);
    ~Manager();

    void add_figure(Shape* figure);
    void del_figure(const Shape* figure);
    void del_last();

    Shape* get_figure(int pos) const { return figures[pos]; };
    int get_count() const { return count; };

    void draw_all(const Renderer& renderer) const;
    void move_all(double time) const;

private:
    int count;
    int size;
    Shape** figures;
};



class BasicWindow {
public:
    BasicWindow(int x_size, int y_size) : size(x_size, y_size) {};

    int get_size_x() const;
    int get_size_y() const;

private:
    Pair<int> size;
};

class Window : public BasicWindow {
public:
    Window(int x_size, int y_size, COLORREF color);
    
    HDC get_hdc() const;
    RGBQUAD* get_buf() const;
    COLORREF get_color() const;

private:
    HDC screen;
    RGBQUAD* buf_screen;
    COLORREF color;
};

class Texture : public BasicWindow {
public:
    Texture(int x_size, int y_size, COLORREF color, int coord_x, int coord_y);
    ~Texture();

    HDC get_hdc() const;
    RGBQUAD* get_buf() const;
    COLORREF get_color() const;
    
    int get_coord_x() const;
    int get_coord_y() const;
    
    void show_on(const HDC& target) const;

private:
    Pair<int> coord;
    HDC screen;
    RGBQUAD* buf_screen;
    COLORREF color;
};


bool CheckCollision(const Bubble* lhs, const Bubble* rhs);
void ProceedCollision(Bubble* lhs, Bubble* rhs);
int CheckBounceWall(const Bubble* bubble, const Coordinates* coord);
void ProceedBounceWall(Bubble* bubble, const Coordinates* coord, int wall_type);

void ProceedMoving(Manager& manager, Renderer& render);
RGBQUAD ToRGBQUAD(COLORREF color);