#include <GL/freeglut.h>
#include <GL/gl.h>

void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  glClearColor(1.0, 1.0, 1.0, 1.0);

  glutSwapBuffers();
  glutPostRedisplay();
}

void myIdle() {}

void myKeyboard(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // ESC
    glutLeaveMainLoop();
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(400, 300);
  glutCreateWindow("CS6610 homework1");

  glutDisplayFunc(myDisplay);
  glutIdleFunc(myIdle);
  glutKeyboardFunc(myKeyboard);

  glutMainLoop();

  return 0;
}