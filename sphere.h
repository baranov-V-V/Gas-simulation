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
    Window* get_window();
    Window* get_window() const { return window; };
    
    void set_coordinates(Coordinates* new_coordinates) { coordinates = new_coordinates; };
    Coordinates* get_coordinates() {return coordinates; };
    Coordinates* get_coordinates() const {return coordinates; };

    void show_on(const MainWindow& target) const;
    
    void clear() const;
    void draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color = TX_BLACK, int thickness = 1);
    void set_pixel(double x, double y, COLORREF color);
    void draw_circle(double x, double y, double r, COLORREF color = TX_BLACK, int thickness = 1);
    void draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color = TX_BLACK, int thinkness = 1);

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
    Shape();
    Shape(int x, int y) : coord(x, y) {};
    virtual void draw(const Renderer& renderer) const = 0;
    virtual void move(double time) = 0;

    double get_x() {return coord.x;};
    double get_y() {return coord.y;};

    double get_x() const {return coord.x;};
    double get_y() const {return coord.y;};

protected:
    Pair<double> coord;
};

class Circle : public Shape {
public:
    Circle(double x, double y, double raduis, RGBQUAD color);
    virtual void draw(const Renderer& renderer) const override;
    
    double get_radius() {return radius;};
    double get_radius() const {return radius;};  

private:
    inline bool check_bound(double x, double y) const;

    double radius;
    RGBQUAD color;
};

class Bubble : public Circle {
public:
    Bubble(double x, double y, double radius, double v_x, double v_y, double mass, RGBQUAD color);

    void move(double time);
    MathVector2D<double> get_speed();
    
    void set_speed(double v_x, double v_y);
    void set_speed(MathVector2D<double>& new_speed) {
        speed = new_speed;
    }

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

    Shape* get_figure(int pos) { return figures[pos]; };
    int get_count() { return count; };

    void draw_all(const Renderer& renderer);
    void move_all(double time);

private:
    int count;
    int size;
    Shape** figures;
};

class Window {
public:
    Window(int x_size, int y_size, COLORREF color, int coord_x, int coord_y);
    Window();
    ~Window();

    HDC get_hdc() const;
    
    int get_size_x() const;
    int get_size_y() const;
    int get_coord_x() const;
    int get_coord_y() const;
    COLORREF get_color() const;
    RGBQUAD* get_buf() const;

    void show_on(const HDC& target) const;

private:
    HDC screen;
    RGBQUAD* buf_screen;
    COLORREF color;
    Pair<int> size;
    Pair<int> coord;
};

class MainWindow {
public:
    MainWindow(int x_size, int y_size);

    HDC get_hdc() const;
    RGBQUAD* get_buf() const;

    int get_size_x() const;
    int get_size_y() const;

private:
    HDC screen;
    RGBQUAD* buf_screen;
    Pair<int> size;
};

bool CheckCollision(const Bubble* lhs, const Bubble* rhs);
void ProceedCollision(Bubble* lhs, Bubble* rhs);

int CheckBounceWall(const Bubble* bubble, const Coordinates* coord);
void ProceedBounceWall(Bubble bubble, int wall_type);

void ProceedMoving(Manager& manager, Renderer& render, MainWindow& window);