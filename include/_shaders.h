#pragma once


const char* VertexShader = R"(
    #version 330
    precision highp float;

    uniform float       Scale;
    uniform mat4        M;
    uniform mat4        VP;
    uniform mat4        MVP;
    uniform int         UseFixColor;
    uniform vec3        Color;

    uniform vec3        LightAmbient;

    layout(location = 0) in vec3 in_vPos;
    layout(location = 1) in vec2 in_tPos;
    layout(location = 2) in vec3 in_Normal;

    out     vec3    vPos;
    out     vec2    tPos;
    out     vec3    Normal;

    void main(){
        gl_Position = vec4(in_vPos, 1.0f) * MVP;
        if(0 == UseFixColor){
            tPos    = in_tPos;
        }else{
            tPos    = in_tPos; //don't care
        }
        //Normal = (vec4(in_Normal, 0.0f) * M).xyz;
        Normal = in_Normal;
	}
)";


const char* GeometryShader = R"()";
const char* FragmentShader = R"(
    #version 330
    precision highp float;

    struct Spotlight{
        bool    enabled;
        vec3    color;
        float   intensity_diffuse;
        vec3    direction;
    };

    uniform sampler2D       gSampler;
    uniform int             UseFixColor;
    uniform vec3            Color;
    uniform int             UseAlphaOverride;
    uniform float           Alpha;

    uniform vec3            LightAmbient;
    uniform Spotlight       Spotlights[64];


    in      vec3    vPos;
    in      vec2    tPos;
    out     vec3    Normal;
    out     vec4    FragColor;

    float GetAlpha(){
        if(0 == UseAlphaOverride){  return 1.0f;
        }else{                      return Alpha;
        }
    }

    void main(){
        float a = GetAlpha();
        vec4 retval = vec4(1.0f,1.0f,1.0f,1.0f);

        for(int i=0; i<64;++i){
            if(Spotlights[i].enabled){
                //calculate spotlight effects
            }
        }

        if(0 == UseFixColor){   retval = texture(gSampler, tPos);
        }else{                  retval = vec4(Color, a);
        }

        FragColor = vec4( retval.xyz * LightAmbient, retval.w);
    }
)";
