#version 460 core

layout(location = 0) out vec4 color;

in vec3 clr;

uniform float alpha;

void main() {
    color = vec4(clr, alpha);
    return;
}