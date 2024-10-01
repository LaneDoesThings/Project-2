/*
Graphics Pgm 2 for Lane Wright

The snowflake is animated using the timer_CB function that upadtes its global
position snowflakeY and calls glutPostRedisplay.

The snowman is animated using the keyboard_CB that updates its global posision
boxX whenever the keys 'k' or 'l' are pressed.

Text is generated using a bitmap and put in a dispay list in main.

The display is handled using display_CB and updates the position of the
snowflake and snowman, checks for collisions, and calls the text display list.


*/

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "OpenGL445Setup-aug24.h"
#include <iostream>
#include <math.h>

#define canvas_Width 600
#define canvas_Height 600
#define framerate 60.0
#define z_Plane -1 // Specified in assignment documentation
#define bottom_Box_Size 120
#define middle_Box_Size 100
#define top_Box_Size 80
#define snowflake_Size 50
#define snowflake_Angle 45
#define toRadians(degrees) (degrees * M_PI / 180.0)
#define hitList 1 // id for the display list when the snowman gets hit
#define startList                                                              \
  2 // id for the display list that shows the text at the beginning

void timer_CB(int id);

bool move = false;
bool hit = false;
int boxX = 270;
int boxBoundingLeft = boxX - (bottom_Box_Size / 2);
int boxBoundingRight = boxX + (bottom_Box_Size / 2);
int boxBoundingTop = bottom_Box_Size + middle_Box_Size + top_Box_Size;
int snowflakeHalfSize = snowflake_Size / 2;
int snowflakeX = ((snowflake_Size / 2) + 75);
int snowflakeY = (canvas_Height - 1) - (snowflake_Size / 2);
int snowflakeBoundingLeft = snowflakeX - snowflakeHalfSize;
int snowflakeBoundingRight = snowflakeX + snowflakeHalfSize;
int snowflakeBoundingBottom = snowflakeY - snowflakeHalfSize;
float msec = 0;
double diagonalX = snowflakeHalfSize * cos(toRadians(snowflake_Angle));
double diagonalY = snowflakeHalfSize * sin(toRadians(snowflake_Angle));

// Draws snowflake where the center point is at snowflakeX, snowflakeY
void snowflake() {
  glColor3f(0.8, 0.9, 0.0);
  glBegin(GL_LINES);

  // All of the following lines are drawn by going to the snowflake's center
  // point and adding a vertex half of the line width to the "left" and then to
  // the "right" For example with line width 50 and center point 30 the first
  // vertex is at 30-25=5 and the second vertex is at 30+25 = 55 which creates a
  // line centered at 30 that is 50 units long.

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

void snowman() {
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);

  // Bottom Box
  int bottomBoxMid = bottom_Box_Size / 2;
  int bottomboxLeft = boxX - bottomBoxMid;
  int bottomBoxRight = boxX + bottomBoxMid;
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
  int middleBoxleft = boxX - middleBoxMid;
  int middleBoxRight = boxX + middleBoxMid;
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
  int topBoxLeft = boxX - topBoxMid;
  int topBoxRight = boxX + topBoxMid;
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
}

void collision() {
  // Create a "box" around the snowflake that encapsulates it
  snowflakeBoundingLeft = snowflakeX - snowflakeHalfSize;
  snowflakeBoundingRight = snowflakeX + snowflakeHalfSize;
  snowflakeBoundingBottom = snowflakeY - snowflakeHalfSize;

  // Create a "box" around the snowman that encapsulates it
  boxBoundingLeft = boxX - (bottom_Box_Size / 2);
  boxBoundingRight = boxX + (bottom_Box_Size / 2);
  boxBoundingTop = bottom_Box_Size + middle_Box_Size + top_Box_Size;

  // If the snowflake's bounding box is inside the snowman's bounding box they
  // are intersecting
  if (snowflakeBoundingBottom < boxBoundingTop &&
      snowflakeBoundingRight > boxBoundingLeft &&
      snowflakeBoundingLeft < boxBoundingRight && !hit) {
    hit = true;
    glutTimerFunc(1200, timer_CB, 1); // Text timer
  }
}

// Callback for the display event
// Handles drawing things to screen and refreshing screen
void display_CB() {
  glClearColor(0.0, 0.0, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  if (hit) {
    glColor3f(1.0, 0.0, 0.0);
    glRasterPos2i(boxX - 40, bottom_Box_Size + (middle_Box_Size / 2));
    glCallList(hitList);
  }
  if (!move) {
    glCallList(startList);
  }

  snowman();   // Draw the snowman
  snowflake(); // Draw the snowflake
  collision(); // Check for collision between the snowman and snowflake
  glFlush();
}

// Callback that fires whenever an ascii key is pressed
void keyboard_CB(unsigned char key, int x, int y) {
  if (move == false) {
    move = true;
  }

  // If 'k' is pressed and the snowman isn't touching the left side of the
  // canvas
  if (key == 'k' && boxX - (bottom_Box_Size / 2) > 0) {
    boxX -= 3;
  }
  // If 'l' is pressed and the snowman isn't touching the right side of the
  // canvas
  else if (key == 'l' && boxX + (bottom_Box_Size / 2) < canvas_Width) {
    boxX += 3;
  }
}

// Callback that fires after set time
void timer_CB(int id) {
  // Framerate timer
  if (id == 0) {
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
  }
  // Hit text timer
  if (id == 1) {
    hit = false;
  }
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  char canvas_Name[] = "Project 2 - Lane Wright";
  glutInit(&argc, argv);
  my_setup(canvas_Width, canvas_Height, canvas_Name);

  // Display list for when the snowman gets hit
  glNewList(hitList, GL_COMPILE);
  glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char *)"Ouch, Cold");
  glEndList();

  // Display list for beginning
  glNewList(startList, GL_COMPILE);
  glColor3f(1.0, 1.0, 1.0);
  glRasterPos2i((canvas_Width / 2) - 100, canvas_Height - 20);
  glutBitmapString(GLUT_BITMAP_8_BY_13,
                   (const unsigned char *)"Press any key to Start");
  glEndList();

  // Set up event handlers
  glutDisplayFunc(display_CB);
  glutTimerFunc(1000 / framerate, timer_CB, 0);
  glutKeyboardFunc(keyboard_CB);

  glutMainLoop();
  return 0;
}