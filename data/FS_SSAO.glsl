#version 330
/*

//SSAO
// get these data from the previous Shader Unit in the pipeline
in vec2 vs_texCoords;

uniform sampler2D u_texBlit;
uniform sampler2D u_texNoise;
uniform sampler2D u_texDepth;
uniform sampler2D u_texNormal;

uniform mat4 u_mtxProjInv;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;
float g_scale;
float g_bias;
float g_intensity;

vec3 WSPositionFromDepth(vec2 vTexCoord)
{
    // Get the depth value for this pixel
    float z = texture2D(u_texDepth, vTexCoord).r;
    // Get x/w and y/w from the viewport position
    float x = vTexCoord.x * 2 - 1;
    float y = vTexCoord.y * 2 - 1;
    vec4 vProjectedPos = vec4(x, y, z, 1.0f);
    // Transform by the inverse projection matrix
    vec4 vPositionVS =  u_mtxProjInv*vProjectedPos;
    // Divide by w to get the view-space position
    //return vPositionWS.xyz;

    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;

}

float doAmbientOcclusion( vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm)
{
//    vec3 diff = WSPositionFromDepth(tcoord+uv) - p ;//* 2.0f - vec3(1.0f);
    vec3 diff = WSPositionFromDepth(tcoord+uv) - p ;//* 2.0f - vec3(1.0f);

    //vec3 diff = getPosition(tcoord + uv) - p;
//    vec3 diff = vec3(tcoord+uv,p[2]) - p;
    vec3 v = normalize(diff);
    float d = length(diff)*g_scale;
    return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

void main()
{


    g_scale = 1;
    g_bias = 0.00015;
    g_intensity = 15;
    const vec2 vec[4] = vec2[](vec2(1,0),vec2(-1,0),
                               vec2(0,1),vec2(0,-1));
    float depth = texture2D(u_texDepth, vs_texCoords).r;
    vec2 rand = normalize(texture2D(u_texNoise, vs_texCoords).rg * 2.0f - 1.0f );
    vec3 p = vec3( vs_texCoords.x * 2.0f - 1.0f, vs_texCoords.y * 2.0f - 1.0f, depth );
    //vec3 p = WSPositionFromDepth(vs_texCoords);
    vec3 n = normalize(texture2D(u_texNormal,vs_texCoords).xyz * 2.0f - 1.0f);

//    float3 p = getPosition(i.uv);
//    float3 n = getNormal(i.uv);
//    float2 rand = getRandom(i.uv);

    float ao = 0.0f;
    float rad = 0.0002/depth;

    int iterations = 4;
    for (int j = 0; j < iterations; ++j)
    {
      vec2 coord1 = vec[j]*rad;//reflect(vec[j],rand)*rad;
      vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707,
                         coord1.x*0.707 + coord1.y*0.707);

      ao += doAmbientOcclusion(vs_texCoords,coord1*0.25, p, n);
      ao += doAmbientOcclusion(vs_texCoords,coord2*0.5, p, n);
      ao += doAmbientOcclusion(vs_texCoords,coord1*0.75, p, n);
      ao += doAmbientOcclusion(vs_texCoords,coord2, p, n);
    }
    ao/=iterations*4.0;
    out_fragColor = vec4(1-ao)*texture2D(u_texBlit,vs_texCoords);
}
*/


//  SSAO ET BLOOM COMBINES

// get these data from the previous Shader Unit in the pipeline
in vec2 vs_texCoords;

uniform sampler2D u_texBlit;
uniform sampler2D u_texNoise;
uniform sampler2D u_texDepth;
uniform sampler2D u_texNormal;

uniform mat4 u_mtxProjInv;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;
float g_scale;
float g_bias;
float g_intensity;

vec3 WSPositionFromDepth(vec2 vTexCoord)
{
    // Get the depth value for this pixel
    float z = texture2D(u_texDepth, vTexCoord).r;
    // Get x/w and y/w from the viewport position
    float x = vTexCoord.x * 2 - 1;
    float y = vTexCoord.y * 2 - 1;
    vec4 vProjectedPos = vec4(x, y, z, 1.0f);
    // Transform by the inverse projection matrix
    vec4 vPositionVS =  u_mtxProjInv*vProjectedPos;
    // Divide by w to get the view-space position
    //return vPositionWS.xyz;

    // Divide by w to get the view-space position
    return vPositionVS.xyz / vPositionVS.w;

}

float doAmbientOcclusion( vec2 tcoord, vec2 uv, vec3 p, vec3 cnorm)
{
//    vec3 diff = WSPositionFromDepth(tcoord+uv) - p ;//* 2.0f - vec3(1.0f);
    vec3 diff = WSPositionFromDepth(tcoord+uv) - p ;//* 2.0f - vec3(1.0f);

    //vec3 diff = getPosition(tcoord + uv) - p;
//    vec3 diff = vec3(tcoord+uv,p[2]) - p;
    vec3 v = normalize(diff);
    float d = length(diff)*g_scale;
    return max(0.0,dot(cnorm,v)-g_bias)*(1.0/(1.0+d))*g_intensity;
}

void main()
{


    g_scale = 1;
    g_bias = 0.00015;
    g_intensity = 15;
    const vec2 vec[4] = vec2[](vec2(1,0),vec2(-1,0),
                               vec2(0,1),vec2(0,-1));
    float depth = texture2D(u_texDepth, vs_texCoords).r;
    vec2 rand = normalize(texture2D(u_texNoise, vs_texCoords).rg * 2.0f - 1.0f );
    vec3 p = vec3( vs_texCoords.x * 2.0f - 1.0f, vs_texCoords.y * 2.0f - 1.0f, depth );
    //vec3 p = WSPositionFromDepth(vs_texCoords);
    vec3 n = normalize(texture2D(u_texNormal,vs_texCoords).xyz * 2.0f - 1.0f);

//    float3 p = getPosition(i.uv);
//    float3 n = getNormal(i.uv);
//    float2 rand = getRandom(i.uv);

    float ao = 0.0f;
    float rad = 0.0002/depth;

    int iterations = 4;
    for (int j = 0; j < iterations; ++j)
    {
      vec2 coord1 = vec[j]*rad;//reflect(vec[j],rand)*rad;
      vec2 coord2 = vec2(coord1.x*0.707 - coord1.y*0.707,
                         coord1.x*0.707 + coord1.y*0.707);

      ao += doAmbientOcclusion(vs_texCoords,coord1*0.25, p, n);
      ao += doAmbientOcclusion(vs_texCoords,coord2*0.5, p, n);
      ao += doAmbientOcclusion(vs_texCoords,coord1*0.75, p, n);
      ao += doAmbientOcclusion(vs_texCoords,coord2, p, n);
    }
    ao/=iterations*4.0;

    vec4 sum = vec4(0);
    vec2 texcoord = vs_texCoords;//vec2(gl_TexCoord[0]);
    int j;
    int i;
    vec4 color;
    for( i= -4 ;i < 4; i++)
    {
         for (j = -3; j < 3; j++)
         {
             sum += texture2D(u_texBlit, texcoord + vec2(j, i)*0.0004) * 0.25;
         }
    }
        if (texture2D(u_texBlit, texcoord).r < 0.2)
     {
        color = sum*sum*0.005 + texture2D(u_texBlit, texcoord);
     }
     else
     {
         if (texture2D(u_texBlit, texcoord).r < 0.5)
         {
             color = sum*sum*0.005 + texture2D(u_texBlit, texcoord);
         }
         else
         {
             color = sum*sum*0.005 + texture2D(u_texBlit, texcoord);
         }
    }
    out_fragColor = vec4(1-ao)*color;
}


/*

//  BLOOM

// get these data from the previous Shader Unit in the pipeline
in vec2 vs_texCoords;

uniform sampler2D u_texBlit;
uniform sampler2D u_texNoise;
uniform sampler2D u_texDepth;
uniform sampler2D u_texNormal;

out vec4 color;

void main()
{
   vec4 sum = vec4(0);
   vec2 texcoord = vs_texCoords;//vec2(gl_TexCoord[0]);
   int j;
   int i;

   for( i= -4 ;i < 4; i++)
   {
        for (j = -3; j < 3; j++)
        {
            sum += texture2D(u_texBlit, texcoord + vec2(j, i)*0.0004) * 0.25;
        }
   }
       if (texture2D(u_texBlit, texcoord).r < 0.2)
    {
       color = sum*sum*0.012 + texture2D(u_texBlit, texcoord);
    }
    else
    {
        if (texture2D(u_texBlit, texcoord).r < 0.5)
        {
            color = sum*sum*0.009 + texture2D(u_texBlit, texcoord);
        }
        else
        {
            color = sum*sum*0.0075 + texture2D(u_texBlit, texcoord);
        }
    }
}
*/
/*
//AUTRE PISTE POUR LE SSAO
in vec2 vs_texCoords;

uniform sampler2D u_texBlit;
uniform sampler2D u_texNoise;
uniform sampler2D u_texDepth;
uniform sampler2D u_texNormal;

uniform mat4 u_mtxProjInv;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;
float g_scale;
float g_bias;
float g_intensity;

void main()
{
    ivec2 vTexSize = textureSize( u_texDepth, 0 );
    float occlusion = 0.0;
// uncomment to get basic version
//    return texture( u_texShadow, vs_texCoordsShadow.xyz );

    vec2 vOffset = 1.0f / vTexSize;

    vOffset *= 10; // take a samples further away !


    for( int y = -1 ; y <= 1 ; y++ )
    {
        for( int x = -1 ; x <= 1 ; x++ )
        {
            vec2 Offsets = vec2( x * vOffset.x, y * vOffset.y );

            float diff = texture2D(u_texDepth,vs_texCoords + Offsets).r - texture2D(u_texDepth,vs_texCoords).r;
            if(diff > 0 && diff < 0.0005)
            {
                occlusion += diff;
            }
        }
    }


    float fAverageOcclusionFactor = occlusion / 9.0;
    out_fragColor = vec4(1- 1000*fAverageOcclusionFactor);

}
*/
