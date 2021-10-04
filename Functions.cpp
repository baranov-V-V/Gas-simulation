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
    max.x = dots[0].x, min.x = dots[0].x;
    max.y = dots[0].y, min.y = dots[0].y;

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

double Sqr(double num) {
    return num * num;
}

double Func1(double x) {
    return x * x * x * x - 3 * x * x * x + x * x - 20;
}

double Func2(double x) {
    return x * x - 3 * x - 10;
}

