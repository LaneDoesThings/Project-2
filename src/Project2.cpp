
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include "OpenGL445Setup-aug24.h"

#define canvas_Width 600
#define canvas_Height 600
#define framerate 60.0
#define z_Plane -1 // Specified in assignment documentation
#define box_Center 270
#define bottom_Box_Size 120
#define middle_Box_Size 100
#define top_Box_Size 80
#define snowflake_Size 50
#define snowflake_Angle 45
#define toRadians(degrees) (degrees * M_PI / 180.0)

float msec = 0;
bool move = false;
int snowflakeHalfSize = snowflake_Size / 2;
int snowflakeX = ((snowflake_Size / 2) + 75);
int snowflakeY = (canvas_Height - 1) - (snowflake_Size / 2);
double diagonalX = snowflakeHalfSize * cos(toRadians(snowflake_Angle));
double diagonalY = snowflakeHalfSize * sin(toRadians(snowflake_Angle));

// Draws snowflakes where the center point is at snowflakeX, snowflakeY
void snowflake() {

  /*
  A note on the magic numbers 21.65063509 and 12.5.
  These are the X and Y coords needed for a line of length 50
  with a 60° angle. I would do this programmatically i.e (snowflake_Size / 2 *
  sin\cos(60°)) but the program assignment disallowed libraries and I don't know
  if math is allowed.
  */

  glColor3f(0.8,0.9,0.0);
  glBegin(GL_LINES);


  // Horizontal Line
  glVertex3d(snowflakeX - snowflakeHalfSize, snowflakeY, z_Plane);
  glVertex3d(snowflakeX + snowflakeHalfSize, snowflakeY, z_Plane);

  // Vertical Line
  glVertex3d(snowflakeX, snowflakeY + snowflakeHalfSize, z_Plane);
  glVertex3d(snowflakeX, snowflakeY - snowflakeHalfSize, z_Plane);

  // Diagonal Line 1
  glVertex3d(snowflakeX - diagonalX, snowflakeY + diagonalY, z_Plane);
  glVertex3d(snowflakeX + diagonalX, snowflakeY - diagonalY, z_Plane);

  // Diagonal Line 2
  glVertex3d(snowflakeX + diagonalX, snowflakeY + diagonalY, z_Plane);
  glVertex3d(snowflakeX - diagonalX, snowflakeY - diagonalY, z_Plane);

  glEnd();
}

// Callback for the display event
// Handles drawing things to screen and refreshing screen
void display_CB() {
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);

  // Bottom Box
  int bottomBoxMid = bottom_Box_Size / 2;
  int bottomboxLeft = box_Center - bottomBoxMid;
  int bottomBoxRight = box_Center + bottomBoxMid;
  glVertex3i(bottomboxLeft, 1, z_Plane);
  glVertex3i(bottomBoxRight, 1, z_Plane);
  glVertex3i(bottomBoxRight, 1, z_Plane);
  glVertex3i(bottomBoxRight, bottom_Box_Size, z_Plane);
  glVertex3i(bottomBoxRight, bottom_Box_Size, z_Plane);
  glVertex3i(bottomboxLeft, bottom_Box_Size, z_Plane);
  glVertex3i(bottomboxLeft, bottom_Box_Size, z_Plane);
  glVertex3i(bottomboxLeft, 1, z_Plane);

  // Middle Box
  int middleBoxMid = middle_Box_Size / 2;
  int middleBoxleft = box_Center - middleBoxMid;
  int middleBoxRight = box_Center + middleBoxMid;
  int middleBoxBottom = bottom_Box_Size + 1;
  int middleBoxTop = bottom_Box_Size + middle_Box_Size;
  glColor3f(1.0, 1.0, 1.0);
  glVertex3i(middleBoxleft, middleBoxBottom, z_Plane);
  glVertex3i(middleBoxRight, middleBoxBottom, z_Plane);
  glVertex3i(middleBoxRight, middleBoxBottom, z_Plane);
  glVertex3i(middleBoxRight, middleBoxTop, z_Plane);
  glVertex3i(middleBoxRight, middleBoxTop, z_Plane);
  glVertex3i(middleBoxleft, middleBoxTop, z_Plane);
  glVertex3i(middleBoxleft, middleBoxTop, z_Plane);
  glVertex3i(middleBoxleft, middleBoxBottom, z_Plane);

  // Top Box
  int topBoxMid = top_Box_Size / 2;
  int topBoxLeft = box_Center - topBoxMid;
  int topBoxRight = box_Center + topBoxMid;
  int topBoxBottom = bottom_Box_Size + middle_Box_Size + 1;
  int topBoxTop = bottom_Box_Size + middle_Box_Size + top_Box_Size;
  glColor3f(1.0, 1.0, 1.0);
  glVertex3i(topBoxLeft, topBoxBottom, z_Plane);
  glVertex3i(topBoxRight, topBoxBottom, z_Plane);
  glVertex3i(topBoxRight, topBoxBottom, z_Plane);
  glVertex3i(topBoxRight, topBoxTop, z_Plane);
  glVertex3i(topBoxRight, topBoxTop, z_Plane);
  glVertex3i(topBoxLeft, topBoxTop, z_Plane);
  glVertex3i(topBoxLeft, topBoxTop, z_Plane);
  glVertex3i(topBoxLeft, topBoxBottom, z_Plane);

  glEnd();

  snowflake(); // Draw the snowflake
  glFlush();
}

// Callback that fires whenever an ascii key is pressed
void keyboard_CB(unsigned char key, int x, int y) {
  move = true;
  

}

// Callback that fires after set time
void timer_CB(int id) {
  msec += 1000.0 / framerate;

  // If it has been 20 miliseconds and the keyboard has been pressed
  // Move the snowflake
  if (msec >= 20 && move) {
    // We have hit the bottom of the screen
    if ((snowflakeY - snowflakeHalfSize) <= 0) {
      snowflakeY = (canvas_Height - 1) - (snowflake_Size / 2);
    }
    msec = 0;
    snowflakeY -= 4;
  }

  // Run the timer again
  glutTimerFunc(1000 / framerate, timer_CB, 0);
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  char canvas_Name[] = "Project 2 - Lane Wright";
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  // Set up event handlers
  glutDisplayFunc(display_CB);
  glutTimerFunc(1000 / framerate, timer_CB, 0);
  glutKeyboardFunc(keyboard_CB);

  glutMainLoop();
  return 0;
}