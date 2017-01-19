#include "Texture.h"

#include <QImage>

//====================================================================================================================================
Texture::Texture()
:   m_iTextureID         ( 0 )
{
}
//====================================================================================================================================
Texture::~Texture()
{
    ASSERT( 0 == m_iTextureID , "m_iTextureID should be 0 (here it is '%d') - have you called destroy() ?.\n", m_iTextureID );
}

//====================================================================================================================================
void Texture::setupInUnit( GLuint _iTextureUnit )
{
    // Specify on which texture unit the uniform texture sampler must be bound
    // Activates the texture unit 0
    glActiveTexture( GL_TEXTURE0 + _iTextureUnit );
    //
    glBindTexture( GL_TEXTURE_2D, m_iTextureID );
}

//====================================================================================================================================
bool Texture::createFrom( const char* _strFilepath, GLint _iMinFilter, GLint _iMaxFilter, GLint _iWrapS, GLint _iWrapT )
{
    // required to have the OpenGL functions working - this is because of our use of OpenGL with Qt
    initializeOpenGLFunctions();

    QImage img( _strFilepath );
    QImage img0 = img.convertToFormat(QImage::Format_RGB888,Qt::NoOpaqueDetection);

    unsigned char* pDataImage0 = img0.scanLine(0);

    // Creates the Texture object
    glGenTextures( 1, & m_iTextureID );

    glBindTexture( GL_TEXTURE_2D, m_iTextureID );
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _iMinFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _iMaxFilter );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _iWrapS );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _iWrapT );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, img0.width(), img0.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, pDataImage0 );
    }
    glBindTexture( GL_TEXTURE_2D, 0 );

    return true;
}

//====================================================================================================================================
void Texture::destroy()
{
    glDeleteTextures( 1, & m_iTextureID );
    m_iTextureID = 0;
}
