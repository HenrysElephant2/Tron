#version 120
//#extension GL_ARB_explicit_attrib_location : require
//#extension GL_ARB_explicit_uniform_location : require

//varying vec3 View;
//varying vec3 Light;
//varying vec3 Normal;
uniform sampler2D data;

//layout (location = 0) out vec4 FragColor;
//layout (location = 1) out vec4 BrightColor;
//attribute vec4 FragColor;
//attribute vec4 BrightColor;

void main()
{
   gl_FragColor = gl_Color;
}
