#include "openglrenderer.h"

#include <iostream>
#include <stdio.h>

#include "shaders.h"

#include "stdio.h"
#include <cstring>

using namespace pttoth;
using namespace pttoth::engine;

GLint OpenGLRenderer::
        compileShader(GLuint shader,
                      char* const sourcecode){
    size_t length = strlen( sourcecode );
    glShaderSource( shader, 1, ( const GLchar ** )&sourcecode, &length );
    glCompileShader( shader );

    GLint status;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
    return status;
}

OpenGLRenderer::
        OpenGLRenderer(){
}

bool OpenGLRenderer::
        init(SDL_Window* window){
    _window = window;

    //Use OpenGL 3.1 core
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    //Create context
    _glcontext = SDL_GL_CreateContext(_window);
    if( _glcontext == NULL ) {
        std::cerr << "OpenGL context could not be created! SDL Error: "
                  << SDL_GetError() << "\n";
        return false;
    }

    //Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if( glewError != GLEW_OK ) {
        std::cerr << "Error initializing GLEW! "
                  << glewGetErrorString( glewError ) << "\n";
        return false;
    }

    //Initialize OpenGL
    if( !initGL() ) {
        std::cerr << "Unable to initialize OpenGL!\n";
        return false;
    }

    //compile imported vertex shader
    GLuint result;
    result = compileShader(_vertexshader, VERTEXSHADER_BASIC);
    if( result == GL_FALSE ){
        std::cerr << "vertex shader compilation failed\n";
        return false;
    }

    //compile imported fragment shader
    result = compileShader(_fragmentshader, FRAGMENTSHADER_BASIC);
    if( result == GL_FALSE ){
        std::cerr << "fragment shader compilation failed\n";
        return false;
    }

    _program = glCreateProgram();
    glAttachShader( _program, _vertexshader );
    glAttachShader( _program, _fragmentshader );

    return true;
}

void OpenGLRenderer::
        render(float t, float dt){

}
