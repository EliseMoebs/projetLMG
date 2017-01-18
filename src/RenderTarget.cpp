#include "RenderTarget.h"

#include <QImage>

//====================================================================================================================================
RenderTarget::RenderTarget()
    :   m_iWidth            ( 0 )
    ,   m_iHeight           ( 0 )
    ,   m_iFBO              ( 0 )
    ,   m_iAttachmentColor0 ( 0 )
    ,   m_iAttachmentDepth  ( 0 )
{}
//====================================================================================================================================
RenderTarget::~RenderTarget()
{
    ASSERT( 0 == m_iFBO ,            "m_iFBO should be 0 (here it is '%d') - have you called destroy() ?.\n", m_iFBO );
    ASSERT( 0 == m_iAttachmentColor0,"m_iAttachmentColor0 should be 0 (here it is '%d') - have you called destroy() ?.\n", m_iAttachmentColor0 );
    ASSERT( 0 == m_iAttachmentDepth, "m_iAttachmentDepth should be 0 (here it is '%d') - have you called destroy() ?.\n", m_iAttachmentDepth );

}
//====================================================================================================================================
bool RenderTarget::create( GLsizei _iWidth, GLsizei _iHeight, GLint _iColor0Format, GLint _iDepthFormat )
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    ASSERT( 0 == m_iFBO ,            "m_iFBO should be 0 (here it is '%d') - have you already called create() ?.\n", m_iFBO );
    ASSERT( 0 == m_iAttachmentColor0,"m_iAttachmentColor0 should be 0 (here it is '%d') - have you already called create() ?.\n", m_iAttachmentColor0 );
    ASSERT( 0 == m_iAttachmentDepth, "m_iAttachmentDepth should be 0 (here it is '%d') - have you already called create() ?.\n", m_iAttachmentDepth );
    //--------------------------------------------------------------------------------------------------------------------

    // required to have the OpenGL functions working - this is because of our use of OpenGL with Qt
    initializeOpenGLFunctions();

    m_iWidth    = _iWidth;
    m_iHeight   = _iHeight;

    // Creates the Frame Buffer Object - FBO - that will be used as a replacement for the default Frame Buffer -----------------
    glGenFramebuffers( 1, & m_iFBO );
    glBindFramebuffer( GL_FRAMEBUFFER, m_iFBO );
    {
        int iDrawBuffersCount = 0;
        GLenum aDrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

        if( 0 != _iColor0Format )
        {
            iDrawBuffersCount++;

            // Creates the Texture Object that will be used as color buffer ------------------------------------------------------------
            glGenTextures( 1 , & m_iAttachmentColor0 );

            // "Bind" the created texture : all future texture functions will modify this texture
            glBindTexture( GL_TEXTURE_2D, m_iAttachmentColor0 );

            // Put an empty image to OpenGL - see the last "0"
            glTexImage2D( GL_TEXTURE_2D, 0, _iColor0Format, _iWidth, _iHeight, 0 , _iColor0Format, GL_UNSIGNED_BYTE, 0 );

            // Required - but no need for a linear filtering
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

            // Set "m_iAttachmentColor0" as the color_buffer_0
            glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_iAttachmentColor0, 0 );
        }

        if( 0 != _iDepthFormat )
        {
            // Creates the Render Buffer that will be used as Z buffer -----------------------------------------------------------------
//            glGenRenderbuffers( 1, & m_iAttachmentDepth );
//            glBindRenderbuffer( GL_RENDERBUFFER, m_iAttachmentDepth );
//            glRenderbufferStorage( GL_RENDERBUFFER, _iDepthFormat, _iWidth, _iHeight );
//            glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_iAttachmentDepth );

            // Creates the Texture Object that will be used as depth buffer ------------------------------------------------------------
            glGenTextures( 1, &m_iAttachmentDepth);

            // "Bind" the created texture : all future texture functions will modify this texture
            glBindTexture( GL_TEXTURE_2D, m_iAttachmentDepth);


            // Put an empty image to OpenGL - see the last "0"
            glTexImage2D( GL_TEXTURE_2D, 0,_iDepthFormat, _iWidth, _iHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );

            // Required - but no need for a linear filtering
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

            // Set "m_iAttachmentDepth" as the depth_buffer
            glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_iAttachmentDepth, 0 );
        }

        // Specify list of color buffers to be drawn into --------------------------------------------------------------------------
        glDrawBuffers( iDrawBuffersCount, aDrawBuffers );
    }
    //
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );

    // Make sure the FBO has been created properly
    ASSERT( glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE, "FBO %d creation is incomplete!\n ", m_iFBO );

    return glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE;
}
//====================================================================================================================================
void RenderTarget::destroy()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    ASSERT( 0 != m_iFBO ,            "m_iFBO should not be 0 (here it is '%d') - have you called create() ?.\n", m_iFBO );
//    ASSERT( 0 != m_iAttachmentColor0,"m_iAttachmentColor0 not should be 0 (here it is '%d') - have you called create() ?.\n", m_iAttachmentColor0 );
//    ASSERT( 0 != m_iAttachmentDepth, "m_iAttachmentDepth not should be 0 (here it is '%d') - have you called create() ?.\n", m_iAttachmentDepth );
    //--------------------------------------------------------------------------------------------------------------------

    if( 0 != m_iAttachmentDepth )
    {
        glDeleteTextures( 1 , & m_iAttachmentDepth );
//        glDeleteRenderbuffers( 1, & m_iAttachmentDepth );
        m_iAttachmentDepth = 0;
    }


    if( 0 != m_iAttachmentColor0 )
    {
        glDeleteTextures( 1 , & m_iAttachmentColor0 );
        m_iAttachmentColor0 = 0;
    }

    glDeleteFramebuffers( 1, & m_iFBO );
    m_iFBO = 0;
}
//====================================================================================================================================
void RenderTarget::bind()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    ASSERT( 0 != m_iFBO , "m_iFBO should not be 0 (here it is '%d') - have you called create() ?.\n", m_iFBO );
    //--------------------------------------------------------------------------------------------------------------------

    glBindFramebuffer( GL_FRAMEBUFFER, m_iFBO );
    glViewport( 0, 0, m_iWidth, m_iHeight );
}
//====================================================================================================================================
void RenderTarget::unbind()
{
    glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
//====================================================================================================================================
void RenderTarget::writeToFile( const std::string& _rstrFilePath )
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    ASSERT( 0 != m_iFBO , "m_iFBO should not be 0 (here it is '%d') - have you called create() ?.\n", m_iFBO );
    //--------------------------------------------------------------------------------------------------------------------

    // this is called Pixel Transfer operation: http://www.opengl.org/wiki/Pixel_Transfer
    const int iPixelCount = m_iWidth * m_iHeight;

    uchar* aPixels      = new uchar[ iPixelCount * 4 ];


    glBindFramebuffer( GL_FRAMEBUFFER, m_iFBO );
    {
        if( 0 != m_iAttachmentColor0 )
        {
            glReadPixels( 0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, aPixels );
        }
        else
        if( 0 != m_iAttachmentDepth )
        {
            float* aFloatPixels      = new float[ iPixelCount ];

            glReadPixels( 0, 0, m_iWidth, m_iHeight, GL_DEPTH_COMPONENT, GL_FLOAT, aFloatPixels );

            // Copy every pixel from aPixels to aImgPixels,
            // and converting the depth float value from {0.f; 1.f} to a ubyte rgb component {0;255}
            for( int j = 0 ; j < m_iHeight; j++ )
            {
                for( int i = 0 ; i < m_iWidth; i++ )
                {
                    const int iSrcPixel = j * m_iWidth  + i;
                    const int iDstPixel = 4 * ( j * m_iWidth  + i);

                    aPixels[ iDstPixel + 0 ] = static_cast< uchar > ( aFloatPixels[ iSrcPixel ] * 255 );  // R
                    aPixels[ iDstPixel + 1 ] = static_cast< uchar > ( aFloatPixels[ iSrcPixel ] * 255 );  // G
                    aPixels[ iDstPixel + 2 ] = static_cast< uchar > ( aFloatPixels[ iSrcPixel ] * 255 );  // B
                    aPixels[ iDstPixel + 3 ] = static_cast< uchar > ( aFloatPixels[ iSrcPixel ] * 255 );  // A
                }
            }

            delete [] aFloatPixels;
        }
    }
    glBindFramebuffer( GL_FRAMEBUFFER,  0 );

    QImage qImage = QImage( aPixels, m_iWidth, m_iHeight, QImage::Format_ARGB32 );

    // flip the image vertically, cause OpenGL has them upside down!
    qImage = qImage.mirrored(false, true);
    qImage = qImage.rgbSwapped();

    qImage.save( _rstrFilePath.c_str() );

    // delete the arrays AFTER qImage.save(...) !
    delete [] aPixels;

}


