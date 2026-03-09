#include <visualization/force_arrows.hh>
#include <app/scene.hh>
#include <utils/math.hh>

namespace cfd {

ForceArrows::ForceArrows() {
}

ForceArrows::~ForceArrows() {
}

void ForceArrows::generate() {
    generate_arrow_mesh(lift_arrow_, 1.0f, config_.arrow_head_size);
    generate_arrow_mesh(drag_arrow_, 1.0f, config_.arrow_head_size);
    generate_arrow_mesh(side_arrow_, 1.0f, config_.arrow_head_size);
    generate_arrow_mesh(thrust_arrow_, 1.0f, config_.arrow_head_size);
}

void ForceArrows::update(const AerodynamicForces& forces, const ScramjetState& scramjet_state) {
    current_lift_ = forces.lift;
    current_drag_ = forces.drag;
    current_side_ = forces.side;
    current_thrust_ = scramjet_state.thrust;
}

void ForceArrows::render() {
    if (!visible_) return;
}

void ForceArrows::generate_arrow_mesh(Mesh& mesh, float length, float head_size) {
    mesh.vertices.clear();
    mesh.indices.clear();
    
    Vertex v;
    
    v.position[0] = 0.0f; v.position[1] = 0.0f; v.position[2] = 0.0f;
    mesh.vertices.push_back(v);
    v.position[0] = length;
    mesh.vertices.push_back(v);
    
    mesh.indices.push_back(0);
    mesh.indices.push_back(1);
}

}
