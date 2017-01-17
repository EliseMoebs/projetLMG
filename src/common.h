#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <vector>

#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec2.hpp>                 // glm::vec4
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective, glm::lookAtRH
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr
#include <glm/gtc/matrix_inverse.hpp>


#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7

//====================================================================================================================================
// DATA related

#define PATH_TO_DATA "data/"

//====================================================================================================================================
// ERROR & DEBUG related

#define DEBUG

// DEBUG_BREAK
#ifdef  WIN32
#   define  DEBUG_BREAK()    __debugbreak()
#else
#   define  DEBUG_BREAK()    asm("int $3")
#endif


// LOG
#define LOG( _message, ... )  fprintf( stdout, (_message), __VA_ARGS__ );fflush( stdout )

// ASSERT
#ifdef DEBUG
#   define ASSERT(_assert, ...)   \
        if( !(_assert) ) {  \
            LOG( "Assertion failed !\n.FILE = '%s'\n.LINE='%d'\n.FUNCTION='%s'\n", __FILE__ , __LINE__, __FUNCTION__);\
            LOG( __VA_ARGS__ );\
            DEBUG_BREAK();\
            assert(_assert); \
        }
#else
#   define ASSERT(_assert, ...)
#endif


//====================================================================================================================================
std::string readFileSrc( const std::string& _rstrFilePath );
//====================================================================================================================================
void computeAxisAlignedBoundingBox( std::vector< glm::vec3 > _aVertices, glm::vec3& _rOutMin, glm::vec3& _rOutMax );
//====================================================================================================================================
bool loadOBJ(   const char* _strPath
            ,   std::vector< glm::vec3 >& _rOutVertices
            ,   std::vector< glm::vec2 >& _rOutUVs
            ,   std::vector< glm::vec3 >& _rOutNormals );

#endif // COMMON_H

