#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <GL/glut.h> 

#define WIDTH 500
#define HEIGHT 500
#define POINT_SIZE 10

using namespace std;

//definindo variaveis globais
vector<pair<int,int>> *points;
pair<double, double> *ponto;
bool flag = false;
int tamX, tamY;


pair<double, double>* getPoint(int x1, int y1)
{
    for (vector<pair<double, double>>::iterator it = points->begin(); it != points->end(); ++it)
    {
        if (it->first == x1 && it->second == y1)
        {
            return it;
        }
    }



    return NULL;
}

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
    glViewport(0, 0, WIDTH, HEIGHT);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}


   /*  glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH/2, WIDTH/2, -HEIGHT/2, HEIGHT/2, -100, 100);
    glViewport( 0,0, 500, 500 );
    //glOrtho(10.0, 10.0, 1.0, 1.0, -1.0, 1.0);
    //glMatrixMode(GL_MODELVIEW);
}*/

void point()
{
    if (!flag)
    {
        //manage point's position

    } 
    else
    {
        //add point to screen
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();    

        glColor3f(1, 0, 0);
        glPointSize(POINT_SIZE);
        glBegin(GL_POINTS);
        
        for (vector<pair<int,int>>::iterator it = points.begin(); it != points.end(); ++it)
        {
            glVertex2f(it->first, it->second); 
        }
        flag = false; 
        glEnd();
        glutSwapBuffers();
    }
}

void onCLick(int button, int state, int x, int y)
{
    float half = (float) POINT_SIZE * 0.5f;
    double xTemp = x;
    double yTemp = y;
    double x1 = xTemp + half;
    double y1 = yTemp - half;

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        //add points
        if (points.size() < 4)
        {       
            points.push_back(make_pair(x1,y1));

            printf("x = %d,  y = %d\n", points.back().first, points.back().second);
            printf("x = %d,  y = %d\n ", x, y);  

            if (points.size() == 4) flag = true;
        }
        else 
        {
            //check clicked in a point
            pair<double, double> *aux = getPoint(x1, y1);
            if (aux != NULL)
            {
                ponto = aux;
            }
        }
    }
    else if (state == GLUT_UP)
    {
        ponto = NULL;
    }
}

void dragPoint(int x, int y)
{
    if (!ponto)  return;

    float half = (float) POINT_SIZE * 0.5f;
    double xTemp = x;
    double yTemp = y;
    double x1 = xTemp + half;
    double y1 = yTemp - half;    

    ponto->first = x1;
    ponto->second = y1;
}


int main(int iArgc, char** cppArgv) {
    //pegando dados da entrada
    freopen("entrada.in", "r", stdin);
    scanf("%d%d", &tamX, &tamY);


    glutInit(&iArgc, cppArgv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("Interpolacao Bilineara Interativa");
    glutDisplayFunc(point);
    glutIdleFunc(point);
    //glutDisplayFunc(point);
    glutMouseFunc(onCLick);
    glutPassiveMotionFunc(dragPoint);
    Initialize();
    glutMainLoop();
    return 0;
}