#version 330

// get the vertex position, and texCoords, as vec3 and vec2 input - sent from the CPU, via VertexBuffer
layout(location=0) in vec3 vtx_position;

uniform mat4 u_mtxWorld;
uniform mat4 u_mtxProjView;

void main()
{
    gl_Position = u_mtxProjView * u_mtxWorld * vec4( vtx_position, 1.0f );
}
