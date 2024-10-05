#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;


uniform bool horizontal;

uniform float size = 1.0f;


uniform float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);//(0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);



void main()
{             
     
     vec2 tex_offset_img = 1.0 / textureSize(image,0); 

     float result = texture(image, TexCoords).a * weight[0] ;
    
     if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(image, TexCoords + vec2(tex_offset_img.x * i, 0.0)).a* weight[i];
            result += texture(image, TexCoords - vec2(tex_offset_img.x * i, 0.0)).a * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(image, TexCoords + vec2(0.0,tex_offset_img.x * i)).a * weight[i];
            result += texture(image, TexCoords - vec2(0.0,tex_offset_img.x * i)).a * weight[i];
         }
     }

     FragColor = vec4(1.0f,1.0f,1.0f,result);
}


    
    

    /*
    
    if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += (texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb+texture(downscale, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb) * weight[i]*strength;//
            result += (texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb+texture(downscale, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb) * weight[i]*strength;//
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
            result +=(texture(image, TexCoords + vec2(0.0,tex_offset.x * i)).rgb+texture(downscale, TexCoords + vec2(0.0,tex_offset.x * i)).rgb)* weight[i]*strength;// 
            result += (texture(image, TexCoords - vec2(0.0,tex_offset.x * i)).rgb+texture(downscale, TexCoords - vec2(0.0,tex_offset.x * i)).rgb)* weight[i]*strength;//
         }
     }
    


    if(horizontal)
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(downscale, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i]*strength;
            result += texture(downscale, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i]*strength;
         }
     }
     else
     {
         for(int i = 1; i < 5; ++i)
         {
            result += texture(downscale, TexCoords + vec2(0.0,tex_offset.x * i)).rgb* weight[i]*strength;
            result += texture(downscale, TexCoords - vec2(0.0,tex_offset.x * i)).rgb* weight[i]*strength;
         }
     }
    */