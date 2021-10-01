#include "TXLib.h"
#include <cmath>
#include "MathVector2D.h"

using std::rand;

const double dt = 0.03;


/*
    ������� ���������� ���� � � ��������� ��� �������

    ������� ����������� ���������� ��� Shape.
    ������� ������� ��� ������� - �����(1) + �����(1) = ��(2)
    �����(1) + ��(n) = ��(n + 1)
    ��(n) + ��(m) = n + m �����(1)
    ������� �������  (��� ������������)
    ���������� ������� � ��������
    ������� ������(������) - �������� �������
    ������� ������ ����� 2 �������: ���-�� �������, ���-�� �����������.
    ������� ������ - ������, ������� �������-��������, � � ��� ����
*/

enum WALLS {
    LEFT    = 1,
    RIGHT   = 2,
    UP      = 3,
    DOWN    = 4,
    NO_WALL = -1
};

enum OBJECT_TYPES {
    NOT_SPECIFIED = -1,
    TYPE_CIRCLE = 0,
    TYPE_RECT = 1,
    TYPE_COUNT = 2
};

class BasicPhys;
class Window;
class Renderer;
class MainWindow;
class Frame;
class VirtualWindow;

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
    Renderer(VirtualWindow* window, Coordinates* coord);

    void set_window(VirtualWindow* new_window);
    VirtualWindow* get_window() const { return window; };
    
    void set_coordinates(Coordinates* new_coordinates) { coordinates = new_coordinates; };
    Coordinates* get_coordinates() const { return coordinates; };
    
    void clear() const;
    void draw_line(double x_begin, double y_begin, double x_end, double y_end, COLORREF color = TX_BLACK, int thickness = 1) const;
    void set_pixel(double x, double y, COLORREF color) const;
    void draw_circle(double x, double y, double r, COLORREF color = TX_BLACK, int thickness = 1) const;
    void draw_rectangle(double x1, double y1, double x2, double y2, COLORREF color = TX_BLACK, int thinkness = 1) const;

    inline int to_pixel_x(double coord) const;
    inline int to_pixel_y(double coord) const;
    
    double to_coord_x(int x) const;
    double to_coord_y(int y) const;

    double get_scale_x() const { return scale.x; };
    double get_scale_y() const { return scale.y; };  

  private:
    VirtualWindow* window;
    Coordinates* coordinates;
    Pair<double> scale;
};


class BasicPhys {
  public:
    BasicPhys(double v_x, double v_y, double mass) : speed(v_x, v_y), mass(mass) {};

    virtual void move(double time) = 0;

    MathVector2D<double> get_speed();
    void set_speed(double v_x, double v_y);
    void set_speed(MathVector2D<double> new_speed);
    double get_mass() const { return mass; };
    double set_mass(double new_mass) { mass = new_mass; }; 

  protected:
    MathVector2D<double> speed;
    double mass;
};

class Shape {
  public:
    Shape(double x, double y) : coord(x, y), shape_type(NOT_SPECIFIED) {};
    virtual void draw(const Renderer& renderer) const = 0;

    double get_x() const {return coord.x;};
    double get_y() const {return coord.y;};
    int get_type() const {return shape_type;};

  protected:
    Pair<double> coord;
    int shape_type;
};

/*
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

class Rectangl: public Shape {
  public:
    Rectangl(double x1, double y1, double x2, double y2, COLORREF color);
    virtual void draw(const Renderer& renderer) const override;
    
    double get_x1() const { return coord1.x; };
    double get_y1() const { return coord1.y; }; 

    double get_x2() const { return coord2.x; };
    double get_y2() const { return coord2.y; };

    double get_size_x() const { return size.x; };
    double get_size_y() const { return size.y; };

  protected:
    Pair<double> size;
    Pair<double> coord1;
    Pair<double> coord2;

  private:
    COLORREF color;
}; */

class PhysShape : public Shape, public BasicPhys {
  public:
    PhysShape(double x, double y, double v_x, double v_y, double mass) :
        Shape(x, y), BasicPhys(v_x, v_y, mass) {};
};

class PhysCircle : public PhysShape {
  public:
    PhysCircle(double x, double y, double radius, double v_x, double v_y, double mass, COLORREF color);
    PhysCircle();
    void move(double time) override;
    virtual void draw(const Renderer& renderer) const override;
    
    double get_radius() const { return radius; };  

  private:
    inline bool check_bound(double x, double y) const;

    double radius;
    COLORREF color;
};

class PhysRect: public PhysShape {
  public:
    virtual void draw(const Renderer& renderer) const override;
    PhysRect(double x1, double y1, double x2, double y2, double v_x, double v_y, double mass, COLORREF color);
    PhysRect();
    void move(double time) override;
    
    double get_x1() const { return coord1.x; };
    double get_y1() const { return coord1.y; }; 

    double get_x2() const { return coord2.x; };
    double get_y2() const { return coord2.y; };

    double get_size_x() const { return size.x; };
    double get_size_y() const { return size.y; };

  protected:
    Pair<double> size;
    Pair<double> coord1;
    Pair<double> coord2;

  private:
    COLORREF color;
};
/*
class PhysRect: public Rectangl, public BasicPhys {
  public:
    PhysRect(double x1, double y1, double x2, double y2, double v_x, double v_y, double mass, COLORREF color);
    void move(double time) override;
};

class PhysCircle : public Circle, public BasicPhys {
  public:
    PhysCircle(double x, double y, double radius, double v_x, double v_y, double mass, COLORREF color);
    void move(double time) override;
};
*/

class Manager {
  public:
    Manager(int max_count);
    ~Manager();

    void add_figure(PhysShape* figure);
    void del_figure(const PhysShape* figure);
    void del_last();

    PhysShape* get_figure(int pos) const { return figures[pos]; };
    int get_count() const { return count; };

    void draw_all(const Renderer& renderer) const;
    void move_all(double time) const;

  private:
    int count;
    int size;
    PhysShape** figures;
};


class Frame {
  public:
    Frame(int x_size, int y_size) : size(x_size, y_size) {};

    int get_size_x() const;
    int get_size_y() const;

  protected:
    Pair<int> size;
};

class VirtualWindow : public Frame {
  public:
    VirtualWindow(int x_size, int y_size, COLORREF color);
    
    HDC get_hdc() const;
    RGBQUAD* get_buf() const;
    COLORREF get_color() const;

  protected:
    HDC screen;
    RGBQUAD* buf_screen;
    COLORREF color;
};

class Window : public VirtualWindow {
  public:
    Window(int x_size, int y_size, COLORREF color);
};

class Texture : public VirtualWindow {
  public:
    Texture(int x_size, int y_size, COLORREF color, int coord_x, int coord_y);
    ~Texture();
    
    int get_coord_x() const;
    int get_coord_y() const;
    
    void show_on(const HDC& target) const;

  private:
    Pair<int> coord;
};



class Button : public Frame {
  public:
    Button(int size_x, int size_y, int x, int y);

    int get_coord_x() const { return coord.x; };
    int get_coord_y() const { return coord.y; };
    
    virtual void draw(const Renderer& render) const = 0;

  protected:
    Pair<int> coord;
};

class TextButton : public Button {
  public:
    TextButton(int x_size, int y_size, int x, int y, COLORREF color, char* text);
    TextButton();
    TextButton(const TextButton&);
    ~TextButton();
    //TextButton& operator=(const TextButton& source);

    COLORREF get_color() const { return color; };
    const char* get_text() const { return text; };
    int get_len() const { return len; };
    
    bool check_press() const;
    void make_action() const;
    void draw(const Renderer& render) const override;

  private:
    COLORREF color;
    char* text;
    int len;
};

class ButtonManager {
  public:
    ButtonManager(int max_count);
    ~ButtonManager();

    void add_button(Button* button);
    void del_button(const Button* button);
    void del_last();

    Button* get_button(int pos) const { return buttons[pos]; };
    int get_count() const { return count; };

    void draw_all(const Renderer& renderer) const;
    void check_press() const;

  private:
    int count;
    int size;
    Button** buttons;
};


class Graph {
public:
    Graph(double max_x, double min_x, double step, double (*func)(double x));
    Graph(Pair<double>* dots, int count);
    ~Graph();

    void draw(const Renderer& render) const;
    void draw_axis(const Renderer& render);
    void print_dots() const;

private:
    void calc_dots();

    Pair<double> min;
    Pair<double> max;
    double (*func)(double x);
    double step;
    Pair<double>* dots;
    int dots_count;
};


void ProceedMoving(Manager& manager, Renderer& render);

RGBQUAD ToRGBQUAD(COLORREF color);

void CheckCollision_CC(PhysShape* lhs, PhysShape* rhs, Manager& manager);
void CheckCollision_CR(PhysShape* lhs, PhysShape* rhs, Manager& manager);
void CheckCollision_RC(PhysShape* lhs, PhysShape* rhs, Manager& manager);
void CheckCollision_RR(PhysShape* lhs, PhysShape* rhs, Manager& manager);

void ProceedCollision(PhysCircle* lhs, PhysCircle* rhs);
int CheckWall_C(PhysShape* object, const Coordinates* coord);
int CheckWall_R(PhysShape* object, const Coordinates* coord);
void ProceedWall(PhysShape* object, const Coordinates* coord, int wall_type);

void CheckAllCollisions(Manager& manager, Coordinates* coord);