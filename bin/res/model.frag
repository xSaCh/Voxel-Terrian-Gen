#version 330 core

uniform sampler2D texture_diffuse1;

in vec2 TexCoords;

void main()
{    
    gl_FragColor = texture(texture_diffuse1, TexCoords);
    // gl_FragColor = vec4(0.4,0.2,0.5,1.0);
}