#version 330

uniform mat4 u_mtxWorld;
uniform mat4 u_mtxCameraProjView;
uniform mat4 u_mtxLightProjView;

//uniform mat4 u_mtxNormal;

uniform vec3 u_vCameraPositionWS;
uniform vec3 u_vLightPositionWS;

// get the vertex position, normal, colour as vec3 input - sent from the CPU, VertexBuffer
layout (location=0) in vec3 vtx_position;
layout (location=1) in vec3 vtx_normal;
layout (location=2) in vec2 vtx_texCoords;

// transfer these data to the Fragment Shader !
out vec3 vs_vNormal;
out vec3 vs_vToCamera;
out vec3 vs_vToLight;
out vec2 vs_texCoords;


const mat4 mtxBias  = mat4  ( vec4( 0.5f, 0.0f, 0.0f, 0.0f )
                            , vec4( 0.0f, 0.5f, 0.0f, 0.0f )
                            , vec4( 0.0f, 0.0f, 0.5f, 0.0f )
                            , vec4( 0.5f, 0.5f, 0.5f, 1.0f )
                            );

void main()
{
    // we are going to do all the light related calculations in WORLD SPACE
    vec4 vWorldPosition = u_mtxWorld * vec4( vtx_position, 1.0f );

    // normal in world space
//    vs_vNormal	= ( u_mtxNormal * vec4( vtx_normal, 1.0f ) ).xyz;
    vs_vNormal	= mat3( u_mtxWorld ) * vtx_normal;

    // direction to camera
    vs_vToCamera= u_vCameraPositionWS.xyz - vWorldPosition.xyz;

    // direction to light
    vs_vToLight	= u_vLightPositionWS.xyz - vWorldPosition.xyz;

    // transfert vertex colour from this Shader Unit (Vertex) to the next Shader Unit - in our case Fragment
    vs_texCoords = vtx_texCoords;

    gl_Position = u_mtxCameraProjView * vWorldPosition;
}
