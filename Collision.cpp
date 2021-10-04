#include "sphere.h"

bool Test_CC(PhysCircle* lhs, PhysCircle* rhs) {
    double distance = (lhs->get_x() - rhs->get_x()) * (lhs->get_x() - rhs->get_x()) +
                      (lhs->get_y() - rhs->get_y()) * (lhs->get_y() - rhs->get_y());

    return distance <= ((lhs->get_radius() + rhs->get_radius()) * (lhs->get_radius() + rhs->get_radius()));
};

void CheckCollision_CC(PhysShape* p_lhs, PhysShape* p_rhs, Manager& manager) {
    PhysCircle* lhs = (PhysCircle*) p_lhs;
    PhysCircle* rhs = (PhysCircle*) p_rhs;

    if (Test_CC(lhs, rhs)) {
        //fprintf(stderr, "Here!\n");

        //std::cerr << "start speed: lhs: " << lhs->get_speed() << " rhs: " << rhs->get_speed() << "\n";

        MathVector2D<double> v1 = lhs->get_speed();
        MathVector2D<double> v2 = rhs->get_speed();

        double x1 = lhs->get_x(), y1 = lhs->get_y(); 
        double x2 = rhs->get_x(), y2 = rhs->get_y();

        double lhs_mass = lhs->get_mass();
        double rhs_mass = rhs->get_mass();

        double energy_total = v1 * v1 * lhs_mass / 2 + v2 * v2 * rhs_mass / 2;
        double energy_activation = 0;

        //checking enough energy
        if ((energy_total) < energy_activation) {
            double dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
            double cos = (y1 - y2) / dist, sin = (x2 - x1) / dist;

            MathVector2D<double> t(cos, sin);
            MathVector2D<double> n(sin, -cos);

            lhs->set_speed(t * (v1 * t) + n * (v2 * n));
            rhs->set_speed(t * (v2 * t) + n * (v1 * n));

            //std::cerr << "final speed: lhs: " << lhs->get_speed() << " rhs: " << rhs->get_speed() << "\n";

            while(Test_CC(lhs, rhs)) {  
                lhs->move(dt);
                rhs->move(dt);
            }
        }
        else {
            double avg_r = (lhs->get_radius() + rhs->get_radius());
            PhysRect* rect = new PhysRect(std::min(x1, x2), std::min(y1, y2), std::min(x1, x2) + avg_r, std::min(y1, y2) + avg_r,
                                          v1.get_x() + v2.get_x(), v1.get_y() + v2.get_y(), lhs_mass + rhs_mass, RGB(255, int(energy_total) % 256, 0));
            manager.del_figure(p_lhs);
            manager.del_figure(p_rhs);
            manager.add_figure(rect);
        }
    };
};

bool Test_CR(PhysCircle* lhs, PhysRect* rhs) {
    return ((abs(rhs->get_x() - lhs->get_radius() - lhs->get_x()) < (rhs->get_size_x() / 2)) ||
            (abs(lhs->get_x() - lhs->get_radius() - rhs->get_x()) < (rhs->get_size_x() / 2))) && 
           ((abs(lhs->get_y() - lhs->get_radius() - rhs->get_y()) < (rhs->get_size_y() / 2)) ||
            (abs(rhs->get_y() - lhs->get_radius() - lhs->get_y()) < (rhs->get_size_y() / 2)));
};

void CheckCollision_CR(PhysShape* p_lhs, PhysShape* p_rhs, Manager& manager) {
    PhysCircle* lhs = (PhysCircle*) p_lhs;
    PhysRect* rhs = (PhysRect*) p_rhs;

    if (Test_CR(lhs, rhs)) {
        rhs->set_mass(rhs->get_mass() + lhs->get_mass());
        rhs->set_new_size(rhs->get_size_x() * 11 / 10, rhs->get_size_y() * 11 / 10);
        manager.del_figure(lhs);
    }
};

void CheckCollision_RC(PhysShape* lhs, PhysShape* rhs, Manager& manager) {
    CheckCollision_CR(rhs, lhs, manager);
};

bool Test_RR(PhysRect* lhs, PhysRect* rhs) {
    return ((abs(rhs->get_x() - lhs->get_x2()) < (rhs->get_size_x() / 2)) ||
            (abs(lhs->get_x1() - rhs->get_x()) < (rhs->get_size_x() / 2))) && 
           ((abs(lhs->get_y1() - rhs->get_y()) < (rhs->get_size_y() / 2)) ||
            (abs(rhs->get_y() - lhs->get_y2()) < (rhs->get_size_y() / 2)));
}

void CheckCollision_RR(PhysShape* p_lhs, PhysShape* p_rhs, Manager& manager) {
    PhysRect* lhs = (PhysRect*) p_lhs;
    PhysRect* rhs = (PhysRect*) p_rhs;
    
    if (Test_RR(lhs, rhs)) {
        fprintf(stderr, "collided R:%p with R:%p\n", lhs, rhs);
        
        int count = lhs->get_mass() + rhs->get_mass();
        MathVector2D<double> v1 = lhs->get_speed();
        MathVector2D<double> v2 = rhs->get_speed();

        double max_v_x = v1.get_x() + v2.get_x();
        double max_v_y = v1.get_y() + v2.get_y();

        double x = (rhs->get_x() + lhs->get_x()) / 2;
        double y = (rhs->get_y() + lhs->get_y()) / 2;
        double r = max(max(rhs->get_size_x(), rhs->get_size_y()), max(lhs->get_size_x(), lhs->get_size_y()));

        manager.del_figure(lhs);
        manager.del_figure(rhs);

        fprintf(stderr, "count: %d, r: %lg, x: %lg, y: %lg", count, r, x, y);

        double angle = 0;
        double d_angle = 2 * 3.1415 / count;
        for (int i = 0; i < count; ++i) {
            PhysCircle* tmp = new PhysCircle(x + r * cos(angle), y + r * sin(angle), r / 3, 1 * cos(angle), 1 * sin(angle), 1, RGB(128, 128, rand() % 256));
            angle += d_angle;
            manager.add_figure(tmp);
        }   

        /*
        double x1 = lhs->get_x(), y1 = lhs->get_y(); 
        double x2 = rhs->get_x(), y2 = rhs->get_y();
        double dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        double cos = (y1 - y2) / dist, sin = (x2 - x1) / dist;

        MathVector2D<double> t(cos, sin);
        MathVector2D<double> n(sin, -cos);

        lhs->set_speed(t * (v1 * t) + n * (v2 * n));
        rhs->set_speed(t * (v2 * t) + n * (v1 * n));

        while(Test_RR(lhs, rhs)) {  
            lhs->move(dt);
            rhs->move(dt);
        }
        */
        //fprintf(stderr, "end collision!\n");
    }
};

/*
void ProceedCollision(PhysCircle* lhs, PhysCircle* rhs) {
    MathVector2D<double> v1 = lhs->get_speed();
    MathVector2D<double> v2 = rhs->get_speed();

    //std::cerr << "start speed: lhs: " << lhs->get_speed() << " rhs: " << rhs->get_speed() << "\n";

    double x1 = lhs->get_x(), y1 = lhs->get_y(); 
    double x2 = rhs->get_x(), y2 = rhs->get_y();
    double dist = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    double cos = (y1 - y2) / dist, sin = (x2 - x1) / dist;

    MathVector2D<double> t(cos, sin);
    MathVector2D<double> n(sin, -cos);

    lhs->set_speed(t * (v1 * t) + n * (v2 * n));
    rhs->set_speed(t * (v2 * t) + n * (v1 * n));   

    //std::cerr << "final speed: lhs: " << lhs->get_speed() << " rhs: " << rhs->get_speed() << "\n";
}; 
*/

int CheckWall_C(PhysShape* p_object, const Coordinates* coord) {
    PhysCircle* object = (PhysCircle*) p_object;

    if ((object->get_x() - object->get_radius()) < coord->get_min_x()) {
        return LEFT;
    }
    if ((object->get_x() + object->get_radius()) > coord->get_max_x()) {
        return RIGHT;
    }
    if ((object->get_y() - object->get_radius()) < coord->get_min_y()) {
        return DOWN;
    }
    if ((object->get_y() + object->get_radius()) > coord->get_max_y()) {
        return UP;
    }
    return NO_WALL;
};

int CheckWall_R(PhysShape* p_object, const Coordinates* coord) {
    PhysRect* object = (PhysRect*) p_object;

    if (object->get_x1() < coord->get_min_x()) {
        return LEFT;
    }
    if (object->get_x2() > coord->get_max_x()) {
        return RIGHT;
    }
    if (object->get_y1() < coord->get_min_y()) {
        return DOWN;
    }
    if (object->get_y2() > coord->get_max_y()) {
        return UP;
    }
    return NO_WALL;
};

void ProceedWall(PhysShape* object, const Coordinates* coord, int wall_type) {
    MathVector2D<double> curr_speed = object->get_speed();

    switch(wall_type) {
        case UP:
            object->set_speed(curr_speed.get_x(), -curr_speed.get_y());
            break;

        case DOWN:
            object->set_speed(curr_speed.get_x(), -curr_speed.get_y());
            break;

        case LEFT:
            object->set_speed(-curr_speed.get_x(), curr_speed.get_y());
            break;

        case RIGHT:
            object->set_speed(-curr_speed.get_x(), curr_speed.get_y());
            break; 
    }

    object->move(dt);
};