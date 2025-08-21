#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout (location = 2) in vec2 textureCoords;

uniform mat4 mvp;

out vec4 v_Color;
out vec2 v_TextureCoords;

void main() {
    gl_Position = mvp * vec4(position, 0.0, 1.0);
    v_Color = color;
    v_TextureCoords = textureCoords;
}