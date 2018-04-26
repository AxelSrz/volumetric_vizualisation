
#include "GL/glut.h"
#include "GL/freeglut.h"
#include "GL/gl.h"

#include <stdlib.h>

using namespace std;

/* Global variables */
char title[] = "3D Shapes";
int data [5][5][5];
double r1 = 0.0, g1 = 0.0, b1 = 1.0, r2 = 1.0, g2 = 0.0, b2 = 0.0;
double delta = 1.0;

GLfloat pos_x = 0, pos_y = 0;

void specialKeys( int key, int x, int y )
{
    const float step = 2.0;
    if (key == GLUT_KEY_RIGHT)
        pos_y += step;
    else if (key == GLUT_KEY_LEFT)
        pos_y -= step;
    else if (key == GLUT_KEY_UP)
        pos_x -= step;
    else if (key == GLUT_KEY_DOWN)
        pos_x += step;
    glutPostRedisplay();
}


void genTriangles(int x, int y, int z, int val) {
  double p = val / 12.0;
  double r = (1.0-p) * r1 + p * r2;
  double g = (1.0-p) * g1 + p * g2;
  double b = (1.0-p) * b1 + p * b2;

  glColor4f(r, g, b, 0.5f);     // Red

  //Bottom face
  glVertex3f(x*delta, y*delta, z*delta);
  glVertex3f(x*delta + delta, y*delta, z*delta);
  glVertex3f(x*delta, y*delta, z*delta + delta);

  glVertex3f(x*delta + delta, y*delta, z*delta + delta);
  glVertex3f(x*delta, y*delta, z*delta + delta);
  glVertex3f(x*delta + delta, y*delta, z*delta);

  //Left face
  glVertex3f(x*delta, y*delta, z*delta);
  glVertex3f(x*delta, y*delta, z*delta + delta);
  glVertex3f(x*delta, y*delta + delta, z*delta + delta);

  glVertex3f(x*delta, y*delta, z*delta);
  glVertex3f(x*delta, y*delta + delta, z*delta + delta);
  glVertex3f(x*delta, y*delta + delta, z*delta);

  //Right face
  glVertex3f(x*delta + delta, y*delta, z*delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta + delta);

  glVertex3f(x*delta + delta, y*delta, z*delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta + delta);
  glVertex3f(x*delta + delta, y*delta, z*delta + delta);

  //Back face
  glVertex3f(x*delta, y*delta, z*delta);
  glVertex3f(x*delta, y*delta + delta, z*delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta);

  glVertex3f(x*delta, y*delta, z*delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta);
  glVertex3f(x*delta + delta, y*delta, z*delta);

  //Front face
  glVertex3f(x*delta, y*delta, z*delta + delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta + delta);
  glVertex3f(x*delta, y*delta + delta, z*delta + delta);

  glVertex3f(x*delta, y*delta, z*delta + delta);
  glVertex3f(x*delta + delta, y*delta, z*delta + delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta + delta);

  //Top face
  glVertex3f(x*delta, y*delta + delta, z*delta);
  glVertex3f(x*delta, y*delta + delta, z*delta + delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta);

  glVertex3f(x*delta + delta, y*delta + delta, z*delta + delta);
  glVertex3f(x*delta + delta, y*delta + delta, z*delta);
  glVertex3f(x*delta, y*delta + delta, z*delta + delta);
}

/* Initialize OpenGL Graphics */
void initGL() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
  glClearDepth(1.0f);                   // Set background depth to farthest
  glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
  glShadeModel(GL_SMOOTH);   // Enable smooth shading
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears and
whenever the window needs to be re-painted. */
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
  glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

  // Render a pyramid consists of 4 triangles
  glLoadIdentity();                  // Reset the model-view matrix
  // Set the camera
  gluLookAt( 0.0f, 0.0f, 20.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f);

  glRotatef(pos_y, 0, 1, 0);
  glRotatef(pos_x, 1, 0, 0);

  glTranslatef(-2.5, -2.5, -2.5);

  glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
  for(int i=0; i<5; i++){
    for(int j=0; j<5; j++){
      for(int k=0; k<5; k++){
        genTriangles(i, j, k, data[i][j][k]);
      }
    }
  }

  glEnd();   // Done drawing the pyramid

  glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
}

  /* Handler for window re-size event. Called back when the window first appears and
  whenever the window is re-sized with its new width and height */
  void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
  }

  /* Main function: GLUT runs as a console application starting at main() */
  int main(int argc, char** argv) {

    for(int i=0; i<5; i++){
      for(int j=0; j<5; j++){
        for(int k=0; k<5; k++){
          data[i][j][k] = i+j+k;
        }
      }
    }

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);       // Register callback handler for window re-size event
    glutSpecialFunc(specialKeys);
    initGL();                       // Our own OpenGL initialization
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
  }
