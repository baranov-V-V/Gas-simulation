#include "sphere.h"

int main() {
    Window main_window(850, 750);
    
    //Window window(750, 750, TX_WHITE, 100, 0);
    Coordinates coord(0, 10, 0, 10);
    Renderer render(&main_window, &coord);
    Manager manager(10);

    Bubble b1(1, 1, 0.5, 1, 2, 1, RGBQUAD {255, 255, 0, 0});
    Bubble b2(3, 3, 1, 1, 3, 1, RGBQUAD {128, 128, 0, 0});
    Bubble b3(7, 7, 1.5, 1, 4, 1, RGBQUAD {255, 0, 0, 0});
    
    //b1.draw(render);
    //b2.draw(render);
    //b3.draw(render);

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