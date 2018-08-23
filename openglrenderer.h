#pragma once

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
//#include <gl/glu.h> //seriously outdated (where does the turoial need this?)

namespace pttoth{
namespace engine{

class OpenGLRenderer{
//Graphics program
    GLuint _program = 0;
    GLint gVertexPos2DLocation = -1; //ez miez
    GLuint _vbo = 0;
    GLuint _ibo = 0;
    GLuint _vertexshader = 0;
    GLuint _fragmentshader = 0;

    SDL_GLContext _glcontext;
    SDL_Window* _window;

    //compiles gives shader and returns GL_TRUE if successful, and GL_FALSE otherwise
    GLint compileShader(GLuint shader, char* const sourcecode);

public:
    OpenGLRenderer();
    bool init(SDL_Window* window);
    void render(float t, float dt);

    //Shader loading utility programs
    void printProgramLog( GLuint program );
    void printShaderLog( GLuint shader );
};


}}

