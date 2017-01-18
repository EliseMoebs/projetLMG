#version 330

// get these data from the previous Shader Unit in the pipeline
in vec2 vs_texCoords;
in vec3 vs_vNormal;
in vec3 vs_vToCamera;
in vec3 vs_vToLight;

uniform sampler2D u_texDiffuse;

struct LightProperties
{
    vec3   vAmbiant;
    vec3   vDiffuse;
    vec3   vSpecular;
};
uniform LightProperties u_light;

struct MaterialProperties
{
    vec3   vAmbiant;
    vec3   vDiffuse;
    vec3   vSpecular;
    float  fSpecPower;
};
uniform MaterialProperties u_material;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns intensity of reflected ambient lighting
vec3 ambientLighting()
{
    return u_light.vAmbiant.rgb;
//    return u_material.vAmbiant.rgb * u_light.vAmbiant.rgb;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns intensity of diffuse reflection
vec3 diffuseLighting( in vec3 N, in vec3 L )
{

    // calculation as for Lambertian reflection
    float NdotL = dot( N, L );
    float diffuseTerm = clamp( NdotL, 0, 1 );

//    return u_material.vDiffuse.rgb * u_light.vDiffuse.rgb * diffuseTerm;

//    vec3 texColour = texture( u_texDiffuse, vs_texCoords ).rgb;
//    return texColour * u_light.vDiffuse.rgb * diffuseTerm;

    return u_light.vDiffuse.rgb * diffuseTerm;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// returns intensity of specular reflection
vec3 specularLightingPhong( in vec3 N, in vec3 L, in vec3 V )
{
    // Light vector reflected by the Normal
    vec3 R = reflect( -L, N );

    float specularTerm = pow( max( dot( R, V ), 0.f ), u_material.fSpecPower );

    return u_material.vSpecular.rgb * u_light.vSpecular.rgb * specularTerm;
}


#define BASE_SHADOW     0.3f
#define EPSILON         0.0001f

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main(void)
{
    // re-normalize vectors after interpolation
    vec3 L = normalize(vs_vToLight);
    vec3 V = normalize(vs_vToCamera);
    vec3 N = normalize(vs_vNormal);


    // get Blinn-Phong reflectance components
    vec3 Iamb = ambientLighting();
    vec3 Idif = diffuseLighting( N, L );
    vec3 Ispe = specularLightingPhong( N, L, V );

    // diffuse color of the object from texture
    vec3 texColour = texture( u_texDiffuse, vec2( vs_texCoords.x, 1 - vs_texCoords.y) ).rgb;


    out_fragColor.xyz = ( Iamb + Idif ) * texColour  + Ispe ;
//    out_fragColor.xyz = vec3(fShadow);

    out_fragColor.a = 1;
}
