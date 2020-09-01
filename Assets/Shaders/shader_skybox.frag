#version 440 core
in vec3 textureCoords;

uniform samplerCube skybox;

out vec4 color;

void main()
{
	color = texture(skybox, textureCoords);
}