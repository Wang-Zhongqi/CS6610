#version 460 core

layout(location = 0) out vec4 color;

in vec3 vPosition;
in vec3 vNormal;

const vec3 vColor = vec3(1.0, 0.01, 0.01);
const vec3 eyeDir = vec3(0.0, 0.0, 1.0);
const vec3 light = vec3(1.0, 1.0, 1.0);
const float ka = 0.1;
const float kd = 1;
const float ks = 100;

uniform float alpha;
uniform vec3 lightPosition;

void main() {
    // color = vec4(normalize(vNormal), alpha);

    vec3 ambient = light * ka * vColor;

    vec3 lightDir = normalize(lightPosition - vPosition);
    vec3 diffuse = light * kd * max(0.0, dot(vNormal, lightDir)) * vColor;
    vec3 halfDir = normalize(lightDir + eyeDir);
    vec3 specular = light * ks * max(0.0, pow(dot(vNormal, halfDir), 128)) * vColor;
    color = vec4(ambient + diffuse + specular, alpha);
}