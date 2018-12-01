
uniform sampler2D tex;
uniform int width;
uniform int height;
uniform bool horizontal;
//uniform float weight[5] = float[5](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    float weight[5];
    weight[0] = 0.227027;
    weight[1] = 0.1945946;
    weight[2] = 0.1216216;
    weight[3] = 0.054054;
    weight[4] = 0.016216;

    vec2 texSize = vec2(1400, 900);
    vec2 tex_offset = 1.0 / texSize; // gets size of single texel
    vec3 result = texture2D(tex, gl_TexCoord[0].xy).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture2D(tex, gl_TexCoord[0].xy + vec2(tex_offset.x * float(i), 0.0)).rgb * weight[i];
	    //result += texture2D(tex, gl_TexCoord[0].xy + vec2(gl_TexCoord[0].x + float(i - 2)/512., 0.0)).rgb * weight[i];
            result += texture2D(tex, gl_TexCoord[0].xy - vec2(tex_offset.x * float(i), 0.0)).rgb * weight[i];
            //result += texture2D(tex, gl_TexCoord[0].xy - vec2(gl_TexCoord[0].x + float(i - 2)/512., 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture2D(tex, gl_TexCoord[0].xy + vec2(0.0, tex_offset.y * float(i))).rgb * weight[i];
            //result += texture2D(tex, gl_TexCoord[0].xy + vec2(0.0, gl_TexCoord[0].y + float(i - 2)/512.)).rgb * weight[i];
            result += texture2D(tex, gl_TexCoord[0].xy - vec2(0.0, tex_offset.y * float(i))).rgb * weight[i];
            //result += texture2D(tex, gl_TexCoord[0].xy - vec2(0.0, gl_TexCoord[0].y + float(i - 2)/512.)).rgb * weight[i];
        }
    }
    gl_FragColor = vec4(result, 1.0);
    //gl_FragColor = texture2D(tex, gl_TexCoord[0].xy) + vec4(.5,.5,.5,0.);
}