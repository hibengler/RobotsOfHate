

/*$Revision: 1.13 $*/



// // // // // // // // // // icosahedron  vvvvvvvvvvv

#define X .525731112119133606
#define Z .850650808352039932

#define NUMBER_HEX_VERTS 7
#define NUMBER_HEX_INDCS 6

#define NUMBER_HEX_VERTS_X4 28
#define NUMBER_HEX_INDCS_X3 18

#define MAX_TILES_CUMUL 20
#define NUMBER_VERT_CUMUL 140
/* ^^^ number_hex_verticies * number_cummulative */
#define MAX_NUMBER_VERT_CUMUL_X4 560
#define MAX_NUMBER_INDCS_CUMUL_X3 360

/* ^^^ number_hex_verticies * number_cummulative *4 */


static GLuint icosahedronVertCount = 12;
static GLuint icosahedronIndCount = 60;

static GLfloat icosahedronVertsx[] =
        {
                -X, 0.0, Z,//0
                X, 0.0, Z,//1
                -X, 0.0, -Z,//2
                X, 0.0, -Z,//3
                0.0, Z, X,//4
                0.0, Z, -X,//5
                0.0, -Z, X,//6
                0.0, -Z, -X,//7
                Z, X, 0.0,//8
                -Z, X, 0.0,//9
                Z, -X, 0.0,//10
                -Z, -X, 0.0//11
        };
static GLfloat icosahedronVerts[] =//4TH  ELEMENT WILL GET USED FOR TRANSFORMATION XREF (index of transform matrix per set).
{
				0.0,  -0.01,		-0.25,	1.0,//0
				0.0,  0.15,	-0.15,	1.0,//1
				0.0,  0.3,		-0.075,	1.0,//2
				0.0,  0.55,	-0.1,	1.0,//3
				0.0,  0.6,		-0.1,	1.0,//4
				0.0,  0.7,		-0.075,	1.0,//5
				0.0,  0.75,	0.075,	1.0,//6
				0.05, 0.2,		-0.1,	1.0,//7
				-0.05,0.2,		-0.1,	1.0,//8
				0.05, 0.4,		0.0,	1.0,//9
				-0.05,0.4,		0.0,	1.0,//10
				0.1,  0.45,	0.0,	1.0,//11
				-0.1, 0.45,	0.0,	1.0,//12
				0.15, 0.3,		0.0,	1.0,//13
				-0.15,0.3,		0.0,	1.0,//14
				0.1,  0.1,		-0.175,	1.0,//15
				-0.1, 0.1,		-0.175,	1.0,//16
				0.2,  0.2,		0.025,	1.0,//17
				-0.2, 0.2,		0.025,	1.0,//18
				0.25, 0.4,		0.05,	1.0,//19
				-0.25,0.4,		0.05,	1.0,//20
				0.15, 0.55,	0.05,	1.0,//21
				-0.15,0.55,	0.05,	1.0,//22
				0.1,  0.575,	0.025,	1.0,//23
				-0.1, 0.575,	0.025,	1.0,//24
				0.05, 0.0,		-0.25,	1.0,//25
				-0.05,0.0,		-0.25,	1.0,//26
				0.125, 0.05,	-0.15,	1.0,//27
				-0.125,0.05,	-0.15,	1.0//28
};
static GLfloat icosahedronColors[12][4] =
        {
                {1.0f,1.0f,0.0f,1.0f}, //0r
                {0.0f,3.0f,0.0f,1.0f}, //1dg
                {0.0f,0.6f,0.0f,1.0f}, //2 grn
                {1.0f,1.0f,0.0f,1.0f}, //3y
                {0.3f,0.3f,0.0f,1.0f}, //4o
                {1.0f,0.0f,1.0f,1.0f}, //5mag
                {1.0f,0.6f,0.7f,1.0f}, //6lpink
                {0.0f,1.0f,1.0f,1.0f}, //7cyan
                {1.0f,0.6f,0.6f,1.0f}, //8pink
                {1.0f,1.0f,1.0f,1.0f}, //9wh
                {0.7f,0.7f,1.0f,1.0f}, //10bluewhite
                {0.5f,0.0f,0.5f,1.0f}  //11dk magenta
        };


static GLushort icosahedronIndices[60] =
        {
	1, 4, 0, 
	4, 9, 0, 
	4, 5, 9, 
	8, 5, 4,	
	1, 8, 4, 
	1, 10, 8, 
	10, 3, 8, 
	8, 3, 5, 
	3, 2, 5, 
	3, 7, 2, 
	3, 10, 7, 
	10, 6, 7, 
	6, 11, 7, 
	6, 0, 11,	
	6, 1, 0, 
	10, 1, 6, 
	11, 0, 9, 
	2, 11, 9, 
	5, 2, 9, 
	11, 2, 7

        };

// // // // // // // // // // icosahedron  ^^^^^^^^^^^^


static GLuint IBO_length = 0;//actually indices cumulative length
static GLuint VERT_length = 0;//will be incremented as filled
static GLuint COL_length = 0;
        
static GLuint flatHexVertCountatStart = 7;
static GLuint flatHexIndCountatStart = 6;

static GLfloat flatHexVertsCumul[MAX_NUMBER_VERT_CUMUL_X4];
static GLuint flatHexIndicesCumul[MAX_NUMBER_INDCS_CUMUL_X3];
static GLuint flatHexIndices[]  = {
	1,0,6,//a
	1,6,2,//b
	2,6,3,//c
	3,6,4,//d
	6,5,4,//e
	6,0,5//f
	
};
static GLfloat flatHexVerts[] = {//gen from:  0 xc, .5p yc, .3 r, 0 ang
	0.15,0.29,0.0,1.0,
	-0.15,0.29,0.0,1.0,
	-0.3,0.55,0.0,1.0,
	-0.15,0.81,0.0,1.0,
	0.15,0.81,0.0,1.0,
	0.3,0.55,0.0,1.0,
	0.0,0.55,0.0,1.0	
};

static GLfloat flatHexColorsCumul[MAX_NUMBER_VERT_CUMUL_X4];
static GLfloat flatHexColors[28] =
        {
                1.0f,1.0f,0.0f,0.5f, //0r
                0.0f,0.6f,0.0f,0.5f, //1dg
                0.0f,0.6f,0.0f,0.5f, //2 grn
                1.0f,1.0f,0.0f,0.5f, //3y
                0.3f,0.3f,0.0f,0.5f, //4o
                1.0f,0.0f,1.0f,0.5f, //5mag
                1.0f,1.0f,1.0f,1.0f //6wh
              // mellower version of center color  1.0f,0.6f,0.7f,1.0f //6lpink
        };  
        
//vvvvv maybe later        
static GLfloat flatHexColorInstructions[7][7];// = {// { hop | Alpha Or other prime param | other instruction code | clamp to color(rgba) }
//};
//^^^^^



static GLuint lineHexVertCountatStart = 12;
static GLuint lineHexIndCountatStart = 12;
static GLuint lineHexIndicesCumul[MAX_NUMBER_INDCS_CUMUL_X3];
static GLfloat lineHexVertsCumul[MAX_NUMBER_VERT_CUMUL_X4];
static GLushort lineHexIndices[]  = {
	0,6,5,
	1,7,6,
	2,8,7,
	3,9,8,
	4,10,9,
	5,11,10,
	0,7,6,
	1,8,7,
	2,9,8,
	3,10,9,
	4,11,10,
	5,6,11
	
	
	
	
	/* 	0,1,6,//a switched
	1,6,2,//b switched
	2,6,3,//c switched
	3,6,4,//d switched order 
	6,5,4,//e
	6,0,5//f */
	
};
static GLfloat lineHexVerts[] = {//gen from:  0 xc, .5p yc, .3 r, 0 ang
	//at v1.9 these are just placeholders
	0.15,0.29,0.0,1.0,
	-0.15,0.29,1.0,1.0,
	-0.3,0.55,0.0,1.0,
	-0.15,0.81,0.0,1.0,
	0.15,0.81,0.0,1.0,
	0.3,0.55,0.0,1.0,	
	0.15,0.29,0.0,1.0,
	-0.15,0.29,1.0,1.0,
	-0.3,0.55,0.0,1.0,
	-0.15,0.81,0.0,1.0,
	0.15,0.81,0.0,1.0,
	0.3,0.55,0.0,1.0
	/* 0.25, 0.07, 0.0,1.0,
	-0.25, 0.07, 0.0,1.0,
	-0.5, 0.5, 0.0,1.0,
	-0.25, 0.93, 0.0,1.0,
	0.25, 0.93, 0.0,1.0,
	0.5, 0.5, 0.0,1.0,
	0.0, 0.5, 0.0,1.0, //center */
	
};
static GLfloat lineHexColorsCumul[MAX_NUMBER_VERT_CUMUL_X4];
static GLfloat lineHexColors[48] =
        {
                0.0f,1.0f,1.0f,0.5f, //cy
                0.0f,1.0f,0.0f,0.5f, //g
                0.0f,1.0f,1.0f,0.5f, //cy
                0.0f,1.0f,0.0f,0.5f, //g
                0.0f,1.0f,1.0f,0.5f, //cy
                0.0f,1.0f,0.0f,0.5f, //g
                
                0.0f,1.0f,0.0f,0.5f, //g
                0.0f,1.0f,1.0f,0.5f, //cy
                0.0f,1.0f,0.0f,0.5f, //g
                0.0f,1.0f,1.0f,0.5f, //cy
                0.0f,1.0f,0.0f,0.5f, //g
                1.0f,0.8f,1.0f,0.5f //6whpur center
              // mellower version of center color  1.0f,0.6f,0.7f,1.0f //6lpink
              
/* pretty good distrubuted                 1.0f,0.0f,0.0f,0.5f, //0r
                0.0f,0.6f,0.0f,0.5f, //1dg
                0.0f,0.0f,0.6f,0.5f, //2 blu
                1.0f,1.0f,1.0f,0.5f, //3wh
                0.3f,0.3f,0.0f,0.5f, //4y
                1.0f,0.0f,1.0f,0.5f, //5mag
                0.6f,0.0f,1.0f,0.5f //6pur center */
              
        };  
        









 GLfloat octahedronVerts[6][3] = {
	{ 1.0f, 0.0f, 0.0f }, 
	{ 0.0f, 1.0f, 0.0f }, 
	{ 0.0f, 0.0f, 1.0f }, 
	{ -1.0f, 0.0f, 0.0f }, 
	{ 0.0f, -1.0f, 0.0f }, 
	{ 0.0f, 0.0f, -1.0f } 
	}; 


        
// // // // // // // // // // one way of notating the tetra.  vvvvvvvvvvv
static const GLfloat tetrahedronVerts[4][3] = {
        {  0.94280904158206336586,0.,-0.333333333333333333  },   /* index 0 */
        { -0.47140452079103168293,0.81649658092772603272, -0.333333333333333333333333 },   /* index 1 */
        { -0.47140452079103168293,-0.81649658092772603272, -0.333333333333333333333333 },   /* index 2 */
        { 0.,0.,1.  }   /* index 3 */
   }; /* index 3 */

 static const GLfloat tetrahedronColors[4][4] = {
        {  1.0,  1.0,  0.0, 1.0  },   // red //
        {  0.0,  1.0,  0.0, 0.4   },   // green //
        {  0.0,  0.0,  1.0, 1.0   },   // blue //
        {  1.0,  1.0,  1.0, 1.0   } }; // white //

static const GLushort tetrahedronIndices[12] = //GL_TRIANGLE_STRIP version- see glDrawElements { 0, 1, 2, 3, 0, 1 } remember [6];
{
 0,1,2,//rgb
 2,1,3,
 2,3,0,//rwb
 0,3,1};




        
// // // // // // // // // // one way of notating the tetra.  vvvvvvvvvvv
static const GLfloat cubeVerts[8][3] = {
        { 0.5773502691896257645,0.57735026918962576450,0.57735026918962576450 }, //sqrt(3)/3)
        { 0.5773502691896257645,0.57735026918962576450,-0.57735026918962576450 },
        { 0.5773502691896257645,-0.57735026918962576450,-0.57735026918962576450 },
        { 0.5773502691896257645,-0.57735026918962576450,0.57735026918962576450 },
        { -0.5773502691896257645,0.57735026918962576450,0.57735026918962576450 }, //sqrt(3)/3)
        { -0.5773502691896257645,0.57735026918962576450,-0.57735026918962576450 },
        { -0.5773502691896257645,-0.57735026918962576450,-0.57735026918962576450 },
        { -0.5773502691896257645,-0.57735026918962576450,0.57735026918962576450 }
   }; /* index 3 */

 static const GLfloat cubeColors[4][4] = {
        {  1.0,  1.0,  0.0, 1.0  },   // red //
        {  0.0,  1.0,  0.0, 0.4   },   // green //
        {  0.0,  0.0,  1.0, 1.0   },   // blue //
        {  1.0,  1.0,  1.0, 1.0   } }; // white //

static const GLushort cubeIndices[36] = //GL_TRIANGLE_STRIP version- see glDrawElements { 0, 1, 2, 3, 0, 1 } remember [6];
{
 0,1,2,// 0123
 2,3,0,
 0,1,5,  // 0154
 5,4,0,
 1,2,6,  // 1265
 6,5,1,
 2,3,7,  // 2376
 7,6,2,
 3,0,4,  // 3047
 4,7,3,
 7,4,5,  // 7456
 5,6,7
 };
 

