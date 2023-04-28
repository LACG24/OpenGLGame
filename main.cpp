
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <random>
#include <iomanip>
#include "RgbImage.h"

#define NTextures 6
GLuint	texture[NTextures];
char* filename0 = "C:/Users/LACG2/OneDrive/Escritorio/plataformas/pasto.bmp";
//Variables dimensiones de la pantalla
int WIDTH=700;
int HEIGTH=700;
//Variables para establecer los valores de gluPerspective
float FOVY=60.0;
float ZNEAR=0.01;
float ZFAR=900.0;
//Variables para definir la posicion del observador
//gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X=40.0; //15
float EYE_Y=10.0; //10
float EYE_Z=40.0; //15
float CENTER_X=4.0;
float CENTER_Y=1.0;
float CENTER_Z=0.0;
float UP_X=0;
float UP_Y=1;
float UP_Z=0;
//Variables para dibujar los ejes del sistema
float X_MIN=-500;
float X_MAX=500;
float Y_MIN=-500;
float Y_MAX=500;
float Z_MIN=-500;
float Z_MAX=500;
//Size del tablero
int DimBoard = 200;
float Theta=0;
float PI = 3.14159265359;
float Direction[3] = {1.0,0.0, 0.0};
/*
 * Read a texture map from a BMP bitmap file.
 */
void loadTextureFromFile(char *filename, int index)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	//glEnable(GL_DEPTH_TEST);

	RgbImage theTexMap( filename );

    //generate an OpenGL texture ID for this texture
    glGenTextures(1, &texture[index]);
    //bind to the new texture ID
    glBindTexture(GL_TEXTURE_2D, texture[index]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0,
                     GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
    theTexMap.Reset();
}

float DegToRad(float g)
{
      return ((g*PI)/180.0);
}

void drawAxis()
{
     glLineWidth(3.0);
     //X axis in red
     glColor3f(1.0f,0.0f,0.0f);
     glBegin(GL_LINES);
       glVertex3f(X_MIN,0.0,0.0);
       glVertex3f(X_MAX,0.0,0.0);
     glEnd();
     //Y axis in green
     glColor3f(0.0f,1.0f,0.0f);
     glBegin(GL_LINES);
       glVertex3f(0.0,Y_MIN,0.0);
       glVertex3f(0.0,Y_MAX,0.0);
     glEnd();
     //Z axis in blue
     glColor3f(0.0f,0.0f,1.0f);
     glBegin(GL_LINES);
       glVertex3f(0.0,0.0,Z_MIN);
       glVertex3f(0.0,0.0,Z_MAX);
     glEnd();
     glLineWidth(1.0);
 }

 void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, (GLfloat)WIDTH/HEIGTH, ZNEAR, ZFAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    glClearColor(0,0,0,0);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    loadTextureFromFile(filename0,0);
    srand(time(nullptr));
}

void LookAt()
{
    float x, z;
    x = Direction[0];
    z = Direction[2];
    Direction[0] = x * cos(DegToRad(Theta)) + z * sin(DegToRad(Theta));
    Direction[2] = -x * sin(DegToRad(Theta)) + z * cos(DegToRad(Theta));
    CENTER_X = EYE_X + Direction[0];
    CENTER_Z = EYE_Z + Direction[2];
}

class Plataforma{
public:
    float Position[3];
    int Size;
    Plataforma(float x, float y, float z, int s){
        Position[0] = x;
        Position[1] = y;
        Position[2] = z;
        Size = s;
    }
    void drawPlataforma(){
        glPushMatrix();
        glTranslated(Position[0],Position[1],Position[2]);
        glColor3f(1.0,1.0,0.0);
        glutWireCube(Size);
        glPopMatrix();
    }
};
Plataforma plataforma(10.0,3.0,0.0,6);
class Cube{
public:
    //float h = 1.0;
    bool u = true;
    bool j = false;
    float buffer = 0;
    float Position[3] = {0.0, 1.0, 0.0};
    float Direction[3] = {0.0, 0.0, 0.0};
    // referencia a la plataforma

    void draw(){
        glPushMatrix();
        glTranslatef(Position[0],Position[1],Position[2]);
        glColor3f(1.0,1.0,1.0);
        glutSolidCube(2.0);
        glPopMatrix();
    }

    void updateDirection(int dir){
       if (dir == 1){
            Direction[0] += 1.0;
       }
       else if (dir == 2){
            Direction[0] -= 1.0;
       }
       else if (dir == 3){
            Direction[2] += 1.0;
       }
       else if (dir == 4){
            Direction[2] -= 1.0;
       }
       else if (dir  == 5){
            j = true;
            if (Position[1] < 10.0 && u == true){
                Position[1]+=1.0;
                if (Position[1] == 10.0){
                    u = false;
                }
            }
            else if (u == false){
                Position[1]-=1.0;
                if (Position[1] == 1.0){
                    u = true;
                    j = false;
                }
            }
            else return;
                if (buffer == 1){
                    Position[0]+=1.0;
                }
                else if (buffer == 2){
                    Position[0]-=1.0;
                }
                else return;
       }

       else return;
    }
/*
    void update(){
        Position[0] += Direction[0];
        Position[1] += Direction[1];
        Position[2] += Direction[2];
        buffer = Direction[0];
        Direction[0] = 0.0;
        Direction[1] = 0.0;
        Direction[2] = 0.0;
    }
    */
void update(){
    // Verificar si el cubo está sobre la plataforma
    if (Position[0] >= plataforma.Position[0] - plataforma.Size/2 &&
        Position[0] <= plataforma.Position[0] + plataforma.Size/2 &&
        Position[2] >= plataforma.Position[2] - plataforma.Size/2 &&
        Position[2] <= plataforma.Position[2] + plataforma.Size/2 &&
        Position[1] < plataforma.Position[1] + 1.0 + 2.0/2 &&
        Position[1] > plataforma.Position[1] + 1.0 - 2.0/2)
    {
        Position[1] = plataforma.Position[1] + 1.0 + 2.0/2;
        j = false;
    }
    else {
        Position[1] += Direction[1];
        j = true;
    }

    Position[0] += Direction[0];
    Position[2] += Direction[2];
    buffer = Direction[0];
    Direction[0] = 0.0;
    Direction[1] = 0.0;
    Direction[2] = 0.0;
}

};

Cube cubo;

Plataforma p2(15.0,13.0,0.0,6);
Plataforma p3(20.0,23.0,0.0,6);
Plataforma p4(25.0,33.0,0.0,6);
 void update()
 {
     if (cubo.j == true)    cubo.updateDirection(5);
     cubo.update();
 }

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //drawAxis();


    glPushMatrix();
    glColor3f(0.53, 0.81, 0.92);
    glTranslatef(500.0f, 0.0f, 500.0f); // mover la esfera a una posición adecuada
    glutSolidSphere(1000.0, 100, 100); // crear la esfera con un radio de 1000 unidades y 100 segmentos
    glPopMatrix();

    glColor3f(0.0, 0.5, 0.0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]); // Asignar la textura al piso
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3d(-DimBoard, 0.0, -DimBoard);
        glTexCoord2f(1.0, 0.0); glVertex3d(DimBoard, 0.0, -DimBoard);
        glTexCoord2f(1.0, 1.0); glVertex3d(DimBoard, 0.0, DimBoard);
        glTexCoord2f(0.0, 1.0); glVertex3d(-DimBoard, 0.0, DimBoard);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    update();
    cubo.draw();
    plataforma.drawPlataforma();
    p2.drawPlataforma();
    p3.drawPlataforma();
    p4.drawPlataforma();

    Sleep(50);
    glutSwapBuffers();
}

void idle()
{
     display();
}

//ifs que logren captar si hay dos teclas presionadas al mismo tiempo
void keyboard(unsigned char key, int x, int y)
{
    int buffer = 0;
    if (key == 'd'){
        buffer = 1;
        cubo.updateDirection(1);
            EYE_X += cubo.Direction[0];
            //EYE_Y += cubo.Direction[1];
            EYE_Z += cubo.Direction[2];
            CENTER_X = cubo.Position[0];
            //CENTER_Y = cubo.Position[1];
            CENTER_Z = cubo.Position[2];
    }
    if (key == 'a'){
        buffer = 2;
        cubo.updateDirection(2);
            EYE_X += cubo.Direction[0];
            //EYE_Y += cubo.Direction[1];
            EYE_Z += cubo.Direction[2];
            CENTER_X = cubo.Position[0];
            //CENTER_Y = cubo.Position[1];
            CENTER_Z = cubo.Position[2];
    }
    if (key == 's'){
        buffer = 3;
        cubo.updateDirection(3);
            EYE_X += cubo.Direction[0];
            //EYE_Y += cubo.Direction[1];
            EYE_Z += cubo.Direction[2];
            CENTER_X = cubo.Position[0];
            //CENTER_Y = cubo.Position[1];
            CENTER_Z = cubo.Position[2];
    }
    if (key == 'w'){
        buffer = 4;
        cubo.updateDirection(4);
            EYE_X += cubo.Direction[0];
            //EYE_Y += cubo.Direction[1];
            EYE_Z += cubo.Direction[2];
            CENTER_X = cubo.Position[0];
            //CENTER_Y = cubo.Position[1];
            CENTER_Z = cubo.Position[2];
    }
    if (key == ' '){
        cubo.updateDirection(5);
        //cubo.j = true;
        cubo.buffer = buffer;
    }

    glLoadIdentity();
    gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    //Reactualizar el sistema grafico de acuerdo al observador
    //forzar a opengl
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(WIDTH, HEIGTH);
    glutCreateWindow("Juego de plataformas");
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    //glutSpecialFunc(SpecialInput);
    glutMainLoop();
    return 0;
}
