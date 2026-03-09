#pragma once

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <functional>
#include <unordered_map>
#include <cstring>

namespace cfd {
namespace gl {

class Loader {
public:
    static void init() {
        static Loader loader;
    }
    
    static void* get_proc(const char* name) {
        static Loader loader;
        return loader.load(name);
    }
    
    void* load(const char* name) {
        auto it = functions_.find(name);
        if (it != functions_.end()) {
            return reinterpret_cast<void*>(it->second);
        }
        
        void* proc = reinterpret_cast<void*>(glfwGetProcAddress(name));
        if (proc) {
            functions_[name] = reinterpret_cast<std::uintptr_t>(proc);
        }
        return proc;
    }
    
private:
    Loader() {
        load_core_functions();
    }
    
    void load_core_functions() {
        functions_["glCreateShader"] = reinterpret_cast<std::uintptr_t>(glCreateShader);
        functions_["glDeleteShader"] = reinterpret_cast<std::uintptr_t>(glDeleteShader);
        functions_["glShaderSource"] = reinterpret_cast<std::uintptr_t>(glShaderSource);
        functions_["glCompileShader"] = reinterpret_cast<std::uintptr_t>(glCompileShader);
        functions_["glGetShaderiv"] = reinterpret_cast<std::uintptr_t>(glGetShaderiv);
        functions_["glGetShaderInfoLog"] = reinterpret_cast<std::uintptr_t>(glGetShaderInfoLog);
        functions_["glCreateProgram"] = reinterpret_cast<std::uintptr_t>(glCreateProgram);
        functions_["glDeleteProgram"] = reinterpret_cast<std::uintptr_t>(glDeleteProgram);
        functions_["glAttachShader"] = reinterpret_cast<std::uintptr_t>(glAttachShader);
        functions_["glLinkProgram"] = reinterpret_cast<std::uintptr_t>(glLinkProgram);
        functions_["glGetProgramiv"] = reinterpret_cast<std::uintptr_t>(glGetProgramiv);
        functions_["glGetProgramInfoLog"] = reinterpret_cast<std::uintptr_t>(glGetProgramInfoLog);
        functions_["glUseProgram"] = reinterpret_cast<std::uintptr_t>(glUseProgram);
        functions_["glUniform1i"] = reinterpret_cast<std::uintptr_t>(glUniform1i);
        functions_["glUniform1f"] = reinterpret_cast<std::uintptr_t>(glUniform1f);
        functions_["glUniform2f"] = reinterpret_cast<std::uintptr_t>(glUniform2f);
        functions_["glUniform3f"] = reinterpret_cast<std::uintptr_t>(glUniform3f);
        functions_["glUniform3fv"] = reinterpret_cast<std::uintptr_t>(glUniform3fv);
        functions_["glUniform4f"] = reinterpret_cast<std::uintptr_t>(glUniform4f);
        functions_["glUniformMatrix4fv"] = reinterpret_cast<std::uintptr_t>(glUniformMatrix4fv);
        functions_["glGetUniformLocation"] = reinterpret_cast<std::uintptr_t>(glGetUniformLocation);
        functions_["glGenVertexArrays"] = reinterpret_cast<std::uintptr_t>(glGenVertexArrays);
        functions_["glBindVertexArray"] = reinterpret_cast<std::uintptr_t>(glBindVertexArray);
        functions_["glDeleteVertexArrays"] = reinterpret_cast<std::uintptr_t>(glDeleteVertexArrays);
        functions_["glGenBuffers"] = reinterpret_cast<std::uintptr_t>(glGenBuffers);
        functions_["glBindBuffer"] = reinterpret_cast<std::uintptr_t>(glBindBuffer);
        functions_["glDeleteBuffers"] = reinterpret_cast<std::uintptr_t>(glDeleteBuffers);
        functions_["glBufferData"] = reinterpret_cast<std::uintptr_t>(glBufferData);
        functions_["glEnableVertexAttribArray"] = reinterpret_cast<std::uintptr_t>(glEnableVertexAttribArray);
        functions_["glVertexAttribPointer"] = reinterpret_cast<std::uintptr_t>(glVertexAttribPointer);
        functions_["glVertexAttrib4fv"] = reinterpret_cast<std::uintptr_t>(glVertexAttrib4fv);
        functions_["glDrawArrays"] = reinterpret_cast<std::uintptr_t>(glDrawArrays);
        functions_["glDrawElements"] = reinterpret_cast<std::uintptr_t>(glDrawElements);
    }
    
    std::unordered_map<std::string, std::uintptr_t> functions_;
};

#define GL_PROC(name) \
    static std::function<decltype(name)> name = \
        [](auto&&... args) -> decltype(auto) { \
            static auto proc = Loader::get_proc(#name); \
            using FuncType = decltype(name)(*); \
            return reinterpret_cast<FuncType>(proc)(args...); \
        }

}
}
