#pragma once

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include <cstdint>

namespace cfd_ogl {

typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC) (GLenum shaderType);
typedef void (APIENTRYP PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void (APIENTRYP PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (APIENTRYP PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef GLuint (APIENTRYP PFNGLCREATEPROGRAMPROC) (void);
typedef void (APIENTRYP PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (APIENTRYP PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* params);
typedef void (APIENTRYP PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (APIENTRYP PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (APIENTRYP PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (APIENTRYP PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (APIENTRYP PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRYP PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRYP PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (APIENTRYP PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRYP PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef GLint (APIENTRYP PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (APIENTRYP PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef void (APIENTRYP PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (APIENTRYP PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (APIENTRYP PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (APIENTRYP PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (APIENTRYP PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (APIENTRYP PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void (APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (APIENTRYP PFNGLVERTEXATTRIB4FVPROC) (GLuint index, const GLfloat* v);
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices);

class GL {
public:
    static void init(GLFWwindow* window) {
        glCreateShader = reinterpret_cast<PFNGLCREATESHADERPROC>(glfwGetProcAddress("glCreateShader"));
        glDeleteShader = reinterpret_cast<PFNGLDELETESHADERPROC>(glfwGetProcAddress("glDeleteShader"));
        glShaderSource = reinterpret_cast<PFNGLSHADERSOURCEPROC>(glfwGetProcAddress("glShaderSource"));
        glCompileShader = reinterpret_cast<PFNGLCOMPILESHADERPROC>(glfwGetProcAddress("glCompileShader"));
        glGetShaderiv = reinterpret_cast<PFNGLGETSHADERIVPROC>(glfwGetProcAddress("glGetShaderiv"));
        glGetShaderInfoLog = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(glfwGetProcAddress("glGetShaderInfoLog"));
        glCreateProgram = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(glfwGetProcAddress("glCreateProgram"));
        glDeleteProgram = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(glfwGetProcAddress("glDeleteProgram"));
        glAttachShader = reinterpret_cast<PFNGLATTACHSHADERPROC>(glfwGetProcAddress("glAttachShader"));
        glLinkProgram = reinterpret_cast<PFNGLLINKPROGRAMPROC>(glfwGetProcAddress("glLinkProgram"));
        glGetProgramiv = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(glfwGetProcAddress("glGetProgramiv"));
        glGetProgramInfoLog = reinterpret_cast<PFNGLGETPROGRAMINFOLOGPROC>(glfwGetProcAddress("glGetProgramInfoLog"));
        glUseProgram = reinterpret_cast<PFNGLUSEPROGRAMPROC>(glfwGetProcAddress("glUseProgram"));
        glUniform1i = reinterpret_cast<PFNGLUNIFORM1IPROC>(glfwGetProcAddress("glUniform1i"));
        glUniform1f = reinterpret_cast<PFNGLUNIFORM1FPROC>(glfwGetProcAddress("glUniform1f"));
        glUniform2f = reinterpret_cast<PFNGLUNIFORM2FPROC>(glfwGetProcAddress("glUniform2f"));
        glUniform3f = reinterpret_cast<PFNGLUNIFORM3FPROC>(glfwGetProcAddress("glUniform3f"));
        glUniform3fv = reinterpret_cast<PFNGLUNIFORM3FVPROC>(glfwGetProcAddress("glUniform3fv"));
        glUniform4f = reinterpret_cast<PFNGLUNIFORM4FPROC>(glfwGetProcAddress("glUniform4f"));
        glUniformMatrix4fv = reinterpret_cast<PFNGLUNIFORMMATRIX4FVPROC>(glfwGetProcAddress("glUniformMatrix4fv"));
        glGetUniformLocation = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(glfwGetProcAddress("glGetUniformLocation"));
        glGenVertexArrays = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(glfwGetProcAddress("glGenVertexArrays"));
        glBindVertexArray = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(glfwGetProcAddress("glBindVertexArray"));
        glDeleteVertexArrays = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(glfwGetProcAddress("glDeleteVertexArrays"));
        glGenBuffers = reinterpret_cast<PFNGLGENBUFFERSPROC>(glfwGetProcAddress("glGenBuffers"));
        glBindBuffer = reinterpret_cast<PFNGLBINDBUFFERPROC>(glfwGetProcAddress("glBindBuffer"));
        glDeleteBuffers = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(glfwGetProcAddress("glDeleteBuffers"));
        glBufferData = reinterpret_cast<PFNGLBUFFERDATAPROC>(glfwGetProcAddress("glBufferData"));
        glEnableVertexAttribArray = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(glfwGetProcAddress("glEnableVertexAttribArray"));
        glVertexAttribPointer = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(glfwGetProcAddress("glVertexAttribPointer"));
        glVertexAttrib4fv = reinterpret_cast<PFNGLVERTEXATTRIB4FVPROC>(glfwGetProcAddress("glVertexAttrib4fv"));
        glDrawArrays = reinterpret_cast<PFNGLDRAWARRAYSPROC>(glfwGetProcAddress("glDrawArrays"));
        glDrawElements = reinterpret_cast<PFNGLDRAWELEMENTSPROC>(glfwGetProcAddress("glDrawElements"));
    }
    
    static PFNGLCREATESHADERPROC glCreateShader;
    static PFNGLDELETESHADERPROC glDeleteShader;
    static PFNGLSHADERSOURCEPROC glShaderSource;
    static PFNGLCOMPILESHADERPROC glCompileShader;
    static PFNGLGETSHADERIVPROC glGetShaderiv;
    static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
    static PFNGLCREATEPROGRAMPROC glCreateProgram;
    static PFNGLDELETEPROGRAMPROC glDeleteProgram;
    static PFNGLATTACHSHADERPROC glAttachShader;
    static PFNGLLINKPROGRAMPROC glLinkProgram;
    static PFNGLGETPROGRAMIVPROC glGetProgramiv;
    static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
    static PFNGLUSEPROGRAMPROC glUseProgram;
    static PFNGLUNIFORM1IPROC glUniform1i;
    static PFNGLUNIFORM1FPROC glUniform1f;
    static PFNGLUNIFORM2FPROC glUniform2f;
    static PFNGLUNIFORM3FPROC glUniform3f;
    static PFNGLUNIFORM3FVPROC glUniform3fv;
    static PFNGLUNIFORM4FPROC glUniform4f;
    static PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
    static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
    static PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
    static PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
    static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
    static PFNGLGENBUFFERSPROC glGenBuffers;
    static PFNGLBINDBUFFERPROC glBindBuffer;
    static PFNGLDELETEBUFFERSPROC glDeleteBuffers;
    static PFNGLBUFFERDATAPROC glBufferData;
    static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
    static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
    static PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
    static PFNGLDRAWARRAYSPROC glDrawArrays;
    static PFNGLDRAWELEMENTSPROC glDrawElements;
};

}
