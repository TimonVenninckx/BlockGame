#version 330 core

in vec3 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2DArray texture1;

void main()
{
	FragColor = texture(texture1, TexCoord);
    FragColor -= vec4(vec3(abs(Normal.z * 0.3)),0.0);  // for shading if block is facing north or south

}