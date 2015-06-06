/*
 -----------------------------------------------------------------------------
 OpenGL Tutorial
 VOXAR Labs
 Computer Science Center - CIn
 Federal University of Pernambuco - UFPE
 http://www.cin.ufpe.br/~voxarlabs
 
 Referencias:
 Funcoes de C/C++:
	http://www.cplusplus.com/
 Copia online do Red Book (OpenGL Programming Guide):
	http://fly.cc.fer.hr/~unreal/theredbook/
 Referencia online para os comandos OpenGL (Man pages):
	http://www.opengl.org/sdk/docs/man/
 
 -----------------------------------------------------------------------------
 */

#include "openGL_tutorial.h"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include "WorldObject.h"

#define MAX_CONTROLPOINTS 100

GLfloat mouse_x, mouse_y;
GLfloat window_width = 800.0;
GLfloat window_height = 800.0;

vector3 observerPosition = { 0, 10, 0 };
vector3 observerRotation = { 0, 0, 0 };

vector3 clickRotation;

WorldObject obj;

bool mouseDown = false;

int xToScreen(float x);
int yToScreen(float x);
float screenToX(int x);
float screenToY(int x);

void multiply4(float* a, float* b, float* result);

void updateCameraMatrix()
{
    glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    
    /*float fov = 45;
    float aspect = (float)window_width/(float)window_height;
    float near = observerPosition.z + observerVector.z;//1.f;
    float far = near + 100.f;
    
    /*float top = near * tanf((M_PI / 180.f) * (fov / 2.f));
    float bottom = -top;
    float right = top * aspect;
    float left = -right;*
    
    float top = observerPosition.y + (near * tanf((M_PI / 180.f) * (fov / 2.f)));
    float bottom = observerPosition.y - top;
    float right = (top * aspect);
    float left = -right;
    
    GLfloat result[16] =
    {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };
    GLfloat qmatrix[16] =
    { (2.f-near)/(right-left), 0, (right+left)/(right-left), 0,
        0, (2.f-near)/(top-bottom), (top+bottom)/(top-bottom), 0,
        0, 0, -((far+near)/(far-near)), -((2.f*far*near)/(far-near)),
        0, 0, -1.f, 0 };
    
    GLfloat translation[16] =
    {
        1, 0, 0, -observerPosition.x,
        0, 1, 0, -observerPosition.y,
        0, 0, 1, -observerPosition.z,
        0, 0, 0, 1
    };

    /*GLfloat rotationX[16] =
    {
        1, 0, 0, 0,
        0, cosf(observerRotation.x * (M_PI / 180.f)), -sinf(observerRotation.x * (M_PI / 180.f)), 0,
        0, sinf(observerRotation.x * (M_PI / 180.f)), cosf(observerRotation.x * (M_PI / 180.f)), 0,
        0, 0, 0, 1
    };
    
    GLfloat rotationY[16] =
    {
        cosf(observerRotation.y), 0, sinf(observerRotation.y), 0,
        0, 1, 0, 0,
        -sinf(observerRotation.y), 0, cosf(observerRotation.y), 0,
        0, 0, 0, 1
    };*/

    //multiply4(matrix, rotationX, result);
    //multiply4(matrix, rotationY, result);
    //multiply4(matrix, translation, result);
    
    GLfloat matrix[16] =
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    //glLoadMatrixf(matrix);
    glLoadIdentity();
    gluPerspective(45, (float)window_width/(float)window_height, 1.f, 100.f);
    glRotatef(observerRotation.x, 1, 0, 0);
    glRotatef(observerRotation.y, 0, 1, 0);
    glTranslatef(-observerPosition.x,
                 -observerPosition.y,
                 -observerPosition.z);

}

void myreshape (GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    window_width = (GLfloat) w;
    window_height = (GLfloat) h;
    
    //gluPerspective(45, (float)w/(float)h, 1.f, 100.f);
    //glOrtho(0, window_width, window_height, 0, -1.0, -1.0);
    
    updateCameraMatrix();
    //glTranslatef(-3.f, -3.0, -10.f);
    //glRotatef(-45, 0, 0, 0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //glClearDepth(1.0f);
    //glEnable(GL_DEPTH_TEST);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void multiply4(GLfloat* a, GLfloat* b, GLfloat* result)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < 4; inner++) {
                int index = (row * 4) + col;
                int indexa = (row * 4) + inner;
                int indexb = (inner * 4) + col;
                result[index] += a[indexa] * b[indexb];
            }
        }
    }
}

void drawFloor()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    
    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);
    
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1000, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    
    float step = 1.f;
    int count = 1000;
    for(int x = -count; x < count; x++)
    {
        float pos = step * (float)x;
        
        glColor3f(.2, .2, .2);
        glVertex3f(-100000, 0, pos);
        glVertex3f(100000, 0, pos);
        
        glVertex3f(pos, 0, -100000);
        glVertex3f(pos, 0, 100000);
    }

    glEnd();
}

// aqui o sistema de coordenadas da tela est· variando de -1 a 1 no eixo x e y
void mydisplay()
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    drawFloor();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    obj.Draw();
    
    glFlush();
    //glutPostRedisplay();
}

void handlePassiveMotion(int x, int y)
{
    
}

void handleMotion(int x, int y)
{
    observerRotation.x = clickRotation.x + (y - mouse_y) * 0.1f;
    observerRotation.y = clickRotation.y + (x - mouse_x) * 0.1f;
    updateCameraMatrix();
    glutPostRedisplay();
}

void handleMouse(int btn, int state, int x, int y)
{
    if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        clickRotation = observerRotation;
        mouseDown = true;
        mouse_x = x;
        mouse_y = y;
    }
    else if(state == GLUT_UP)
    {
        mouseDown = false;
    }
    
    printf("handleMouse(%i, %i, %i, %i)\n", btn, state, x, y);
}

float screenToX(int x)
{
    float coord = ((float)x / (float)window_width);
    return (coord - 0.5) * 2;
}

float screenToY(int x)
{
    float coord = 1 - ((float)x / (float)window_height);
    return (coord - 0.5) * 2;
}

int xToScreen(float x)
{
    // f(x) = 2x - 1
    // f(x) + 1 = 2x
    // (f(x) + 1) / 2 = x
    
    float coord = (x + 1.0f) / 2.0f;
    return coord * (float)window_width;
}

int yToScreen(float x)
{
    float coord = (x + 1.0f) / 2.0f;
    return (1 - coord) * (float)window_height;
}

void hadleKeyboard(unsigned char key, int x, int y)
{
    if(key == ESC){
        exit(0);
    }
    else if(key == '1')
    {
        obj.Translate(-0.1, 0, 0);
    }
    else if(key == '2')
    {
        obj.Translate(0.1, 0, 0);
    }
    else if(key == '3')
    {
        obj.Translate(0, -0.1, 0);
    }
    else if(key == '4')
    {
        obj.Translate(0, 0.1, 0);
    }
    else if(key == '5')
    {
        obj.Translate(0, 0, -1);
    }
    else if(key == '6')
    {
        obj.Translate(0, 0, 0.1);
    }
    else if(key == '7')
    {
        obj.Rotate(10, 0, 0);
    }
    else if(key == '8')
    {
        obj.Rotate(0, 10, 0);
    }
    else if(key == '9')
    {
        obj.Rotate(0, 0, 10);
    }
    else if(key == '-' || key == '_')
    {
        obj.AddScale(-.01f);
    }
    else if(key == '+' || key == '=')
    {
        obj.AddScale(.01f);
    }
    else if(key == 'w')
    {
        // when (0, 0, 0) z--
        // when (90, 0, 0) y--
        // when (0, 90, 0) x++
        // when (0, 0, 90) z--
        
        observerPosition.x += .1f * sinf(observerRotation.y * M_PI / 180.f);
        observerPosition.y -= .1f * sinf(observerRotation.x * M_PI / 180.f);
        observerPosition.z -= .1f * cosf(observerRotation.y * M_PI / 180.f);
        updateCameraMatrix();
    }
    else if(key == 'a')
    {
        observerPosition.x -= .1f * cosf(observerRotation.y * M_PI / 180.f);
        observerPosition.z -= .1f * sinf(observerRotation.y * M_PI / 180.f);
        updateCameraMatrix();
    }
    else if(key == 'd')
    {
        observerPosition.x += .1f * cosf(observerRotation.y * M_PI / 180.f);
        observerPosition.z += .1f * sinf(observerRotation.y * M_PI / 180.f);
        updateCameraMatrix();
    }
    else if(key == 's')
    {
        observerPosition.x -= .1f * sinf(observerRotation.y * M_PI / 180.f);
        observerPosition.y += .1f * sinf(observerRotation.x * M_PI / 180.f);
        observerPosition.z += .1f * cosf(observerRotation.y * M_PI / 180.f);
        updateCameraMatrix();
    }
    
    glutPostRedisplay();
}

void hadleSpecialKeyboard(int key, int x, int y)
{
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("OpenGL");
    
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(mydisplay);
    glutReshapeFunc(myreshape);
    glutMouseFunc(handleMouse);
    glutPassiveMotionFunc(handlePassiveMotion);
    glutMotionFunc(handleMotion);
    glutKeyboardUpFunc(hadleKeyboard);
    glutSpecialUpFunc(hadleSpecialKeyboard);
    
    glEnable(GL_CULL_FACE);
    glDepthMask(true);
    glDepthFunc(GL_LEQUAL);
    
    obj.SetModel("/Users/igorlira/Desktop/yoda.obj");
    //obj.AddScale(-.9);
    obj.Rotate(80, 90, 0);
    //obj.Translate(0, 0, 100);
    
    glutMainLoop();
    return 0;
}
