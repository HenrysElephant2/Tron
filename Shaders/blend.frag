//#version 330 core
//out vec4 FragColor;

//in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture2D(scene, gl_TexCoord[0].xy).rgb;      
    vec3 bloomColor = texture2D(bloomBlur, gl_TexCoord[0].xy).rgb;
    //if(bloom)
        hdrColor += bloomColor * 2.0; // additive blending
    // tone mapping
    //vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));
    vec3 result = hdrColor;
    gl_FragColor = vec4(result, 1.0);
    //gl_FragColor = vec4(hdrColor, 1.0);
}