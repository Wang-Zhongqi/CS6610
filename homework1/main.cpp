#include <GL/freeglut.h>
#include <GL/gl.h>

void myDisplayFunc() {
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();

  glutSwapBuffers();
}

void myIdleFunc() {
  glClearColor(0, 0, 0, 1.0);

  glutPostRedisplay();
}

void myKeyboardFunc(unsigned char key, int x, int y) {
  switch (key) {
  case 27: // ESC
    glutLeaveMainLoop();
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char **argv) {
  if (argc < 0) {
    return 0;
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(1920, 1080);
  glutCreateWindow("CS6610 homework2");

  glutDisplayFunc(myDisplayFunc);
  glutIdleFunc(myIdleFunc);
  glutKeyboardFunc(myKeyboardFunc);

  glutMainLoop();

  return 0;
}