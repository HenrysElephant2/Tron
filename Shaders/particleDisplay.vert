#version 120
//struct _Particle{
//	float x;
//	float y;
//	float z;
//	int ID;
//} Particle;

//layout(location = 0) in 

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);

}


const float rainbowColors[21] = float[21](
		1.0,0.0,0.0,
		1.0,.5,0.0,
		1.0,1.0,0.0,
		0.0,1.0,0.0,
		0.0,1.0,1.0,
		0.0,0.0,1.0,
		1.0,0.0,1.0
	);

void setColor(float x) {
	float frequency = 2;
	float rawI = frequency * (x);
	float colorI = mod(floor(rawI),7.0);
	float weight = rawI - floor(rawI);
	float red   = rainbowColors[int(mod((colorI+1.0),7.0)*3.0)] * weight + rainbowColors[int(colorI*3)] * (1.0-weight);
	float green = rainbowColors[int(mod((colorI+1.0),7.0)*3.0+1.0)] * weight + rainbowColors[int(colorI*3)] * (1.0-weight);
	float blue  = rainbowColors[int(mod((colorI+1.0),7.0)*3.0+2.0)] * weight + rainbowColors[int(colorI*3)] * (1.0-weight);

	gl_FrontColor = vec4(red,green,blue,1.0);
}

uniform sampler2D data;
uniform bool rainbow;

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    float a = (gl_Vertex.w);
    float b = 256.0;//2.0;
    float c = floor(mod(a, b));
    float d = floor(a / b);
    vec2 coords = vec2(c/512.0, d/512.0);
    vec3 offset = texture2D(data, coords).rgb;
    vec4 loc = vec4(offset + gl_Vertex.xyz,1.0);
    //loc = vec4(offset,1.0);
    gl_Position = gl_ModelViewProjectionMatrix * loc;
    //gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    if(rainbow)
    {
    	if(loc.y <= 5)
	    {
	    	float rand = random(vec2(gl_Vertex.w,.5583939));
	    	float frequency = 6.0;
	    	gl_FrontColor = vec4(sin(rand*frequency)*.5+.5,sin(rand*frequency+2.0)*.5+.5,sin(rand*frequency+4.0)*.5+.5,1.0);
	    }
    	else setColor(loc.y * loc.y + loc.z * loc.z);
    }
    else gl_FrontColor = gl_Color;
}