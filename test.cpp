#include "sphere.h"

int main() {
    Window main_window(850, 750, TX_WHITE);
    
    Coordinates coord(0, 10, 0, 10);
    Renderer render(&main_window, &coord);
    Manager manager(10);

    Bubble b1(1, 1, 0.5, 1, 1, 1, RGB(255, 255, 0));
    Bubble b2(3, 3, 1, 6, 1, 2, RGB(128, 128, 0));
    Bubble b3(7, 7, 1.5, 7, 3, 1, RGB(255, 0, 0));

    manager.add_figure(&b1);
    manager.add_figure(&b2);
    manager.add_figure(&b3);

    ProceedMoving(manager, render);
    
    /*
    manager.draw_all(render);
    render.show_on(main_window);
    render.clear();
    txSleep(1000);

    manager.del_figure(&b1);
    manager.del_last();
    
    manager.draw_all(render);
    render.show_on(main_window);
    */

    return 0;
}