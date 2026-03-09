#include <rendering/gl_functions.hh>

namespace cfd {
namespace gl {

PFNGLCREATESHADERPROC GL::glCreateShader = nullptr;
PFNGLDELETESHADERPROC GL::glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC GL::glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC GL::glCompileShader = nullptr;
PFNGLGETSHADERIVPROC GL::glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC GL::glGetShaderInfoLog = nullptr;
PFNGLCREATEPROGRAMPROC GL::glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC GL::glDeleteProgram = nullptr;
PFNGLATTACHSHADERPROC GL::glAttachShader = nullptr;
PFNGLLINKPROGRAMPROC GL::glLinkProgram = nullptr;
PFNGLGETPROGRAMIVPROC GL::glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC GL::glGetProgramInfoLog = nullptr;
PFNGLUSEPROGRAMPROC GL::glUseProgram = nullptr;
PFNGLUNIFORM1IPROC GL::glUniform1i = nullptr;
PFNGLUNIFORM1FPROC GL::glUniform1f = nullptr;
PFNGLUNIFORM2FPROC GL::glUniform2f = nullptr;
PFNGLUNIFORM3FPROC GL::glUniform3f = nullptr;
PFNGLUNIFORM3FVPROC GL::glUniform3fv = nullptr;
PFNGLUNIFORM4FPROC GL::glUniform4f = nullptr;
PFNGLUNIFORMMATRIX4FVPROC GL::glUniformMatrix4fv = nullptr;
PFNGLGETUNIFORMLOCATIONPROC GL::glGetUniformLocation = nullptr;
PFNGLGENVERTEXARRAYSPROC GL::glGenVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC GL::glBindVertexArray = nullptr;
PFNGLDELETEVERTEXARRAYSPROC GL::glDeleteVertexArrays = nullptr;
PFNGLGENBUFFERSPROC GL::glGenBuffers = nullptr;
PFNGLBINDBUFFERPROC GL::glBindBuffer = nullptr;
PFNGLDELETEBUFFERSPROC GL::glDeleteBuffers = nullptr;
PFNGLBUFFERDATAPROC GL::glBufferData = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC GL::glEnableVertexAttribArray = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC GL::glVertexAttribPointer = nullptr;
PFNGLVERTEXATTRIB4FVPROC GL::glVertexAttrib4fv = nullptr;
PFNGLDRAWARRAYSPROC GL::glDrawArrays = nullptr;
PFNGLDRAWELEMENTSPROC GL::glDrawElements = nullptr;

}
}
