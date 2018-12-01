//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoords;

//varying vec3 aPos;
//varying vec2 aTexCoords;

//attribute vec2 TexCoords;

void main()
{
    //TexCoords = aTexCoords;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    //gl_Position = vec4(aPos, 1.0);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    //gl_Position = gl_Vertex;
}