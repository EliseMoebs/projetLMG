#ifndef __GPUPARAMS_H__
#define __GPUPARAMS_H__

#include "common.h"

#include <QOpenGLFunctions_3_3_Core>


/// Light properties for phong shading
struct LightProperties
{
    glm::vec3   m_vAmbiant;
    glm::vec3   m_vDiffuse;
    glm::vec3   m_vSpecular;

    LightProperties()
    :   m_vAmbiant  ( 0.1f )
    ,   m_vDiffuse  ( 1.f, 1.f, 1.f )
    ,   m_vSpecular ( 1.0f, 1.0f, 1.0f )
    {}
};

/// Material properties for phong shading
struct MaterialProperties
{
    glm::vec3   m_vAmbiant;
    glm::vec3   m_vDiffuse;
    glm::vec3   m_vSpecular;
    float       m_fSpecularPower;

    MaterialProperties()
    :   m_vAmbiant      ( 0.1f, 0.4f, 0.4f )
    ,   m_vDiffuse      ( 0.3f, 0.4f, 1.0f )
    ,   m_vSpecular     ( 0.1f, 0.1f, 0.1f )
    ,   m_fSpecularPower( 30.f )
    {}
};



/// GPUProgram Parameters for the phong textured shader program
struct GPUParamsPhongTextured : protected QOpenGLFunctions_3_3_Core
{
    GLuint                  m_aiUniformLightProp[3];            ///< GLSL uniform location for each light property
    GLuint                  m_iUniformLightPosition;            ///< GLSL uniform location for light position
    GLuint                  m_aiUniformMaterialProp[4];         ///< GLSL uniform location for each material property
    GLuint                  m_iUniformCameraPosition;           ///< GLSL uniform location for camera position
    GLuint                  m_iUniformCameraProjView;           ///< GLSL uniform location for concatenanted camera ( Projection x View   ) matrix
    GLuint      			m_iUniformWorld;                    ///< GLSL uniform location for a World matrix
    GLuint                  m_iUniformSampler;                  ///< GLSL uniform location for the sampler "u_texDiffuse"
    GLuint                  m_iUniformSampler2;                 ///< GLSL uniform location for the sampler "u_texShadow"
    GLuint      			m_iUniformLightProjView;            ///< GLSL uniform location for concatenanted light ( Projection x View ) matrix

    /// Builds this parameter set from the given GPUProgram
    void buildFrom( const GPUProgram& _rProgram )
    {
        // required to have the OpenGL functions working - this is because of our use of OpenGL with Qt
        initializeOpenGLFunctions();

        GPUProgram& rProgram = const_cast< GPUProgram& >( _rProgram );

        m_aiUniformLightProp[0]     = rProgram.getUniformLocation( "u_light.vAmbiant" );
        m_aiUniformLightProp[1]     = rProgram.getUniformLocation( "u_light.vDiffuse" );
        m_aiUniformLightProp[2]     = rProgram.getUniformLocation( "u_light.vSpecular" );

        m_aiUniformMaterialProp[0]  = rProgram.getUniformLocation( "u_material.vAmbiant" );
        m_aiUniformMaterialProp[1]  = rProgram.getUniformLocation( "u_material.vDiffuse" );
        m_aiUniformMaterialProp[2]  = rProgram.getUniformLocation( "u_material.vSpecular" );
        m_aiUniformMaterialProp[3]  = rProgram.getUniformLocation( "u_material.fSpecPower" );

        m_iUniformCameraPosition    = rProgram.getUniformLocation( "u_vCameraPositionWS" );
        m_iUniformLightPosition     = rProgram.getUniformLocation( "u_vLightPositionWS" );


        m_iUniformWorld             = rProgram.getUniformLocation( "u_mtxWorld" );
        m_iUniformCameraProjView    = rProgram.getUniformLocation( "u_mtxCameraProjView" );
        m_iUniformLightProjView     = rProgram.getUniformLocation( "u_mtxLightProjView" );

        // Retrieves the uniform location for the texture SAMPLER used in the GLSL Fragment Shader
        m_iUniformSampler           = rProgram.getUniformLocation( "u_texDiffuse" );
        m_iUniformSampler2          = rProgram.getUniformLocation( "u_texShadow" );

    }

    /// Sends the uniforms var related to the scene from the CPU to the GPU
    void sendSceneDataToGPU( const glm::mat4& _rmtxCameraProjView, const LightProperties& _rLightProp, const MaterialProperties& _rMaterialProp
                            , const glm::vec3& _rvCameraPosition , const glm::vec3& _rvLightPosition, const glm::mat4& _rmtxLightProjView )
    {
        glUniformMatrix4fv( m_iUniformCameraProjView, 1, GL_FALSE, glm::value_ptr(_rmtxCameraProjView ) );

        glUniform3fv( m_aiUniformLightProp[0], 1, glm::value_ptr(_rLightProp.m_vAmbiant) );
        glUniform3fv( m_aiUniformLightProp[1], 1, glm::value_ptr(_rLightProp.m_vDiffuse) );
        glUniform3fv( m_aiUniformLightProp[2], 1, glm::value_ptr(_rLightProp.m_vSpecular) );

        glUniform3fv( m_aiUniformMaterialProp[0], 1, glm::value_ptr(_rMaterialProp.m_vAmbiant) );
        glUniform3fv( m_aiUniformMaterialProp[1], 1, glm::value_ptr(_rMaterialProp.m_vDiffuse) );
        glUniform3fv( m_aiUniformMaterialProp[2], 1, glm::value_ptr(_rMaterialProp.m_vSpecular) );
        glUniform1f( m_aiUniformMaterialProp[3], _rMaterialProp.m_fSpecularPower );

        glUniform3fv( m_iUniformCameraPosition, 1, glm::value_ptr( _rvCameraPosition ) );
        glUniform3fv( m_iUniformLightPosition, 1, glm::value_ptr( _rvLightPosition ) );

        // Specify on which texture unit the uniform texture sampler must be bound
        glUniform1i( m_iUniformSampler , 0 );

        // Shadow Map use parameters
        glUniformMatrix4fv( m_iUniformLightProjView, 1, GL_FALSE, glm::value_ptr(_rmtxLightProjView) );
        glUniform1i( m_iUniformSampler2 , 1 );

    }

    /// Sends the uniforms var related to the model from the CPU to the GPU
    void sendModelDataToGPU( const glm::mat4& _rmtxWorld )
    {
        glUniformMatrix4fv( m_iUniformWorld, 1, GL_FALSE, glm::value_ptr( _rmtxWorld ) );
    }
};

/// GPUProgram Parameters for the shadow map building shader program
struct GPUParamsSSAO : protected QOpenGLFunctions_3_3_Core
{
   // GLuint      			m_iUniformShadowMapProjView;        ///< GLSL uniform location for concatenanted ( Projection x View ) matrix
   // GLuint      			m_iUniformShadowMapWorld;           ///< GLSL uniform location for World matrix

    GLuint                  m_iUniformSamplerBlit;              ///<GLSL uniform location for the sampler "u_texBlit"
    GLuint                  m_iUniformSamplerNoise;             ///<GLSL uniform location for the sampler "u_texNoise"
    GLuint                  m_iUniformSamplerDepth;             ///<GLSL uniform location for the sampler "u_texDepth"
    GLuint                  m_iUniformSamplerNormal;            ///<GLSL uniform location for the sampler "u_texNormal"


    /// Builds this parameter set from the given GPUProgram
    void buildFrom( const GPUProgram& _rProgram )
    {
        // required to have the OpenGL functions working - this is because of our use of OpenGL with Qt
        initializeOpenGLFunctions();

        GPUProgram& rProgram = const_cast< GPUProgram& >( _rProgram );

     //   m_iUniformShadowMapWorld    = rProgram.getUniformLocation( "u_mtxWorld" );
       // m_iUniformShadowMapProjView = rProgram.getUniformLocation( "u_mtxProjView" );

        m_iUniformSamplerBlit = rProgram.getUniformLocation( "u_texBlit" );
        m_iUniformSamplerNoise = rProgram.getUniformLocation( "u_texNoise" );
        m_iUniformSamplerDepth = rProgram.getUniformLocation( "u_texDepth" );
        m_iUniformSamplerNormal = rProgram.getUniformLocation("u_texNormal");

    }

    /// Sends the uniforms var related to the model from the CPU to the GPU
    void sendDataToGPU( /*const glm::mat4& _rmtxWorld, const glm::mat4& _rmtxLightProjView */)
    {
        //glUniformMatrix4fv( m_iUniformShadowMapWorld, 1, GL_FALSE, glm::value_ptr(_rmtxWorld) );
       // glUniformMatrix4fv( m_iUniformShadowMapProjView, 1, GL_FALSE, glm::value_ptr(_rmtxLightProjView) );

        glUniform1i(m_iUniformSamplerBlit,0);
        glUniform1i(m_iUniformSamplerNoise,1);
        glUniform1i(m_iUniformSamplerDepth,2);
        glUniform1i(m_iUniformSamplerNormal,3);

    }
};


#endif // __GPUPARAMS_H__

