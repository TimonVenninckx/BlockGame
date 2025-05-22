#version 330 core

in vec3 TexCoord;

out vec4 FragColor;

uniform sampler2DArray texture1;

void main()
{
	FragColor = vec4(0.0,0.0,0.3,1.0);
	FragColor = texture(texture1, TexCoord);
}