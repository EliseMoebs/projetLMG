#include "main.h"


const float TPGLWindow::s_fCameraZInit = 20.f;

//====================================================================================================================================
TPGLWindow::TPGLWindow()
    : m_bDebugShadowMap             ( false )
    , m_vObjectTranslate            (0)
    , m_vObjectEulerAngles          (0)
    , m_mtxCharacterWorld           (0)
    , m_mtxGroundWorld              (0)
    , m_mtxLightProjView            (0)
    , m_mtxCameraProjView           (0)
    , m_mtxCameraView               (0)
    , m_mtxCameraProj               (0)
    , m_paramsSceneMap         ()
    , m_paramsSSAO             ()
    , m_textureCharacter            ()
    , m_textureGround               ()
    , m_textureNoise                ()
    , m_bAlphaBlend                 ( false )
    , m_RenderTarget                ()
    , m_GPUProgramSceneMap     ()
    , m_GPUProgramSSAO         ()
    , m_MeshGround                  ()
    , m_MeshCharacter               ()
{
    m_vCameraPosition   = glm::vec3(0,0,s_fCameraZInit);

    m_timer.start();

    // request the window to render in loop
    setAnimating(true);
}

//====================================================================================================================================
TPGLWindow::~TPGLWindow()
{
    m_RenderTarget.destroy();

    destroyGPUPrograms();

    destroyTextures();

    destroyMeshes();
}


//====================================================================================================================================
void TPGLWindow::initialize()
{
    // Prints the GL Version
    LOG("GL Version : %s\n",(char*)glGetString(GL_VERSION));

    createMeshes();

    createTextures();

    createGPUPrograms();

    m_RenderTarget.create( this->width(), this->height(), GL_RGB, GL_RGB, GL_DEPTH_COMPONENT );

}


//====================================================================================================================================
void TPGLWindow::update()
{
    // NB : Just like render(), this method is called every frame, as often/fast as possible

//    static int siFrameID        = 0;

    // Update light position, so that it is animated
    float   fTimeElapsed        = (float) m_timer.elapsed();
    m_vLightPosition            = glm::vec3( 30 * cos(fTimeElapsed/1000), 30, 30 * sin(fTimeElapsed/1000) );

    // make sure the matrix data are init to some valid values
    updateMatrices();

//    siFrameID++;
}

//====================================================================================================================================
void TPGLWindow::render()
{
    // Specify winding order Counter ClockZise (even though it's default on OpenGL) ----------------
    glFrontFace( GL_CCW );

    // set back face culling
    glCullFace( GL_BACK );

    // Enables (Back) Face Culling -----------------------------------------------------------------
    glEnable( GL_CULL_FACE );

    // Enables Depth Test --------------------------------------------------------------------------
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    // Enables Depth Write -------------------------------------------------------------------------
    glDepthMask( GL_TRUE );

    // Disables Alpha Blending ----------------------------------------------------------------------
    glDisable( GL_BLEND );


    // ------------------------------------------------------------------------------------------------
    // Builds the shadow map - should be called only when necessary to avoid wasting perf !
    buildSceneMap();

    // ------------------------------------------------------------------------------------------------
    // Draws onscreen, into the default FrameBuffer ---------------------------------------------------

    // Specifies the viewport size -----------------------------------------------------------------
    const float retinaScale = devicePixelRatio();
    glViewport( 0, 0, width() * retinaScale, height() * retinaScale );

    // Specify the color to use when clearing theframebuffer --------------------------------------
    glClearColor( 0.05f, 0.2f, 0.3f, 1.0f );

    // Clears the framebuffer ---------------------------------------------------------------------
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );


    // Starts using the given GPU Program ---------------------------------------------------------
    m_GPUProgramSSAO.bind();
    {
        setupTexturesInUnit(m_RenderTarget.getTextureColor0(),0);
        setupTexturesInUnit(m_textureNoise.getID(),1);
        setupTexturesInUnit(m_RenderTarget.getTextureDepth(),2);
        setupTexturesInUnit(m_RenderTarget.getTextureColor1(),3);

        glm::mat4 inv;
        inv = glm::inverse(m_mtxCameraProj);

        m_paramsSSAO.sendDataToGPU(m_mtxCameraProj);

        m_MeshScreen.draw();

        /*
        // Setup the Texture to be used in unit 1 - shadow
        setupTexturesInUnit( m_RenderTarget.getTextureDepth(), 1 );

        // Sends the uniforms var related to the scene from the CPU to the GPU --------------------
        m_paramsSceneMap.sendSceneDataToGPU( m_mtxCameraProjView, m_lightProp, m_materialProp
                                                      , m_vCameraPosition, m_vLightPosition, m_mtxLightProjView );


        // MARCUS
        {
            // Setup the Texture to be used in unit 0 - diffuse
            setupTexturesInUnit( m_textureCharacter.getID(), 0 );

            // Sends the uniforms var related to Marcus from the CPU to the GPU -----------------------
            m_paramsSceneMap.sendModelDataToGPU( m_mtxCharacterWorld );

            m_MeshCharacter.draw();
        }

        // GROUND
        {
            // Setup the Texture to be used in unit 0 - diffuse
            setupTexturesInUnit( m_textureGround.getID(), 0 );

            // Sends the uniforms var related to the ground from the CPU to the GPU -------------------
            m_paramsSceneMap.sendModelDataToGPU( m_mtxGroundWorld );

            m_MeshGround.draw();
        }
*/
    }
    // Stops using the GPU Program ----------------------------------------------------------------
    m_GPUProgramSSAO.unbind();
}

//====================================================================================================================================
void TPGLWindow::resizeEvent(QResizeEvent* /*_pEvent*/)
{
    if( isInitialized() )
    {
        m_RenderTarget.destroy();

        m_RenderTarget.create( this->width(), this->height(), GL_RGB, GL_RGB, GL_DEPTH_COMPONENT );
     }

    // Force the update of the perspective matrix
    updateMatrices();
}

//====================================================================================================================================
void TPGLWindow::updateMatrices()
{
    const glm::vec3 vRight  (1,0,0);
    const glm::vec3 vUp     (0,1,0);
    const glm::vec3 vFront  (0,0,-1);
    const glm::vec3 vCenter (0,0,-1);

    // CAMERA ------------------------------------------------------------------------------------------------
    {
        // Projection matrix is "simply" a perspective matrix, so use the right tool function from GLM
        glm::mat4 mtxCameraProjection       = glm::perspective(glm::radians(45.0f), (float)width()/height(), 0.1f, 200.f);

        // View matrix is built from the common "lookAt" paradigm, using the RH (Right-Handed) coordinate system
        glm::mat4 mtxCameraView     = glm::lookAtRH( m_vCameraPosition, vCenter, vUp );
//        glm::mat4 mtxCameraView     = glm::lookAtRH( m_vCameraPosition, m_vCameraPosition + vFront, vUp );

        m_mtxCameraView             = mtxCameraView;
        m_mtxCameraProj = glm::inverse( mtxCameraProjection);
        m_mtxCameraProjView         = mtxCameraProjection * mtxCameraView;
    }

    // LIGHT ------------------------------------------------------------------------------------------------
    {
        glm::mat4 mtxLightProjection= glm::ortho<float>(-30,30,-30,30,-20,100);
        glm::mat4 mtxLightView      = glm::lookAt( m_vLightPosition, glm::vec3(0,0,0), vUp );
        m_mtxLightProjView          = mtxLightProjection * mtxLightView;
    }

    // MARCUS ------------------------------------------------------------------------------------------------
    {
        // Finally, build the World matrix for the model taking into account its translate and orientation
        glm::mat4 mtxWorld          = glm::translate(glm::mat4(1.0f), m_vObjectTranslate);
        mtxWorld                    = glm::rotate(mtxWorld, m_vObjectEulerAngles.y, vUp );
        m_mtxCharacterWorld         = glm::rotate(mtxWorld, glm::radians(-90.0f), vRight);      // fixup for data loaded from OBJ with Z up
    }

    // GROUND ------------------------------------------------------------------------------------------------
    {
        glm::mat4 mtxWorld          = glm::translate(glm::mat4(1.0f), m_vObjectTranslate);
        mtxWorld                    = glm::rotate(mtxWorld, m_vObjectEulerAngles.y, vUp);
        m_mtxGroundWorld            = mtxWorld;
    }


    if( m_bDebugShadowMap )
    {
        glm::mat4 mtxSwap   = m_mtxCameraProjView;
        m_mtxCameraProjView = m_mtxLightProjView;
        m_mtxLightProjView  = mtxSwap;
    }
}


//====================================================================================================================================
void TPGLWindow::createMeshes()
{
    // Creates a Ground Mesh - rendered as triangles !
    {
        GLfloat fSize = 100.f;

        // triangle strips - remember the system is right handed, so Z is "backwards"
        const GLfloat afPositions[] =   { -fSize, 0.0f, -fSize
                                        , -fSize, 0.0f,  fSize
                                        ,  fSize, 0.0f, -fSize
                                        ,  fSize, 0.0f,  fSize
                                        };
        const GLfloat afNormals[] =     { 0.0f,  1.0f, 0.0f
                                        , 0.0f,  1.0f, 0.0f
                                        , 0.0f,  1.0f, 0.0f
                                        , 0.0f,  1.0f, 0.0f
                                        };


        const GLfloat afUVs[] = { 0.0f, 1.0f
                                , 0.0f, 0.0f
                                , 1.0f, 1.0f
                                , 1.0f, 0.0f
                                };

        m_MeshGround.createFrom( GL_TRIANGLE_STRIP, 4, (glm::vec3*) afPositions, (glm::vec3*) afNormals, (glm::vec2*) afUVs, 0, 0 );
    }

    // Creates a Character from the OBJ file
    {
        std::vector< glm::vec3 > aVertices;
        std::vector< glm::vec2 > aUVs;
        std::vector< glm::vec3 > aNormals;

        aVertices.reserve(35000);
        aUVs.reserve(35000);
        aNormals.reserve(35000);

        const bool bOk = loadOBJ( PATH_TO_DATA "Marcus/Marcus.obj", aVertices, aUVs, aNormals );
        ASSERT( bOk, "Could not load the OBJ file %s\n", PATH_TO_DATA "Marcus/Marcus.obj" );

        glm::vec3 min, max;
        computeAxisAlignedBoundingBox( aVertices, min, max );


        m_vCameraPosition  = glm::vec3( 0, max.y - min.y, s_fCameraZInit );

        m_MeshCharacter.createFrom( GL_TRIANGLES, aVertices.size(), & aVertices[0], & aNormals[0], & aUVs[0], 0, 0 );
    }

    //blit
    // Creates the Screen Mesh - used to blit the FBO texture onto the screen
    {
        const GLfloat afPositions[] =   { -1.0f,  1.0f, 0.0f
                                        , -1.0f, -1.0f, 0.0f
                                        , 1.0f,  1.0f, 0.0f
                                        , 1.0f, -1.0f, 0.0f
                                        };

        const GLfloat afUVs[] = { 0.0f, 1.0f
                                , 0.0f, 0.0f
                                , 1.0f, 1.0f
                                , 1.0f, 0.0f
                                };

        m_MeshScreen.createFrom( GL_TRIANGLE_STRIP, 4, (glm::vec3*) afPositions, 0, (glm::vec2*) afUVs, 0, 0 );
    }
}

//====================================================================================================================================
void TPGLWindow::destroyMeshes()
{
    m_MeshScreen.destroy();

    m_MeshCharacter.destroy();

    m_MeshGround.destroy();
}

//====================================================================================================================================
void TPGLWindow::createTextures()
{
//    glEnable(GL_TEXTURE_2D);
    m_textureCharacter.createFrom( PATH_TO_DATA "/Marcus/Marcus_full_D.tga" );
    m_textureGround.createFrom( PATH_TO_DATA "/earth.png" );
    m_textureNoise.createFrom( PATH_TO_DATA "/noise_texture.png");
}

//====================================================================================================================================
void TPGLWindow::destroyTextures()
{
    m_textureCharacter.destroy();
    m_textureGround.destroy();
    m_textureNoise.destroy();
}

//====================================================================================================================================
void TPGLWindow::setupTexturesInUnit( GLuint _iTextureID, GLuint _iTextureUnit )
{
    // Specify on which texture unit the uniform texture sampler must be bound
    // Activates the texture unit 0
    glActiveTexture( GL_TEXTURE0 + _iTextureUnit );
    //
    glBindTexture( GL_TEXTURE_2D, _iTextureID );
}


//====================================================================================================================================
void TPGLWindow::createGPUPrograms()
{
    // Create the PHONG TEXTUREDprogram from files
    m_GPUProgramSceneMap.createFromFiles( "VS_scene_map.glsl", "", "FS_scene_map.glsl" );
    // Get uniforms locations for PHONG TEXTURED shaders
    m_paramsSceneMap.buildFrom( m_GPUProgramSceneMap );

    // Create the SHADOW MAP program from files
    m_GPUProgramSSAO.createFromFiles( "VS_SSAO.glsl", "", "FS_SSAO.glsl" );
    // Get uniforms locations for SHADOW MAP shaders
    m_paramsSSAO.buildFrom( m_GPUProgramSSAO );
}

//====================================================================================================================================
void TPGLWindow::destroyGPUPrograms()
{
    m_GPUProgramSSAO.destroy();

    m_GPUProgramSceneMap.destroy();
}

//====================================================================================================================================
void TPGLWindow::buildSceneMap()
{
    // Draws offscreen, into a FBO with only a Depth Texture attached ! -------------------------------
    m_RenderTarget.bind();
    {
        // Specify the depth buffer clear color -------------------------------------------------------
        glClearDepth( 1.f );

        // Clears the framebuffer - depth buffer only -------------------------------------------------
        glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Starts using the given GPU Program ---------------------------------------------------------
        m_GPUProgramSceneMap.bind();
        {
            // Sends the uniforms var from the CPU to the GPU -----------------------------------------
            //m_paramsSSAO.sendDataToGPU( m_mtxCharacterWorld, m_mtxCameraProjView );


            // Sends the uniforms var related to the scene from the CPU to the GPU --------------------
            m_paramsSceneMap.sendSceneDataToGPU( m_mtxCameraProjView, m_lightProp, m_materialProp
                                                          , m_vCameraPosition, m_vLightPosition, m_mtxLightProjView );


            // MARCUS
            {
                // Setup the Texture to be used in unit 0 - diffuse
                setupTexturesInUnit( m_textureCharacter.getID(), 0 );

                // Sends the uniforms var related to Marcus from the CPU to the GPU -----------------------
                m_paramsSceneMap.sendModelDataToGPU( m_mtxCharacterWorld );

                m_MeshCharacter.draw();
            }

            // GROUND
            {
                // Setup the Texture to be used in unit 0 - diffuse
                setupTexturesInUnit( m_textureGround.getID(), 0 );

                // Sends the uniforms var related to the ground from the CPU to the GPU -------------------
                m_paramsSceneMap.sendModelDataToGPU( m_mtxGroundWorld );

                m_MeshGround.draw();
            }


            // m_MeshGround.draw();
//            m_MeshCharacter.draw();

        }
        // Stops using the GPU Program ----------------------------------------------------------------
        m_GPUProgramSceneMap.unbind();
    }
    m_RenderTarget.unbind();
}


//====================================================================================================================================
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;

#ifdef Q_OS_MAC  // what about Linux ?
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
#endif
    format.setSamples(16);
    format.setDepthBufferSize(24);

    TPGLWindow window;
    window.setFormat(format);
    window.resize(800, 600);
    window.show();
//    window.showFullScreen();

    return app.exec();
}
