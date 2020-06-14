#include <GL/glut.h>
#include <math.h>
#include "glm.h"
#include "imageloader.h"
#define RED 1
#define GRASS 2
#define BLACKANDWHITE 3
#define WALKING 5
#define ARMS 6
#define INTERACTING 4
#define STOP 7
static int rightShoulderx = 0 ,rightShoulderz = 0 ,leftShoulderx = 0, leftShoulderz = 0, rightElbow = 0,leftElbow = 0, rightKnee = 0, leftKnee = 0, rightHipx=0,rightHipz=0, leftHipx=0, leftHipz=0, rightFingerBase = 0, rightFingerUp = 0, leftFingerBase = 0, leftFingerUp = 0;
double eye[] = { 25, 0, 0 };
double center[] = { 0, 1, 0 };
double up[] = { 0, 1, 0 };
int WalkingPoses[8][8]={{15,15,-10,5,15,-25,-10,-25},
               {25,25,-20,15,25,-45,-20,-45},
               {15,15,-10,5,15,-25,-10,-25},
               {0,0,0,0,0,0,0,0},{-10,5,15,15,-10,-25,15,-25},{-20,15,25,25,-20,-45,25,-45},{-10,5,15,15,-10,-25,15,-25},{0,0,0,0,0,0,0,0}};
int ArmPoses[10][4]={{-45,-45,0,0},{-90,-90,0,0},{-90,-90,-35,-35},{-90,-90,-70,-70},{-90,-90,-110,-110},
               {-90,-90,-70,-70},{-90,-90,-35,-35},{-90,-90,0,0},{-45,-45,0,0},{0,0,0,0}};
int BallPoses[8][3]={{45,0,0},{90,0,0},{45,0,0},{0,0,0},{0,-20,-40},{0,-40,-80},{0,-20,-40},{0,0}};
int frame =0 ; 
int walking = 0;
int arms = 0;
int interacting = 0;           
bool rightkneeup=false;
bool rightkneedown=false;
bool righthipup=false;
bool righthipdown = false;
bool leftkneeup = false;
bool leftkneedown = false;
bool lefthipup = false;
bool lefthipdown = false;
static int ballAngle=0;
GLfloat speed=0,verTheta =0, horTheta=0;
GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */
int moving, startx, starty;
Image* image = loadBMP("data/grass.bmp");
GLMmodel* pmodel1;
GLMmodel* pmodel2;
GLMmodel* pmodel3;
GLMmodel* pmodel4;
GLMmodel* pmodel5;
GLMmodel* pmodel6;
char* path;
// RGBA
GLfloat light_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = {0.5,10.0, 0.0, 1.0 };
GLfloat lightPos1[] = {-0.5,-5.0,-2.0, 1.0 };
// Material Properties
GLfloat mat_amb_diff[] = {0.643, 0.753, 0.934, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = {100.0 };  
//left teapot specular
GLfloat teapotl_diff[] = { 0.0,0.0, 1.0, 1.0 };
GLfloat teapotl_specular[] = {1.0, 1.0, 1.0, 1.0 };
GLfloat teapotl_shininess[] = {10.0 };  
//middle teapot diffuse
GLfloat teapotm_diff[] = { 1.0, 0, 0.0, 1.0 };
GLfloat teapotm_specular[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat teapotm_shininess[] = {1.0 };  
//right teapot glosy
GLfloat teapotr_diff[] = { 1.0, .0, 0.0, 1.0 };
GLfloat teapotr_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat teapotr_shininess[] = {1000.0 };  
//cube
GLfloat cube_diff[] = {1.0,0.0, 0.0, 1.0 };
GLfloat cube_specular[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat cube_shininess[] = {10.0 }; 
GLuint loadTexture(Image* image) {
      GLuint textureId;
      glGenTextures(1, &textureId); //Make room for our texture
      glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
      //Map the image to the texture
      glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                               0,                            //0 for now
                               GL_RGB,                       //Format OpenGL uses for image
                               image->width, image->height,  //Width and height
                               0,                            //The border of the image
                               GL_RGB, //GL_RGB, because pixels are stored in RGB format
                               GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                                 //as unsigned numbers
                               image->pixels);               //The actual pixel data
      return textureId; //Returns the id of the texture
}
GLuint _textureId; //The id of the texture
void processMenuEvents(int option) {
	switch (option) {
		case RED :
			image = loadBMP("data/RED.bmp");
         _textureId = loadTexture(image);
      	delete image;
      	break;
		case GRASS :
			image = loadBMP("data/grass.bmp");
         _textureId = loadTexture(image);
      	delete image;
         break;
		case BLACKANDWHITE :
			image = loadBMP("data/floor.bmp");
         _textureId = loadTexture(image);
      	delete image; break;
      case WALKING :
         walking = 1,arms = 0 , interacting = 0;
         break;
      case ARMS :
			arms =1, walking = 0 , interacting = 0;
         break;
      case INTERACTING :
         interacting =1 , arms = 0 , walking = 0;
			break;
      case STOP :
         walking = 0 , arms = 0 , interacting = 0;
			break;
	}
}
void drawmodel1(char* path,GLMmodel* pmodel)
{
	if (!pmodel) {
		pmodel = glmReadOBJ(path);

		if (!pmodel) exit(0);
		glmUnitize(pmodel);
		glmFacetNormals(pmodel);
		glmVertexNormals(pmodel, 90.0);
		glmScale(pmodel, .15);
       // Turn on the power
        glEnable(GL_LIGHTING);
        // Flip light switch
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        // assign light parameters
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties         
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	GLfloat lightColor1[] = {1.0f, 1.0f,  1.0f, 1.0f };
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
        glEnable(GL_NORMALIZE);
        //Enable smooth shading
        glShadeModel(GL_SMOOTH);
        // Enable Depth buffer
        glEnable(GL_DEPTH_TEST);
      //   startList = glGenLists(4);

	}
	glmDraw(pmodel, GLM_SMOOTH | GLM_MATERIAL);
}
void clearRotation (void){
   leftKnee = 0;
   rightKnee = 0;
   leftShoulderx = 0;
   rightShoulderx = 0;
   leftHipx = 0 ;
   rightHipx = 0 ;
   leftElbow = 0;
   rightElbow = 0;
   leftShoulderx=0;
   rightShoulderx=0;
   ballAngle=0;
}
void setPoses(int frame){
   
   if (walking == 1)
   {
   rightHipx = WalkingPoses[frame][0];
   rightKnee = WalkingPoses[frame][1];
   leftHipx = WalkingPoses[frame][2];
   leftKnee = WalkingPoses[frame][3];
   leftShoulderx=WalkingPoses[frame][4];
   leftElbow=WalkingPoses[frame][5];
   rightShoulderx=WalkingPoses[frame][6];
   rightElbow=WalkingPoses[frame][7];

   }
   else if (arms == 1)
   {
   rightShoulderx = ArmPoses[frame][0];
   leftShoulderx = ArmPoses[frame][1];
   rightElbow = ArmPoses[frame][2];
   leftElbow = ArmPoses[frame][3];
   }
   else if(interacting==1)
   {
      rightKnee=BallPoses[frame][0];
      rightHipx=BallPoses[frame][1];
      ballAngle=BallPoses[frame][2];

   }
}
void initRendering() {
     	//  Image* image = loadBMP("RED.bmp");
      	_textureId = loadTexture(image);
      	delete image;
}
void createGLUTMenus() {

	int menu;
   int Animation;
   int Floor;
	
	//add entries to our menu
   Floor  = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Grass",GRASS);
	glutAddMenuEntry("Black & White",BLACKANDWHITE);
   Animation  = glutCreateMenu(processMenuEvents);
   glutAddMenuEntry("Ball Kick",INTERACTING);
   glutAddMenuEntry("Walking",WALKING);
   glutAddMenuEntry("Dumbbells Lifting",ARMS);
   glutAddMenuEntry("Stop",STOP);

   menu = glutCreateMenu(processMenuEvents);
	glutAddSubMenu("Floor", Floor);
	glutAddSubMenu("Animation", Animation);
	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
void timer(int value)
{
   clearRotation();
   if (walking == 1){
      frame = frame % 8;
      setPoses(frame);
      frame++;
   }
   else if (arms == 1)
   {
      frame = frame % 10;
      setPoses(frame);
      frame++;
   }
   else if (interacting == 1)
   {
      frame = frame % 8;
      setPoses(frame);
      frame++;
    
    }
   glutPostRedisplay();
   glutTimerFunc(1, timer, 0);
   }
void init(void)
{
    glMatrixMode(GL_PROJECTION);
	gluPerspective(65.0
   , (GLfloat)1024 / (GLfloat)869, 1.0, 60.0);
}
void RightArm(void)
{
   //draw upper arm
   glTranslatef(0.0,0.5,0.0);
   glRotatef((GLfloat) rightShoulderx,1.0,0.0,0.0);
   glTranslatef(0.0,-0.5,0.0);

   glTranslatef(0.0,0.5,0.0);
   glRotatef((GLfloat) rightShoulderz,0.0,0.0,1.0);
   glTranslatef(0.0,-0.5,0.0);

   glPushMatrix();
   glScalef(0.25,1.0,0.5);
   glutSolidCube(1.0);
   glPopMatrix();

   // draw lower arm
   glTranslatef(0.0,-0.5,0.0);
   glRotatef((GLfloat) rightElbow,1.0,0.0,0.0);
   glTranslatef(0.0,-0.5,0.0);
   glPushMatrix();
   glScalef(0.25,1.0,0.5);
   glutSolidCube(1.0);
   glPopMatrix();
   
       //Draw lower finger  1
   glPushMatrix();
   glTranslatef(-0.2, -0.4, 0.0);
   glRotatef((GLfloat)rightFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, -0.2, 0.2);
   glPushMatrix();
   glScalef(0.1, 0.2, 0.1);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger  1
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)rightFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.2, 0.1);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();

   //Draw lower finger  2
   glPushMatrix();
   glTranslatef(0.0, -0.4, 0.0);
   glRotatef((GLfloat)rightFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.1, -0.2, 0.225);
   glPushMatrix();
   glScalef(0.1, 0.3,0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger  2
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)rightFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();

   //Draw lower finger  3
   glPushMatrix();
   glTranslatef(0.1, -0.4, 0.0);
   glRotatef((GLfloat)rightFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2,0.075);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger  3
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)rightFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();

 //Draw lower finger  4
   glPushMatrix();
   glTranslatef(0.1, -0.4, 0.0);
   glRotatef((GLfloat)rightFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2, -0.075);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger 4
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)rightFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   glPopMatrix();
   //Draw lower finger 5
   glPushMatrix();
   glTranslatef(0.1, -0.4, 0.0);
   glRotatef((GLfloat)rightFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2, -0.225);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger 5
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)rightFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();


}
void LeftArm(void)
{
   //draw upper arm

   glTranslatef(0.0,0.5,0.0);
   glRotatef((GLfloat) leftShoulderx,1.0,0.0,0.0);
   glTranslatef(0.0,-0.5,0.0);

   glTranslatef(0.0,0.5,0.0);
   glRotatef((GLfloat) leftShoulderz,0.0,0.0,1.0);
   glTranslatef(0.0,-0.5,0.0);

   glPushMatrix();
   glScalef(0.25,1.0,0.5);
   glutSolidCube(1.0);
   glPopMatrix();

   //draw lower arm

   glTranslatef(0.0,-0.5,0.0);
   glRotatef((GLfloat) leftElbow,1.0,0.0,0.0);
   glTranslatef(0.0,-0.5,0.0);
   glPushMatrix();
   glScalef(0.25,1.0,0.5);
   glutSolidCube(1.0);
   glPopMatrix();
   
   
       //Draw lower finger  1
   glPushMatrix();
   glTranslatef(-0.1, -0.4, 0.0);
   glRotatef((GLfloat)leftFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.15, -0.2, 0.2);
   glPushMatrix();
   glScalef(0.1, 0.2, 0.1);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger  1
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)leftFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.2, 0.1);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();

   //Draw lower finger  2
   glPushMatrix();
   glTranslatef(-0.2, -0.4, 0.0);
   glRotatef((GLfloat)leftFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.1, -0.2, 0.225);
   glPushMatrix();
   glScalef(0.1, 0.3,0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger  2
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)leftFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();

   //Draw lower finger  3
   glPushMatrix();
   glTranslatef(-0.1, -0.4, 0.0);
   glRotatef((GLfloat)leftFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2,0.075);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger  3
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)leftFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();

 //Draw lower finger  4
   glPushMatrix();
     glTranslatef(-0.1, -0.4, 0.0);
   glRotatef((GLfloat)leftFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2, -0.075);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger 4
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)leftFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   glPopMatrix();
   //Draw lower finger 5
   glPushMatrix();
   glTranslatef(-0.1, -0.4, 0.0);
   glRotatef((GLfloat)leftFingerBase, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.2, -0.225);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();


   //Draw upper finger 5
   glTranslatef(0.0, 0.0, 0.0);
   glRotatef((GLfloat)leftFingerUp, 0.0, 0.0, 1.0);
   glTranslatef(0.0, -0.3, 0.0);
   glPushMatrix();
   glScalef(0.1, 0.3, 0.075);
   glutSolidCube(1);
   glPopMatrix();

   glPopMatrix();


}
void RightLeg(void)
{

   //draw upper leg

   glTranslatef(0.05,0.8,0.0);
   glRotatef((GLfloat) rightHipx,1.0,0.0,0.0);
   glTranslatef(0.0,-0.8,0.0);

   glTranslatef(0.05,0.8,0.0);
   glRotatef((GLfloat) rightHipz,0.0,0.0,1.0);
   glTranslatef(0.0,-0.8,0.0);

   glPushMatrix();
   glScalef(0.5,1.6,0.5);
   glutSolidCube(1.0);
   glPopMatrix();

   //draw lower leg

   glTranslatef(0.0,-0.8,0.0);
   glRotatef((GLfloat) rightKnee,1.0,0.0,0.0);
   glTranslatef(0.0,-0.8,0.0);
   glPushMatrix();
   glScalef(0.45,1.6,0.5);
   glutSolidCube(1.0);
   glPopMatrix();
   
   //draw foot
   
   glTranslatef(0.05,-0.8,0.0);
   glPushMatrix();
   glScalef(0.45,0.5,0.8);
   glutSolidCube(1.0);
   glPopMatrix();

}
void LeftLeg(void)
{

   //draw upper leg

   glTranslatef(-0.05,0.8,0.0);
   glRotatef((GLfloat) leftHipx,1.0,0.0,0.0);
   glTranslatef(0.0,-0.8,0.0);

   glTranslatef(-0.05,0.8,0.0);
   glRotatef((GLfloat) leftHipz,0.0,0.0,1.0);
   glTranslatef(0.0,-0.8,0.0);

   glPushMatrix();
   glScalef(0.45,1.6,0.5);
   glutSolidCube(1.0);
   glPopMatrix();

   //draw lower leg

   glTranslatef(0.0,-0.8,0.0);
   glRotatef((GLfloat) leftKnee,1.0,0.0,0.0);
   glTranslatef(0.0,-0.8,0.0);
   glPushMatrix();
   glScalef(0.45,1.6,0.5);
   glutSolidCube(1.0);
   glPopMatrix();
   
   //draw foot
   
   glTranslatef(-0.05,-0.8,0.0);
   glPushMatrix();
   glScalef(0.5,0.5,0.8);
   glutSolidCube(1.0);
   glPopMatrix();

}
void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}
void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}
void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta))*(a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta))*(a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta))*(a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta)*p[0];
	temp[1] += cos(theta)*p[1];
	temp[2] += cos(theta)*p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];

}
void Left()
{
   verTheta = -0.05;
   rotatePoint( up,verTheta, eye);
}
void Right()
{
   verTheta =0.05;
   rotatePoint( up,verTheta, eye);
}
void Up()
{
   horTheta = -0.05;
   double viewVector[]={center[0]-eye[0],center[1]-eye[1],center[2]-eye[2]};
   double perpendicular[3];
   crossProduct(viewVector, up,perpendicular);
   normalize(perpendicular);
   rotatePoint(perpendicular,horTheta, eye);
   rotatePoint(perpendicular,horTheta, up);
}
void Down()
{	
   horTheta =0.05;
   double viewVector[]={center[0]-eye[0],center[1]-eye[1],center[2]-eye[2]};
   double perpendicular[3];
   crossProduct(viewVector, up,perpendicular);
   normalize(perpendicular);
   rotatePoint(perpendicular,horTheta, eye);
   rotatePoint(perpendicular,horTheta, up);
}
void moveForward()
{
   speed = 0.1;
	double direction[3];
   direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	eye[0]    += direction[0] * speed;
	eye[1]    += direction[1] * speed;
	eye[2]    += direction[2] * speed;
}
void moveBack()
{
   speed = 0.1;
	double direction[3];
   direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];

	eye[0]    -= direction[0] * speed;
	eye[1]    -= direction[1] * speed;
	eye[2]    -= direction[2] * speed;
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT );
   glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
    glPushMatrix();
        glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        glPopMatrix();
        //materials properties
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glPushMatrix();
   glPushMatrix();
	glEnable(GL_TEXTURE_2D);
   glScalef(0.5,0.5,0.5);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBegin(GL_QUADS);
       
	glNormal3f(0.0,-1.0,0.0);
	glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-40.0f, -5.0f, -60.0f);
        glTexCoord2f(5.0f,  0.0f);
        glVertex3f(-40.0f, -5.0f,  60.0f);
        glTexCoord2f(5.0f,  20.0f);
        glVertex3f( 40.0f, -5.0f,  60.0f);
        glTexCoord2f(0.0f, 20.0f);
        glVertex3f( 40.0f, -5.0f, -60.0f);
        glEnd();
	glDisable(GL_TEXTURE_2D);
   glPopMatrix();
	glTranslatef(0.5,1.0,0.0);
	glScalef(0.85,0.85,0.85);
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	//draw body
	glTranslatef (-1.0, 0.0, -17.0);
   glPushMatrix();
   
   glTranslatef(0,0,30);
   glScalef(60,80,60);
   drawmodel1("data/football goal.obj",pmodel2);

   glPopMatrix();
	glPushMatrix();
   
	glScalef (1.1, 2.0, 0.5);
	glutSolidCube (1.0);
   
	glPopMatrix();

	//draw head

	glTranslatef (0.0, 1.7, 0.0);
	glPushMatrix();
	glutWireSphere (0.4,8.0,8.0);
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef (0.75, -1.2, 0.0);
	RightArm();
      if(arms==1)
   { glPushMatrix();
   glTranslatef(0,-0.8,0);
   glRotatef((GLfloat)90,0.0,1.0,0.0);
   
   glScalef(5,5,5);
   drawmodel1("data/dumbbell.obj",pmodel5);

   glPopMatrix();}
	glPopMatrix();


	glPushMatrix();
	glTranslatef (-0.75, -1.2, 0.0);
	LeftArm();
   if(arms==1)
   {
          glPushMatrix();
   glTranslatef(0,-0.8,0);
   glRotatef((GLfloat) 90,0.0,1.0,0.0);
   
   glScalef(5,5,5);
   drawmodel1("data/dumbbell.obj",pmodel6);

   glPopMatrix();
   }
	glPopMatrix();

	glPushMatrix();
	glTranslatef (0.25, -3.5, 0.0);
	RightLeg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef (-0.25, -3.5, 0.0);
	LeftLeg();
	glPopMatrix();

	glPopMatrix();
   if (interacting == 1){
   glPushMatrix();
   glTranslatef(0.0,-0.8,-14.0);
   glRotatef((GLfloat) ballAngle,1.0,0.0,0.0);
   glTranslatef(0.2,-2,0.5);
   glScalef(3,3,3);
   drawmodel1("data/soccerball.obj",pmodel1);

   glPopMatrix();
   }
   
   glPushMatrix();
   glTranslatef(-15,0,3);
   glRotatef((GLfloat) 90,0.0,1.0,0.0);
   // glTranslatef(0.0,-0.8,0.0);
   
   glScalef(40,40,40);
   drawmodel1("data/Bench_LowRes.obj",pmodel3);

   glPopMatrix();
   glPushMatrix();
   glTranslatef(-15,0,-12);
   glRotatef((GLfloat) 90,0.0,1.0,0.0);
   // glTranslatef(0.0,-0.8,0.0);
   
   glScalef(40,40,40);
   drawmodel1("data/Bench_LowRes.obj",pmodel4);

   glPopMatrix();
   
	glPopMatrix();
	glutSwapBuffers();
}
void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:Left(); break;
	case GLUT_KEY_RIGHT: Right(); break;
	case GLUT_KEY_UP:  Up(); break;
	case GLUT_KEY_DOWN: Down(); break;
	}

	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
	// List all youe keyboard keys from assignment two her for body movement
	switch (key)
	{
	case '8':
		moveForward();
		glutPostRedisplay();
		break;
	case '2':
		moveBack();
		glutPostRedisplay();
		break;
   case 's':
       if(rightShoulderx>-90)
      rightShoulderx = (rightShoulderx - 5) % 360;
      glutPostRedisplay();
      break;
   case 'S':
       if(rightShoulderx<20)
      rightShoulderx = (rightShoulderx + 5) % 360;
      glutPostRedisplay();
      break;
   case 'x':
       if(rightShoulderz<120)
      rightShoulderz = (rightShoulderz + 5) % 360;
      glutPostRedisplay();
      break;
   case 'X':
       if(rightShoulderz>0)
      rightShoulderz = (rightShoulderz - 5) % 360;
      glutPostRedisplay();
      break;
   case 'a':
       if(leftShoulderx>-90)
      leftShoulderx = (leftShoulderx - 5) % 360;
      glutPostRedisplay();
      break;
   case 'A':
       if(leftShoulderx< 20)
      leftShoulderx = (leftShoulderx + 5) % 360;
      glutPostRedisplay();
      break;
   case 'z':
       if(leftShoulderz>-120)
      leftShoulderz = (leftShoulderz - 5) % 360;
      glutPostRedisplay();
      break;
   case 'Z':
       if(leftShoulderz<0)
      leftShoulderz = (leftShoulderz + 5) % 360;
      glutPostRedisplay();
      break;
   case 'e':
       if(rightElbow>-150)
      rightElbow = (rightElbow - 5) % 360;
      glutPostRedisplay();
      break;

   case 'E':
       if(rightElbow<0)
      rightElbow = (rightElbow + 5) % 360;
      glutPostRedisplay();
      break;
   case 'w':
       if(leftElbow>-150)
      leftElbow = (leftElbow - 5) % 360;
      glutPostRedisplay();
      break;
   case 'W':
       if(leftElbow<0)
      leftElbow = (leftElbow + 5) % 360;
      glutPostRedisplay();
      break;
       
   case 'h':
       if(rightHipx<60)
      rightHipx = (rightHipx + 5) % 360;
      glutPostRedisplay();
      break;
   case 'H':
       if(rightHipx>-90)
      rightHipx = (rightHipx - 5) % 360;
      glutPostRedisplay();
      break;
   case 'n':
       if(rightHipz<80)
      rightHipz = (rightHipz + 5) % 360;
      glutPostRedisplay();
      break;
   case 'N':
       if(rightHipz>0)
      rightHipz = (rightHipz - 5) % 360;
      glutPostRedisplay();
      break;
   case 'g':
       if(leftHipx<60)
      leftHipx = (leftHipx + 5) % 360;
      glutPostRedisplay();
      break;
   case 'G':
       if(leftHipx>-90)
      leftHipx = (leftHipx - 5) % 360;
      glutPostRedisplay();
      break;
   case 'b':
       if(leftHipz>-80)
      leftHipz = (leftHipz - 5) % 360;
      glutPostRedisplay();
      break;
   case 'B':
       if(leftHipz<0)
      leftHipz = (leftHipz + 5) % 360;
      glutPostRedisplay();
      break;
   case 'k':
       if(rightKnee<90)
      rightKnee = (rightKnee + 5) % 360;
      glutPostRedisplay();
      break;
   case 'K':
       if(rightKnee>0)
      rightKnee = (rightKnee - 5) % 360;
      glutPostRedisplay();
      break;
   case 'j':
       if(leftKnee<90)
      leftKnee = (leftKnee + 5) % 360;
      glutPostRedisplay();
      break;
   case 'J':
       if(leftKnee>0)
      leftKnee = (leftKnee - 5) % 360;
      glutPostRedisplay();
      break;   
   case 'f':
       if(rightFingerBase>-60)
      rightFingerBase = (rightFingerBase - 5) % 360;
      glutPostRedisplay();
      break;
   case 'F':
       if(rightFingerBase<10)
      rightFingerBase = (rightFingerBase + 5) % 360;
      glutPostRedisplay();
      break;
   case 'v':
       if(rightFingerUp>-80)
      rightFingerUp = (rightFingerUp - 5) % 360;
      glutPostRedisplay();
      break;
   case 'V':
       if(rightFingerUp <15)
      rightFingerUp = (rightFingerUp + 5) % 360;
      glutPostRedisplay();
      break;        
   case 'd':
       if(leftFingerBase<60)
      leftFingerBase = (leftFingerBase + 5) % 360;
      glutPostRedisplay();
      break;
   case 'D':
       if(leftFingerBase>-10)
      leftFingerBase = (leftFingerBase - 5) % 360;
      glutPostRedisplay();
      break;
   case 'c':
       if(leftFingerUp<80)
      leftFingerUp = (leftFingerUp + 5) % 360;
      glutPostRedisplay();
      break;
   case 'C':
       if(leftFingerUp>-15)
      leftFingerUp = (leftFingerUp - 5) % 360;
      glutPostRedisplay();
      break;
   case 'p':
      horTheta =(horTheta + 5) ;
      break;
   case 'l':
      horTheta =(horTheta - 5) ;
      break;
   case 'm':
      verTheta =(verTheta + 5) ;
      break;
   case 'i':
      verTheta =(verTheta - 5) ;
      break;
   case 'u':
      speed =(speed + 0.1) ;
      break;
   case 'y':
      speed =(speed - 0.1) ;
      break;
   case 27:
      exit(0);
      break;
	default:
		break;
	}
}
static void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
}
static void motion(int x, int y)
{
  if (moving) {
    if((x - startx)>0)
    {
      Right();
    }
    else if((x - startx)<0)
    {
      Left();
    }
   glutPostRedisplay();
  }
}
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 100);
   glutTimerFunc(0, timer, 0);
      //  glutTimerFunc(0, timer, 1);
	glutCreateWindow("body");  
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
	init();
   initRendering();
   createGLUTMenus();
	glutDisplayFunc(display);
   glutSpecialFunc(specialKeys);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}