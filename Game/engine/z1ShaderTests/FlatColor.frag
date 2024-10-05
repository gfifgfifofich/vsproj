#version 330 core
out vec4 FragColor;

in vec2 UV;

uniform float en_Time = 1.0f;
uniform vec4 color = {1.0f,1.0f,1.0f,1.0f};

uniform vec4 en_objColor ={0.0f,1.0f,1.0f,1.0f};

uniform sampler2D en_objTexture;

vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos( 6.28318*(c*t+d) );
}

void main()
{     
    vec2 uv = UV - vec2(0.5f);
    uv *= 2.0f;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    
    for (float i = 0.0; i < 4.0; i++) {
        uv = fract(uv * 1.5) - 0.5;

        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + i*.4 + en_Time*.4);

        d = sin(d*8. + en_Time)/8.;
        d = abs(d);

        d = pow(0.01 / d, 1.2);

        finalColor += col * d;
    }
        
    FragColor = ( vec4(clamp(finalColor*finalColor,0.0f,10.0f), 1.0) * color * en_objColor ); 
     






     
}
/*
  
*/
/*
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    float aspect_ratio = 1.0f;
	vec2 uv = UV.xy;
    uv -= vec2(0.5, 0.5 * aspect_ratio);
    float rot = radians(-30. -iTime); // radians(45.0*sin(iTime));
    mat2 rotation_matrix = mat2(cos(rot), -sin(rot), sin(rot), cos(rot));
   	uv = rotation_matrix * uv;
    vec2 scaled_uv = 20.0 * uv; 
    vec2 tile = fract(scaled_uv);
    float tile_dist = min(min(tile.x, 1.0-tile.x), min(tile.y, 1.0-tile.y));
    float square_dist = length(floor(scaled_uv));
    
    float edge = sin(iTime-square_dist*20.);
    edge = mod(edge * edge, edge / edge);

    float value = mix(tile_dist, 1.0-tile_dist, step(1.0, edge));
    edge = pow(abs(1.0-edge), 2.2) * 0.5;
    
    value = smoothstep( edge-0.05, edge, 0.95*value);
    
    
    value += square_dist*.1;
    value *= 0.8 - 0.2;
    FragColor = vec4(pow(value, 2.), pow(value, 1.5), pow(value, 1.2), 1.); 
}



*/

 /*
	vec2 uv = UV.xy ;
	vec2 pos = (uv.xy-0.5);
	vec2 cir = ((pos.xy*pos.xy+sin(uv.x*18.0+iTime)/25.0*sin(uv.y*7.0+iTime*1.5)/1.0)+uv.x*sin(iTime)/16.0+uv.y*sin(iTime*1.2)/16.0);
	float circles = (sqrt(abs(cir.x+cir.y*0.5)*25.0)*5.0);
	FragColor = vec4(sin(circles*1.25+2.0),abs(sin(circles*1.0-1.0)-sin(circles)),abs(sin(circles)*1.0),1.0);
    */

	//float modx = sin(gl_FragCoord.x*gl_FragCoord.x*0.3f*0.3f)+2.0f;
	//float mody = cos(gl_FragCoord.y*0.3f*gl_FragCoord.y*0.3f)+2.0f;
	//FragColor = vec4(UV.x*modx,UV.y*mody,0.0f,1.0f);  