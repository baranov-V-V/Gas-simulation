#include "sphere.h"

Circle::Circle(double x, double y, double radius, COLORREF color) : 
    Shape(x, y), radius(radius), color(color) {};

void Circle::draw(const Renderer& renderer) const {
    Window* window = renderer.get_window();
    Coordinates* coordinates = renderer.get_coordinates(); 
    RGBQUAD* win_buf = window->get_buf();
    RGBQUAD place_color = ToRGBQUAD(color);

    int x1 = renderer.to_pixel_x(coord.x - radius);
    int x2 = renderer.to_pixel_x(coord.x + radius);
    int y1 = renderer.to_pixel_y(coord.y + radius);
    int y2 = renderer.to_pixel_y(coord.y - radius);
    
    for(int iy = y2; iy < y1; ++iy) {
        for(int ix = x1; ix < x2; ++ix) {
            if (this->check_bound(renderer.to_coord_x(ix), renderer.to_coord_y(iy)) &&
                ix + iy * window->get_size_x() < window->get_size_x() * window->get_size_y()) {
                win_buf[ix + iy * window->get_size_x()] = place_color;
            }
        }
    }

};

bool Circle::check_bound(double x, double y) const {
    return (((x - coord.x) * (x - coord.x) + (y - coord.y) * (y - coord.y)) <= (radius * radius));
};

Bubble::Bubble(double x, double y, double radius, double v_x, double v_y, double mass, COLORREF color) :
    Circle(x, y, radius, color), speed(v_x, v_y), mass(mass) {};

void Bubble::move(double time) {
    coord.x += speed.get_x() * time;
    coord.y += speed.get_y() * time;
};

MathVector2D<double> Bubble::get_speed() {
    return speed;
};

void Bubble::set_speed(double v_x, double v_y) {
    speed.set_x(v_x);
    speed.set_y(v_y);
};

void Bubble::set_speed(MathVector2D<double> new_speed) {
    speed = new_speed;
}

bool CheckCollision(const Bubble* lhs, const Bubble* rhs) {
    double distance = (lhs->get_x() - rhs->get_x()) * (lhs->get_x() - rhs->get_x()) +
                      (lhs->get_y() - rhs->get_y()) * (lhs->get_y() - rhs->get_y());

    return distance <= (lhs->get_radius() + rhs->get_radius()) * (lhs->get_radius() + rhs->get_radius());
};

void ProceedCollision(Bubble* lhs, Bubble* rhs) {
    MathVector2D<double> v1 = lhs->get_speed();
    MathVector2D<double> v2 = rhs->get_speed();

    std::cerr << "start speed: lhs: " << lhs->get_speed() << " rhs: " << rhs->get_speed() << "\n";

    double x1 = lhs->get_x(), y1 = lhs->get_y(); 
    double x2 = rhs->get_x(), y2 = rhs->get_y();
    double dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    double cos = (y1 - y2) / dist, sin = (x2 - x1) / dist;

    MathVector2D<double> t(cos, sin);
    MathVector2D<double> n(sin, -cos);

    lhs->set_speed(t * (v1 * t) + n * (v2 * n));
    rhs->set_speed(t * (v2 * t) + n * (v1 * n));   

    std::cerr << "final speed: lhs: " << lhs->get_speed() << " rhs: " << rhs->get_speed() << "\n";

    while(CheckCollision(lhs, rhs)) {
        lhs->move(dt);
        rhs->move(dt);
    }
};

int CheckBounceWall(const Bubble* bubble, const Coordinates* coord) {
    
    //fprintf(stderr, "max_y: %lf\n", coord->get_max_y());

    if ((bubble->get_x() - bubble->get_radius()) < coord->get_min_x()) {
        return LEFT;
    }
    if ((bubble->get_x() + bubble->get_radius()) > coord->get_max_x()) {
        return RIGHT;
    }
    if ((bubble->get_y() - bubble->get_radius()) < coord->get_min_y()) {
        return DOWN;
    }
    if ((bubble->get_y() + bubble->get_radius()) > coord->get_max_y()) {
        return UP;
    }
    return NO_WALL;
}

void ProceedBounceWall(Bubble* bubble, const Coordinates* coord, int wall_type) {
    MathVector2D<double> curr_speed = bubble->get_speed();

    switch(wall_type) {
        case UP:
            bubble->set_speed(curr_speed.get_x(), -curr_speed.get_y());
            break;

        case DOWN:
            bubble->set_speed(curr_speed.get_x(), -curr_speed.get_y());
            break;

        case LEFT:
            bubble->set_speed(-curr_speed.get_x(), curr_speed.get_y());
            break;

        case RIGHT:
            bubble->set_speed(-curr_speed.get_x(), curr_speed.get_y());
            break; 
    }

    while(CheckBounceWall(bubble, coord) != NO_WALL) {
        bubble->move(dt);
    }

};

void CheckAllCollisions(Manager& manager, const Coordinates* coord) {
    for (int i = 0; i < manager.get_count(); ++i) {
        Bubble* figure = (Bubble*) manager.get_figure(i);
        int wall_check = CheckBounceWall(figure, coord);
    
        if (wall_check != NO_WALL) {
            ProceedBounceWall(figure, coord, wall_check);
        }

        for (int j = 0; j < manager.get_count(); ++j) {
            Bubble* figure_coll = (Bubble*)manager.get_figure(j);
            if (i != j && CheckCollision(figure, figure_coll)) {    
                //fprintf(stderr, "Collided lhs:%d, rhs:%d\n", i, j);
                ProceedCollision(figure, figure_coll);
            }
        }
    }
};

void ProceedMoving(Manager& manager, Renderer& render) {
    double time = dt;

    txBegin();

    while (!GetAsyncKeyState(VK_ESCAPE)) {
        CheckAllCollisions(manager, render.get_coordinates());
        render.clear();
        manager.draw_all(render);
        manager.move_all(time);
        txSleep(30);
    }
    
    txEnd();
};

RGBQUAD ToRGBQUAD(COLORREF color) {
    return {GetBValue(color), GetGValue(color), GetRValue(color), 0};
};