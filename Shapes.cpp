#include "sphere.h"

/*
PhysCircle::PhysCircle(double x, double y, double radius, COLORREF color) : 
    Shape(x, y), radius(radius), color(color) {
    shape_type = TYPE_CIRCLE;
};
*/

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

PhysRect::PhysRect(double x1, double y1, double x2, double y2, double v_x, double v_y, double mass, COLORREF color) :
    PhysShape((x1 + x2) / 2, (y1 + y2) / 2, v_x, v_y, mass), coord1(x1, y1), coord2(x2, y2), color(color), size(x2 - x1, y2 - y1) {
    shape_type = TYPE_RECT;
};

PhysRect::PhysRect() : coord1(0, 0), coord2(1, 1), color(TX_RED), size(1, 1), PhysShape(1 / 2, 1 / 2, 1, 1, 1) {
    shape_type = TYPE_RECT;
}

void PhysRect::move(double time) {
    coord.x += speed.get_x() * time;
    coord.y += speed.get_y() * time;

    coord1.x += speed.get_x() * time;
    coord1.y += speed.get_y() * time;

    coord2.x += speed.get_x() * time;
    coord2.y += speed.get_y() * time;
};

double PhysRect::set_new_size(double x, double y) {
    size.x = x;
    size.y = y;
    coord2.x = coord1.x + x;
    coord2.y = coord1.y + y;
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

    typedef void (*ObjectCollisions)(PhysShape* lhs, PhysShape* rhs, Manager& manager);
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
                CollisionFuncs[type_lhs][type_rhs](figure, figure_coll, manager);
            }
        }
        
    }
};

RGBQUAD ToRGBQUAD(COLORREF color) {
    return {GetBValue(color), GetGValue(color), GetRValue(color), 0};
};