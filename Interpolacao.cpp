#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <GL/glut.h> 

#define WIDTH 500
#define HEIGHT 500
#define POINT_SIZE 10

enum name {PONTO_AB, PONTO_CD, PONTO_AD, PONTO_BC};


using namespace std;

typedef pair<double,double> pdd;

//definindo variaveis globais
vector<pdd> mainPoints;
vector<vector<pdd>> grade;
pdd *ponto;
int tamX, tamY;
bool arrastando = false;


bool isInsideMargin(pdd point, double x, double y)
{
    double margin = 15;
    return ((point.first >= x && point.first <= x + margin) || (point.first <= x && point.first >= x - margin))
            && ((point.second >= y && point.second <= y + margin) || (point.second <= y && point.second >= y - margin));

}

pdd* getPoint(int x1, int y1)
{
    int t = mainPoints.size();
    for(int i = 0; i < t; ++i)
    {
        if(isInsideMargin(mainPoints[i], x1,y1))
            return &mainPoints[i];
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

void makeLinePoints(pdd p1, pdd p2)
{
    double x, y;
    for (double j = 1;; j+= 0.2)
    {
        x = ((1 - j)*p1.first) + (j*p2.first);
        y = ((1 - j)*p1.second) + (j*p2.second);
        if(x > WIDTH || x < 0 || y > HEIGHT || y < 0) break;
        grade.push_back(make_pair(x,y));
    }
    for (double j = 1;; j-= 0.2)
    {
        x = ((1 - j)*p1.first) + (j*p2.first);
        y = ((1 - j)*p1.second) + (j*p2.second);
        if(x > WIDTH || x < 0 || y > HEIGHT || y < 0) break;
        grade.push_back(make_pair(x,y));
    }
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
        //add point to screen
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();    

        glColor3f(1, 0, 0);
        glPointSize(POINT_SIZE);
        glBegin(GL_POINTS);
        for (vector<pdd>::iterator it = mainPoints.begin(); it != mainPoints.end(); ++it)
        {
            glVertex2f(it->first, it->second); 
            if (mainPoints.size() > 1 && grade.empty()) makeLinePoints(mainPoints[0], mainPoints[1]);
        }

        glColor3f(1, 1  , 0);
        for (vector<pdd>::iterator it = grade.begin(); it != grade.end(); ++it)
        {
            glVertex2f(it->first, it->second); 
        }

        glEnd();
        glutSwapBuffers();
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
        //add mainPoints
        if (mainPoints.size() < 4)
        {       
            mainPoints.push_back(make_pair(x1,y1));

            printf("x = %lf,  y = %lf\n", mainPoints.back().first, mainPoints.back().second);
            printf("x = %d,  y = %d\n ", x, y);  
        }
        else 
        {
            //check clicked in a point
            pdd *aux = getPoint(x1, y1);
            if (aux != NULL && !arrastando)
            {
                printf("entra\n");
                ponto = aux;
                arrastando = true;
            }
            else if(arrastando)
            {
              printf("não entra\n");
              ponto = NULL;  
              arrastando = false;
            } 
        }
    }
}

void dragPoint(int x, int y)
{

    if (!ponto)  return;
    printf("arrasta\n");
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
    //glutDisplayFunc(point);
    glutIdleFunc(point);
    //glutDisplayFunc(point);
    glutMouseFunc(onCLick);
    glutPassiveMotionFunc(dragPoint);
    Initialize();
    glutMainLoop();
    return 0;
}