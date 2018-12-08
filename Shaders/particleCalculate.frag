

uniform sampler2D data;
uniform float dTime;
uniform bool init;


const float size = 512.0;

float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

void main()
{      
    if(gl_TexCoord[0].x < .5)
    {
		vec3 location = texture2D(data, gl_TexCoord[0].xy).rgb;   
		vec2 velXY = vec2(gl_TexCoord[0].x + .5, gl_TexCoord[0].y);  
		vec3 velocity = texture2D(data, velXY).rgb;
		if(init)
		{
			float random1 = random(gl_TexCoord[0].xy * dTime);
			float random2 = random(vec2(dTime, random1));
			float random3 = random(vec2(random1, random2));
			gl_FragColor = vec4(random1*40.0-20.0,random2*15.0,random3*10.0 - 5.0,1.0);
		}
		else {
			vec4 outLocation = vec4(location + (velocity * dTime), 1.0);
			if(outLocation.y < 0.0)
				outLocation.y = 0.0;
			gl_FragColor = outLocation;
			//gl_FragColor = vec4(location,1.0) ;
		}
    }
    else {
		vec3 velocity = texture2D(data, gl_TexCoord[0].xy).rgb;
		vec2 locXY = vec2(gl_TexCoord[0].x - .5, gl_TexCoord[0].y);  
		vec3 location = texture2D(data, locXY).rgb;
		

		if(init)
		{
			float random1 = random(gl_TexCoord[0].xy * dTime);
			float random2 = random(vec2(dTime, random1));
			float random3 = random(vec2(random1, random2));
			gl_FragColor = vec4(random1*100.0,random2*200.0-90.0,random3*80.0 - 40.0,1.0);
		}
		else {
			if(location.y <= 0.0)
			{
				velocity.y = -.5 * velocity.y ;

				velocity.x -= 1.0;
				if(velocity.x < 0.0)
					velocity.x = 0.0;

				velocity.z -= 1.0;
				if(velocity.z < 0.0)
					velocity.z = 0.0;
			}
			gl_FragColor = vec4(velocity.x,velocity.y - (98.0/1.5 * dTime), velocity.z, 1.0);
			//gl_FragColor = vec4(velocity,1.0);
		}
    }
}

