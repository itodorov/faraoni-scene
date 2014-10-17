/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include <windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>
#include <cmath>

static int slices = 16;
static int stacks = 16;

/* GLUT callback Handlers */

struct Point3f
{
    float x,y,z;

    Point3f(): x(0), y(0), z(0) {}
    Point3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

void normalize (Point3f *v)
{
    // calculate the length of the vector
    float len = (float)(sqrt((v->x * v->x) + (v->y * v->y) + (v->z * v->z)));

    // avoid division by 0
    if (len == 0.0f)
        len = 1.0f;

    // reduce to unit size
    v->x /= len;
    v->y /= len;
    v->z /= len;
}

Point3f get_normal (const Point3f &v0, const Point3f &v1, const Point3f &v2)
{
    Point3f a, b;

    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
    a.x = v0.x - v1.x;
    a.y = v0.y - v1.y;
    a.z = v0.z - v1.z;
    // b
    b.x = v1.x - v2.x;
    b.y = v1.y - v2.y;
    b.z = v1.z - v2.z;

    // calculate the cross product and place the resulting vector
    // into the address specified by vertex_t *normal
    Point3f normal;
    normal.x = (a.y * b.z) - (a.z * b.y);
    normal.y = (a.z * b.x) - (a.x * b.z);
    normal.z = (a.x * b.y) - (a.y * b.x);

    // normalize
    //normalize(&normal);
    return normal;
}

inline void glFace3f(const Point3f &point1, const Point3f &point2, const Point3f &point3) {
    Point3f normal = get_normal(point1, point2, point3);
    glNormal3f(normal.x, normal.y, normal.z);
    glVertex3f(point1.x, point1.y, point1.z);
    glVertex3f(point2.x, point2.y, point2.z);
    glVertex3f(point3.x, point3.y, point3.z);
}

inline void pyramid(Point3f top, float height){
   // std::cout<<top.x<< " "<<top.y<<" " <<top.x<< " " <<height<<std::endl;
    Point3f base1 = Point3f(top.x-height, top.y-height, top.z-height);
    Point3f base2 = Point3f(top.x-height, top.y-height, top.z+height);
    Point3f base3 = Point3f(top.x+height, top.y-height, top.z+height);
    Point3f base4 = Point3f(top.x+height, top.y-height, top.z-height);

    glColor3f(0,0,0);
    glFace3f(base1, base2, top);
  //  glColor3f(0,1,0);
    glFace3f(base2, base3, top);
 //   glColor3f(0,0,1);
    glFace3f(base3, base4, top);
  //  glColor3f(1,0,1);
    glFace3f(base4, base1, top);

    glColor3f(1,1,1);
    glFace3f(base2, base1, base4);
    glFace3f(base3, base2, base4);

}

inline void rpyramid(Point3f top, float height){
   // std::cout<<top.x<< " "<<top.y<<" " <<top.x<< " " <<height<<std::endl;
    Point3f base1 = Point3f(top.x-height, top.y+height, top.z-height);
    Point3f base2 = Point3f(top.x-height, top.y+height, top.z+height);
    Point3f base3 = Point3f(top.x+height, top.y+height, top.z+height);
    Point3f base4 = Point3f(top.x+height, top.y+height, top.z-height);

    glColor3f(0,0,0);
    glFace3f(base1, top, base2);
  //  glColor3f(0,1,0);
    glFace3f(base2, top, base3);
 //   glColor3f(0,0,1);
    glFace3f(base3, top, base4);
  //  glColor3f(1,0,1);
    glFace3f(base4, top, base1);

    glColor3f(1,1,1);
    glFace3f(base2, base1, base4);
    glFace3f(base3, base2, base4);

}

static void renderScene1()
{
    for(int x=-10; x<=10; x++)
        for(int y=-10;y<=10; y++)
        {
            if( (x+y) % 2 == 0) continue;
            for(int i = -100; i<=100;i++)
            {
                pyramid(Point3f(x,i,y), 0.4);
            }
        }
}

static void drawRecursivePyramid(Point3f top, float height)
{
    if(height <= 1){
        pyramid(top, height);
        rpyramid(Point3f(top.x,-top.y,top.z), height);
        return;
    }

    Point3f base1 = Point3f(top.x-height/2, top.y-height/2, top.z-height/2);
    Point3f base2 = Point3f(top.x-height/2, top.y-height/2, top.z+height/2);
    Point3f base3 = Point3f(top.x+height/2, top.y-height/2, top.z+height/2);
    Point3f base4 = Point3f(top.x+height/2, top.y-height/2, top.z-height/2);

    drawRecursivePyramid(top, height / 2);
    drawRecursivePyramid(base1, height / 2);
    drawRecursivePyramid(base2, height / 2);
    drawRecursivePyramid(base3, height / 2);
    drawRecursivePyramid(base4, height / 2);

}

static void renderScene2()
{
     for(int x=-5; x<=5; x++)
        for(int y=-5;y<=5; y++)
        {
            if( (x+y) % 2 == 0) continue;
            if(std::abs(x) <=1 && std::abs(y)<=1) continue;

            for(int i = -1; i<=0;i++)
            {
                drawRecursivePyramid(Point3f(x*8,i*8,y*8), 8);
            }
        }
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*2000.0;

    const double scene1Fog = 0.1;
    const double scene2Fog = 0.03;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat fogColor[] = {1.f, 1.f, 1.f, 1};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogf(GL_FOG_DENSITY, scene2Fog + (sin(10*t) + 1) * scene2Fog);
    glHint (GL_FOG_HINT, GL_NICEST);
    glPushMatrix();
        glTranslated(0,0,-8);
     //   glScaled(0.3,0.3,0.3);

        glPushMatrix();
        glRotated(-a/100,0,1,0);

        glBegin(GL_TRIANGLES);

        renderScene2();

        glEnd();
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y);

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 0.0f, 5.0f, 1.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_FOG);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutMainLoop();

    return EXIT_SUCCESS;
}



static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}
