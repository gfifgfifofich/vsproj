#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Texture1;
uniform sampler2D Texture2;

uniform vec2 proportions = vec2(1.0f,1.0f);

void main()
{
	vec4 t1 = texture(Texture1,TexCoords).rgba;
	vec4 t2 = texture(Texture2,TexCoords).rgba;

	if(t2.r * t2.a<=1.0f &&
	   t2.g * t2.a<=1.0f && 
	   t2.b * t2.a<=1.0f)
		FragColor = vec4(t1.rgb*proportions.x+t2.rgb*proportions.y,(min(t2.a + t1.a,1.0f)));
	else
		FragColor = vec4(t1.rgb*proportions.x+(t2.rgb - (1.0f - proportions.y)),(min(t2.a + t1.a,1.0f)));

}