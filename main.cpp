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

inline void glFace3f(const Point3f &point1, const Point3f &point2, const Point3f &point3,
                     const Point3f &normal1, const Point3f &normal2, const Point3f &normal3) {
    glNormal3f(normal1.x, normal1.y, normal1.z);
    glVertex3f(point1.x, point1.y, point1.z);
    glNormal3f(normal2.x, normal2.y, normal2.z);
    glVertex3f(point2.x, point2.y, point2.z);
    glNormal3f(normal3.x, normal3.y, normal3.z);
    glVertex3f(point3.x, point3.y, point3.z);
}

inline void pyramid(Point3f top, float side){
    Point3f base1 = Point3f(top.x-side/2, top.y-side, top.z-side/2);
    Point3f base2 = Point3f(top.x-side/2, top.y-side, top.z+side/2);
    Point3f base3 = Point3f(top.x+side/2, top.y-side, top.z+side/2);
    Point3f base4 = Point3f(top.x+side/2, top.y-side, top.z-side/2);

    Point3f topNormal = Point3f(0,1,0);
    Point3f normal1 = Point3f(-1,-1,0);
    Point3f normal2 = Point3f(-1, 1,0);
    Point3f normal3 = Point3f( 1, 1,0);
    Point3f normal4 = Point3f( 1,-1,0);

    glColor3f(1,0,0);
    glFace3f(base1, base4, top, normal1, normal4, topNormal);
    glColor3f(0,1,0);
    glFace3f(base4, base3, top, normal4, normal3, topNormal);
    glColor3f(0,0,1);
    glFace3f(base3, base2, top, normal3, normal2, topNormal);
    glColor3f(1,0,1);
    glFace3f(base2, base1, top, normal2, normal1, topNormal);

    glColor3f(1,1,1);
    glFace3f(base1, base2, base4, normal1, normal2, normal4);
    glFace3f(base2, base3, base4, normal2, normal3, normal4);

}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*2000.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glTranslated(0,0,-8);
     //   glScaled(0.3,0.3,0.3);

        glPushMatrix();
        glRotated(a/100,0,1,0);

        glBegin(GL_TRIANGLES);
        for(int x=-10; x<=10; x++)
        for(int y=-10;y<=10; y++)
        {
            if( (x+y) % 2 == 0) continue;
            for(int i = -100; i<=100;i++)
            {
                pyramid(Point3f(x,i,y), 0.8);
            }
        }
        glEnd();
        glPopMatrix();
    glPopMatrix();

    glutSwapBuffers();
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

    glClearColor(0,0,0,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

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
