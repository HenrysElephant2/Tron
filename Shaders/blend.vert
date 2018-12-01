//#version 330 core
//layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec2 aTexCoords;

//out vec2 TexCoords;
varying vec3 aPos;

void main()
{
    //TexCoords = aTexCoords;
    //gl_Position = vec4(aPos, 1.0);
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord1;
    //gl_Position = vec4(aPos, 1.0);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}