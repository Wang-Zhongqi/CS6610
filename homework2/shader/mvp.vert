#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec4 vColor;

out vec3 clr;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * vec4(pos, 1.0);
    clr = vColor.rgb;
    return;
}