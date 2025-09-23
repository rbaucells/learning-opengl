#version 330 core

layout (location = 0) in vec2 position;

uniform float alpha;
uniform mat4 mvp;
uniform vec4 color;

out vec4 v_Color;
out float v_Alpha;

void main() {
    gl_Position = mvp * vec4(position, 0.0, 1.0);

    v_Alpha = alpha;
    v_Color = color;
}