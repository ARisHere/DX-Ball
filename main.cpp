#include <stdio.h>
#include <graphics.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <math.h>


const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 700;
const int SUPPORT_WIDTH = 200;
const int SUPPORT_HEIGHT = 10;
const float SUPPORT_SPEED = 0.5;
const int BALL_RADIUS = 10;
const float BALL_SPEED = 0.2;
const int BRICK_ROWS = 5;
const int BRICK_COLUMNS = 15;
const int BRICK_WIDTH = 70;
const int BRICK_HEIGHT = 20;
const int BRICK_GAP = 10;


float supportX, supportY;
float ballX, ballY;
float ballDX, ballDY;
bool bricks[BRICK_ROWS][BRICK_COLUMNS];
bool keys[256];
bool startMoving = false;


void initOpenGL() {
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    supportX = WINDOW_WIDTH / 2 - SUPPORT_WIDTH / 2;
    supportY = WINDOW_HEIGHT - SUPPORT_HEIGHT - 5;

    ballX = supportX + SUPPORT_WIDTH / 2;
    ballY = supportY - BALL_RADIUS;

    ballDX = BALL_SPEED;
    ballDY = BALL_SPEED;

    for (int i = 0; i < BRICK_ROWS; ++i) {
        for (int j = 0; j < BRICK_COLUMNS; ++j) {
            bricks[i][j] = true;
        }
    }
}


void drawSupport() {
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(supportX, supportY);
    glVertex2f(supportX + SUPPORT_WIDTH, supportY);
    glVertex2f(supportX + SUPPORT_WIDTH, supportY + SUPPORT_HEIGHT);
    glVertex2f(supportX, supportY + SUPPORT_HEIGHT);
    glEnd();
}

void drawBall() {
    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= 360; i++) {
        float angle = i * 3.14159f / 180.0f;
        float x = ballX + BALL_RADIUS * cos(angle);
        float y = ballY + BALL_RADIUS * sin(angle);
        glVertex3f(x, y, 0);
    }
    glEnd();
}


void drawBricks() {
    for (int i = 0; i < BRICK_ROWS; ++i) {
        for (int j = 0; j < BRICK_COLUMNS; ++j) {
            if (bricks[i][j]) {
                int brickX = j * (BRICK_WIDTH + BRICK_GAP);
                int brickY = i * (BRICK_HEIGHT + BRICK_GAP);
                glColor3f(0, 1, 0);
                glBegin(GL_QUADS);
                glVertex2f(brickX, brickY);
                glVertex2f(brickX + BRICK_WIDTH, brickY);
                glVertex2f(brickX + BRICK_WIDTH, brickY + BRICK_HEIGHT);
                glVertex2f(brickX, brickY + BRICK_HEIGHT);
                glEnd();
            }
        }
    }
}

void MoveBall() {
    if (!startMoving)
        return;

    ballX += ballDX;
    ballY += ballDY;

    if (ballX - BALL_RADIUS <= 0 || ballX + BALL_RADIUS >= WINDOW_WIDTH)
        ballDX = -ballDX;

    if (ballY - BALL_RADIUS <= 0 || ballY + BALL_RADIUS >= WINDOW_HEIGHT)
        ballDY = -ballDY;

    if (ballY + BALL_RADIUS >= supportY && ballY - BALL_RADIUS <= supportY + SUPPORT_HEIGHT &&
        ballX + BALL_RADIUS >= supportX && ballX - BALL_RADIUS <= supportX + SUPPORT_WIDTH) {
        ballDY = -ballDY;
    }

    int brickWidthWithGap = BRICK_WIDTH + BRICK_GAP;
    int brickHeightWithGap = BRICK_HEIGHT + BRICK_GAP;
    int i = (ballY - BALL_RADIUS) / brickHeightWithGap;
    int j = ballX / brickWidthWithGap;
    if (i >= 0 && i < BRICK_ROWS && j >= 0 && j < BRICK_COLUMNS && bricks[i][j]) {
        bricks[i][j] = false;
        ballDY = -ballDY;
    }


    if (ballY - BALL_RADIUS >= WINDOW_HEIGHT) {
        ballX = WINDOW_WIDTH / 2;
        ballY = WINDOW_HEIGHT / 2;
        ballDX = BALL_SPEED;
        ballDY = BALL_SPEED;
        startMoving = false;
    }

}

void MoveSupport() {
    if (keys['a'] || keys['A'])
        supportX -= SUPPORT_SPEED;
    if (keys['d'] || keys['D'])
        supportX += SUPPORT_SPEED;

    if (supportX < 0) supportX = 0;
    if (supportX > WINDOW_WIDTH - SUPPORT_WIDTH) supportX = WINDOW_WIDTH - SUPPORT_WIDTH;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawSupport();
    drawBall();
    MoveBall();
    drawBricks();

    glutSwapBuffers();
}

void idle() {
    MoveSupport();
    glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y) {
    keys[key] = true;

    if (key == 13) {
        startMoving = true;
    }
}

void keyReleased(unsigned char key, int x, int y) {
    keys[key] = false;
}


int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("DX BALL by TEAM CHANGE MAKER");

    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);

    initOpenGL();
    glutMainLoop();

    return 0;
}
