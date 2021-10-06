#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int tex;

void main() {
	if (tex == 0)
		color = texture(texture1, TexCoord);
	if (tex == 1)
		color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	if (tex == 2)
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}