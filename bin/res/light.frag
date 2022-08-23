#version 330 core
out vec4 fragColor;

uniform vec3 col;

void main() 
{
    gl_FragColor =  vec4(col,1.0f);
}