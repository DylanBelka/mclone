#version 330 core
layout(location = 0) in vec4 aPos_UV; // both position and uv

out vec2 texCoord;

void main()
{
	texCoord = aPos_UV.zw;
	gl_Position = vec4(aPos_UV.xy, 1.0, 1.0);
}