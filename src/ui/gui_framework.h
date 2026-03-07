#ifndef GUI_FRAMEWORK_H
#define GUI_FRAMEWORK_H

#include "raylib.h"
#include <stdbool.h>
#include <stdint.h>

#define GUI_COLOR_PANEL         (Color){37, 37, 38, 255}
#define GUI_COLOR_PANEL_LIGHT  (Color){45, 45, 48, 255}
#define GUI_COLOR_BORDER       (Color){62, 62, 66, 255}
#define GUI_COLOR_ACCENT       (Color){0, 122, 204, 255}
#define GUI_COLOR_ACCENT_DARK  (Color){14, 99, 156, 255}
#define GUI_COLOR_TEXT         (Color){204, 204, 204, 255}
#define GUI_COLOR_TEXT_DIM     (Color){133, 133, 133, 255}
#define GUI_COLOR_SUCCESS      (Color){78, 201, 176, 255}
#define GUI_COLOR_WARNING      (Color){221, 177, 0, 255}
#define GUI_COLOR_ERROR        (Color){241, 76, 76, 255}
#define GUI_COLOR_BUTTON       (Color){60, 60, 65, 255}
#define GUI_COLOR_BUTTON_HOVER (Color){70, 70, 75, 255}

typedef struct {
    Rectangle bounds;
    bool visible;
    int id;
    void *data;
    void (*draw)(void*);
    void (*update)(void*);
} GUIElement;

typedef struct {
    Rectangle bounds;
    const char *text;
    bool pressed;
    bool hovered;
    Color bg_color;
    Color text_color;
} Button;

typedef struct {
    Rectangle bounds;
    float value;
    float min_value;
    float max_value;
    float step;
    const char *label;
    const char *unit;
    bool dragging;
    Color slider_color;
    Color thumb_color;
} Slider;

typedef struct {
    Rectangle bounds;
    bool *value;
    const char *label;
    const char *option_a;
    const char *option_b;
    bool hovered;
} Toggle;

typedef struct {
    Rectangle bounds;
    char text[256];
    bool editing;
    int cursor_pos;
    int select_start;
    int select_end;
    Color bg_color;
    Color text_color;
    bool numbers_only;
} TextInput;

typedef struct {
    Rectangle bounds;
    const char *title;
    bool collapsed;
    GUIElement *elements;
    int element_count;
    Color header_color;
} Panel;

typedef struct {
    Rectangle bounds;
    float value;
    float min_value;
    float max_value;
    bool dragging;
    int segments;
    int active_segment;
} SegmentSlider;

Button gui_button(Rectangle bounds, const char *text);
bool gui_button_draw(Button *btn);

Slider gui_slider(Rectangle bounds, float value, float min_val, float max_val, float step, const char *label, const char *unit);
bool gui_slider_draw(Slider *slider);
float gui_slider_update(Slider *slider);

Toggle gui_toggle(Rectangle bounds, bool *value, const char *label, const char *opt_a, const char *opt_b);
bool gui_toggle_draw(Toggle *toggle);

TextInput gui_text_input(Rectangle bounds, const char *placeholder, bool numbers_only);
bool gui_text_input_draw(TextInput *input);
void gui_text_input_update(TextInput *input);

Panel gui_panel(Rectangle bounds, const char *title);
void gui_panel_draw(Panel *panel);

SegmentSlider gui_segment_slider(Rectangle bounds, float value, float min_val, float max_val, int segments);
bool gui_segment_slider_draw(SegmentSlider *slider);
float gui_segment_slider_update(SegmentSlider *slider);

void gui_draw_text(const char *text, Vector2 pos, int font_size, Color color);
void gui_draw_rectangle(Rectangle rect, Color color);
void gui_draw_border(Rectangle rect, Color color, int thickness);

void gui_set_style(void);
int gui_get_text_width(const char *text, int font_size);

#endif
