#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Texture;

uniform int mode = 0;// mode: 0 - all, 1 - (r,g,b,1.0f) , 2 - (0,0,0,a), 3 - (a,a,a,a),  4 - (a,a,a,1.0f)  

void main()
{
	vec4 t1 = texture(Texture,TexCoords).rgba;
    if(mode == 0)//all
	    FragColor = t1;
    else if (mode == 1)//(r,g,b,1.0f)
    {
	    FragColor = vec4(t1.rgb,1.0f);
    }
    else if (mode == 2)//(0,0,0,a)
    {
	    FragColor =  vec4(0.0f,0.0f,0.0f,t1.a);
    }
    else if (mode == 3)//(a,a,a,a)  
    {
	    FragColor =  vec4(t1.a);
    }
    else if (mode == 4)//(a,a,a,1.0f)  
    {
	    FragColor =  vec4(t1.r,t1.g,t1.b,1.0f);
    }
    else if (mode == 5)//(-a * 0.1f,a * 0.1f,1.0f,1.0f) // better heightmap look   
    {
        float r = t1.a;
        if(r<0.0f)
            r = 0.0f;
        float b = abs(t1.a);

        float g = 0.5f -  max(r,b);
        if(t1.a >=0.00001f || t1.a <=-0.00001f )
	        FragColor =  vec4(r*0.2f,g,b*0.2f,1.0f);
        else
            FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
    }
}