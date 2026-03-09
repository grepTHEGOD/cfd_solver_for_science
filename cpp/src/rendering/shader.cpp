#include <rendering/shader.hh>
#include <rendering/gl_functions.hh>
#include <utils/logger.hh>

#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

namespace cfd {

Shader::~Shader() {
    if (program_) {
        ogl::GL::glDeleteProgram(program_);
    }
}

Shader::Shader(Shader&& other) noexcept : program_(other.program_), uniform_cache_(std::move(other.uniform_cache_)) {
    other.program_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (program_) {
            ogl::GL::glDeleteProgram(program_);
        }
        program_ = other.program_;
        uniform_cache_ = std::move(other.uniform_cache_);
        other.program_ = 0;
    }
    return *this;
}

bool Shader::compile_from_source(const std::string& vertex_source, const std::string& fragment_source, const std::string& geometry_source) {
    uint32_t vertex_shader = ogl::GL::glCreateShader(GL_VERTEX_SHADER);
    uint32_t fragment_shader = ogl::GL::glCreateShader(GL_FRAGMENT_SHADER);
    
    const char* v_src = vertex_source.c_str();
    const char* f_src = fragment_source.c_str();
    
    ogl::GL::glShaderSource(vertex_shader, 1, &v_src, nullptr);
    ogl::GL::glShaderSource(fragment_shader, 1, &f_src, nullptr);
    
    ogl::GL::glCompileShader(vertex_shader);
    ogl::GL::glCompileShader(fragment_shader);
    
    int success;
    ogl::GL::glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        ogl::GL::glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        LOG_ERROR("Vertex shader failed: {}", info_log);
        return false;
    }
    
    ogl::GL::glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        ogl::GL::glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        LOG_ERROR("Fragment shader failed: {}", info_log);
        return false;
    }
    
    program_ = ogl::GL::glCreateProgram();
    ogl::GL::glAttachShader(program_, vertex_shader);
    ogl::GL::glAttachShader(program_, fragment_shader);
    ogl::GL::glLinkProgram(program_);
    
    ogl::GL::glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (!success) {
        char info_log[512];
        ogl::GL::glGetProgramInfoLog(program_, 512, nullptr, info_log);
        LOG_ERROR("Shader program linking failed: {}", info_log);
        return false;
    }
    
    ogl::GL::glDeleteShader(vertex_shader);
    ogl::GL::glDeleteShader(fragment_shader);
    
    LOG_DEBUG("Shader compiled successfully");
    return true;
}

bool Shader::compile_from_files(const std::string& vertex_path, const std::string& fragment_path, const std::string& geometry_path) {
    std::ifstream vfile(vertex_path);
    std::ifstream ffile(fragment_path);
    
    if (!vfile.is_open() || !ffile.is_open()) {
        LOG_ERROR("Failed to open shader files");
        return false;
    }
    
    std::stringstream vstream, fstream;
    vstream << vfile.rdbuf();
    fstream << ffile.rdbuf();
    
    return compile_from_source(vstream.str(), fstream.str());
}

void Shader::use() const {
    if (program_) {
        ogl::GL::glUseProgram(program_);
    }
}

void Shader::unbind() const {
    ogl::GL::glUseProgram(0);
}

void Shader::set_int(const std::string& name, int value) {
    ogl::GL::glUniform1i(get_uniform_location(name), value);
}

void Shader::set_float(const std::string& name, float value) {
    ogl::GL::glUniform1f(get_uniform_location(name), value);
}

void Shader::set_vec2(const std::string& name, float x, float y) {
    ogl::GL::glUniform2f(get_uniform_location(name), x, y);
}

void Shader::set_vec3(const std::string& name, float x, float y, float z) {
    ogl::GL::glUniform3f(get_uniform_location(name), x, y, z);
}

void Shader::set_vec3(const std::string& name, const float* value) {
    ogl::GL::glUniform3fv(get_uniform_location(name), 1, value);
}

void Shader::set_vec4(const std::string& name, float x, float y, float z, float w) {
    ogl::GL::glUniform4f(get_uniform_location(name), x, y, z, w);
}

void Shader::set_mat4(const std::string& name, const float* value) {
    ogl::GL::glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, value);
}

int Shader::get_uniform_location(const std::string& name) {
    auto it = uniform_cache_.find(name);
    if (it != uniform_cache_.end()) {
        return it->second;
    }
    
    int location = ogl::GL::glGetUniformLocation(program_, name.c_str());
    uniform_cache_[name] = location;
    return location;
}

ShaderLibrary& ShaderLibrary::instance() {
    static ShaderLibrary instance;
    return instance;
}

void ShaderLibrary::add(const std::string& name, Shader&& shader) {
    shaders_[name] = std::move(shader);
}

Shader* ShaderLibrary::get(const std::string& name) {
    auto it = shaders_.find(name);
    if (it != shaders_.end()) {
        return &it->second;
    }
    return nullptr;
}

bool ShaderLibrary::exists(const std::string& name) const {
    return shaders_.find(name) != shaders_.end();
}

void ShaderLibrary::load_all() {
}

}
