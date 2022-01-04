#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 texCoord;

uniform vec2 cardPosition;
uniform float cardScale;
uniform float cardZ;

void main() {
	texCoord = inTexCoord;
	gl_Position = vec4((inPosition.xy * cardScale) + cardPosition, cardZ, 1.0f);
}