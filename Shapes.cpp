#include "sphere.h"

Circle::Circle(double x, double y, double radius, RGBQUAD color) : 
    Shape(x, y), radius(radius), color(color) {};

void Circle::draw(const Renderer& renderer) const {
    Window* window = renderer.get_window();
    Coordinates* coordinates = renderer.get_coordinates(); 
    RGBQUAD* win_buf = window->get_buf();

    //fprintf(stderr, "here!");

    int x1 = renderer.to_pixel_x(coord.x - radius);
    int x2 = renderer.to_pixel_x(coord.x + radius);
    int y1 = renderer.to_pixel_y(coord.y + radius);
    int y2 = renderer.to_pixel_y(coord.y - radius);

    //fprintf(stderr, "x1: %d, y1: %d\nx2: %d, y2: %d\n", x1, y1, x2, y2);

    //double abb = renderer.to_coord_x(x1);
    
    for(int iy = y2; iy < y1; ++iy) {
        for(int ix = x1; ix < x2; ++ix) {
            if (this->check_bound(renderer.to_coord_x(ix), renderer.to_coord_y(iy)) &&
                ix + iy * window->get_size_x() < window->get_size_x() * window->get_size_y()) {
                //fprintf(stderr, "x: %d, y: %d\n", ix, iy);
                win_buf[ix + iy * window->get_size_x()] = color;
            }
        }
    }

};

bool Circle::check_bound(double x, double y) const {
    return (((x - coord.x) * (x - coord.x) + (y - coord.y) * (y - coord.y)) <= (radius * radius));
};

Bubble::Bubble(double x, double y, double radius, double v_x, double v_y, double mass, RGBQUAD color) :
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

    MathVector2D<double> tmp1 = t * (v1 * t) + n * (v2 * n);
    MathVector2D<double> tmp2 = t * (v2 * t) + n * (v1 * n);

    lhs->set_speed(tmp1);
    rhs->set_speed(tmp2);
    
    /*
    double v1_x = cos * (v1 * t) + sin * (v2 * n);

    fprintf(stderr, "v1_x: %lg, scalar_v1*t: %lg, scalar_v2*n: %lg\n", v1_x, (v1 * t), (v2 * n));
    std::cerr << "v1_x: " << v1_x << "scalar_v1*t: " << v1 * t << "scalar_v2*n: " << v2 * n << "\n";
    fprintf(stderr, "x1: %lg, y1: %lg\n", x1, y1);
    fprintf(stderr, "x2: %lg, y2: %lg\n", x2, y2);

    std::cerr << "cos: " << cos << " sin: " << sin << "\n";
    std::cerr << "tmp1: " << tmp1 << " tmp2: " << tmp2 << "\n";

    //lhs->set_speed((v1 * t) * cos + (v2 * n) * sin, (v1 * t) * sin - (v2 * n) * cos);
    //rhs->set_speed((v2 * t) * cos + (v1 * n) * sin, (v2 * t) * sin - (v1 * n) * cos);
    */

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

void ProceedBounceWall(Bubble* bubble, int wall_type) {
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

    bubble->move(dt);
};

void CheckAllCollisions(Manager& manager, const Coordinates* coord) {
    for (int i = 0; i < manager.get_count(); ++i) {
        Bubble* figure = (Bubble*)manager.get_figure(i);
        int wall_check = CheckBounceWall(figure, coord);
        
        //fprintf(stderr, "wall status: %d\n", wall_check);
        //fprintf(stderr, "x: %lf, y: %lf\n", figure->get_x(), figure->get_y());

        if (wall_check != NO_WALL) {
            ProceedBounceWall(figure, wall_check);
        }

        bool is_collided = false;
        for (int j = 0; j < manager.get_count(); ++j) {
            Bubble* figure_coll = (Bubble*)manager.get_figure(j);
            if (i != j && CheckCollision(figure, figure_coll)) {

                fprintf(stderr, "Collided lhs:%d, rhs:%d\n", i, j);

                ProceedCollision(figure, figure_coll);
                is_collided = true;
            }
        }

        if(is_collided) {
            break;
        }
    }
};

void ProceedMoving(Manager& manager, Renderer& render, MainWindow& window) {
    double time = dt;

    //manager.draw_all(render);
    //render.show_on(window);

    while (!GetAsyncKeyState(VK_ESCAPE)) {
        manager.draw_all(render);
        manager.move_all(time);
        render.show_on(window);
        render.clear();
        CheckAllCollisions(manager, render.get_coordinates());
        txSleep(30);
    }
    
};


/*
uint8_t GetB(uint32_t color)
{
    return color & 0xFF;
}

uint8_t GetG(uint32_t color)
{
    return (color >> kByteSize) & 0xFF;
}

uint8_t GetR(uint32_t color)
{
    return (color >> (2 * kByteSize)) & 0xFF;
}

uint8_t GetA(uint32_t color)
{
    return (color >> (3 * kByteSize)) & 0xFF;
}
*/