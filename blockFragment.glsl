#version 330 core

out vec4 FragColor;

in vec2 texCoord;
flat in int blockTypeIndex;

uniform sampler2D ourTexture;

const uniform vec2 blockTypeTexCoordModifiers[8] = 
{
	vec2(0.0, 0.0), // garbage/air
	vec2(0.00, 48.0), // grass 
	vec2(16.0, 48.0), // grass top
	vec2(32.0, 48.0), // dirt
	vec2(48.0, 48.0), // stone
	vec2(0.00, 32.0), // wood
	vec2(16.0, 32.0), // wood top
	vec2(32.0, 32.0) // leaves
};

void main()
{
	const float textureMapDims = 64.0; // 64 pixel width and height
	const float blockTextureDims = 16.0; // each block is a 16x16 texture
	const float normalizedCoordMultiplier = 1.0 / textureMapDims; // texture coords are normalized, multiply by this value for coords

	vec2 tc = texCoord;

	tc /= textureMapDims / blockTextureDims; // texture map is 64x64 and each block is a 16x16 texture

	tc += blockTypeTexCoordModifiers[blockTypeIndex] * normalizedCoordMultiplier;

	FragColor = texture(ourTexture, tc);
}