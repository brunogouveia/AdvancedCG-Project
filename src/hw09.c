/*
 *  Geodesic Sphere
 *  This program requires OpenGL 4.0 or above
 *
 *  Demonstrate a tesselation shader by drawing a sphere
 *  from an icosahedron.
 *
 *  Based on The Little Grashopper tutorial
 *  http://prideout.net/blog/?p=48
 *
 *  Key bindings:
 *  +/-        Increase/decrease inner and outer
 *  i/I        Increase/decrease inner
 *  o/O        Increase/decrease outer
 *  r          Reset inner/outer levels
 *  a          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx239.h"
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int zh=0;         //  Light angle
float Ylight=2;   //  Light elevation
int shader=0;     //  Shader
int N;            //  Number of faces
int Position;     //  Position VBO
int Inner=40;      //  Tesselation inner level
int Outer=40;      //  Tesselation outer level
int move=1;       //  Move light
float fov=55.0;   //  Field of view (angles)
float asp=1;      //  Screen aspect ratio
float dim=2;      //  World dimension
float elv=-10;    //  Light elevation
int texture;      //  Terrain texture
int heightMap;    //  Height texture
int normalMap;    //  Normal texture

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   int id;

   //  Set shader
   glUseProgram(shader);

   //  Controls for tesselation level
   id = glGetUniformLocation(shader,"Inner");
   if (id>=0) glUniform1f(id,Inner);
   id = glGetUniformLocation(shader,"Outer");
   if (id>=0) glUniform1f(id,Outer);

   // Render the scene
   glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
   glClearColor(0,0,0,1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Update light position
   float position[]  = {(float)(2*Cos(zh)),Ylight,(float)(2*Sin(zh)),1.0};
   SetLightPosition(position);

   glPatchParameteri(GL_PATCH_VERTICES,3);
   // glDrawElements(GL_PATCHES,N,GL_UNSIGNED_INT,0);
   DrawCube(shader, heightMap, texture, normalMap);

   //  Display parameters
   glColor3f(1,1,1);
   glWindowPos2i(5,5);
   Print("FPS=%d Angle=%d,%d Inner=%d Outer=%d",
      FramesPerSecond(),th,ph,Inner,Outer);
   //  Render the scene and make it visible
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Set model view matrix
   float Ex = -2*dim*Sin(th)*Cos(ph);
   float Ey = +2*dim        *Sin(ph);
   float Ez = +2*dim*Cos(th)*Cos(ph);
   SetModelViewMatrix(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
   //  Set projection
   SetProjectionMatrix(M_PI*fov/180.0, asp, dim/16.0, dim*16.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle movement
   else if (ch == 's' || ch == 'S')
      move = 1-move;
   //  Move light
   else if (ch == '[')
      zh += 5;
   else if (ch == ']')
      zh -= 5;
   //  Move light
   else if (ch == '{')
      elv++;
   else if (ch == '}')
      elv--;
   //  Reset levels
   else if (ch == 'r')
   {
      Inner = 3;
      Outer = 2;
   }
   //  Both levels
   else if (ch == '+')
   {
      Inner++;
      Outer++;
   }
   else if (ch == '-')
   {
      if (Inner>1) Inner--;
      if (Outer>1) Outer--;
   }
   //  Inner level
   else if (ch == 'i')
      Inner++;
   else if (ch == 'I' && Inner>1)
      Inner--;
   //  Outer level
   else if (ch == 'o')
      Outer++;
   else if (ch == 'O' && Outer>1)
      Outer--;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (width>0) ? (double)height/width : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set model view matrix
   float Ex = -2*dim*Sin(th)*Cos(ph);
   float Ey = +2*dim        *Sin(ph);
   float Ez = +2*dim*Cos(th)*Cos(ph);
   SetModelViewMatrix(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
   //  Set projection
   SetProjectionMatrix(M_PI*fov/180.0, asp, dim/16.0, dim*16.0);
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   if (move) zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

//
//  Create Shader Program
//
int CreateShaderProgTess(void)
{
   //  Create program
   int prog = glCreateProgram();
   //  Compile shaders
   CreateShader(prog,GL_VERTEX_SHADER         ,"geodesic.vert");
   CreateShader(prog,GL_TESS_CONTROL_SHADER   ,"geodesic.tcs");
   CreateShader(prog,GL_TESS_EVALUATION_SHADER,"geodesic.tes");
   CreateShader(prog,GL_GEOMETRY_SHADER       ,"geodesic.geom");
   CreateShader(prog,GL_FRAGMENT_SHADER       ,"geodesic.frag");
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Bruno Gouveia - hw09");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_4_0) Fatal("OpenGL 4.0 not supported\n");
#endif
   printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Shader program
   shader = CreateShaderProgTess();
   //  Init light
   InitLight(&shader, 1);
   //  Init projection
   InitProjection(&shader, 1);
   //  Init cube
   InitCube();
   // Load textures
   glActiveTexture(GL_TEXTURE0);
   texture = LoadTexBMP("textures/texture.bmp");
   glActiveTexture(GL_TEXTURE1);
   heightMap = LoadTexBMP("textures/heightmap2.bmp");
   glActiveTexture(GL_TEXTURE2);
   normalMap = LoadTexBMP("textures/normalmap2.bmp");
   ErrCheck("init");
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
