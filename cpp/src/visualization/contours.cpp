#include <visualization/contours.hh>
#include <geometry/scramjet_geometry.hh>
#include <utils/math.hh>

namespace cfd {

Contours::Contours() {
}

Contours::~Contours() {
}

void Contours::generate(const ScramjetGeometry& scramjet, float mach, float altitude) {
    generate_surface_contours();
    needs_regeneration_ = false;
}

void Contours::update(float mach, float altitude) {
}

void Contours::render() {
    if (!visible_) return;
}

std::array<float, 4> Contours::get_color_for_value(float value, float min_val, float max_val) {
    float normalized = (value - min_val) / (max_val - min_val);
    normalized = std::clamp(normalized, 0.0f, 1.0f);
    
    if (normalized < 0.25f) {
        return {0.0f, 0.0f, normalized * 4.0f, 1.0f};
    } else if (normalized < 0.5f) {
        return {0.0f, (normalized - 0.25f) * 4.0f, 1.0f - (normalized - 0.25f) * 4.0f, 1.0f};
    } else if (normalized < 0.75f) {
        return {(normalized - 0.5f) * 4.0f, 1.0f, 0.0f, 1.0f};
    } else {
        return {1.0f, 1.0f - (normalized - 0.75f) * 4.0f, 0.0f, 1.0f};
    }
}

void Contours::generate_surface_contours() {
}

}
