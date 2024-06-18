#version 330
precision highp float;

uniform int         WireframeMode;
uniform vec3        WireframeColor;
uniform int         MissingTexture;
uniform int         DrawingAxes;
uniform int         ColorMode;
uniform vec3        Color;
uniform float       t;
uniform float       dt;
uniform mat4        M;
uniform mat4        V;
uniform mat4        Vrot;
uniform mat4        PV;
uniform mat4        PVM;


uniform sampler2D   gSampler;

uniform int     AmbientLight_UseAlphaOverride;
uniform float   AmbientLight_Alpha;

uniform vec3    LightAmbient;

in      vec3    vPos_orig;
in      vec3    vPos;
in      vec2    tPos;
in      vec3    vNormal;
out     vec4    FragColor;

vec4 SampleMissingTexture( vec2 texPos ){
    int xquadrant = int(texPos.x*4) % 4;
    int yquadrant = int(texPos.y*4) % 4;
    int colored = int(0 == (xquadrant + yquadrant) % 2);
    return vec4( 1.0f*colored, 0.0f, 1.0f*colored, 1.0f );
}

void main(){
    vec4 texel = texture( gSampler, tPos );
    //FragColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f ); //white
    //FragColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f ); //red
    //FragColor = vec4( 0.0f, 1.0f, 0.0f, 1.0f ); //green
    //FragColor = vec4( 0.0f, 0.0f, 1.0f, 1.0f ); //blue
    //FragColor = vec4( 1.0f, 0.0f, 1.0f, 1.0f ); //purple
    //FragColor = vec4( 1.0f, 1.0f, 0.0f, 1.0f ); //yellow
    //FragColor = vec4( 0.0f, 1.0f, 1.0f, 1.0f ); //cyan

    //FragColor = vec4( texel.xyz * LightAmbient, texel.w);

    float pi = 3.1415f;

    if( 0 == WireframeMode && 0 != DrawingAxes){        // skip drawing axes without wireframe mode
        discard;
    }else if( 0 != WireframeMode && 0 == DrawingAxes ){ // when drawing wireframe without axes
        FragColor = vec4( WireframeColor.xyz, 1.0f );
    }else if( 0 != WireframeMode && 0 != DrawingAxes ){ // when drawing wireframe with axes
        FragColor = vec4( 1.0f * int( 0.01f < vPos_orig.x ),
                          1.0f * int( 0.01f < vPos_orig.y ),
                          1.0f * int( 0.01f < vPos_orig.z ),
                          1.0f );
    }else if( 0 != ColorMode ){
        if( 1 == ColorMode ){
            FragColor = vec4( Color.xyz, 1.0f ); // draw a fix color
        }else{
            vec4 pulsingColor = vec4( 1+ sin( t  )/2,
                                      1+ sin( t*6 +pi/3 )/2,
                                      1+ sin( t*9 +pi/7 )/2,
                                      1.0f );
            FragColor = pulsingColor;
        }
    }else{
        if( 0 != MissingTexture ){
            FragColor = SampleMissingTexture( tPos );
        }else{
            FragColor = vec4( texel.xyz, texel.w );
        }
    }
}
