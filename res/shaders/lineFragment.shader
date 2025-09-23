#version 330 core

out vec4 color;

in vec4 v_Color;
in float v_Alpha;

void main() {
    color = vec4(v_Color.rgb, v_Color.a * v_Alpha);
}