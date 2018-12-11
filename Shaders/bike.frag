#version 120
//#extension GL_ARB_explicit_attrib_location : require
//#extension GL_ARB_explicit_uniform_location : require

varying vec3 View;
varying vec3 Light;
varying vec3 Normal;
uniform sampler2D tex;

//layout (location = 0) out vec4 FragColor;
//layout (location = 1) out vec4 BrightColor;
//attribute vec4 FragColor;
//attribute vec4 BrightColor;

void main()
{
   //  N is the object normal
   vec3 N = normalize(Normal);
   //  L is the light vector
   vec3 L = normalize(Light);
   //  R is the reflected light vector R = 2(L.N)N - L
   vec3 R = reflect(-L,N);
   //  V is the view vector (eye vector)
   vec3 V = normalize(View);

   //  Diffuse light is cosine of light and normal vectors
   float Id = max(dot(L,N) , 0.0);
   //  Specular is cosine of reflected and view vectors
   float Is = (Id>0.0) ? pow(max(dot(R,V),0.0) , gl_FrontMaterial.shininess) : 0.0;

   //  Sum color types
   vec4 color_mat = gl_FrontMaterial.emission
              + gl_FrontLightProduct[0].ambient
              + Id*gl_FrontLightProduct[0].diffuse
              + Is*gl_FrontLightProduct[0].specular;

   //  Apply texture
   vec4 texcolor = texture2D(tex,gl_TexCoord[0].xy);

   if(texcolor.r >= 0.5 && texcolor.g >= 0.5 && texcolor.b >= 0.5)
   {
	gl_FragColor = gl_Color;
	//BrightColor = gl_Color;
   }
   else {
	gl_FragColor = color_mat * texture2D(tex,gl_TexCoord[0].xy);
	//BrightColor = vec4(0,0,0,0);
   }
}
