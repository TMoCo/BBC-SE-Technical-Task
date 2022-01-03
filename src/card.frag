#version 450 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D cardTexture; // can be card front or back

void main() {
	FragColor = texture(cardTexture, texCoord);
}