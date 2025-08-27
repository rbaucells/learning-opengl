#version 330 core

layout(location = 0) in vec2 position;
layout (location = 1) in vec2 textureCoords;

uniform mat4 mvp;
uniform int channels;
uniform float alpha;

out vec2 v_TextureCoords;
flat out int v_Channels;
out float v_Alpha;

void main() {
    gl_Position = mvp * vec4(position, 0.0, 1.0);
    v_TextureCoords = textureCoords;
    v_Channels = channels;
    v_Alpha = alpha;
}