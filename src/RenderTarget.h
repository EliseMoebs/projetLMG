#ifndef __RENDERTARGET_H__
#define __RENDERTARGET_H__

#include "common.h"

#include <QOpenGLFunctions_3_3_Core>



class RenderTarget : protected QOpenGLFunctions_3_3_Core
{
public:
    /// CTOR
    RenderTarget();
    /// DTOR
    ~RenderTarget(); // not virtual - do not inherit this class !

    /// Creates the RenderTarget and its internal data for COLOR_0 and DEPTH ( if _iColor0Format is 0 skips the COLOR_0, same for _iDepthFormat and DEPTH )
    bool            create( GLsizei _iWidth, GLsizei _iHeight , GLint _iColor0Format = GL_RGB, GLint _iDepthFormat = GL_DEPTH_COMPONENT );

    /// Destroys the internal data
    void            destroy();

    /// Binds the RenderTarget - after that, any draw call will end up in this RenderTarget
    void            bind();

    /// Unbinds the RenderTarget - after that, any draw call will be back again in the default FrameBuffer
    void            unbind();

    /// Returns the id of the Texture Object used as COLOR_O
    inline GLuint   getTextureColor0() const;

    /// Returns the id of the Texture Object used as Depth
    inline GLuint   getTextureDepth() const;

    /// Writes the COLOR_0 buffer to a file - for debugging purpose
    void            writeToFile( const std::string& _rstrFilePath );

private:

    // NB : this will only allow us to use COLOR_0 and DEPTH

    GLsizei                 m_iWidth;               ///<
    GLsizei                 m_iHeight;              ///<
    GLuint                  m_iFBO;                 ///< OpenGL ID for Frame Buffer Object
    GLuint                  m_iAttachmentColor0;    ///< OpenGL ID for Texture Object - used as Color Buffer on the FBO
    GLuint                  m_iAttachmentDepth;     ///< OpenGL ID for Render Buffer - used as Z Buffer on the FBO
};

//====================================================================================================================================
inline GLuint RenderTarget::getTextureColor0() const
{
    return m_iAttachmentColor0;
}
//====================================================================================================================================
inline GLuint RenderTarget::getTextureDepth() const
{
    return m_iAttachmentDepth;
}

#endif // __RENDERTARGET_H__

