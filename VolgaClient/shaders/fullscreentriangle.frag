#version 410
uniform sampler2D gTexture;
in vec2 texCoord;
out vec4 fragColor;

void main(void)
{
    fragColor = texture(gTexture, texCoord);
    fragColor.a = 1.0;
}
