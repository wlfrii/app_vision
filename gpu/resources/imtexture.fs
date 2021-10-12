#version 330 core
out vec4 fragColor;

in vec2 texCoord;

// texture sampler
uniform sampler2D Texture;

void main()
{
    fragColor = texture(Texture, texCoord);
}