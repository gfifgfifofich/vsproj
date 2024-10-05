#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Texture;
uniform sampler2D BaseColor;
uniform sampler2D NormalMap;
uniform vec4 color = vec4(1.0f); 
uniform vec3 position; 
uniform vec2 CameraScale; 
uniform float volume; 

uniform float aspect; 
uniform vec2 scr;
void main()
{             
	vec2 scrSpace;
	vec3 Pos = position;
	Pos.x += 0.5f;
	Pos.y += 0.5f;
	Pos.x /= aspect;
	scrSpace.x =gl_FragCoord.x / scr.x;
	scrSpace.y =gl_FragCoord.y / scr.y;
	vec3 SurfaceNormal = texture(NormalMap,scrSpace).rgb;
	vec4 LightColor = texture(Texture, TexCoords).rgba * color;    
	vec4 BaseCol = texture(BaseColor, scrSpace).rgba;

	scrSpace.x /=aspect;

	scrSpace /= CameraScale;
	Pos.xy /= CameraScale;
	vec3 rel = normalize(vec3(Pos.xy-scrSpace,Pos.z));



	vec4 Col;
	Col = clamp(dot(rel.xyz,SurfaceNormal.xyz),0.0f,1.0f)*LightColor *BaseCol;

	if(SurfaceNormal.x==0 && SurfaceNormal.y==0) Col=LightColor *BaseCol;

	Col = Col.rgba + volume*LightColor.rgba;
	FragColor = vec4(Col.rgb,min(Col.a,1.0f)) ;


}