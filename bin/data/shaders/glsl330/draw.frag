#version 330

uniform sampler2DRect texFrame;

in vec2 texCoordVarying;
out vec4 fragColor;

void main()
{
    vec4 texel = texture(texFrame, texCoordVarying);
    fragColor = texel;
}