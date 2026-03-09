#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

namespace cfd {

class Shader {
public:
    Shader() = default;
    ~Shader();
    
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    
    bool compile_from_source(const std::string& vertex_source, 
                             const std::string& fragment_source,
                             const std::string& geometry_source = "");
    
    bool compile_from_files(const std::string& vertex_path,
                            const std::string& fragment_path,
                            const std::string& geometry_path = "");
    
    void use() const;
    void unbind() const;
    
    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vec2(const std::string& name, float x, float y);
    void set_vec3(const std::string& name, float x, float y, float z);
    void set_vec3(const std::string& name, const float* value);
    void set_vec4(const std::string& name, float x, float y, float z, float w);
    void set_mat4(const std::string& name, const float* value);
    
    uint32_t program() const { return program_; }
    bool is_valid() const { return program_ != 0; }

private:
    bool compile_shader(uint32_t shader, const std::string& source);
    int get_uniform_location(const std::string& name);
    
    uint32_t program_ = 0;
    std::unordered_map<std::string, int> uniform_cache_;
};

class ShaderLibrary {
public:
    static ShaderLibrary& instance();
    
    void add(const std::string& name, Shader&& shader);
    Shader* get(const std::string& name);
    bool exists(const std::string& name) const;
    void load_all();
    
private:
    ShaderLibrary() = default;
    
    std::unordered_map<std::string, Shader> shaders_;
};

}
