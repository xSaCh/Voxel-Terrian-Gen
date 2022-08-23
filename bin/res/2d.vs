#version 330 core
layout (location = 0) in vec3 vertex;

uniform mat4 model;
uniform mat4 projection;

out vec2 ver;

void main()
{
    ver = vertex.xy;
    gl_Position = projection * model * vec4(vertex, 1.0);
}