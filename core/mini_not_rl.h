/*
Since this is derived from raylib, this has a dual licese zlib/libpng and GNU LGPL 2.0

*   LICENSE: zlib/libpng
*
*   Copyright (c) 2014-2018 Ramon Santamaria (@raysan5)
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
* 


Silent Radiance wifi broadcast for digital silent disco.
Copyright (C) 2017-2019 Hibbard M. Engler

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License   
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef NOT_RL_H
#define NOT_RL_H 1


       // EGL library
#if defined(__APPLE__)
#include "KHR/khrplatform.h"
#include "EGL/egl.h"
    #include <OpenGLES/ES3/gl.h>         // OpenGL ES 2.0 library
    #include <OpenGLES/ES3/glext.h>       // OpenGL ES 2.0 extensions library
#else
#ifndef _WIN32
#include <EGL/egl.h>
#endif
    #include <GLES2/gl2.h>          // OpenGL ES 2.0 library
    #include <GLES2/gl2ext.h>       // OpenGL ES 2.0 extensions library

#endif



#define NOT_RL_MAX_SHADER_LOCATIONS        32 
#define NOT_RL_MAX_MATERIAL_MAPS           12      // Maximum number of texture maps stored in shader struc


#define NOT_RL_MAX_QUADS_BATCH 1024





#ifdef __cplusplus
    #define NOT_RL_RMEXTERN extern "C"     // Functions visible from other files (no name mangling of functions in C++)
#else
    #define NOT_RL_RMEXTERN extern         // Functions visible from other files
#endif

    #define NOT_RL_RMDEF NOT_RL_RMEXTERN


// Matrix modes (equivalent to OpenGL)
#define NOT_RL_MODELVIEW                    0x1700      // GL_MODELVIEW
#define NOT_RL_PROJECTION                   0x1701      // GL_PROJECTION
#define NOT_RL_TEXTURE                      0x1702      // GL_TEXTURE


// Primitive assembly draw modes
#define NOT_RL_LINES                        0x0001      // GL_LINES
#define NOT_RL_TRIANGLES                    0x0004      // GL_TRIANGLES
#define NOT_RL_QUADS                        0x0007      // GL_QUADS

#define NOT_RL_MAX_DRAWS_BY_TEXTURE      256 


// Head Mounted Display devices
typedef enum {
    NOT_RL_HMD_DEFAULT_DEVICE = 0,  
    NOT_RL_HMD_OCULUS_RIFT_DK2,
    NOT_RL_HMD_OCULUS_RIFT_CV1,
    NOT_RL_HMD_OCULUS_GO,
    NOT_RL_HMD_VALVE_HTC_VIVE,
    NOT_RL_HMD_SONY_PSVR
} not_rl_VrDeviceType;
    
typedef enum { not_rl_false, not_rl_true } not_rl_bool;

// Shader location point type
typedef enum {
    NOT_RL_LOC_VERTEX_POSITION = 0,
    NOT_RL_LOC_VERTEX_TEXCOORD01,
    NOT_RL_LOC_VERTEX_TEXCOORD02,
    NOT_RL_LOC_VERTEX_NORMAL,
    NOT_RL_LOC_VERTEX_TANGENT,
    NOT_RL_LOC_VERTEX_COLOR,
    NOT_RL_LOC_MATRIX_MVP,
    NOT_RL_LOC_MATRIX_MODEL,
    NOT_RL_LOC_MATRIX_VIEW,
    NOT_RL_LOC_MATRIX_PROJECTION,
    NOT_RL_LOC_VECTOR_VIEW,
    NOT_RL_LOC_COLOR_DIFFUSE,
    NOT_RL_LOC_COLOR_SPECULAR,
    NOT_RL_LOC_COLOR_AMBIENT,
    NOT_RL_LOC_MAP_ALBEDO,          // NOT_RL_LOC_MAP_DIFFUSE
    NOT_RL_LOC_MAP_METALNESS,       // NOT_RL_LOC_MAP_SPECULAR
    NOT_RL_LOC_MAP_NORMAL,
    NOT_RL_LOC_MAP_ROUGHNESS,
    NOT_RL_LOC_MAP_OCCUSION,
    NOT_RL_LOC_MAP_EMISSION,
    NOT_RL_LOC_MAP_HEIGHT,
    NOT_RL_LOC_MAP_CUBEMAP,
    NOT_RL_LOC_MAP_IRRADIANCE,
    NOT_RL_LOC_MAP_PREFILTER,
    NOT_RL_LOC_MAP_BRDF
} not_rl_ShaderLocationIndex;

#define NOT_RL_LOC_MAP_DIFFUSE      NOT_RL_LOC_MAP_ALBEDO
#define NOT_RL_LOC_MAP_SPECULAR     NOT_RL_LOC_MAP_METALNESS


   // Texture formats (support depends on OpenGL version)
    typedef enum {
        NOT_RL_UNCOMPRESSED_GRAYSCALE = 1,     // 8 bit per pixel (no alpha)
        NOT_RL_UNCOMPRESSED_GRAY_ALPHA,
        NOT_RL_UNCOMPRESSED_R5G6B5,            // 16 bpp
        NOT_RL_UNCOMPRESSED_R8G8B8,            // 24 bpp
        NOT_RL_UNCOMPRESSED_R5G5B5A1,          // 16 bpp (1 bit alpha)
        NOT_RL_UNCOMPRESSED_R4G4B4A4,          // 16 bpp (4 bit alpha)
        NOT_RL_UNCOMPRESSED_R8G8B8A8,          // 32 bpp
        NOT_RL_UNCOMPRESSED_R32,               // 32 bpp (1 channel - float)
        NOT_RL_UNCOMPRESSED_R32G32B32,         // 32*3 bpp (3 channels - float)
        NOT_RL_UNCOMPRESSED_R32G32B32A32,      // 32*4 bpp (4 channels - float)
        NOT_RL_COMPRESSED_DXT1_RGB,            // 4 bpp (no alpha)
        NOT_RL_COMPRESSED_DXT1_RGBA,           // 4 bpp (1 bit alpha)
        NOT_RL_COMPRESSED_DXT3_RGBA,           // 8 bpp
        NOT_RL_COMPRESSED_DXT5_RGBA,           // 8 bpp
        NOT_RL_COMPRESSED_ETC1_RGB,            // 4 bpp
        NOT_RL_COMPRESSED_ETC2_RGB,            // 4 bpp
        NOT_RL_COMPRESSED_ETC2_EAC_RGBA,       // 8 bpp
        NOT_RL_COMPRESSED_PVRT_RGB,            // 4 bpp
        NOT_RL_COMPRESSED_PVRT_RGBA,           // 4 bpp
        NOT_RL_COMPRESSED_ASTC_4x4_RGBA,       // 8 bpp
        NOT_RL_COMPRESSED_ASTC_8x8_RGBA        // 2 bpp
    } not_rl_PixelFormat;



// Head-Mounted-Display device parameters
typedef struct not_rl_VrDeviceInfo {
    int hResolution;                // HMD horizontal resolution in pixels
    int vResolution;                // HMD vertical resolution in pixels
    float hScreenSize;              // HMD horizontal size in meters
    float vScreenSize;              // HMD vertical size in meters
    float vScreenCenter;            // HMD screen center in meters
    float eyeToScreenDistance;      // HMD distance between eye and display in meters
    float lensSeparationDistance;   // HMD lens separation distance in meters
    float interpupillaryDistance;   // HMD IPD (distance between pupils) in meters
    float lensDistortionValues[4];  // HMD lens distortion constant parameters
    float chromaAbCorrection[4];    // HMD chromatic aberration correction parameters
} not_rl_VrDeviceInfo;

typedef unsigned char not_rl_byte;

// from camera.h
   // Camera modes
    typedef enum {
        NOT_RL_CAMERA_CUSTOM = 0,
        NOT_RL_CAMERA_FREE,
        NOT_RL_CAMERA_ORBITAL, 
        NOT_RL_CAMERA_FIRST_PERSON,
        NOT_RL_CAMERA_THIRD_PERSON
    } not_nl_CameraMode;
                          
    // Vector2 type
    typedef struct not_rl_Vector2 {   
        float x;
        float y;
    } not_rl_Vector2;          
    
    // Vector3 type
    typedef struct not_rl_Vector3 {
        float x;
        float y;
        float z;
    } not_rl_Vector3;

    // Camera type, defines a camera position/orientation in 3d space
    typedef struct Camera {
        not_rl_Vector3 position;
        not_rl_Vector3 target;
        not_rl_Vector3 up;
        float fovy;
    } Camera;
    
    
   
    // Matrix type (OpenGL style 4x4 - right handed, column major)
    typedef struct not_rl_Matrix {
        float m0, m4, m8, m12;
        float m1, m5, m9, m13;
        float m2, m6, m10, m14;   
        float m3, m7, m11, m15;
    } not_rl_Matrix;


// Quaternion type
typedef struct not_rl_Quaternion {
    float x;
    float y;
    float z;
    float w;
} not_rl_Quaternion;
    

    // Color type, RGBA (32bit)
    typedef struct not_rl_Color {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    } not_rl_Color;
    
    // Rectangle type
    typedef struct not_rl_Rectangle {
        int x;
        int y;
        int width;
        int height;
    } not_rl_Rectangle;


// Image type, bpp always RGBA (32bit)
// NOTE: Data stored in CPU memory (RAM)
typedef struct not_rl_Image {
    void *data;             // Image raw data
    int width;              // Image base width
    int height;             // Image base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} not_rl_Image;

// Texture2D type
// NOTE: Data stored in GPU memory
typedef struct not_rl_Texture2D {
    unsigned int id;        // OpenGL texture id
    int width;              // Texture base width
    int height;             // Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    int format;             // Data format (PixelFormat type)
} not_rl_Texture2D;

// RenderTexture2D type, for texture rendering
typedef struct not_rl_RenderTexture2D {
    unsigned int id;        // OpenGL Framebuffer Object (FBO) id
    not_rl_Texture2D texture;      // Color buffer attachment texture
    not_rl_Texture2D depth;        // Depth buffer attachment texture
    #ifdef __APPLE__
    not_rl_Texture2D color;       // color render buffer
    #endif
} not_rl_RenderTexture2D;

// SpriteFont character info
typedef struct not_rl_CharInfo {
    int value;              // Character value (Unicode)
    not_rl_Rectangle rec;          // Character rectangle in sprite font
    int offsetX;            // Character offset X when drawing
    int offsetY;            // Character offset Y when drawing
    int advanceX;           // Character advance position X
} not_rl_CharInfo;

// SpriteFont type, includes texture and charSet array data
typedef struct not_rl_SpriteFont {
    not_rl_Texture2D texture;      // Font texture
    int baseSize;           // Base size (default chars height)
    int charsCount;         // Number of characters
    not_rl_CharInfo *chars;        // Characters info data
} not_rl_SpriteFont;


// Camera type, defines a camera position/orientation in 3d space
typedef struct not_rl_Camera {
    not_rl_Vector3 position;       // Camera position
    not_rl_Vector3 target;         // Camera target it looks-at
    not_rl_Vector3 up;             // Camera up vector (rotation over its axis)
    float fovy;             // Camera field-of-view apperture in Y (degrees)
} not_rl_Camera;

// Camera2D type, defines a 2d camera
typedef struct not_rl_Camera2D {
    not_rl_Vector2 offset;         // Camera offset (displacement from target)
    not_rl_Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default
} not_rl_Camera2D;

// Bounding box type
typedef struct not_rl_BoundingBox {
    not_rl_Vector3 min;            // Minimum vertex box-corner
    not_rl_Vector3 max;            // Maximum vertex box-corner
} not_rl_BoundingBox;

// Vertex data definning a mesh
// NOTE: Data stored in CPU memory (and GPU)
typedef struct not_rl_Mesh {
    int vertexCount;        // Number of vertices stored in arrays
    int triangleCount;      // Number of triangles stored (indexed or not)

    float *vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    float *texcoords2;      // Vertex second texture coordinates (useful for lightmaps) (shader-location = 5)
    float *normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
    float *tangents;        // Vertex tangents (XYZ - 3 components per vertex) (shader-location = 4)
    unsigned char *colors;  // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices;// Vertex indices (in case vertex data comes indexed)

    unsigned int vaoId;     // OpenGL Vertex Array Object id
    unsigned int vboId[7];  // OpenGL Vertex Buffer Objects id (7 types of vertex data)
} not_rl_Mesh;

// Shader type (generic)
typedef struct not_rl_Shader {
    unsigned int id;                // Shader program id
    int locs[NOT_RL_MAX_SHADER_LOCATIONS]; // Shader locations array
} not_rl_Shader;

// Material texture map
typedef struct not_rl_MaterialMap {
    not_rl_Texture2D texture;      // Material map texture
    not_rl_Color color;            // Material map color
    float value;            // Material map value
} not_rl_MaterialMap;

// Material type (generic)
typedef struct not_rl_Material {
    not_rl_Shader shader;          // Material shader
    not_rl_MaterialMap maps[NOT_RL_MAX_MATERIAL_MAPS]; // Material maps
    float *params;          // Material generic parameters (if required)
} not_rl_Material;

// Model type
typedef struct not_rl_Model {
    not_rl_Mesh mesh;              // Vertex data buffers (RAM and VRAM)
    not_rl_Matrix transform;       // Local transform matrix
    not_rl_Material material;      // Shader and textures data
} not_rl_Model;

// VR Stereo rendering configuration for simulator

typedef struct not_rl_VrStereoConfig {
    not_rl_RenderTexture2D stereoFbo;      // VR stereo rendering framebuffer
    not_rl_Shader distortionShader;        // VR stereo rendering distortion shader
    not_rl_Rectangle eyesViewport[2];      // VR stereo rendering eyes viewports
    not_rl_Matrix eyesProjection[2];       // VR stereo rendering eyes projection matrices
    not_rl_Matrix eyesViewOffset[2];       // VR stereo rendering eyes view offset matrices
} not_rl_VrStereoConfig; 




// Dynamic vertex buffers (position + texcoords + colors + indices arrays)
typedef struct not_rl_DynamicBuffer {
    int vCounter;               // vertex position counter to process (and draw) from full buffer
    int tcCounter;              // vertex texcoord counter to process (and draw) from full buffer
    int cCounter;               // vertex color counter to process (and draw) from full buffer
    float *vertices;            // vertex position (XYZ - 3 components per vertex) (shader-location = 0)
    float *texcoords;           // vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
    unsigned char *colors;      // vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
    unsigned short *indices;    // vertex indices (in case vertex data comes indexed) (6 indices per quad)
                                // NOTE: 6*2 byte = 12 byte, not alignment problem!
    unsigned int vaoId;         // OpenGL Vertex Array Object id
    unsigned int vboId[4];      // OpenGL Vertex Buffer Objects id (4 types of vertex data)
} not_rl_DynamicBuffer;


// Draw call type
// NOTE: Used to track required draw-calls, organized by texture
typedef struct not_rl_DrawCall {
    int vertexCount;
    GLuint vaoId;
    GLuint textureId;
    GLuint shaderId;

    not_rl_Matrix projection;
    not_rl_Matrix modelview;

    // TODO: Store additional draw state data
    //int blendMode;
    //Guint fboId;
} not_rl_DrawCall;
 


void not_rl_SetCameraMode(not_rl_Camera camera, int mode);                // Set camera mode (multiple camera modes available)

extern not_rl_VrStereoConfig vrConfig;         // VR stereo configuration for simulator

not_rl_RenderTexture2D not_rl_LoadRenderTexture(int width, int height);


void not_rl_InitWindow(int width, int height, void *data);
not_rl_VrDeviceInfo not_rl_GetVrDeviceInfo(int vrDeviceType);
void not_rl_InitVrSimulator(not_rl_VrDeviceInfo info);
void not_rl_BeginVrDrawing(void);
void not_rl_EndVrDrawing(void);

int not_rl_GetRandomValue(int min, int max);

#ifdef __cplusplus
#define NOT_RL_CLITERAL
#else
#define NOT_RL_CLITERAL    (not_rl_Color)
#endif
	
#define NOT_RL_BLANK      NOT_RL_CLITERAL{ 0, 0, 0, 1. }           // Blank (Transparent)

extern void not_rl_ClearBackgroundForReals(not_rl_Color);




/* math imports */
//------------------------------------------------------------------------------------
// Functions Declaration to work with Matrix
//------------------------------------------------------------------------------------
/*
NOT_RL_RMDEF float MatrixDeterminant(not_rl_Matrix mat);                      // Compute matrix determinant
NOT_RL_RMDEF float MatrixTrace(not_rl_Matrix mat);                            // Returns the trace of the matrix (sum of the values along the diagonal)
NOT_RL_RMDEF void MatrixTranspose(not_rl_Matrix *mat);                        // Transposes provided matrix
NOT_RL_RMDEF void MatrixInvert(not_rl_Matrix *mat);                           // Invert provided matrix
NOT_RL_RMDEF void MatrixNormalize(not_rl_Matrix *mat);                        // Normalize provided matrix
NOT_RL_RMDEF not_rl_Matrix MatrixIdentity(void);                              // Returns identity matrix
NOT_RL_RMDEF not_rl_Matrix MatrixAdd(not_rl_Matrix left, not_rl_Matrix right);              // Add two matrices 
NOT_RL_RMDEF not_rl_Matrix MatrixSubstract(not_rl_Matrix left, not_rl_Matrix right);        // Substract two matrices (left - right)
NOT_RL_RMDEF not_rl_Matrix MatrixRotate(Vector3 axis, float angle);           // Returns rotation matrix for an angle around an specified axis (angle in radians)
NOT_RL_RMDEF not_rl_Matrix MatrixRotateX(float angle);                        // Returns x-rotation matrix (angle in radians)
NOT_RL_RMDEF not_rl_Matrix MatrixRotateY(float angle);                        // Returns y-rotation matrix (angle in radians)
NOT_RL_RMDEF not_rl_Matrix MatrixRotateZ(float angle);                        // Returns z-rotation matrix (angle in radians)
NOT_RL_RMDEF not_rl_Matrix MatrixScale(float x, float y, float z);            // Returns scaling matrix   
NOT_RL_RMDEF not_rl_Matrix MatrixFrustum(double left, double right, double bottom, double top, double near, double far);  // Returns perspective projection matrix
NOT_RL_RMDEF not_rl_Matrix MatrixPerspective(double fovy, double aspect, double near, double far);                        // Returns perspective projection matrix
NOT_RL_RMDEF not_rl_Matrix MatrixLookAt(Vector3 position, Vector3 target, Vector3 up);  // Returns camera look-at matrix (view matrix)
NOT_RL_RMDEF float *MatrixToFloat(not_rl_Matrix mat);                         // Returns float array of Matrix data

*/

NOT_RL_RMDEF not_rl_Matrix not_rl_MatrixMultiply(not_rl_Matrix left, not_rl_Matrix right);         // Returns two not_rl_Matrix multiplication
NOT_RL_RMDEF not_rl_Matrix not_rl_MatrixTranslate(float x, float y, float z);        // Returns translation matrix
NOT_RL_RMDEF not_rl_Matrix not_rl_MatrixFrustum(double left, double right, double bottom, double top, double near, double far);  // Returns perspective projection matrix
NOT_RL_RMDEF not_rl_Matrix not_rl_MatrixPerspective(double fovy, double aspect, double near, double far);                        // Returns perspective projection matrix
NOT_RL_RMDEF not_rl_Matrix not_rl_MatrixIdentity(void);                              // Returns identity matrix
NOT_RL_RMDEF not_rl_Matrix not_rl_MatrixOrtho(double left, double right, 
   double bottom, double top, double near, double far);    // Returns orthographic projection matrix
NOT_RL_RMDEF float *not_rl_MatrixToFloat(not_rl_Matrix mat);
int not_rl_setupGraphics(int w, int h,int graphics_mode);





void not_rl_rlEnableRenderTexture(unsigned int id);

void not_rl_rlDisableRenderTexture(void);

void not_rl_ClearScreenBuffers(void);

void not_rl_EnableRenderTexture(unsigned int id);


void not_rl_SetMatrixModelview(not_rl_Matrix view);
void not_rl_SetMatrixProjection(not_rl_Matrix proj);

void not_rl_SetShaderValue(not_rl_Shader shader, int uniformLoc, const float *value, int size);
void not_rl_SetShaderValuei(not_rl_Shader shader, int uniformLoc, const int *value, int size);
void not_rl_SetShaderValueMatrix(not_rl_Shader shader, int uniformLoc, not_rl_Matrix mat);

		    
// Get shader uniform location
int not_rl_GetShaderLocation(not_rl_Shader shader, const char *uniformName);


void not_rl_DisableRenderTexture(void);


void not_rl_EnableTexture(unsigned int id);
void not_rl_DisableTexture(void);

void not_rl_Begin(int mode);
void not_rl_End(void);
void not_rl_Vertex2i(int x, int y);
void not_rl_Vertex2f(float x, float y);
void not_rl_Vertex3f(float x, float y, float z);
void not_rl_TexCoord2f(float x, float y);
void not_rl_Normal3f(float x, float y, float z);
void not_rl_Color4ub(not_rl_byte r, not_rl_byte g, not_rl_byte b, not_rl_byte a);
void not_rl_Color3f(float x, float y, float z);
void not_rl_Color4f(float x, float y, float z, float w);
void not_rl_glDraw(void);


#ifndef __APPLE__
#ifdef ANDROID
// NOTE: VAO functionality is exposed through extensions (OES)
static GLvoid (*glGenVertexArrays)(GLsizei n, GLuint *arrays) ;
static GLvoid (*glBindVertexArray)(GLuint array);
static GLvoid (*glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
#else
// NOTE: VAO functionality is exposed through extensions (OES)
static PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
static PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
static PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
//static PFNGLISVERTEXARRAYOESPROC glIsVertexArray;        // NOTE: Fails in We$
#endif
#else
#ifndef __APPLE__
// NOTE: VAO functionality is exposed through extensions (OES)
static GLvoid (*glGenVertexArrays)(GLsizei n, GLuint *arrays) ;
static GLvoid (*glBindVertexArray)(GLuint array);
static GLvoid (*glDeleteVertexArrays)(GLsizei n, const GLuint *arrays);
#endif
#endif



#endif
