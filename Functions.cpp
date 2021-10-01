#include "sphere.h"

int NumOfDigits(int num) {
    int count = 0;
    while (num != 0) {
        ++count;
        num /= 10;
    }
    return count;
}

Graph::Graph(double max_x, double min_x, double step, double (*func)(double x)) {
    assert(max_x > min_x);

    Graph::max.x = max_x;
    Graph::min.x = min_x;
    Graph::step = step;
    Graph::func = func;

    Graph::dots = new Pair<double>[int((max_x - min_x) / step) + 1]; 
    this->calc_dots();   
};

Graph::Graph(Pair<double>* dots, int count) {
    for(int i = 0; i < count; ++i) {
        if (dots[i].x > max.x) {
            max.x = dots[i].x;
        }
        if (dots[i].x < min.x) {
            min.x = dots[i].x;
        }
        if (dots[i].y > max.y) {
            max.y = dots[i].y;
        }
        if (dots[i].y < min.y) {
            min.y = dots[i].y;
        }
    }
    Graph::dots = dots;
    Graph::dots_count = count;
};

Graph::~Graph() {

};

void Graph::calc_dots(){
    double curr_x = min.x;
    double curr_y = func(curr_x);
    double next_x = min.x + step;
    double next_y = func(next_x);

    int i = 0;
    while(curr_x < max.x) {
        dots[i++] = {curr_x, curr_y};

        curr_x = next_x;
        curr_y = next_y;

        next_x = curr_x + step;
        next_y = func(next_x);
    }

    dots_count = i;
}

void Graph::print_dots() const {
    printf("|     x      |     y      |\n");
    for (int i = 0; i < dots_count; ++i) {
        printf("|%-12.6lf|%-12.6lf|\n", dots[i].x, dots[i].y);
    }
}

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

double Sqr(double num) {
    return num * num;
}

double Func1(double x) {
    return x * x * x * x - 3 * x * x * x + x * x - 20;
}

double Func2(double x) {
    return x * x - 3 * x - 10;
}

