#version 440 core
layout (location = 0) in vec3 Pos;

out vec3 textureCoords;

uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
	vec4 position = uProjection * uView * vec4(Pos, 1.0f);
	textureCoords = Pos;
	gl_Position = position.xyww;

}