#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform sampler2D Texture;

void main()
{
	if(TexCoords.z!=0.0f)
	{
		vec4 col = texture(Texture, TexCoords.xy);
		col.xy = vec2(cos(TexCoords.z)*col.x - sin(TexCoords.z)*col.y, sin(TexCoords.z)*col.x + cos(TexCoords.z)*col.y);
		FragColor = col;
	}
	else
		FragColor = texture(Texture, TexCoords.xy);

}