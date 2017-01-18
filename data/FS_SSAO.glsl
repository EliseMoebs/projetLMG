#version 330



// get these data from the previous Shader Unit in the pipeline
in vec2 vs_texCoords;

uniform sampler2D u_texBlit;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;

void main()
{
    out_fragColor = texture( u_texBlit, vs_texCoords );

}
