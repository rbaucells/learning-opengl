#version 330 core
out vec4 color;

in vec2 v_TextureCoords;
flat in int v_Channels;
in float v_Alpha;

uniform sampler2D textureSampler;

void main() {
    vec4 sampledColor = texture(textureSampler, v_TextureCoords);

    switch (v_Channels) {
        case 1:
            sampledColor = vec4(sampledColor.r, sampledColor.r, sampledColor.r, 1.0);
            break;
        case 2:
            sampledColor = vec4(sampledColor.r, sampledColor.r, sampledColor.r, sampledColor.g);
            break;
        case 3:
            sampledColor = vec4(sampledColor.rgb, 1.0);
            break;
        case 4:
            sampledColor = sampledColor;
            break;
        default:
            sampledColor = vec4(1.0, 0.0, 1.0, 1.0); // Magenta
            break;
    }

    color = vec4(sampledColor.rgb, sampledColor.a * v_Alpha);
}