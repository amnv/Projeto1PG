#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <thread>
//#include "glut.h"
#include <chrono>
#include <GL/glut.h> 

using namespace std;

void desenhaGrade()
    {
        glColor3f(0.8f,0.8f,0.8f);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        float x = -10;
        for(;x<=10; x++)
        {
            glVertex2f(-10,x);
            glVertex2f( 10,x);
            glVertex2f(x,-10);
            glVertex2f(x, 10);
        }
        glEnd();
        glColor3f(0.0f,0.0f,0.0f);
        glLineWidth(3.0f);
        glBegin(GL_LINES);
            glVertex2f(-10,0);
            glVertex2f( 10,0);
            glVertex2f(0,-10);
            glVertex2f(0, 10);
        glEnd();

    } 

void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    //desenhaGrade();

    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);
    // glBegin(GL_QUADS);
    //     glColor3f(1.0f, 0.0f, 0.0f);
    //     glVertex2f(-0.8, 0.5);
    //     glVertex2f(0.8, -0.5);
    //     glVertex2f(0.8, 0);
    //     glVertex2f(-0.8, 0);
    // glEnd();

    
    glutSwapBuffers();
}

void Initialize() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( 0.0, 500.0, 0.0, 500.0, 1.0, -1.0 );
    glViewport( 0,0, 500, 500 );
    //glOrtho(10.0, 10.0, 1.0, 1.0, -1.0, 1.0);
    //glMatrixMode(GL_MODELVIEW);
}

void CliqueMouse(int button, int state, int x, int y){ 

    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
        printf("Clicou com o botão esquerdo na Posição  X : %i Y : %i \n",x,y);
        }else if (state == GLUT_UP){
        printf("Soltou Botão esquerdo\n");
        }
    }else if(button == GLUT_RIGHT_BUTTON){
        if(state == GLUT_DOWN){
        printf("Clicou com o botão direito na Posição  X : %i Y : %i \n",x,y);
        }else if(state == GLUT_UP){
        printf("Soltou Botão direito\n");
        }
    }else if (button == GLUT_MIDDLE_BUTTON){
        if(state == GLUT_DOWN){
        printf("Clicou com o botão do meio na Posição  X : %i Y : %i \n",x,y);
        }else if(state == GLUT_UP){
        printf("Soltou Botão do meio\n");
        }
    }
} 

vector<pair<int,int>> pontos;

void point()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();    

    // Preto: RGB especificado como valores de 0 a 1 (float)
    glColor3f(0.0f, 0.0f, 1.0f);
    glColor3f(1.0f, 0.0f, 0.0f); // vermelho
    glPointSize(10.0f); // aumenta o tamanho dos pontos
    glBegin(GL_POINTS);
        glVertex2f(-0.8,-0.5);
        glVertex2f( 0.8,-0.5);
        glVertex2f(-0.8, 0.0);
        glVertex2f( 0.8, 0.0);
        glVertex2f(-0.8, 0.5);
        glVertex2f( 0.8, 0.5);
    glEnd();
    glutSwapBuffers();
}

void Draw2()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_QUADS);
        //glColor3f(1.0f, 0.0f, 0.0f);
        glColor3f(0.0, 0.5, 0.9); 
        for (int i = 0; i < 4; ++i)
        {
          glVertex2f(pontos.back().first, pontos.back().second);
        }
        
    glEnd();
    glutSwapBuffers();
}

void DrawSquad(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (pontos.size() < 4)
        {
            pontos.push_back(make_pair(x/100,y/100));

            printf("x = %d,  y = %d\n", pontos.back().first, pontos.back().second);
            printf("x = %d,  y = %d\n ", x, y);
            if (pontos.size() == 4)  
            {
                glutDisplayFunc(Draw2);
                glutIdleFunc(Draw);
            }
        }
    }
}

int main(int iArgc, char** cppArgv) {
    glutInit(&iArgc, cppArgv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Interpolacao Bilineara Interativa");
    //glutDisplayFunc(Draw);
    //glutIdleFunc(Draw);
    glutDisplayFunc(point);
    glutMouseFunc(DrawSquad);
    Initialize();
    glutMainLoop();
    return 0;
}