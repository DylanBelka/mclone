#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in int aBlockTypeIndex;

out vec2 texCoord;
flat out int blockTypeIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	texCoord = aTexCoord;
	blockTypeIndex = aBlockTypeIndex;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}