#include "sphere.h"

int main() {
    Window main_window(1400, 750, TX_WHITE);
    Texture gas_can(600, 600, TX_WHITE, 0, 0);

    Coordinates coord(0, 10, 0, 10);
    Renderer render(&gas_can, &coord);
    Manager manager(10);

    PhysCircle* b1 = new PhysCircle(1, 1, 0.5, 2, 1, 1, RGB(255, 255, 0));
    PhysCircle* b2 = new PhysCircle(3, 3, 1, 3, 1, 2, RGB(128, 128, 0));
    PhysCircle* b3 = new PhysCircle(7, 7, 1.5, 2, 1, 1, RGB(255, 0, 0));
    //PhysRect r1(8, 1, 9, 2, 1, 1, 2, TX_RED);
    //PhysRect r2(8, 3, 8.5, 5, 1, 1, 2, TX_BLUE);

    //Rectangl r1(1, 1, 1, 1, TX_BLUE);

    manager.add_figure(b1);
    manager.add_figure(b2);
    manager.add_figure(b3);
    //manager.add_figure(&r1);
    //manager.add_figure(&r2);

    Pair<double> dots[5] = {{0, 0}, {1, 1}, {2, 1}, {3, 1}, {4, 2}};
    Coordinates graph_coord(-2, 10, -2, 10);
    Texture graph_texture(600, 150, TX_WHITE, 0, 600);
    Renderer graph_render(&graph_texture, &graph_coord);
    Graph graph(dots, 5);

    double time = dt;
    while (!GetAsyncKeyState(VK_ESCAPE)) {
        render.clear();
        render.draw_rectangle(0, 0, 10, 10, TX_BLACK, 2);
        manager.draw_all(render);
        manager.move_all(time);
        CheckAllCollisions(manager, render.get_coordinates());
        gas_can.show_on(main_window.get_hdc());

        graph.draw_axis(graph_render);
        graph.draw(graph_render);
        //graph_texture.show_on(main_window.get_hdc());

        txSleep(30);
    }
    
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