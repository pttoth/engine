#include <iostream>

#include <cstddef>
#include <assert.h>
#include <cstdio>
#include <vector>
//#include <cstdlib>
#include <cstring>

#include "GL/glew.h"
#include "GL/gl.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_main.h"

#include "engine/glhelpers.h"
#include "engine/mathhelpers.hpp"
#include "engine/shaders.h"
#include "libpng/png.h"

#include "engine/texture.h"
#include "cube.h"

#include "engine/drawhelpers.h"

#include "engine/camera.h"

#include "assimp/scene.h"


#include "newmesh.h"

#include <sstream>

#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wdouble-promotion"

using namespace pt;

//--------------------------------------------------------------------------------
//class declarations

class Spotlight;

//--------------------------------------------------------------------------------
//globals

static          bool            bDisplayFramerate               = true;
static const    char            keyToggleFramerateDisplay       = 'f';
static const    char            keyForceRedraw                  = 'r';
static const    char            keyQuitApplication              = SDLK_ESCAPE;
static const    char            keyCameraForward                = 'w';
static const    char            keyCameraBackward               = 's';
static const    char            keyCameraLeft                   = 'a';
static const    char            keyCameraRight                  = 'd';
static const    char            keyCameraUp                     = SDLK_SPACE;
static const    int             keyCameraDown                   = /*'c';*/ SDLK_LCTRL;
static const    int             keySprint                       = /*'v';*/ SDLK_LSHIFT;
//campbell
static const    int             keyHeadLeftPressed              = SDLK_LEFT;
static const    int             keyHeadRightPressed             = SDLK_RIGHT;
static const    int             keyHeadUpPressed                = SDLK_UP;
static const    int             keyHeadDownPressed              = SDLK_DOWN;

static          float           camera_speed                    = 10.0f;
static          engine::Camera* camera                          = nullptr;

static          GLuint          ShaderProgram;
static const    char*           shNameScale                     = "Scale";
static const    char*           shNameColor                     = "Color";          //rem
static const    char*           shNameM                         = "M";
static const    char*           shNameVP                        = "VP";
static const    char*           shNameMVP                       = "MVP";
static const    char*           shNameUseFixColor               = "UseFixColor";    //rem
static const    char*           shNameGSampler                  = "gSampler";
static const    char*           shNameUseAlphaOverride          = "UseAlphaOverride";
static const    char*           shNameAlpha                     = "Alpha";
static const    char*           shNameLightAmbient              = "LightAmbient";
static const    char*           shNameSpotlights                = "Spotlights";

static          math::float3    shUniColor                      = color_white;      //rem
static          int             shUniScale                      = 1;
static          int             shUniUseFixColor                = 0;                //rem
static          int             shUniUseAlphaOverride           = 0;
static          float           shUniAlpha                      = 1.0f;
static          math::float3    shUniLightAmbient               = math::float3(1.0f, 1.0f, 1.0f);
static          std::vector<Spotlight> shUniSpotlights          ;

static          bool            bKeyForwardPressed              = false;
static          bool            bKeyBackwardPressed             = false;
static          bool            bKeyLeftPressed                 = false;
static          bool            bKeyRightPressed                = false;
static          bool            bKeyUpPressed                   = false;
static          bool            bKeyDownPressed                 = false;
static          bool            bKeySprintPressed               = false;
//campbell
static          bool            bKeyHeadLeftPressed             = false;
static          bool            bKeyHeadRightPressed            = false;
static          bool            bKeyHeadUpPressed               = false;
static          bool            bKeyHeadDownPressed             = false;

static          float           fCampbellHeadSpeed              = 1.0f;

static          bool            mouseAimEnabled                 = false;
static          float           mousespeed_x                    = 0.30f;
static          float           mousespeed_y                    = 0.30f;


//--------------------------------------------------------------------------------
//class definitions
class Spotlight{
#pragma GCC diagnostic ignored "-Wno-covered-switch-default"
    enum UniformEnum{
        ENABLED = 0,
        INTENSITY_DIFFUSE,
        COLOR,
        DIRECTION,
    };

    //note:
    //  the order of members have to be in sync
    //    with shader Spotlight struct!
    bool            m_enabled;

    std::string getShaderUniformName(UniformEnum variable_name, int index = -1) const{
        std::stringstream   ss;
        std::string         membername;

        assert(index < 64);
        switch(variable_name){
            case ENABLED:               membername = "enabled";                 break;
            case COLOR:                 membername = "color";                   break;
            case INTENSITY_DIFFUSE:     membername = "intensity_diffuse";       break;
            case DIRECTION:             membername = "direction";               break;
            default:                    assert(false);                          break;
        }
        ss << shNameSpotlights;
        if(-1 < index){
            ss << "[" << index << "]";

        }else{              //todo: delete this line
            ss << "[0]";    //todo: delete this line

        }
        ss << "." << membername;

        return ss.str();
#pragma GCC diagnostic warning "-Wno-covered-switch-default"
    }

public:
    float           intensity_diffuse;
    math::float3    color;
    math::float3    direction;

    Spotlight():m_enabled(false),
                 intensity_diffuse(0.0f){
    }
    Spotlight(bool enabled_,
              float intensity_diffuse_,
              math::float3 color_,
              math::float3 direction_):
                    m_enabled(enabled_),
                    intensity_diffuse(intensity_diffuse_),
                    color(color_),
                    direction(direction_){
    }
    void enable(){      m_enabled = true; }
    void disable(){     m_enabled = false; }
    bool isEnabled(){   return m_enabled; }

    void loadToGPU(GLuint ShaderProgram,
                   uint32_t index){
        std::string shEnabled   = getShaderUniformName(ENABLED, index);
        std::string shColor     = getShaderUniformName(COLOR, index);
        std::string shIDiffuse  = getShaderUniformName(INTENSITY_DIFFUSE, index);
        std::string shDirection = getShaderUniformName(DIRECTION, index);

        GLuint hEnabled     = gl::GetUniformLocation(ShaderProgram, shEnabled.c_str());
        GLuint hColor       = gl::GetUniformLocation(ShaderProgram, shColor.c_str());
        GLuint hIDiffuse    = gl::GetUniformLocation(ShaderProgram, shIDiffuse.c_str());
        GLuint hDirection   = gl::GetUniformLocation(ShaderProgram, shDirection.c_str());

        if(m_enabled){      gl::Uniform1i(hEnabled, 1);
        }else{              gl::Uniform1i(hEnabled, 0);
        }
        gl::UniformFloat3(  hColor,     color);
        gl::Uniform1f(      hIDiffuse,  intensity_diffuse);
        gl::UniformFloat3(  hDirection, direction);
    }
};



//tria1
static          bool                        tria1_initialized;
static          GLuint                      tria1_vbo;
static          math::float3                tria1_pos;
static          math::float3                tria1_pos_initial;
static          float                       tria1_orient;
static          math::float4                tria1_modelTF;
//tria2
static          bool                        tria2_initialized;
static          GLuint                      tria2_vbo;
static          math::float3                tria2_pos;
static          math::float3                tria2_pos_initial;
static          float                       tria2_orient;
static          math::float4                tria2_modelTF;
//cube
static          Cube                        cube;
static          math::float3                cube_pos_initial;

//tex
static          pt::engine::Texture         tex;
static          GLuint                      tex_vbo;

//campbell

static          std::vector<std::pair<std::string, std::string>> campbell_head_texture_names;
static          std::vector<pt::engine::Texture*>                campbell_head_textures;
static          std::vector<pt::engine::NewMesh*>                campbell_head_meshes;
static          math::float4x4                                   campbell_head_M;
static          std::vector<pt::engine::Texture*>                campbell_body_textures;
static          std::vector<pt::engine::NewMesh*>                campbell_body_meshes;


//lights
float           ambientintensity    = 1.0f;
math::float3    ambientcolor        = color_white;

std::vector<gl::Vertex> pyramids_vertices;
std::vector<math::int3> pyramids_indices;
GLuint pyramids_vbo;
GLuint pyramids_ibo;
bool pyramids_initialized;


int CompileShaders(){
    ShaderProgram = glCreateProgram();


    GLuint  vs_obj      = glCreateShader(GL_VERTEX_SHADER);
    int     vs_length   = strlen(VertexShader);
    gl::ShaderSource(vs_obj, 1, &VertexShader, &vs_length);

    GLuint  gs_obj      = glCreateShader(GL_GEOMETRY_SHADER);
    int     gs_length   = strlen(GeometryShader);
    gl::ShaderSource(gs_obj, 1, &GeometryShader, &gs_length);

    GLuint  fs_obj      = glCreateShader(GL_FRAGMENT_SHADER);
    int     fs_length   = strlen(FragmentShader);
    gl::ShaderSource(fs_obj, 1, &FragmentShader, &fs_length);

    std::vector<GLuint>         shaderobjects;
    std::vector<std::string>    shadernames;
    shaderobjects.push_back(vs_obj);    shadernames.push_back("vertex shader");
    //shaderobjects.push_back(gs_obj);    shadernames.push_back("geometry shader");
    shaderobjects.push_back(fs_obj);    shadernames.push_back("fragment shader");

    GLint success;
    for(size_t i = 0; i<shaderobjects.size(); ++i){
        GLuint      shader      = shaderobjects[i];
        std::string shadername  = shadernames[i];
        gl::CompileShader(shader);
        gl::GetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLchar InfoLog[1024];
            gl::GetShaderInfoLog(shader, sizeof(InfoLog), NULL, InfoLog);
            fprintf(stderr, "\n  %s compilation failed:\n    '%s'\n", shadername.data(), InfoLog);
            return 1;
        }
    }

    gl::AttachShader(ShaderProgram, vs_obj);
    //gl::AttachShader(ShaderProgram, gs_obj);
    gl::AttachShader(ShaderProgram, fs_obj);
    gl::LinkProgram(ShaderProgram);

    gl::GetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar ErrorLog[1024];
        gl::GetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "\n  Error linking shader program: '%s'\n", ErrorLog);
    }

    gl::ValidateProgram(ShaderProgram);
    gl::UseProgram(ShaderProgram);

    GLint uniScale = gl::GetUniformLocation(ShaderProgram, shNameScale);
    gl::Uniform1i(uniScale, shUniScale);

    GLint uniColor = gl::GetUniformLocation(ShaderProgram, shNameColor);
    gl::Uniform3fv(uniColor, 1, shUniColor.v);

    GLint uniUseFixColor = gl::GetUniformLocation(ShaderProgram, shNameUseFixColor);
    gl::Uniform1i(uniUseFixColor, shUniUseFixColor);

    GLint uniUseAlphaOverride = gl::GetUniformLocation(ShaderProgram, shNameUseAlphaOverride);
    gl::Uniform1i(uniUseAlphaOverride, shUniUseAlphaOverride);

    GLint uniAlpha = gl::GetUniformLocation(ShaderProgram, shNameAlpha);
    gl::Uniform1f(uniAlpha, shUniAlpha);

    GLint uniLightAmbient = gl::GetUniformLocation(ShaderProgram, shNameLightAmbient);
    gl::UniformFloat3(uniLightAmbient, shUniLightAmbient);

    GLint uniM = gl::GetUniformLocation(ShaderProgram, shNameM);
    gl::UniformFloat4x4(uniM, gl::DO_TRANSPOSE, math::float4x4::identity);

    GLint uniVP = gl::GetUniformLocation(ShaderProgram, shNameVP);
    gl::UniformFloat4x4(uniVP, gl::DO_TRANSPOSE, math::float4x4::identity);

    GLint uniMVP = gl::GetUniformLocation(ShaderProgram, shNameMVP);
    gl::UniformFloat4x4(uniMVP, gl::DO_TRANSPOSE, math::float4x4::identity);

    int sl_count = 64;

    for(int i=0; i<sl_count; ++i){
        Spotlight sl;
        sl.disable();
        sl.loadToGPU(ShaderProgram, i);
    }

    return 0;
}

int Initialize(SDL_Window** window,
                SDL_GLContext* glcontext){
    std::cout << "initializing...";
    //init SDL
    SDL_Init(SDL_INIT_EVENTS
             | SDL_INIT_TIMER
             );

    //set up OpenGL context
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    /*
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    */

    //create window
    *window = SDL_CreateWindow("OpenGL tutorial: testing window",
                                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                1280, 720,
                                SDL_WINDOW_OPENGL
                             //   |SDL_WINDOW_RESIZABLE
                             //   |SDL_WINDOW_INPUT_GRABBED
                                 );
    //create OpenGL context
    *glcontext = SDL_GL_CreateContext(*window);

    //init glew
    glewExperimental = GL_TRUE;     //note: must be before glewInit() !!!
    GLenum res = glewInit();
    if (res != GLEW_OK){
        fprintf(stderr, "\n  GLEW error: '%s'\n", glewGetErrorString(res));
        SDL_DestroyWindow(*window);
        return 1;
    }

    std::cout << " done\n---\n";
    int majorVersion, minorVersion;
    printf("GL Vendor    : %s\n", glGetString(GL_VENDOR));
    printf("GL Renderer  : %s\n", glGetString(GL_RENDERER));
    printf("GL Version (string)  : %s\n", glGetString(GL_VERSION));
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
    printf("GL Version (integer) : %d.%d\n", majorVersion, minorVersion);
    printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    std::cout << "---\ncompiling shaders...";
    int failed = CompileShaders();
    if( failed ){
        SDL_DestroyWindow(*window);
        return 1;
    }

    //enable depth testing
    glEnable(GL_DEPTH_TEST);

    //enable backface culling
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //draw something
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(*window);
    std::cout << " done\n";
    return 0;
}

template<typename T>
void PrintVector(const char* name, T v, size_t size){
    std::cout << "vec: " << name << "\n";
    for(size_t i=0; i<size; ++i){
        std::cout << v.v[i];
        if(i<size-1){   std::cout << ",  ";
        }else{          std::cout << "\n";
        }
    }
}

void PrintMatrix(const char* name, const math::float4x4 mtx){
    std::cout << "mtx: " << name << "\n";
    for(size_t j=0; j<4; ++j){
        for(size_t i=0; i<4; ++i){
            //std::cout << mtx.m[j][i];
            printf("%.4f", mtx.m[j][i]);
            if(i<4-1){  std::cout << ",    ";
            }else{      std::cout << "\n";
            }
        }
    }
}

#include "engine/modelimporter.h"


void CreateEntities(){
    //create camera
    camera = new engine::Camera();

    //-------------------------------------------------------
    //create double pyramid
    {

        //TODO: release GPU resources!

        //TODO: use position to place the object and not the vertex positions
        pyramids_vertices.clear();
        pyramids_vertices.push_back( math::float3(   0.0f,   0.0f,  -3.0f ) );
        pyramids_vertices.push_back( math::float3(   0.0f,   8.0f, -10.5f ) );
        pyramids_vertices.push_back( math::float3(   4.0f,  -4.0f, -10.5f ) );
        pyramids_vertices.push_back( math::float3(  -4.0f,  -4.0f, -10.5f ) );
        pyramids_vertices.push_back( math::float3(   0.0f,   0.0f, -18.0f ) );

        pyramids_indices.clear();
        pyramids_indices.push_back( math::int3( 0, 1, 2 ) );
        pyramids_indices.push_back( math::int3( 0, 3, 1 ) );
        pyramids_indices.push_back( math::int3( 0, 2, 3 ) );
        pyramids_indices.push_back( math::int3( 4, 2, 1 ) );
        pyramids_indices.push_back( math::int3( 4, 1, 3 ) );
        pyramids_indices.push_back( math::int3( 4, 3, 2 ) );

        GenerateNormals( pyramids_vertices, pyramids_indices );

        gl::GenBuffers(1, &pyramids_vbo);
        gl::GenBuffers(1, &pyramids_ibo);

        gl::BindBuffer(GL_ARRAY_BUFFER,         pyramids_vbo);
        gl::BufferData(GL_ARRAY_BUFFER,
                         pyramids_vertices.size()*sizeof(gl::Vertex),
                         pyramids_vertices.data(),
                         GL_STATIC_DRAW);

        gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramids_ibo);
        gl::BufferData(GL_ELEMENT_ARRAY_BUFFER,
                         pyramids_indices.size()*sizeof(math::int3),
                         pyramids_indices.data(),
                         GL_STATIC_DRAW);
        pyramids_initialized = true;
    }


    //-------------------------------------------------------
    //create double pyramid
    {

        //TODO: release GPU resources!

        //TODO: use position to place the object and not the vertex positions
        pyramids_vertices.clear();
        pyramids_vertices.push_back( math::float3(   0.0f,   0.0f,  -3.0f ) );
        pyramids_vertices.push_back( math::float3(   0.0f,   8.0f, -10.5f ) );
        pyramids_vertices.push_back( math::float3(   4.0f,  -4.0f, -10.5f ) );
        pyramids_vertices.push_back( math::float3(  -4.0f,  -4.0f, -10.5f ) );
        pyramids_vertices.push_back( math::float3(   0.0f,   0.0f, -18.0f ) );

        pyramids_indices.clear();
        pyramids_indices.push_back( math::int3( 0, 1, 2 ) );
        pyramids_indices.push_back( math::int3( 0, 3, 1 ) );
        pyramids_indices.push_back( math::int3( 0, 2, 3 ) );
        pyramids_indices.push_back( math::int3( 4, 2, 1 ) );
        pyramids_indices.push_back( math::int3( 4, 1, 3 ) );
        pyramids_indices.push_back( math::int3( 4, 3, 2 ) );

        GenerateNormals( pyramids_vertices, pyramids_indices );

        gl::GenBuffers(1, &pyramids_vbo);
        gl::GenBuffers(1, &pyramids_ibo);

        gl::BindBuffer(GL_ARRAY_BUFFER,         pyramids_vbo);
        gl::BufferData(GL_ARRAY_BUFFER,
                         pyramids_vertices.size()*sizeof(gl::Vertex),
                         pyramids_vertices.data(),
                         GL_STATIC_DRAW);

        gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramids_ibo);
        gl::BufferData(GL_ELEMENT_ARRAY_BUFFER,
                         pyramids_indices.size()*sizeof(math::int3),
                         pyramids_indices.data(),
                         GL_STATIC_DRAW);
        pyramids_initialized = true;
    }


    //-------------------------------------------------------
    //create triangle
    {
        if(tria1_initialized){
            glDeleteTextures(1, &tria1_vbo);
        }
        std::vector<gl::Vertex> vertices;
        vertices.push_back( math::float3( -1.0f, 0.0f, -1.0f ) );
        vertices.push_back( math::float3(  0.5f, 0.0f, -1.0f ) );
        vertices.push_back( math::float3(  1.0f, 0.0f,  1.0f ) );

        //tria1_orient = M_PI/2;
        tria1_orient = 0;

        gl::GenBuffers(1, &tria1_vbo);

        gl::BindBuffer(GL_ARRAY_BUFFER, tria1_vbo);
        gl::BufferData(GL_ARRAY_BUFFER,
                         vertices.size()*sizeof(gl::Vertex),
                         vertices.data(),
                         GL_STATIC_DRAW);
        tria1_initialized = true;
    }

    //-------------------------------------------------------
    //create second triangle
    {
        if(tria2_initialized){
            glDeleteTextures(1, &tria2_vbo);
        }
        std::vector<gl::Vertex> vertices;
        vertices.push_back( math::float3( -1.0f,  0.0f, -1.0f ) );
        vertices.push_back( math::float3(  1.0f,  0.0f,  1.0f ) );
        vertices.push_back( math::float3(  0.5f,  0.0f, -1.0f ) );

        //tria2_orient = M_PI/2;
        tria2_orient = 0;

        gl::GenBuffers(1, &tria2_vbo);

        gl::BindBuffer(GL_ARRAY_BUFFER, tria2_vbo);
        gl::BufferData(GL_ARRAY_BUFFER,
                         vertices.size()*sizeof(gl::Vertex),
                         vertices.data(),
                         GL_STATIC_DRAW);
        tria2_initialized = true;
    }

    //-------------------------------------------------------
    //create cube
    cube.ShaderProgram      = ShaderProgram;
    cube.shNameMVP          = shNameMVP;
    cube.shNameColor        = shNameColor;
    cube.shNameGSampler     = shNameGSampler;
    cube.shNameUseFixColor  = shNameUseFixColor;
    cube.cam                = camera;


    cube.createRenderContext();

    //-------------------------------------------------------
    //create textured quad
    {
        //tex.bind(GL_TEXTURE0);
        //tex.loadToGPUFromFile("test1_128.png");
        tex.loadToGPUFromFile("../../media/test1_512.png");
        //tex.loadToGPUFromFile("../../media/test1_512x256.png");
        //tex.loadToGPUFromFile("../../media/test1_367x345.png");

        std::vector<gl::Vertex> vertices;
        vertices.push_back( gl::Vertex( math::float3( -5.0f,  1.0f,  5.0f), math::float2(0.0f, 0.0f)) );
        vertices.push_back( gl::Vertex( math::float3(  5.0f,  1.0f,  5.0f), math::float2(1.0f, 0.0f)) );
        vertices.push_back( gl::Vertex( math::float3( -5.0f,  1.0f, -5.0f), math::float2(0.0f, 1.0f)) );
        vertices.push_back( gl::Vertex( math::float3(  5.0f,  1.0f, -5.0f), math::float2(1.0f, 1.0f)) );
        vertices.push_back( gl::Vertex( math::float3( -5.0f,  1.0f, -5.0f), math::float2(0.0f, 1.0f)) );
        vertices.push_back( gl::Vertex( math::float3(  5.0f,  1.0f,  5.0f), math::float2(1.0f, 0.0f)) );
        gl::GenBuffers(1, &tex_vbo);
        gl::BindBuffer(GL_ARRAY_BUFFER, tex_vbo);
        gl::BufferData(GL_ARRAY_BUFFER,
                         vertices.size()*sizeof(gl::Vertex),
                         vertices.data(),
                         GL_STATIC_DRAW);
    }

    //-------------------------------------------------------
    //create campbell head

    {
        campbell_head_M = math::float4x4::identity;
        campbell_head_M.m[3][0] = -1.5f;
        campbell_head_M.m[3][1] = 0.0f;
        campbell_head_M.m[3][2] = 61.883f;

        //set up texture dictionary
        std::pair<std::string, std::string> pair1, pair2, pair3, pair4, pair5;

        pair1.first = std::string("models/characters/common/teethcampbell.msh");    pair1.second = std::string("models/characters/common/teeth.tga");
        pair2.first = std::string("models/characters/common/tongue.msh");           pair2.second = std::string("models/characters/common/tongue.tga");
        pair3.first = std::string("models/characters/Campbell/campbell.msh");       pair3.second = std::string("models/characters/campbell/campbell.tga");
                                                                                    //pair3.second = std::string("models/characters/campbell/dcampbell.tga");
        pair4.first = std::string("models/characters/common/leftcampbell.msh");     pair4.second = std::string("models/characters/common/blue.tga");
        pair5.first = std::string("models/characters/common/rightcampbell.msh");    pair5.second = std::string("models/characters/common/blue.tga");

        campbell_head_texture_names.push_back(pair1);
        campbell_head_texture_names.push_back(pair2);
        campbell_head_texture_names.push_back(pair3);
        campbell_head_texture_names.push_back(pair4);
        campbell_head_texture_names.push_back(pair5);

        std::string texname1 = std::string("../../media/campbell/texture/models/characters/common/teeth.png");
        std::string texname2 = std::string("../../media/campbell/texture/models/characters/common/tongue.png");
        std::string texname3 = std::string("../../media/campbell/texture/campbell.png");
//                    texname3 = std::string("../../media/campbell/texture/dcampbell.png");
        std::string texname4 = std::string("../../media/campbell/texture/models/characters/common/blue.png");
        std::string texname5 = std::string("../../media/campbell/texture/models/characters/common/blue.png");

        pt::engine::Texture* tex1 = new pt::engine::Texture();      campbell_head_textures.push_back(tex1);
        pt::engine::Texture* tex2 = new pt::engine::Texture();      campbell_head_textures.push_back(tex2);
        pt::engine::Texture* tex3 = new pt::engine::Texture();      campbell_head_textures.push_back(tex3);
        pt::engine::Texture* tex4 = new pt::engine::Texture();      campbell_head_textures.push_back(tex4);
        pt::engine::Texture* tex5 = new pt::engine::Texture();      campbell_head_textures.push_back(tex5);

        pt::engine::RegisterTexture(texname1, tex1);
        pt::engine::RegisterTexture(texname2, tex2);
        pt::engine::RegisterTexture(texname3, tex3);
        pt::engine::RegisterTexture(texname4, tex4);
        pt::engine::RegisterTexture(texname5, tex5);

        tex1->loadToGPUFromFile(texname1);
        tex2->loadToGPUFromFile(texname2);
        tex3->loadToGPUFromFile(texname3);
        tex4->loadToGPUFromFile(texname4);
        tex5->loadToGPUFromFile(texname5);

        std::vector<std::string> mat_names;
        //fix materials
        for(size_t i=0; i<campbell_head_meshes.size(); ++i){
            campbell_head_meshes[i]->setMaterialName(campbell_head_texture_names[i].second);
        }

        bool msuc;
        pt::ModelImporter importer;
        msuc = importer.ReadModel("../../media/campbell/mesh/head/campbell.md5mesh", campbell_head_meshes);

        if (!msuc){
            std::cout << "failed to read model file\n";
            return;
        }

        campbell_head_meshes[0]->ShaderProgram = ShaderProgram;
        campbell_head_meshes[1]->ShaderProgram = ShaderProgram;
        campbell_head_meshes[2]->ShaderProgram = ShaderProgram;
        campbell_head_meshes[3]->ShaderProgram = ShaderProgram;
        campbell_head_meshes[4]->ShaderProgram = ShaderProgram;

        campbell_head_meshes[0]->setMaterialName(texname1);
        campbell_head_meshes[1]->setMaterialName(texname2);
        campbell_head_meshes[2]->setMaterialName(texname3);
        campbell_head_meshes[3]->setMaterialName(texname4);
        campbell_head_meshes[4]->setMaterialName(texname5);

        //although this fixes the artifact triangles
        // it also messes up the iris textures in the eyes
/*
        //-----
        //fix the eyes' artifact triangles
        const aiScene* pAiScene = importer.getAiScene();
        {
            auto newfaces = campbell_meshes[3]->mFaces;
            newfaces.clear();
            //exclude the entry under index(1)
            newfaces.push_back(campbell_meshes[3]->mFaces[0]);
            for(size_t i=2; i<newfaces.size(); ++i){
                newfaces.push_back(campbell_meshes[3]->mFaces[i]);
            }
            campbell_meshes[3]->mFaces = newfaces;
        }
        {
            auto newfaces = campbell_meshes[4]->mFaces;
            newfaces.clear();
            //exclude the entry under index(1)
            newfaces.push_back(campbell_meshes[4]->mFaces[0]);
            for(size_t i=2; i<newfaces.size(); ++i){
                newfaces.push_back(campbell_meshes[4]->mFaces[i]);
            }
            campbell_meshes[4]->mFaces = newfaces;
        }
        //-----
*/
        campbell_head_meshes[0]->createRenderContext();
        campbell_head_meshes[1]->createRenderContext();
        campbell_head_meshes[2]->createRenderContext();
        campbell_head_meshes[3]->createRenderContext();
        campbell_head_meshes[4]->createRenderContext();
    }

    //create campbell body

    {
        pt::ModelImporter importer_campbell_body;
        bool success = importer_campbell_body.ReadModel("../../media/campbell/mesh/body/campbell.md5mesh", campbell_body_meshes);

        if (!success){
            std::cout << "failed to read model file\n";
            return;
        }

        std::string texname1 = std::string("../../media/campbell/texture/models/characters/male_npc/marine/marine.png");
        //std::string texname2 = std::string("../../media/campbell/texture/models/characters/common/tongue.png");
        std::string texname2 = std::string("");
    //-----------------
    //fix material info
        campbell_body_meshes[0]->setMaterialName(texname1);
        campbell_body_meshes[0]->setMaterialName(texname2);
    //-----------------
        pt::engine::Texture* tex1 = new pt::engine::Texture();      campbell_body_textures.push_back(tex1);
        pt::engine::Texture* tex2 = new pt::engine::Texture();      campbell_body_textures.push_back(tex2);

        pt::engine::RegisterTexture(texname1, tex1);
        pt::engine::RegisterTexture(texname2, tex2);

        tex1->loadToGPUFromFile(texname1);
        tex2->loadToGPUFromFile(texname2);

        std::vector<std::string> mat_names;

        campbell_body_meshes[0]->ShaderProgram = ShaderProgram;
        campbell_body_meshes[1]->ShaderProgram = ShaderProgram;

        campbell_body_meshes[0]->setMaterialName(texname1);
        campbell_body_meshes[1]->setMaterialName(texname2);

        campbell_body_meshes[0]->createRenderContext();
        campbell_body_meshes[1]->createRenderContext();
    }

    //...

}

void DestroyEntities(){
    if(tria1_initialized){ gl::DeleteBuffers(1, &tria1_vbo); }
    if(tria2_initialized){ gl::DeleteBuffers(1, &tria2_vbo); }
    if(tex.isInitialized() ){
        gl::DeleteBuffers(1, &tex_vbo);
        tex.unload();
    }
    cube.destroyRenderContext();
    delete camera;  camera = nullptr;
}

void DrawContents(){
    assert(camera != nullptr);
    math::float4x4 MVP = math::float4x4::identity;
    math::float4x4 M;
    math::float4x4 V;
    math::float4x4 P;

    //View and Proj matrices
    V = camera->getViewMtx();
    P = camera->getProjMtx();

    GLint uniMVP                = gl::GetUniformLocation(ShaderProgram, shNameMVP);
    GLint uniUseFixColor        = gl::GetUniformLocation(ShaderProgram, shNameUseFixColor);
    GLint uniColor              = gl::GetUniformLocation(ShaderProgram, shNameColor);
    GLint uniGSampler           = gl::GetUniformLocation(ShaderProgram, shNameGSampler);
    GLint uniUseAlphaOverride   = gl::GetUniformLocation(ShaderProgram, shNameUseAlphaOverride);
    GLint uniAlpha              = gl::GetUniformLocation(ShaderProgram, shNameAlpha);

    gl::UniformMatrix4fv(uniMVP, 1, gl::DO_TRANSPOSE, &(math::float4x4::identity.m[0][0]));
    gl::Uniform1i(uniGSampler, 0);
    gl::Uniform1i(uniUseFixColor, 0);

    gl::DisableVertexAttribArray(0);
    gl::DisableVertexAttribArray(1);

    //draw cube
    {
        cube.draw();
    }

    //draw tria1
    {
        math::float3 color = color_white;
        gl::UniformFloat3(uniColor, color);
        gl::Uniform1i(uniUseFixColor, 1);

        gl::EnableVertexAttribArray(0);
        gl::BindBuffer(GL_ARRAY_BUFFER, tria1_vbo);

        //Model matrix
        math::float4x4 Mtrans = math::float4x4::identity;
        Mtrans.m[3][0] = tria1_pos[0];
        Mtrans.m[3][1] = tria1_pos[1];
        Mtrans.m[3][2] = tria1_pos[2];
        math::float4x4 Mrot = math::float4x4::identity;
        math::float4x4 Mscale = math::float4x4::identity;
        M = Mscale * Mrot * Mtrans;
        MVP = M * V * P;

        gl::UniformMatrix4fv(uniMVP, 1, gl::DO_TRANSPOSE, &(MVP.m[0][0]));
        gl::VertexAttribPointer(0, 3,
                                GL_FLOAT, gl::SKIP_NORMALIZE,
                                sizeof(gl::Vertex), VERTEX_OFFSET_POSITION);
        gl::DrawArrays(GL_TRIANGLES, 0, 1*3);
        gl::Uniform1i(uniUseFixColor, 0);
    }

    //draw tria2
    {
        math::float3 color = color_white;
        gl::UniformFloat3(uniColor, color);
        gl::Uniform1i(uniUseFixColor, 1);

        gl::EnableVertexAttribArray(0);
        gl::BindBuffer(GL_ARRAY_BUFFER, tria2_vbo);

        //Model matrix
        math::float4x4 Mtrans = math::float4x4::identity;
        Mtrans.m[3][0] = tria2_pos[0];
        Mtrans.m[3][1] = tria2_pos[1];
        Mtrans.m[3][2] = tria2_pos[2];
        math::float4x4 Mrot = math::float4x4::identity;
        math::float4x4 Mscale = math::float4x4::identity;
        M = Mscale * Mrot * Mtrans;
        MVP = M * V * P;

        gl::UniformMatrix4fv(uniMVP, 1, gl::DO_TRANSPOSE, &(MVP.m[0][0]));
        gl::VertexAttribPointer(0, 3,
                                GL_FLOAT, gl::SKIP_NORMALIZE,
                                sizeof(gl::Vertex), VERTEX_OFFSET_POSITION);
        gl::DrawArrays(GL_TRIANGLES, 0, 1*3);
        gl::Uniform1i(uniUseFixColor, 0);
    }

    //draw texture
    {
        //Model matrix
        math::float4x4 Mtrans = math::float4x4::identity;
        math::float4x4 Mrot = math::float4x4::identity;
        math::float4x4 Mscale = math::float4x4::identity;
        M = Mscale * Mrot * Mtrans;
        MVP = M * V * P;

        gl::UniformMatrix4fv(uniMVP, 1, gl::DO_TRANSPOSE, &(MVP.m[0][0]));
        gl::Uniform1i(uniUseFixColor, 0);
        gl::Uniform1i(uniGSampler, 0);

        gl::EnableVertexAttribArray(0);
        gl::EnableVertexAttribArray(1);

        gl::BindBuffer(GL_ARRAY_BUFFER, tex_vbo);
        gl::VertexAttribPointer(0, 3, GL_FLOAT, gl::SKIP_NORMALIZE, sizeof(gl::Vertex), VERTEX_OFFSET_POSITION );

        tex.bind(GL_TEXTURE0);
        gl::VertexAttribPointer(1, 2, GL_FLOAT, gl::SKIP_NORMALIZE, sizeof(gl::Vertex), VERTEX_OFFSET_TEXTURE );
        gl::DrawArrays(GL_TRIANGLES, 0, 2*3);
    }

    //draw pyramids
    {
        gl::UniformFloat3(uniColor, color_white);
        gl::Uniform1i(uniUseFixColor, 1);
        gl::Uniform1f(uniAlpha, 0.75f);
        gl::Uniform1i(uniUseAlphaOverride, 1);
        gl::EnableVertexAttribArray(0);
        gl::DisableVertexAttribArray(1);
        gl::BindBuffer(GL_ARRAY_BUFFER,         pyramids_vbo);
        gl::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramids_ibo);

        //Model matrix
        math::float4x4 Mtrans = math::float4x4::identity;
        math::float4x4 Mrot = math::float4x4::identity;
        math::float4x4 Mscale = math::float4x4::identity;
        M = Mscale * Mrot * Mtrans;
        MVP = M * V * P;
        gl::UniformMatrix4fv(uniMVP, 1, gl::DO_TRANSPOSE, &(MVP.m[0][0]));

        gl::VertexAttribPointer(0, 3,
                                GL_FLOAT, gl::SKIP_NORMALIZE,
                                sizeof(gl::Vertex), VERTEX_OFFSET_POSITION);

        //draw shape
        gl::DrawElements(GL_TRIANGLES, 6*3, GL_UNSIGNED_INT, 0 );

        //draw border line
        gl::UniformFloat3(uniColor, color_black);
        gl::Uniform1i(uniUseFixColor, 1);
        gl::Uniform1f(uniAlpha, 1.0f);
        gl::Uniform1i(uniUseAlphaOverride, 1);

            //TODO: figure out somehow, so that draw ordering doesn't matter
                //  currently if you draw anything after the transparent shapes
                //    depth test will filter them out, where they should be visible
                //    through transparent surfaces
            //apparently you need to leave transparent shapes to the end of drawing
            //  and sort them by distance to camera, then draw from back to front
            //  note: what's the solution for a case with circular occlusion order for 3 shapes?

        gl::Disable(GL_DEPTH_TEST);

        for(size_t j=0; j<6; ++j){
            gl::DrawElements(GL_LINE_LOOP, 3, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(j*3*sizeof(GLuint)));
        }

        gl::Enable(GL_DEPTH_TEST);
        gl::Uniform1i(uniUseFixColor, 0);
        gl::Uniform1i(uniUseAlphaOverride, 0);
    }

    gl::Uniform1i(uniUseFixColor, 0);
    gl::Uniform1i(uniUseAlphaOverride, 0);

    //draw campbell head mesh
    {
        /*
        math::float4x4 M = math::float4x4::identity;
        campbell_head_M.m[3][0] = -0.5f;
        campbell_head_M.m[3][1] = 0.0f;
        campbell_head_M.m[3][2] = 62.0f;
*/
        /*
        GLint uniM = gl::GetUniformLocation(ShaderProgram, shNameM);
        gl::UniformFloat4x4(uniM, gl::DO_TRANSPOSE, M);
        */

        math::float4x4 MVP = campbell_head_M * camera->getViewMtx() * camera->getProjMtx();
        GLint uniMVP = gl::GetUniformLocation(ShaderProgram, shNameMVP);
        gl::UniformFloat4x4(uniMVP, gl::DO_TRANSPOSE, MVP);

        for(size_t i=0; i<campbell_head_meshes.size(); ++i){
            campbell_head_textures[i]->bind(GL_TEXTURE0);
            pt::engine::drawMesh(campbell_head_meshes[i]);
        }
    }

    //draw campbell body mesh
    {
        math::float4x4 M = math::float4x4::identity;
        M.m[3][0] = 0.5f;
        M.m[3][1] = 0.0f;
        M.m[3][2] = 0.0f;

//        GLint uniM = gl::GetUniformLocation(ShaderProgram, shNameM);
//        gl::UniformFloat4x4(uniM, gl::DO_TRANSPOSE, M);

        math::float4x4 MVP = M * camera->getViewMtx() * camera->getProjMtx();
        GLint uniMVP = gl::GetUniformLocation(ShaderProgram, shNameMVP);
        gl::UniformFloat4x4(uniMVP, gl::DO_TRANSPOSE, MVP);

        for(size_t i=0; i<campbell_body_meshes.size(); ++i){
            campbell_body_textures[i]->bind(GL_TEXTURE0);
            pt::engine::drawMesh(campbell_body_meshes[i]);
        }
    }
    //draw else

}

void HandleInput(float t, float dt){
    float delta = dt / 1000.0f;
    assert(camera);

    if(bKeyForwardPressed){     camera->move( camera->getDir(engine::Camera::Dir::FORWARD)  * delta * camera_speed); }
    if(bKeyBackwardPressed){    camera->move( camera->getDir(engine::Camera::Dir::BACKWARD) * delta * camera_speed); }
    if(bKeyLeftPressed){        camera->move( camera->getDir(engine::Camera::Dir::LEFT)     * delta * camera_speed); }
    if(bKeyRightPressed){       camera->move( camera->getDir(engine::Camera::Dir::RIGHT)    * delta * camera_speed); }
    if(bKeyUpPressed){          camera->move( camera->getDir(engine::Camera::Dir::UP)       * delta * camera_speed); }
    if(bKeyDownPressed){        camera->move( camera->getDir(engine::Camera::Dir::DOWN)     * delta * camera_speed); }

    if(bKeyHeadLeftPressed){    campbell_head_M.m[3][0] += delta * fCampbellHeadSpeed; }
    if(bKeyHeadRightPressed){   campbell_head_M.m[3][0] -= delta * fCampbellHeadSpeed; }
    if(bKeyHeadUpPressed){      campbell_head_M.m[3][2] += delta * fCampbellHeadSpeed; }
    if(bKeyHeadDownPressed){    campbell_head_M.m[3][2] -= delta * fCampbellHeadSpeed; }

}

void UpdateScene(float t, float dt){
    //float fi = t/ 3000.0f * 2* M_PI;
    HandleInput(t, dt);
    //tria1_pos.x = tria1_pos_initial.x + 2 * 0.5f * cosf( fi );
    //tria1_pos.y = tria1_pos_initial.y + 2 * 0.5f * sinf( fi );

    //tria1_orient = fi;
}

void draw(SDL_Window* window){
    static uint32_t drawcount = 0;
    static uint32_t lastsecond = 0;

    uint32_t currentsecond = SDL_GetTicks();
    uint32_t dt = currentsecond - lastsecond;
    ++drawcount;
    assert(dt >= 0.0f);
    if(dt > 1000.0f){
        if(bDisplayFramerate){
            std::cout << "framerate: " << drawcount << "fps\t\r";
        }
        lastsecond = currentsecond;
        drawcount = 0;
    }
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //implement entity drawing here:
    DrawContents();

    SDL_GL_SwapWindow(window);
}

void readkey(){
//    system("pause");
}

/**
--------------------------------------
            main function
--------------------------------------
 */
int main(int /*argc*/, char** /*argv*/){
    SDL_Window*     window;
    SDL_GLContext   glcontext;
    if(0 != Initialize(&window, &glcontext)){
        std::cout << "Initialization failed, aborting launch\n";
        readkey();
        return 1;
    }

    CreateEntities();

    float t = 0.0f, dt = 0.0f, prevt = 0.0f;

    SDL_Event ev;
    bool done = false;
    while(!done){
        t   = static_cast<float>( SDL_GetTicks() );
        dt  = t-prevt;
        prevt = t;
        while(SDL_PollEvent(&ev)){
            if(ev.type == SDL_MOUSEBUTTONDOWN){
                mouseAimEnabled = true;
                SDL_SetRelativeMouseMode(SDL_TRUE);
            }
            if(ev.type == SDL_MOUSEBUTTONUP){
                mouseAimEnabled = false;
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            if(ev.type == SDL_MOUSEMOTION){
                if( mouseAimEnabled ){
                    //180 pixel = 30 degree = pi/6
                    camera->moveTarget(-ev.motion.xrel * mousespeed_x /180 * static_cast<float>(M_PI) / 6,
                                       -ev.motion.yrel * mousespeed_y /180 * static_cast<float>(M_PI) / 6);
                }
            }
            GLuint AL = gl::GetUniformLocation(ShaderProgram, shNameLightAmbient);
                        if(ev.type == SDL_KEYDOWN){
                math::float3 v;
                switch(ev.key.keysym.sym){
                    case keyQuitApplication:            done = true;                    break;
                    case keyForceRedraw:                SDL_GL_SwapWindow(window);      break;
                    case keyCameraForward:              bKeyForwardPressed = true;      break;
                    case keyCameraBackward:             bKeyBackwardPressed = true;     break;
                    case keyCameraLeft:                 bKeyLeftPressed = true;         break;
                    case keyCameraRight:                bKeyRightPressed = true;        break;
                    case keyCameraUp:                   bKeyUpPressed = true;           break;
                    case keyCameraDown:                 bKeyDownPressed = true;         break;
                    case keySprint:
                        if(!bKeySprintPressed){
                            camera_speed *= 5;
                        }
                        bKeySprintPressed = true;
                    break;

                    case keyHeadLeftPressed:            bKeyHeadLeftPressed = true;     break;
                    case keyHeadRightPressed:           bKeyHeadRightPressed = true;    break;
                    case keyHeadUpPressed:              bKeyHeadUpPressed = true;       break;
                    case keyHeadDownPressed:            bKeyHeadDownPressed = true;     break;
                    case 'p':                           std::cout << campbell_head_M.m[3][0] << ","
                                                                  << campbell_head_M.m[3][1] << ","
                                                                  << campbell_head_M.m[3][2] << ")\n";

                    case SDLK_KP_7:
                        ambientcolor.x += 0.02f;
                        if(ambientcolor.x > 1.0f){ ambientcolor.x = 1.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                        break;
                    case SDLK_KP_4:
                        ambientcolor.x -= 0.02f;
                        if(ambientcolor.x < 0.0f){ ambientcolor.x = 0.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                    break;
                    case SDLK_KP_8:
                        ambientcolor.y += 0.02f;
                        if(ambientcolor.y > 1.0f){ ambientcolor.y = 1.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                    break;
                    case SDLK_KP_5:
                        ambientcolor.y -= 0.02f;
                        if(ambientcolor.y < 0.0f){ ambientcolor.y = 0.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                    break;
                    case SDLK_KP_9:
                        ambientcolor.z += 0.02f;
                        if(ambientcolor.z > 1.0f){ ambientcolor.z = 1.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                    break;
                    case SDLK_KP_6:
                        ambientcolor.z -= 0.02f;
                        if(ambientcolor.z < 0.0f){ ambientcolor.z = 0.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                    break;

                    case SDLK_KP_PLUS:
                        ambientintensity += 0.02f;
                        if(ambientintensity > 1.0f){ ambientintensity = 1.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                    break;

                    case SDLK_KP_MINUS:
                        ambientintensity -= 0.02f;
                        if(ambientintensity < 0.0f){ ambientintensity = 0.0f; }
                        gl::UniformFloat3(AL, ambientcolor * ambientintensity);
                    break;
                    case keyToggleFramerateDisplay:
                        bDisplayFramerate = !bDisplayFramerate;
                        std::cout << "                \t\r";
                    break;
                }
            }
            if(ev.type == SDL_KEYUP){
                switch(ev.key.keysym.sym){
                    case keyCameraForward:          bKeyForwardPressed = false;         break;
                    case keyCameraBackward:         bKeyBackwardPressed = false;        break;
                    case keyCameraLeft:             bKeyLeftPressed = false;            break;
                    case keyCameraRight:            bKeyRightPressed = false;           break;
                    case keyCameraUp:               bKeyUpPressed = false;              break;
                    case keyCameraDown:             bKeyDownPressed = false;            break;
                    case keySprint:
                        if(bKeySprintPressed){
                            camera_speed /= 5;
                        }
                        bKeySprintPressed = false;
                    break;
                    case keyHeadLeftPressed:        bKeyHeadLeftPressed = false;    break;
                    case keyHeadRightPressed:       bKeyHeadRightPressed = false;   break;
                    case keyHeadUpPressed:          bKeyHeadUpPressed = false;      break;
                    case keyHeadDownPressed:        bKeyHeadDownPressed = false;    break;
                }
            }

            if(ev.quit.type == SDL_QUIT){
                done = true;
                break;
            }
        }
        UpdateScene(t, dt);
        draw(window);
        //SDL_Delay(16); // ~ 60fps
        //SDL_Delay(33); // ~ 30fps
    }

    DestroyEntities();
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);

    readkey();

    return 0;
}


