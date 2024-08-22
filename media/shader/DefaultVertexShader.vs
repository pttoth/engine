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
    vec4 mColor;

    float mIntensity;
    float mAngle;
    float mRadius;
    int   mEnabled;
    // padding (0 bytes)
};

layout(std140) uniform LightingInfo{
    ConeLight   coneLights[128];

} lightingInfo;

// -----------------------------------
const int coneLightsMaximum = 128;


uniform int         AxisDrawMode;
uniform int         SkyboxMode;
uniform mat4        M;
uniform mat4        Mrot;
uniform mat4        PVM;

layout(location = 0) in vec3 in_vPos;
layout(location = 1) in vec2 in_tPos;
layout(location = 2) in vec3 in_vNormal;

out     vec3    fragmentPosModel;
out     vec3    fragmentPosWorld;
out     vec3    vPos;
out     vec2    tPos;
out     vec3    vNormal;

void main(){
    fragmentPosModel = in_vPos;
    fragmentPosWorld = (M * vec4( in_vPos , 1 )).xyz;

    if( 0 != SkyboxMode ){
        // Skybox drawing
        gl_Position = vec4(in_vPos.x, in_vPos.y, 0.9999999f, 1.0f); // z: skybox is as far away as possible (at the far end of the [0,1) depth spectrum)
                                                                    //      more precision to 1.0f kills the skybox
    }else{
        // normal drawing
        //gl_Position = PVM * vec4(in_vPos, 1.0f);
        gl_Position = frameInfo.PV * M * vec4(in_vPos, 1.0f);

        gl_Position.z *= -1; // TODO: fix depth value
                             //   this is a dirty hotfix for a depth issue, as the View and Projection seem fine,
                             //   but the depth value is the inverse of what is needed.
    }

    vPos = gl_Position.xyz; // TODO: needed?

    tPos    = in_tPos;
    vNormal = (Mrot * vec4( in_vNormal, 1)).xyz;
}
