#version 330
precision highp float;

// ----- shader/FrameInfo.ubo -----
// FrameInfo v0.1
layout(std140) uniform FrameInfo{
    // Row and column-major notations and clarifications in the GL specs are awful, they only serve to confuse.
    // C memory layout is row-oriented, but the value layout in it is column-major!
    // Don't get deceived by the 'layout(row_major)' definitions for matrices.
    // Both in C structs and the shader code, all matrices are column-major !!! (P * V * M) !!!
                      float t;          // current time
                      float dt;         // delta time since last frame
    layout(row_major) mat4  V;          // View matrix
    layout(row_major) mat4  Vrot;       // View matrix rotation component (without translation)
    layout(row_major) mat4  P;          // Projection matrix
    layout(row_major) mat4  PV;         // P * V
    layout(row_major) mat4  PVrotInv;   // inv(Vrot) * inv(P)
} frameInfo;
// --------------------------------

// ----- shader/LightingInfo.ubo -----
// LightingInfo v0.1

struct ConeLight{
    mat4 mTransform;
    vec3 mColor;

    float mIntensity;
    float mAngle;
    float mRadius;
    int   mEnabled;
    // padding (0 bytes)
};

uniform ConeLight   coneLights[128];

struct PointLight{
    mat4 mTransform;
    vec3    mPos;
    // padding (4 bytes)
    vec3    mColor;
    // padding (4 bytes)
    float   mIntensity;
    float   mRadius;
    int     mEnabled;
    // padding (4 bytes)
};

uniform PointLight   pointLights[128];


//layout(std140) uniform LightingInfo{
//        ConeLight   coneLights[128];

//    } lightingInfo;

// -----------------------------------
const int coneLightsMaximum = 128;


uniform mat4 M;         // Model matrix
uniform mat4 PVM;       // P * V * M

uniform Lighting{
    vec3 ambient;

} lighting;

uniform int         WireframeMode;
uniform vec3        WireframeColor;
uniform int         NormalVectorDisplayMode;
uniform int         MissingTexture;
uniform int         AxisDrawMode;
uniform int         SkyboxMode;
uniform int         ColorMode;
uniform vec3        Color;
uniform int         FixUVScaleMode;
uniform float       FixUVScale;

uniform vec3        LightAmbient;

uniform sampler2D   gSampler;

float pi    = 3.14159265358979323846;
float t     = frameInfo.t;
float dt    = frameInfo.dt;

float nearZ = 1.0f;
float farZ  = 100000.0f;

in      vec3    fragmentPosModel;
in      vec3    fragmentPosWorld;
in      vec3    vPos;
in      vec2    tPos;
in      vec3    vNormal;
out     vec4    FragColor;

vec4 SampleMissingTexture( vec2 texPos ){
    int x = int(texPos.x*4) % 4;
    int y = int(texPos.y*4) % 4;
    int isBrightField = int(0 == (x + y) % 2);
    vec3 fieldColor = vec3( 1.0f*isBrightField, 0.0f, 1.0f*isBrightField ); // purple-black squares
    return vec4( fieldColor , 1.0f );
}

void main(){
    vec4 texel;

    vec2 texPos = tPos;
    if( 0 != FixUVScaleMode ){
        // @TODO: finish
        //  make UV dependant on vPos
    }

    if( 0 != MissingTexture ){                  // texture is missing, draw fallback texture
        texel = SampleMissingTexture( texPos );
    }else{                                       // draw texture normally
        texel = texture( gSampler, texPos );
    }

    //FragColor = vec4( 1.0f, 1.0f, 1.0f, 1.0f ); //white
    //FragColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f ); //red
    //FragColor = vec4( 0.0f, 1.0f, 0.0f, 1.0f ); //green
    //FragColor = vec4( 0.0f, 0.0f, 1.0f, 1.0f ); //blue
    //FragColor = vec4( 1.0f, 0.0f, 1.0f, 1.0f ); //purple
    //FragColor = vec4( 1.0f, 1.0f, 0.0f, 1.0f ); //yellow
    //FragColor = vec4( 0.0f, 1.0f, 1.0f, 1.0f ); //cyan

    //FragColor = vec4( texel.xyz * LightAmbient, texel.w);
    FragColor = vec4( 1.0f, 2/3, 0.0f, 1.0f ); // orange

    if( 0 != SkyboxMode ){
        //vec4 pixelVectorScreen = vec4( vPos.x, vPos.y, -1.0f, 1.0f );
        vec4 pixelVectorScreen = vec4( vPos.x, vPos.y, -vPos.z, 1.0f ); // -Z needed here, as the Projection matrix inverts Z with the -1 component in [3][2]
        vec4 pixelVectorWorld  = frameInfo.PVrotInv * pixelVectorScreen;
        pixelVectorWorld = normalize(pixelVectorWorld);

        float MyPitch = asin( pixelVectorWorld.z );                     //  [-pi/2; pi/2] // |input| must be < 1
        float MyYaw = atan( pixelVectorWorld.y, pixelVectorWorld.x );   //  [-pi; pi]

        float MyU = 0.5 - ( MyYaw /pi ) /2;
        float MyV = 0.5 + ( MyPitch * 2/pi) /2;
        vec2 MyUV = vec2( MyU, MyV );
        FragColor = texture( gSampler, MyUV );

    }else if( 0 == WireframeMode && 0 != AxisDrawMode){  // skip drawing axes without wireframe mode
        discard;

    }else if( 0 != WireframeMode && 0 == AxisDrawMode ){ // when drawing a non-axis in wireframe mode
        FragColor = vec4( WireframeColor.xyz, 1.0f );

    }else if( 0 != WireframeMode && 0 != NormalVectorDisplayMode ){ // when drawing normal vectors
        FragColor = vec4( 1.0f, 2/3, 0.0f, 1.0f ); // orange

    }else if( 0 != WireframeMode && 0 != AxisDrawMode ){ // when drawing an axis in wireframe mode
        FragColor = vec4( 1.0f * int( 0.0000001f < fragmentPosModel.x ),
                          1.0f * int( 0.0000001f < fragmentPosModel.y ),
                          1.0f * int( 0.0000001f < fragmentPosModel.z ),
                          1.0f );

    }else if( 0 != ColorMode ){             // drawing a fixed-color surface
        if( 1 == ColorMode ){
            FragColor = vec4( Color.xyz, 1.0f ); // draw a fix color
        }else{
            vec4 pulsingColor = vec4( 1+ sin( t  )/2,
                                      1+ sin( t*6 +pi/3 )/2,
                                      1+ sin( t*9 +pi/7 )/2,
                                      1.0f );
            FragColor = pulsingColor;
        }

    }else{                                  // drawing a textured surface normally
        vec3 totalLightColor = LightAmbient;
        totalLightColor = vec3( 0.2f, 0.2f, 0.2f );
        //totalLightColor = vec3( 1,1,1 );
        for( int i=0; i<coneLightsMaximum; ++i ){
            vec3 spotColor = vec3( 0,0,0 );
            if( 0 < coneLights[i].mEnabled ){
                // @TODO: calc spotlight direction, range, angle

                mat4 spotTf = coneLights[i].mTransform;
                vec3 spotPos = vec3( spotTf[3][0],
                                     spotTf[3][1],
                                     spotTf[3][2] );

                vec4 spotDir4           = spotTf * vec4(1,0,0,1) - vec4(spotPos, 1); // @TODO: optimize...
                vec3 spotDir            = normalize( spotDir4.xyz );
                vec3 fragDir            = normalize( fragmentPosWorld - spotPos );

                float spotAngle            = coneLights[i].mAngle;
                float spotDirFragAngleCos  = dot( spotDir, fragDir);
                float spotDirFragAngle     = acos( spotDirFragAngleCos );

                vec3  normal = vNormal;

                float surfLightAngleCos = dot( normal, -fragDir );
                float surfLightAngle    = acos( surfLightAngleCos );

                float intensity = coneLights[i].mIntensity;
                float radius = coneLights[i].mRadius;

                bool fragIsInLightCone = ( 2*spotDirFragAngle < spotAngle );

                float fragDistance      = length( fragmentPosWorld - spotPos );
                bool  fragIsInRange     = fragDistance <= radius;
                float fragRangeFactor   = 1 - (fragDistance / radius);

                float fragLightFactor = fragRangeFactor
                                        * int( fragIsInRange )
                                        * int( fragIsInLightCone )
                                        * int( 0 <= surfLightAngleCos )
                                        * surfLightAngleCos
                                        * intensity;

                // if frag is lit, add lightcolor, otherwise add zero
                spotColor = coneLights[i].mColor * fragLightFactor;
                //spotColor = normal;
                totalLightColor += spotColor;
            }
        }


for( int i=0; i<coneLightsMaximum; ++i ){ //@TODO: rename
vec3 spotColor = vec3( 0,0,0 );
if( 0 < pointLights[i].mEnabled ){
    vec3 spotPos = pointLights[i].mPos; // @TODO: rename

    vec3 fragDir = normalize( fragmentPosWorld - spotPos );
    vec3  normal = vNormal;
    float intensity = pointLights[i].mIntensity;

    float surfLightAngleCos = dot( normal, -fragDir );
    float surfLightAngle    = acos( surfLightAngleCos );

    float radius = pointLights[i].mRadius;

    float fragDistance      = length( fragmentPosWorld - spotPos );
    bool  fragIsInRange     = fragDistance <= radius;
    float fragRangeFactor   = 1 - (fragDistance / radius);

    float fragLightFactor = fragRangeFactor
                            * int( fragIsInRange )
                            * int( 0 <= surfLightAngleCos )
                            * surfLightAngleCos
                            * intensity;

    // if frag is lit, add lightcolor, otherwise add zero
    spotColor = pointLights[i].mColor * fragLightFactor;
    //spotColor = normal;
    totalLightColor += spotColor;
}
}





        // visualize depth instead
        bool drawDepth = false;
        if( drawDepth ){
            float depth = gl_FragCoord.z /gl_FragCoord.w /(farZ-nearZ);
            FragColor = vec4( vec3( depth), 1.0f );
        }else{
            // output color
            FragColor = vec4( texel.x * totalLightColor.x,
                              texel.y * totalLightColor.y,
                              texel.z * totalLightColor.z,
                              texel.w );
        }
    }
}
