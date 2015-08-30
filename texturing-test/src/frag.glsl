#version 400 core

in vec2 TexCoord;
out vec4 outColor;

uniform sampler2D textureData;

void main()
{
    outColor = mix(vec4(1.0,1.0,1.0,1.0), texture(textureData, TexCoord), 0.5);
}
