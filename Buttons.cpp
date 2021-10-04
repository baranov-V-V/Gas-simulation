#include "sphere.h"

Button::Button(int x_size, int y_size, int x, int y) :
    Frame(x_size, y_size), coord(x, y) {};

TextButton::TextButton(int x_size, int y_size, int x, int y, COLORREF color, char* text, void (*Action)(Manager& manager)) : 
    Button(x_size, y_size, x, y), color(color), Action(Action) {
    len = strlen(text);
    TextButton::text = new char[len + 1];
    strcpy(TextButton::text, text);
};

TextButton::TextButton() : 
    Button(0, 0, 0, 0), color(TX_WHITE) {
    len = 0;
    TextButton::text = nullptr;
};

TextButton::TextButton(const TextButton& source) : 
    Button(source.get_size_x(), source.get_size_y() , source.get_coord_x(), source.get_coord_y()), color(source.get_color()) {
    len = source.get_len();
    text = new char[len + 1];
    strcpy(text, source.get_text());
};

TextButton::~TextButton() {
    delete[] text;
};

void TextButton::make_action(Manager& manager) const {
    Action(manager);
}

ButtonManager::ButtonManager(int max_count) : size(max_count), count(0) {
    ButtonManager::buttons = (Button**) calloc(max_count, sizeof(Button*));
};

ButtonManager::~ButtonManager() {
    for (int i = 0; i < count; ++i) {
        this->del_last();
    }
    free(buttons);
};

void ButtonManager::draw_all(const Renderer& renderer) const {
    for (int i = 0; i < count; ++i) {
        buttons[i]->draw(renderer);
    }
};

void ButtonManager::check_all(const Renderer& render, Manager& manager) const {

    static clock_t time = 0;
    static bool is_pressed = false;

    if (is_pressed) {
        if (clock() - time < CLOCKS_PER_SEC * 1) {
            return;    
        }
        else {
            is_pressed = false;
        }
    }

    for (int i = 0; i < this->get_count(); ++i) {
        TextButton* button = (TextButton*) this->get_button(i);
        if (button->check_press(render) && !is_pressed) {
            time = clock();
            is_pressed = true;
            button->make_action(manager);
        }
    }
};

void ButtonManager::add_button(Button* button) {
    if (count < size) {
        buttons[count++] = button;
    }
};

void ButtonManager::del_button(const Button* button) {
    int pos = 0;
    for (int i = 0; i < count; ++i) {
        if (buttons[i] == button) {
            for (int j = i; j < count - 1; ++j) {
                buttons[j] = buttons[j + 1];   
            }
            count--;
            break;
        }
    }
};

void ButtonManager::del_last() {
    buttons[--count] = nullptr;
};

void ActionAddCircle(Manager& manager) {
    double x = 2;
    double y = 2;
    double r = 0.5;
    PhysCircle* tmp = new PhysCircle(x, y, r, rand() % 3, rand() % 3, 1, RGB(128, rand() % 256, rand() % 256));
    manager.add_figure(tmp);
}

void ActionAddRect(Manager& manager) {
    double x = 1;
    double y = 1;
    PhysRect* tmp = new PhysRect(x, y, x + 0.5, y + 0.6, rand() % 3, rand() % 3, 1 + rand() % 3, RGB(255, rand() % 256, rand() % 256));
    manager.add_figure(tmp);
}

void ActionIncSpeed(Manager& manager) {
    for (int i = 0; i < manager.get_count(); ++i) {
        PhysShape* shape = manager.get_figure(i);
        shape->set_speed(shape->get_speed().get_x() * 11 / 10, shape->get_speed().get_y() * 11 / 10); 
    }
}

void ActionDecSpeed(Manager& manager) {
    for (int i = 0; i < manager.get_count(); ++i) {
        PhysShape* shape = manager.get_figure(i);
        shape->set_speed(shape->get_speed().get_x() * 9 / 10, shape->get_speed().get_y() * 9 / 10); 
    }
}