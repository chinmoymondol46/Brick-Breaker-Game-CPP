#include<windows.h>
#include<MMSystem.h>
#include<stdio.h>
#include<iostream>
#include<GL/glut.h>
#include<string>
#include<sstream>

using namespace std;

const int brickAmount = 100;


float barX = 340, barY = 465, barWidth = 80, barheight = 5;
float ballX = 375, ballY=455, ballWH = 10, ballVelX = 0.1, ballVelY = 0.1;
int score = 0;
bool flag = true, flag2 = true;

struct bricks
{
    float x;
    float y;
    float width;
    float height;
    bool isAlive = true;
};
bricks bricksArray[brickAmount];

void createBricks()
{
    float brickX = 186.7, brickY = 50;
    for(int i=0;i<brickAmount;i++)
    {
        if(brickX > 574)
            {
            brickX = 186.7;
            brickY += 11;
            }
        bricksArray[i].x = brickX;
        bricksArray[i].y = brickY;
        bricksArray[i].width = 38.66;
        bricksArray[i].height = 10;
        brickX += 39.66;
    }
    glColor3ub(153, 0, 0);
    glBegin(GL_QUADS);
    for(int i=0; i<brickAmount; i++)
    {
        if(bricksArray[i].isAlive == true)
        {
            glVertex2f(bricksArray[i].x, bricksArray[i].y);
            glVertex2f(bricksArray[i].x + bricksArray[i].width, bricksArray[i].y);
            glVertex2f(bricksArray[i].x + bricksArray[i].width, bricksArray[i].y + bricksArray[i].height);
            glVertex2f(bricksArray[i].x, bricksArray[i].y + bricksArray[i].height);
        }
    }
    glEnd();
}
void print(int a)
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3ub(0, 0, 0);
    glRasterPos2f(660, 40);
    stringstream ss;
    ss << a;
    string s = "Score: "+ss.str();
    int len = s.length();
    for(int i = 0; i < len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
    glPopAttrib();
}
void message(bool a)
{
    if(a == false)
    {
        glPushAttrib(GL_CURRENT_BIT);
        glColor3ub(0, 0, 0);
        glRasterPos2i(230, 300);
        stringstream ss;
        ss << score;
        string s = "GAME OVER. Your Score: " +ss.str() + ". Click to Exit.";
        int len = s.length();
        for(int i = 0; i < len; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
        }
        glPopAttrib();
    }
}

void completeMessage(bool a)
{
    if(a == false)
    {
        glPushAttrib(GL_CURRENT_BIT);
        glColor3ub(0, 0, 0);
        glRasterPos2f(20, 400);
        stringstream ss;
        ss << score;
        string s = "STAGE COMPLETE. Your score: "+ss.str()+". Click to Exit.";
        int len = s.length();
        for(int i = 0; i < len; i++){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[i]);
        }
        glPopAttrib();
    }
}

bool checkCollision(float aX, float aY, float aW, float aH, float bX, float bY, float bW, float bH)
{
    if ( aY+aH < bY )
        return false;
    else if ( aY > bY+bH )
        return false;
    else if ( aX+aW < bX )
        return false;
    else if ( aX > bX+bW )
        return false;
    else
    {
        sndPlaySound("Bounce.wav", SND_ASYNC);
        return true;
    }
}

void moveBall()
{
    if(score >= 1000)
    {
        barX = 340;
        barY = 465;
        ballX = 375;
        ballY = 455;
        ballVelX = 0;
        ballVelY = 0;
        flag2 = false;
        Sleep(1000);
    }
    else
        {
        ballX += ballVelX;
        for (int i=0; i<brickAmount; i++)
        {
            if(bricksArray[i].isAlive == true)
            {
                if(checkCollision(ballX, ballY, ballWH, ballWH, bricksArray[i].x, bricksArray[i].y, bricksArray[i].width, bricksArray[i].height) == true)
                {
                    ballVelX = -ballVelX;
                    bricksArray[i].isAlive = false;
                    score += 10;
                    break;
                }
            }
        }
        ballY -= ballVelY;
        for (int i=0; i<brickAmount; i++)
        {
            if(bricksArray[i].isAlive == true){
                if(checkCollision(ballX, ballY, ballWH, ballWH, bricksArray[i].x, bricksArray[i].y, bricksArray[i].width, bricksArray[i].height) == true){
                    ballVelY = -ballVelY;
                    bricksArray[i].isAlive = false;
                    score += 10;
                    break;
                }
            }
        }
        if(ballX < 0)
            {
            sndPlaySound("Bounce.wav", SND_ASYNC);
            ballVelX = -ballVelX;
        }
        else if(ballX+ballWH >= 760)
        {
            sndPlaySound("Bounce.wav", SND_ASYNC);
            ballVelX = -ballVelX;
        }
        if(ballY < 0)
        {
            sndPlaySound("Bounce.wav", SND_ASYNC);
            ballVelY = -ballVelY;
        }
        else if(ballY+ballWH >= 480)
        {
            sndPlaySound("Game Over.wav", SND_ASYNC);
            barX = 340;
            barY = 465;
            ballX = 375;
            ballY = 455;
            ballVelX = 0;
            ballVelY = 0;
            flag = false;
            Sleep(1000);
        }
        if(checkCollision(ballX, ballY, ballWH, ballWH, barX, barY, barWidth, barheight) == true)
        {
            ballVelY = -ballVelY;
        }
    }
    glutPostRedisplay();
}
void Display(void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (0.0, 0.0, 0.0);

    glBegin(GL_QUADS); //ball
    glColor3ub(64, 64, 64);
    glVertex2f(ballX, ballY);
    glVertex2f(ballX+ballWH, ballY);
    glVertex2f(ballX+ballWH, ballY+ballWH);
    glVertex2f(ballX, ballY+ballWH);
    glEnd();

    glBegin(GL_QUADS); //bar
    glColor3ub(255, 153, 51);
    glVertex2f(barX, barY);
    glVertex2f(barX+barWidth, barY);
    glVertex2f(barX+barWidth, barY+barheight);
    glVertex2f(barX, barY+barheight);
    glEnd();

    print(score);
    createBricks();
    message(flag);
    completeMessage(flag2);

    glutSwapBuffers();
}
void init()
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glViewport(0, 0, 760, 480);
    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 760.0, 480.0, 0.0);
}
void keyboard(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_LEFT:
            barX -= 30;
            if(barX < 0)
            {
                barX = 0;
            }
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            barX += 30;
            if(barX+barWidth > 760)
            {
                barX = 480-barWidth;
            }
            glutPostRedisplay();
            break;
        default:
            break;
    }
}

void mouse(int button,int state,int x,int y)
{
    switch(button)
    {
        case GLUT_LEFT_BUTTON:
            if(state==GLUT_DOWN)
            {
                if((flag == false || flag2 == false) && ballX == 375 && ballY == 455)
                {
                    exit(3);
                }
                flag = true;
                if((ballVelX <= 0 && ballVelY <= 0)&& (ballX == 375 && ballY == 455))
                {
                    ballVelX = 0.1;
                    ballVelY = 0.1;
                }
                glutIdleFunc(moveBall);
            }
            break;
        default:
            break;
    }
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(760,480);
    glutInitWindowPosition(100,150);
    glutCreateWindow("Ball & Paddle (DX-Ball)");
    glutDisplayFunc(Display);
    glutSpecialFunc(keyboard);
    glutMouseFunc(mouse);
    init();
    glutMainLoop();
}
