#include "sphere.h"

Button::Button(int x_size, int y_size, int x, int y) :
    Frame(x_size, y_size), coord(x, y) {};

TextButton::TextButton(int x_size, int y_size, int x, int y, COLORREF color, char* text) : 
    Button(x_size, y_size, x, y), color(color) {
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

void TextButton::draw(const Renderer& render) const {
    render.draw_rectangle(coord.x, coord.y, coord.x + size.x, coord.y + size.y, color, 4);

    txSelectFont("Comic Sans MS", size.y / 2, size.x / len * 2 / 3, FW_DONTCARE, false, false, false, 0, render.get_window()->get_hdc());
    txTextOut(coord.x + size.x / 6, render.get_window()->get_size_y() - (coord.y + size.y / 4), text, render.get_window()->get_hdc());
};

bool TextButton::check_press() const {

}

void TextButton::make_action() const {

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



