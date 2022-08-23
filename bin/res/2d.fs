#version 330 core

uniform vec4 spriteColor;
uniform sampler2D tex; 
uniform float borderRadius;

in vec2 ver;

float sdRoundRect( in vec2 p, in vec2 b, in float r ) {
    vec2 q = abs(p)-b+r;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r;
    //  return length(max(abs(p)-b+r,0.0))-r;
}


void main()
{    
    // float br = borderRadius;
    // if(borderRadius == 0.9f) 
    //     br = 0.4f;
    
    // if(br > 0)
    // {
    //    vec2 centerPos = ver - vec2(0.5, 0.5);
    
    //    float fDist = sdRoundRect(centerPos, vec2(0.5, 0.5), br);

    //    float smoothedAlpha =  1-smoothstep(0, 0.02, fDist);

    //     vec4 c = texture(tex,ver)*spriteColor;

    //    gl_FragColor =  mix(vec4(0.0f),vec4(c.x,c.y,c.z,smoothedAlpha),smoothedAlpha);
    // }
    // else
        // gl_FragColor = texture(tex,ver)*spriteColor;
    gl_FragColor = vec4(vec3(0.8f),1.0f);
}   