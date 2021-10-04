#include "sphere.h"

int main() {
    Window main_window(1400, 750, TX_WHITE);
    Texture gas_can(600, 600, TX_WHITE, 0, 0);

    Coordinates coord(0, 10, 0, 10);
    Renderer render(&gas_can, &coord);
    Manager manager(1000);

    PhysCircle* b1 = new PhysCircle(1, 1, 0.5, 2, 1, 1, RGB(255, 255, 0));
    PhysCircle* b2 = new PhysCircle(3, 3, 1, 3, 1, 1, RGB(128, 128, 0));
    PhysCircle* b3 = new PhysCircle(7, 7, 1.5, 2, 1, 1, RGB(255, 0, 0));
    PhysRect* r1 = new PhysRect(8, 1, 9, 2, 1, 1, 1, TX_RED);
    PhysRect* r2 = new PhysRect(8, 3, 8.5, 5, 1, 1, 1, TX_BLUE);

    //Rectangl r1(1, 1, 1, 1, TX_BLUE);

    manager.add_figure(b1);
    manager.add_figure(b2);
    manager.add_figure(b3);
    manager.add_figure(r1);
    manager.add_figure(r2);

    Pair<double> dots[5] = {{0, 0}, {1, 1}, {2, 1}, {3, 1}, {4, 2}};
    Coordinates graph_coord(-2, 10, -2, 10);
    Texture graph_texture(600, 150, TX_WHITE, 0, 600);
    Renderer graph_render(&graph_texture, &graph_coord);
    Graph graph(dots, 5);

    Coordinates button_coord(0, 800, 0, 750);
    Texture button_texture(800, 750, TX_WHITE, 600, 0);
    Renderer button_render(&button_texture, &button_coord);
    
    TextButton* button1 = new TextButton(200, 80, 100, 650, TX_BLUE, "Add Circle!", ActionAddCircle);
    TextButton* button2 = new TextButton(200, 80, 100, 500, TX_BLUE, "Add Rectangle!", ActionAddRect);
    TextButton* button3 = new TextButton(200, 80, 100, 350, TX_BLUE, "Inc Temperature!", ActionIncSpeed);
    TextButton* button4 = new TextButton(200, 80, 100, 200, TX_BLUE, "Dec Temperature!", ActionDecSpeed);

    ButtonManager button_manager(10);
    button_manager.add_button(button1);
    button_manager.add_button(button2);
    button_manager.add_button(button3);
    button_manager.add_button(button4);
    button_manager.draw_all(button_render);

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

        button_manager.draw_all(button_render);
        button_manager.check_all(button_render, manager);        
        button_texture.show_on(main_window.get_hdc());

        txSleep(30);
    }


    return 0;
}