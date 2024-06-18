#version 330
precision highp float;

uniform int         DrawingAxes;
uniform float       t;
uniform float       dt;
uniform mat4        M;
uniform mat4        V;
uniform mat4        Vrot;
uniform mat4        PV;
uniform mat4        PVM;

layout(location = 0) in vec3 in_vPos;
layout(location = 1) in vec2 in_tPos;
layout(location = 2) in vec3 in_vNormal;

out     vec3    vPos_orig;
out     vec3    vPos;
out     vec2    tPos;
out     vec3    vNormal;

void main(){
    vPos_orig = in_vPos;

    gl_Position = PVM * vec4(in_vPos, 1.0f);
    tPos    = in_tPos;
    vNormal = in_vNormal;
}
