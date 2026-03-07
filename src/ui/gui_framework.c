#ifndef GUI_FRAMEWORK_C
#define GUI_FRAMEWORK_C

#include "gui_framework.h"
#include <string.h>
#include <stdio.h>
#include <math.h>

Button gui_button(Rectangle bounds, const char *text) {
    Button btn = {
        .bounds = bounds,
        .text = text,
        .pressed = false,
        .hovered = false,
        .bg_color = GUI_COLOR_BUTTON,
        .text_color = GUI_COLOR_TEXT
    };
    return btn;
}

bool gui_button_draw(Button *btn) {
    if (!btn) return false;
    
    bool clicked = false;
    Vector2 mouse = GetMousePosition();
    
    btn->hovered = CheckCollisionPointRec(mouse, btn->bounds);
    
    if (btn->hovered) {
        btn->bg_color = GUI_COLOR_BUTTON_HOVER;
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            btn->pressed = true;
        } else if (btn->pressed) {
            btn->pressed = false;
            clicked = true;
        }
    } else {
        btn->bg_color = GUI_COLOR_BUTTON;
        btn->pressed = false;
    }
    
    DrawRectangleRec(btn->bounds, btn->bg_color);
    DrawRectangleLinesEx(btn->bounds, 1, GUI_COLOR_BORDER);
    
    int text_width = MeasureText(btn->text, 14);
    float text_x = btn->bounds.x + (btn->bounds.width - text_width) / 2;
    float text_y = btn->bounds.y + (btn->bounds.height - 14) / 2;
    
    DrawText(btn->text, (int)text_x, (int)text_y, 14, btn->text_color);
    
    return clicked;
}

Slider gui_slider(Rectangle bounds, float value, float min_val, float max_val, float step, const char *label, const char *unit) {
    Slider slider = {
        .bounds = bounds,
        .value = value,
        .min_value = min_val,
        .max_value = max_val,
        .step = step,
        .label = label,
        .unit = unit,
        .dragging = false,
        .slider_color = GUI_COLOR_ACCENT,
        .thumb_color = GUI_COLOR_TEXT
    };
    return slider;
}

bool gui_slider_draw(Slider *slider) {
    if (!slider) return false;
    
    Vector2 mouse = GetMousePosition();
    bool value_changed = false;
    
    if (slider->dragging) {
        if (IsMouseButtonUp(MOUSE_LEFT_BUTTON)) {
            slider->dragging = false;
        } else {
            float new_value = slider->min_value + 
                (mouse.x - slider->bounds.x) / slider->bounds.width * 
                (slider->max_value - slider->min_value);
            new_value = fmax(slider->min_value, fmin(slider->max_value, new_value));
            new_value = roundf(new_value / slider->step) * slider->step;
            if (new_value != slider->value) {
                slider->value = new_value;
                value_changed = true;
            }
        }
    } else if (CheckCollisionPointRec(mouse, slider->bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        slider->dragging = true;
        float new_value = slider->min_value + 
            (mouse.x - slider->bounds.x) / slider->bounds.width * 
            (slider->max_value - slider->min_value);
        new_value = fmax(slider->min_value, fmin(slider->max_value, new_value));
        new_value = roundf(new_value / slider->step) * slider->step;
        if (new_value != slider->value) {
            slider->value = new_value;
            value_changed = true;
        }
    }
    
    DrawRectangleRec(slider->bounds, GUI_COLOR_PANEL_LIGHT);
    DrawRectangleLinesEx(slider->bounds, 1, GUI_COLOR_BORDER);
    
    float slider_width = slider->bounds.width - 20;
    float thumb_x = slider->bounds.x + 10 + 
        ((slider->value - slider->min_value) / (slider->max_value - slider->min_value)) * slider_width;
    
    DrawRectangle((int)(slider->bounds.x + 10), (int)(slider->bounds.y + slider->bounds.height/2 - 3),
        (int)slider_width, 6, GUI_COLOR_BORDER);
    
    float fill_width = thumb_x - (slider->bounds.x + 10);
    DrawRectangle((int)(slider->bounds.x + 10), (int)(slider->bounds.y + slider->bounds.height/2 - 3),
        (int)fill_width, 6, slider->slider_color);
    
    DrawCircleV((Vector2){thumb_x, slider->bounds.y + slider->bounds.height/2}, 8, slider->thumb_color);
    DrawCircleV((Vector2){thumb_x, slider->bounds.y + slider->bounds.height/2}, 6, GUI_COLOR_PANEL_LIGHT);
    
    char value_str[64];
    if (slider->step >= 1.0f) {
        snprintf(value_str, sizeof(value_str), "%.0f %s", slider->value, slider->unit ? slider->unit : "");
    } else if (slider->step >= 0.1f) {
        snprintf(value_str, sizeof(value_str), "%.1f %s", slider->value, slider->unit ? slider->unit : "");
    } else {
        snprintf(value_str, sizeof(value_str), "%.2f %s", slider->value, slider->unit ? slider->unit : "");
    }
    
    int label_width = MeasureText(slider->label, 12);
    DrawText(slider->label, (int)(slider->bounds.x), (int)(slider->bounds.y - 18), 12, GUI_COLOR_TEXT_DIM);
    
    int value_width = MeasureText(value_str, 12);
    DrawText(value_str, (int)(slider->bounds.x + slider->bounds.width - value_width), (int)(slider->bounds.y - 18), 12, GUI_COLOR_ACCENT);
    
    return value_changed;
}

float gui_slider_update(Slider *slider) {
    return slider->value;
}

Toggle gui_toggle(Rectangle bounds, bool *value, const char *label, const char *opt_a, const char *opt_b) {
    Toggle toggle = {
        .bounds = bounds,
        .value = value,
        .label = label,
        .option_a = opt_a,
        .option_b = opt_b,
        .hovered = false
    };
    return toggle;
}

bool gui_toggle_draw(Toggle *toggle) {
    if (!toggle) return false;
    
    bool clicked = false;
    Vector2 mouse = GetMousePosition();
    
    toggle->hovered = CheckCollisionPointRec(mouse, toggle->bounds);
    
    if (toggle->hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *(toggle->value) = !*(toggle->value);
        clicked = true;
    }
    
    DrawRectangleRec(toggle->bounds, GUI_COLOR_PANEL_LIGHT);
    DrawRectangleLinesEx(toggle->bounds, 1, GUI_COLOR_BORDER);
    
    int label_width = MeasureText(toggle->label, 12);
    DrawText(toggle->label, (int)(toggle->bounds.x), (int)(toggle->bounds.y - 16), 12, GUI_COLOR_TEXT_DIM);
    
    float toggle_width = 60;
    float toggle_height = 24;
    float toggle_x = toggle->bounds.x + toggle->bounds.width - toggle_width - 5;
    float toggle_y = toggle->bounds.y + (toggle->bounds.height - toggle_height) / 2;
    
    Rectangle toggle_bg = {toggle_x, toggle_y, toggle_width, toggle_height};
    DrawRectangleRec(toggle_bg, *(toggle->value) ? GUI_COLOR_ACCENT : GUI_COLOR_BORDER);
    
    float knob_x = *(toggle->value) ? toggle_x + toggle_width - 14 : toggle_x + 4;
    DrawCircleV((Vector2){knob_x + 5, toggle_y + toggle_height/2}, 8, GUI_COLOR_TEXT);
    
    const char *active_opt = *(toggle->value) ? toggle->option_b : toggle->option_a;
    int opt_width = MeasureText(active_opt, 10);
    DrawText(active_opt, (int)(toggle_x + (toggle_width - opt_width)/2), (int)(toggle_y + 7), 10, GUI_COLOR_TEXT);
    
    return clicked;
}

TextInput gui_text_input(Rectangle bounds, const char *placeholder, bool numbers_only) {
    TextInput input = {
        .bounds = bounds,
        .text = "",
        .editing = false,
        .cursor_pos = 0,
        .select_start = 0,
        .select_end = 0,
        .bg_color = GUI_COLOR_PANEL_LIGHT,
        .text_color = GUI_COLOR_TEXT,
        .numbers_only = numbers_only
    };
    if (placeholder) {
        strncpy(input.text, placeholder, 255);
    }
    return input;
}

bool gui_text_input_draw(TextInput *input) {
    if (!input) return false;
    
    Vector2 mouse = GetMousePosition();
    bool clicked = CheckCollisionPointRec(mouse, input->bounds);
    
    if (clicked && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        input->editing = true;
    } else if (!clicked && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        input->editing = false;
    }
    
    DrawRectangleRec(input->bounds, input->bg_color);
    DrawRectangleLinesEx(input->bounds, input->editing ? 2 : 1, 
        input->editing ? GUI_COLOR_ACCENT : GUI_COLOR_BORDER);
    
    if (strlen(input->text) == 0 && !input->editing) {
        DrawText("...", (int)(input->bounds.x + 5), (int)(input->bounds.y + 5), 12, GUI_COLOR_TEXT_DIM);
    } else {
        DrawText(input->text, (int)(input->bounds.x + 5), (int)(input->bounds.y + 5), 12, input->text_color);
    }
    
    if (input->editing && ((int)(GetTime() * 2) % 2 == 0)) {
        int cursor_x = input->bounds.x + 5 + MeasureText(input->text, 12);
        DrawLine(cursor_x, (int)(input->bounds.y + 3), cursor_x, (int)(input->bounds.y + input->bounds.height - 3), GUI_COLOR_TEXT);
    }
    
    return false;
}

void gui_text_input_update(TextInput *input) {
    if (!input || !input->editing) return;
    
    int key = GetCharPressed();
    while (key > 0) {
        if (input->numbers_only) {
            if ((key >= '0' && key <= '9') || key == '.' || key == '-') {
                if (strlen(input->text) < 254) {
                    input->text[strlen(input->text)] = (char)key;
                    input->text[strlen(input->text) + 1] = '\0';
                }
            }
        } else {
            if (key >= 32 && key <= 126) {
                if (strlen(input->text) < 254) {
                    input->text[strlen(input->text)] = (char)key;
                    input->text[strlen(input->text) + 1] = '\0';
                }
            }
        }
        key = GetCharPressed();
    }
    
    if (IsKeyPressed(KEY_BACKSPACE) && strlen(input->text) > 0) {
        input->text[strlen(input->text) - 1] = '\0';
    }
}

Panel gui_panel(Rectangle bounds, const char *title) {
    Panel panel = {
        .bounds = bounds,
        .title = title,
        .collapsed = false,
        .elements = NULL,
        .element_count = 0,
        .header_color = GUI_COLOR_PANEL
    };
    return panel;
}

void gui_panel_draw(Panel *panel) {
    if (!panel) return;
    
    DrawRectangleRec(panel->bounds, panel->header_color);
    DrawRectangleLinesEx(panel->bounds, 1, GUI_COLOR_BORDER);
    
    if (panel->title) {
        DrawText(panel->title, (int)(panel->bounds.x + 10), (int)(panel->bounds.y + 8), 14, GUI_COLOR_TEXT);
    }
}

SegmentSlider gui_segment_slider(Rectangle bounds, float value, float min_val, float max_val, int segments) {
    SegmentSlider slider = {
        .bounds = bounds,
        .value = value,
        .min_value = min_val,
        .max_value = max_val,
        .dragging = false,
        .segments = segments,
        .active_segment = 0
    };
    return slider;
}

bool gui_segment_slider_draw(SegmentSlider *slider) {
    if (!slider) return false;
    
    Vector2 mouse = GetMousePosition();
    bool value_changed = false;
    
    slider->active_segment = (int)((slider->value - slider->min_value) / (slider->max_value - slider->min_value) * (slider->segments - 1));
    
    float segment_width = slider->bounds.width / slider->segments;
    
    for (int i = 0; i < slider->segments; i++) {
        Rectangle seg = {
            slider->bounds.x + i * segment_width,
            slider->bounds.y,
            segment_width - 2,
            slider->bounds.height
        };
        
        Color seg_color = (i <= slider->active_segment) ? GUI_COLOR_ACCENT : GUI_COLOR_BORDER;
        
        if (CheckCollisionPointRec(mouse, seg) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            float new_value = slider->min_value + (float)i / (slider->segments - 1) * (slider->max_value - slider->min_value);
            if (new_value != slider->value) {
                slider->value = new_value;
                value_changed = true;
            }
        }
        
        DrawRectangleRec(seg, seg_color);
    }
    
    return value_changed;
}

float gui_segment_slider_update(SegmentSlider *slider) {
    return slider->value;
}

void gui_draw_text(const char *text, Vector2 pos, int font_size, Color color) {
    DrawText(text, (int)pos.x, (int)pos.y, font_size, color);
}

void gui_draw_rectangle(Rectangle rect, Color color) {
    DrawRectangleRec(rect, color);
}

void gui_draw_border(Rectangle rect, Color color, int thickness) {
    DrawRectangleLinesEx(rect, thickness, color);
}

void gui_set_style(void) {
    SetExitKey(KEY_ESCAPE);
}

int gui_get_text_width(const char *text, int font_size) {
    return MeasureText(text, font_size);
}

#endif
