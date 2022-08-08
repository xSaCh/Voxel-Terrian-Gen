#version 330 core

uniform sampler2D tex;

in vec2 texCoords;
void main() 
{
    //  gl_FragColor =  vec4(objCol * (ambientColor + diffuseColor + specularColor) ,1.0f );
    //  gl_FragColor =  vec4(vec3(0.8,0.1,0.2) ,1.0);
    // gl_FragColor =  vec4(vec3(-1.0f) * norm ,1.0f);
    gl_FragColor = texture(tex,texCoords);
}