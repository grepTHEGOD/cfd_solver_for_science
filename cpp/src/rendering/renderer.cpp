#include <rendering/renderer.hh>
#include <rendering/gl_functions.hh>
#include <utils/logger.hh>
#include <utils/math.hh>

#include <GLFW/glfw3.h>

namespace cfd {

Renderer::Renderer() {
    LOG_INFO("Renderer created");
}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize(int width, int height) {
    width_ = width;
    height_ = height;
    
    setup_gl();
    load_shaders();
    create_grid_mesh();
    create_axis_mesh();
    
    LOG_INFO("Renderer initialized");
    return true;
}

void Renderer::shutdown() {
    LOG_INFO("Renderer shutdown");
}

void Renderer::setup_gl() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    
    glEnable(GL_MULTISAMPLE);
    
    glClearColor(settings_.background_color[0],
                settings_.background_color[1],
                settings_.background_color[2],
                settings_.background_color[3]);
}

void Renderer::begin_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::end_frame() {
}

void Renderer::set_viewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render_mesh(const Mesh& mesh, const float* model_matrix, const float* color) {
    if (!mesh.vao) return;
    
    cfd_ogl::GL::glBindVertexArray(mesh.vao);
    
    if (color) {
        cfd_ogl::GL::glVertexAttrib4fv(3, color);
    }
    
    if (!mesh.indices.empty()) {
        cfd_ogl::GL::glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
    } else {
        cfd_ogl::GL::glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mesh.vertices.size()));
    }
    
    cfd_ogl::GL::glBindVertexArray(0);
}

void Renderer::render_grid(float size, int divisions) {
    if (!grid_shader_) return;
    
    grid_shader_->use();
    
    auto view = camera_->view_matrix();
    auto proj = camera_->projection_matrix();
    
    grid_shader_->set_mat4("view", view.data());
    grid_shader_->set_mat4("projection", proj.data());
    
    float model[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    grid_shader_->set_mat4("model", model);
    grid_shader_->set_float("grid_size", size);
    grid_shader_->set_int("divisions", divisions);
    grid_shader_->set_vec4("grid_color", settings_.grid_color[0], settings_.grid_color[1], 
                          settings_.grid_color[2], settings_.grid_color[3]);
    
    cfd_ogl::GL::glBindVertexArray(grid_mesh_.vao);
    cfd_ogl::GL::glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(grid_mesh_.vertices.size()));
    cfd_ogl::GL::glBindVertexArray(0);
}

void Renderer::render_axis(float size) {
    if (!basic_shader_ || !camera_) return;
    
    basic_shader_->use();
    
    auto view = camera_->view_matrix();
    auto proj = camera_->projection_matrix();
    
    basic_shader_->set_mat4("view", view.data());
    basic_shader_->set_mat4("projection", proj.data());
    
    float x_axis_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    float y_axis_color[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float z_axis_color[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    
    float model_x[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    float model_y[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    float model_z[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
    
    basic_shader_->set_mat4("model", model_x);
    basic_shader_->set_vec4("color", x_axis_color[0], x_axis_color[1], x_axis_color[2], x_axis_color[3]);
    
    cfd_ogl::GL::glBindVertexArray(axis_mesh_.vao);
    glLineWidth(2.0f);
    cfd_ogl::GL::glDrawArrays(GL_LINES, 0, 6);
    cfd_ogl::GL::glBindVertexArray(0);
}

void Renderer::render_scramjet(ScramjetGeometry& scramjet, RenderMode mode) {
    if (!basic_shader_ || !camera_) return;
    
    basic_shader_->use();
    
    auto view = camera_->view_matrix();
    auto proj = camera_->projection_matrix();
    
    basic_shader_->set_mat4("view", view.data());
    basic_shader_->set_mat4("projection", proj.data());
    
    float model[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    basic_shader_->set_mat4("model", model);
    basic_shader_->set_vec4("color", settings_.mesh_color[0], settings_.mesh_color[1], 
                           settings_.mesh_color[2], settings_.mesh_color[3]);
    
    if (mode == RenderMode::WIREFRAME) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    render_mesh(scramjet.get_external_mesh(), model);
    
    if (scramjet.show_internal()) {
        render_mesh(scramjet.get_internal_mesh(), model);
    }
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::render_streamlines() {
}

void Renderer::render_contours() {
}

void Renderer::render_force_arrows() {
}

Shader* Renderer::get_shader(const std::string& name) {
    if (name == "basic") return basic_shader_.get();
    if (name == "grid") return grid_shader_.get();
    return nullptr;
}

void Renderer::load_shaders() {
    const char* basic_vert = R"(
        #version 460 core
        layout(location = 0) in vec3 aPos;
        layout(location = 1) in vec3 aNormal;
        layout(location = 2) in vec2 aTexCoord;
        layout(location = 3) in vec4 aColor;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec3 fragPos;
        out vec3 fragNormal;
        out vec2 fragTexCoord;
        out vec4 fragColor;
        
        void main() {
            fragPos = vec3(model * vec4(aPos, 1.0));
            fragNormal = mat3(transpose(inverse(model))) * aNormal;
            fragTexCoord = aTexCoord;
            fragColor = aColor;
            gl_Position = projection * view * vec4(fragPos, 1.0);
        }
    )";
    
    const char* basic_frag = R"(
        #version 460 core
        in vec3 fragPos;
        in vec3 fragNormal;
        in vec2 fragTexCoord;
        in vec4 fragColor;
        
        uniform vec4 color;
        uniform vec3 lightDir;
        uniform vec4 lightColor;
        uniform vec4 ambientColor;
        
        out vec4 fragColorOut;
        
        void main() {
            vec3 norm = normalize(fragNormal);
            float diff = max(dot(norm, normalize(lightDir)), 0.0);
            vec4 diffuse = diff * lightColor;
            vec4 result = (ambientColor + diffuse) * color;
            fragColorOut = result;
        }
    )";
    
    const char* grid_vert = R"(
        #version 460 core
        layout(location = 0) in vec3 aPos;
        
        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        
        void main() {
            gl_Position = projection * view * model * vec4(aPos, 1.0);
        }
    )";
    
    const char* grid_frag = R"(
        #version 460 core
        uniform vec4 grid_color;
        uniform float grid_size;
        uniform int divisions;
        
        out vec4 fragColor;
        
        void main() {
            fragColor = grid_color;
        }
    )";
    
    basic_shader_ = std::make_unique<Shader>();
    basic_shader_->compile_from_source(basic_vert, basic_frag);
    
    grid_shader_ = std::make_unique<Shader>();
    grid_shader_->compile_from_source(grid_vert, grid_frag);
    
    LOG_INFO("Shaders loaded");
}

void Renderer::create_grid_mesh() {
    grid_mesh_.vertices.clear();
    
    float size = 10.0f;
    int divisions = 10;
    float step = size / divisions;
    
    for (int i = 0; i <= divisions; ++i) {
        float pos = -size / 2.0f + i * step;
        
        Vertex v1, v2;
        v1.position[0] = pos; v1.position[1] = 0.0f; v1.position[2] = -size / 2.0f;
        v2.position[0] = pos; v2.position[1] = 0.0f; v2.position[2] = size / 2.0f;
        grid_mesh_.vertices.push_back(v1);
        grid_mesh_.vertices.push_back(v2);
        
        v1.position[0] = -size / 2.0f; v1.position[2] = pos;
        v2.position[0] = size / 2.0f; v2.position[2] = pos;
        grid_mesh_.vertices.push_back(v1);
        grid_mesh_.vertices.push_back(v2);
    }
    
    cfd_ogl::GL::glGenVertexArrays(1, &grid_mesh_.vao);
    cfd_ogl::GL::glGenBuffers(1, &grid_mesh_.vbo);
    
    cfd_ogl::GL::glBindVertexArray(grid_mesh_.vao);
    cfd_ogl::GL::glBindBuffer(GL_ARRAY_BUFFER, grid_mesh_.vbo);
    cfd_ogl::GL::glBufferData(GL_ARRAY_BUFFER, grid_mesh_.vertices.size() * sizeof(Vertex),
                grid_mesh_.vertices.data(), GL_STATIC_DRAW);
    
    cfd_ogl::GL::glEnableVertexAttribArray(0);
    cfd_ogl::GL::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    cfd_ogl::GL::glEnableVertexAttribArray(3);
    cfd_ogl::GL::glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float)*9));
    
    cfd_ogl::GL::glBindVertexArray(0);
}

void Renderer::create_axis_mesh() {
    axis_mesh_.vertices.clear();
    
    Vertex v;
    v.position[1] = v.position[2] = 0.0f;
    v.position[0] = 0.0f;
    axis_mesh_.vertices.push_back(v);
    v.position[0] = 1.0f;
    axis_mesh_.vertices.push_back(v);
    
    v.position[0] = 0.0f; v.position[1] = 0.0f; v.position[2] = 0.0f;
    axis_mesh_.vertices.push_back(v);
    v.position[1] = 1.0f;
    axis_mesh_.vertices.push_back(v);
    
    v.position[1] = 0.0f; v.position[2] = 0.0f;
    axis_mesh_.vertices.push_back(v);
    v.position[2] = 1.0f;
    axis_mesh_.vertices.push_back(v);
    
    cfd_ogl::GL::glGenVertexArrays(1, &axis_mesh_.vao);
    cfd_ogl::GL::glGenBuffers(1, &axis_mesh_.vbo);
    
    cfd_ogl::GL::glBindVertexArray(axis_mesh_.vao);
    cfd_ogl::GL::glBindBuffer(GL_ARRAY_BUFFER, axis_mesh_.vbo);
    cfd_ogl::GL::glBufferData(GL_ARRAY_BUFFER, axis_mesh_.vertices.size() * sizeof(Vertex),
                axis_mesh_.vertices.data(), GL_STATIC_DRAW);
    
    cfd_ogl::GL::glEnableVertexAttribArray(0);
    cfd_ogl::GL::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    cfd_ogl::GL::glBindVertexArray(0);
}

}
