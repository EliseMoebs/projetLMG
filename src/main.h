#ifndef __MAIN_H__
#define __MAIN_H__

#include "openglwindow.h"

#include "common.h"
#include "GPUProgram.h"
#include "GPUParams.h"
#include "Mesh.h"
#include "Texture.h"
#include "RenderTarget.h"


#include <QTime>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/QKeyEvent>


class Model
{

};

//------------------------------------------------------------------------------------

class TPGLWindow : public OpenGLWindow
{
public:
    TPGLWindow();
    virtual ~TPGLWindow();

    void initialize();

    void update();

    void render();

private:


    /// Handles key press event, and move the 3D object
    void keyPressEvent(QKeyEvent *);

    /// Handles resize event
    void resizeEvent(QResizeEvent*);

    ///
    void mouseMoveEvent(QMouseEvent * _pEvent );

    //------------------------------------------------------------------------------------
    /// Update the CameraProjection, CameraView, ObjectWorld matrices
    void updateMatrices();

    //------------------------------------------------------------------------------------
    /// Creates the VertexBufferObject that contains the geometry position
    void createMeshes();

    /// Destroys the VertexArrayObject created earlier
    void destroyMeshes();

    //------------------------------------------------------------------------------------
    /// Creates the Texture object from a texture file
    void createTextures();

    /// Destroys all Texture objects
    void destroyTextures();

    /// Bind the textures to their texture unit
    void setupTexturesInUnit( GLuint _iTextureID, GLuint _iTextureUnit );

    //------------------------------------------------------------------------------------
    /// Creates the GPUPrograms, and build their associated parameters by retrieving uniform locations
    void createGPUPrograms();

    /// Destroys the GPUPrograms
    void destroyGPUPrograms();

    //------------------------------------------------------------------------------------
    /// Builds the shadow map
    void buildSceneMap();


    static const float      s_fCameraZInit;

    //----------------------------------------------------------------------------------------------------------------
    bool                    m_bAlphaBlend;                      ///< Use Alpha Blending ?
    bool                    m_bDebugShadowMap;                  ///<
    QTime       			m_timer;                            ///< Time used to get elapsed time between 2 frames

    //----------------------------------------------------------------------------------------------------------------
    RenderTarget            m_RenderTarget;                     ///< FBO with a Depth Texture that we will render into from the light POV, and use to get the shadowed area

    //----------------------------------------------------------------------------------------------------------------
    GPUProgram              m_GPUProgramSSAO;              ///<
    GPUProgram              m_GPUProgramSceneMap;          ///<

    GPUParamsPhongTextured  m_paramsSceneMap;              ///<
    GPUParamsSSAO      m_paramsSSAO;                  ///<

    //----------------------------------------------------------------------------------------------------------------
    MaterialProperties      m_materialProp;                     ///< Material properties

    Mesh                    m_MeshGround;                       ///< Mesh used for Marcus
    Mesh                    m_MeshCharacter;                    ///< Mesh used for the ground
    Mesh                    m_MeshScreen;                       ///< Mesh used to blit the color texture onto the screen

    Texture                 m_textureCharacter;                 ///< Texture used for diffuse texture of Marcus
    Texture                 m_textureGround;                    ///< Texture used for diffuse texture of the ground
    Texture                 m_textureNoise;                     ///< Texture used for SSAO post-process

    //----------------------------------------------------------------------------------------------------------------
    glm::vec3               m_vCameraPosition;                  ///< Camera position - in World Space
    glm::mat4   			m_mtxCameraProjView;                ///< Matrix transform, for camera  world -> view -> projection coordinates


    //----------------------------------------------------------------------------------------------------------------
    LightProperties         m_lightProp;                        ///< Light properties
    glm::vec3               m_vLightPosition;                   ///< Light position  - in World Space

    glm::mat4   			m_mtxLightProjView;                 ///< Matrix transform, for object -> projection coordinates

    glm::mat4   			m_mtxCharacterWorld;                ///< Matrix transform, for object -> world coordinates
    glm::mat4   			m_mtxGroundWorld;                   ///< Matrix transform, for object -> world coordinates

    glm::vec3   			m_vObjectTranslate;                 ///< Store the 3D object translate component
    glm::vec3   			m_vObjectEulerAngles;               ///< Store the 3D object orientation as euler angles
};
//====================================================================================================================================

//====================================================================================================================================
void TPGLWindow::keyPressEvent(QKeyEvent* _pEvent)
{
    if(QEvent::KeyPress == _pEvent->type() )
    {
//        float   fTimeElapsed= (float) m_timer.elapsed();

        float   fMoveSpeed          = 0.25f;// * fTimeElapsed;
        bool    bHasObjectMoved     = false;


        // handle key press to move the 3D object
        switch( _pEvent->key() )
        {
        case Qt::Key_B:
            m_bAlphaBlend = !m_bAlphaBlend;
            break;
        case Qt::Key_D:
            m_bDebugShadowMap = !m_bDebugShadowMap;
            break;

        case Qt::Key_P:
            m_RenderTarget.writeToFile( "FBO.png" );
            break;

        case Qt::Key_R:
            m_vObjectTranslate = glm::vec3(0, 0, 0);
            m_vCameraPosition  = glm::vec3(0,0, s_fCameraZInit);
            bHasObjectMoved = true;
            break;

        case Qt::Key_Minus:
//            m_vObjectTranslate -= glm::vec3(0, 0, 1) * fMoveSpeed;
            m_vCameraPosition  += glm::vec3(0, 0, 1) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Plus:
//            m_vObjectTranslate += glm::vec3(0, 0, 1) * fMoveSpeed;
            m_vCameraPosition  -= glm::vec3(0, 0, 1) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Left:
//            m_vObjectTranslate -= glm::vec3(1, 0, 0) * fMoveSpeed;
            m_vCameraPosition  -= glm::vec3(1, 0, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Right:
//            m_vObjectTranslate += glm::vec3(1, 0, 0) * fMoveSpeed;
            m_vCameraPosition  += glm::vec3(1, 0, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Up:
//            m_vObjectTranslate += glm::vec3(0, 1, 0) * fMoveSpeed;
            m_vCameraPosition  += glm::vec3(0, 1, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Down:
//            m_vObjectTranslate -= glm::vec3(0, 1, 0) * fMoveSpeed;
            m_vCameraPosition  -= glm::vec3(0, 1, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Space:
            m_vObjectEulerAngles -= glm::vec3(0, glm::radians(1.0f), 0);
            bHasObjectMoved = true;
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            break;

        case Qt::Key_Escape:
            QCoreApplication::exit(0);
            break;
        }
    }
}



//====================================================================================================================================
void TPGLWindow::mouseMoveEvent(QMouseEvent * _pEvent )
{
    if( Qt::LeftButton == _pEvent->buttons() )
    {
        // Do something if you feel like it... :)
    }
}

#endif // __MAIN_H__
