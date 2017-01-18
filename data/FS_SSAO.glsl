#version 330
/*
// get these data from the previous Shader Unit in the pipeline
in vec2 vs_texCoords;

uniform sampler2D u_texBlit;
uniform sampler2D u_texNoise;
uniform sampler2D u_texDepth;


// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;


vec3 normalFromDepth(float depth, vec2 texcoords)
{

  vec2 offset1 = vec2(0.0,0.001);
  vec2 offset2 = vec2(0.001,0.0);

  float depth1 = texture2D(u_texDepth, texcoords + offset1).r;
  float depth2 = texture2D(u_texDepth, texcoords + offset2).r;


  vec3 p1 = vec3(offset1, depth1 - depth);
  vec3 p2 = vec3(offset2, depth2 - depth);

  vec3 normal = cross(p1, p2);
  normal.z = -normal.z;

  return normalize(normal);
}

vec3 reflection(vec3 v1,vec3 v2)
{
    vec3 result= 2.0 * dot(v2, v1) * v2;
    result=v1-result;
    return result;
}




void main()
{
        const float m_Intensity = 1.0;
        const float m_Base = 0.2;

        const float m_Area = 0.0075;
        const float m_Falloff = 0.000001;

        const float m_Radius = 0.0005;
         vec3 sample_sphere[16] = vec3[]( vec3( 0.5381, 0.1856,-0.4319), vec3( 0.1379, 0.2486, 0.4430),
                                                                          vec3( 0.3371, 0.5679,-0.0057), vec3(-0.6999,-0.0451,-0.0019),
                                                                          vec3( 0.0689,-0.1598,-0.8547), vec3( 0.0560, 0.0069,-0.1843),
                                                                          vec3(-0.0146, 0.1402, 0.0762), vec3( 0.0100,-0.1924,-0.0344),
                                                                          vec3(-0.3577,-0.5301,-0.4358), vec3(-0.3169, 0.1063, 0.0158),
                                                                          vec3( 0.0103,-0.5869, 0.0046), vec3(-0.0897,-0.4940, 0.3287),
                                                                          vec3( 0.7119,-0.0154,-0.0918), vec3(-0.0533, 0.0596,-0.5411),
                                                                          vec3( 0.0352,-0.0631, 0.5460), vec3(-0.4776, 0.2847,-0.0271)
                                                                        );

        vec3 random = normalize( texture2D(u_texNoise, vs_texCoords * 4.0).rgb );
        float depth = texture2D(u_texDepth, vs_texCoords).r;
        vec3 position = vec3(vs_texCoords.x,vs_texCoords.y,depth);
        vec3 normal = normalFromDepth(depth, vs_texCoords);
        float radiusDepth = m_Radius/depth;
        float occlusion = 0.0;

        int iterations = 16;
        for (int j = 0; j < iterations; ++j)
        {
                vec3 ray = radiusDepth * reflect(sample_sphere[j], random);
                vec3 hemiRay = position + sign(dot(ray,normal)) * ray;

                float occDepth = texture2D(u_texDepth, clamp(hemiRay.xy,0.0,1.0)).r;
                float difference = depth - occDepth;

                occlusion += step(m_Falloff, difference) * (1.0-smoothstep(m_Falloff, m_Area, difference));
        }

        float ao = 1.0 - m_Intensity * occlusion * (1.0 / iterations);
        float final = clamp(ao + m_Base,0.0,1.0);

        out_fragColor = vec4(final,final,final,1);
}

/*/


// get these data from the previous Shader Unit in the pipeline
in vec2 vs_texCoords;

uniform sampler2D u_texBlit;
uniform sampler2D u_texNoise;
uniform sampler2D u_texDepth;


// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;


vec3 normalFromDepth(float depth, vec2 texcoords)
{

  vec2 offset1 = vec2(0.0,0.001);
  vec2 offset2 = vec2(0.001,0.0);

  float depth1 = texture2D(u_texDepth, texcoords + offset1).r;
  float depth2 = texture2D(u_texDepth, texcoords + offset2).r;


  vec3 p1 = vec3(offset1, depth1 - depth);
  vec3 p2 = vec3(offset2, depth2 - depth);

  vec3 normal = cross(p1, p2);
  normal.z = -normal.z;

  return normalize(normal);
}


void main()
{

    ivec2 vTexSize = textureSize( u_texDepth, 0 );

    vec2 vOffset = 1.0f / vTexSize;
    vOffset*=10;
    float occlusion = 0.0;

    float depth = texture2D(u_texDepth,vs_texCoords).r;
    vec3 random = normalize(texture2D(u_texNoise, vs_texCoords).rgb);
    out_fragColor = vec4(random,1);
    return;
    vec3 position = vec3(vs_texCoords.x,vs_texCoords.y,depth);
    vec3 normal = normalFromDepth(depth, vs_texCoords);

    float rad = 0.00002/position.z;

    for( int y = -1 ; y <= 1 ; y++ )
    {
        for( int x = -1 ; x <= 1 ; x++ )
        {
            vec2 Offsets = vec2( x * vOffset.x, y * vOffset.y );
            Offsets =  reflect(Offsets,random.xy)* rad;
            vec2 vUVoff = ( vs_texCoords.xy + Offsets);
            float diff = abs(texture2D( u_texDepth, vUVoff).r  - depth);
            if(depth > texture2D( u_texDepth, vUVoff).r)
            {
                occlusion += diff;
            }
        }
    }

    out_fragColor = vec4(1-100*occlusion);
    return;

    float fAverageAmbiantOcclusion = occlusion / 9.0;
    out_fragColor = vec4(fAverageAmbiantOcclusion);


}
//*/
