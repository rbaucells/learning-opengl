#version 330 core

in vec4 v_Color;
out vec4 color;
in vec2 v_TextureCoords;

uniform sampler2D textureSampler;

void main() {
//    color = texture(textureSampler, v_TextureCoords);
    color = mix(texture(textureSampler, v_TextureCoords), v_Color, 0.5);
}