#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "common.h"

#include <QOpenGLFunctions_3_3_Core>



class Texture : protected QOpenGLFunctions_3_3_Core
{
public:
    /// CTOR
    Texture();
    /// DTOR
    virtual ~Texture();

    /// Returns the OpenGL id of this Texture
    inline GLuint   getID() const;

    /// Setup this Texture in the given TextureUnit
    void            setupInUnit( GLuint _iTextureUnit );

    /// Creates a Texture from the given file, with the given params
    bool            createFrom( const char* _strFilepath, GLint _iMinFilter = GL_LINEAR, GLint _iMaxFilter = GL_LINEAR, GLint _iWrapS = GL_REPEAT , GLint _iWrapT = GL_REPEAT ) ;

    /// Destroys this Texture
    void            destroy();

private:

    GLuint          m_iTextureID;       ///< OpenGL ID for TextureObject
};

//====================================================================================================================================
GLuint Texture::getID() const
{
    return m_iTextureID;
}


#endif // __TEXTURE_H__

