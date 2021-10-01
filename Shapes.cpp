#include "sphere.h"

/*
PhysCircle::PhysCircle(double x, double y, double radius, COLORREF color) : 
    Shape(x, y), radius(radius), color(color) {
    shape_type = TYPE_CIRCLE;
};
*/

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

bool PhysCircle::check_bound(double x, double y) const {
    return (((x - coord.x) * (x - coord.x) + (y - coord.y) * (y - coord.y)) <= (radius * radius));
};

PhysCircle::PhysCircle(double x, double y, double radius, double v_x, double v_y, double mass, COLORREF color) :
    PhysShape(x, y, v_x, v_y, mass), color(color), radius(radius) {
    shape_type = TYPE_CIRCLE;
};

PhysCircle::PhysCircle() :
    PhysShape(1, 1, 1, 1, 1), color(TX_WHITE), radius(1) {
    shape_type = TYPE_CIRCLE;     
};

void PhysCircle::move(double time) {
    coord.x += speed.get_x() * time;
    coord.y += speed.get_y() * time;
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

PhysRect::PhysRect(double x1, double y1, double x2, double y2, double v_x, double v_y, double mass, COLORREF color) :
    PhysShape((x1 + x2) / 2, (y1 + y2) / 2, v_x, v_y, mass), coord1(x1, y1), coord2(x2, y2), color(color), size(x2 - x1, y2 - y1) {
    shape_type = TYPE_RECT;
};

void PhysRect::move(double time) {
    coord.x += speed.get_x() * time;
    coord.y += speed.get_y() * time;

    coord1.x += speed.get_x() * time;
    coord1.y += speed.get_y() * time;

    coord2.x += speed.get_x() * time;
    coord2.y += speed.get_y() * time;
};

MathVector2D<double> BasicPhys::get_speed() {
    return speed;
};

void BasicPhys::set_speed(double v_x, double v_y) {
    speed.set_x(v_x);
    speed.set_y(v_y);
};

void BasicPhys::set_speed(MathVector2D<double> new_speed) {
    speed = new_speed;
}

void CheckAllCollisions(Manager& manager, Coordinates* coord) {
    typedef int (*WallsChecker)(PhysShape* object, const Coordinates* coord);
    static WallsChecker WallsFuncs[TYPE_COUNT] = {CheckWall_C, CheckWall_R};

    typedef void (*ObjectCollisions)(PhysShape* lhs, PhysShape* rhs);
    static ObjectCollisions CollisionFuncs[TYPE_COUNT][TYPE_COUNT] = {
      {&CheckCollision_CC, &CheckCollision_CR},
      {&CheckCollision_RC, &CheckCollision_RR}
    };

    for (int i = 0; i < manager.get_count(); ++i) {
        PhysShape* figure = manager.get_figure(i);
        
        int type_lhs = figure->get_type();
        int wall_check = WallsFuncs[type_lhs](figure, coord);
    
        if (wall_check != NO_WALL) {
            ProceedWall(figure, coord, wall_check);
        }

        
        for (int j = 0; j < manager.get_count(); ++j) {
            if (i != j) {
                PhysShape* figure_coll = manager.get_figure(j);
                int type_rhs = figure_coll->get_type();    
                //fprintf(stderr, "Collided lhs: %d, rhs: %d\n", i, j);
                CollisionFuncs[type_lhs][type_rhs](figure, figure_coll);
            }
        }
        
    }
};

RGBQUAD ToRGBQUAD(COLORREF color) {
    return {GetBValue(color), GetGValue(color), GetRValue(color), 0};
};