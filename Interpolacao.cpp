#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <GL/glut.h> 

#define WIDTH 500
#define HEIGHT 500
#define POINT_SIZE 15

enum PontosCombinacao {PONTOS_AB, PONTOS_CD, PONTOS_AD, PONTOS_BC, PONTOS_AB_CD, PONTOS_AD_BC};
enum pontoInit {PONTO_A, PONTO_B, PONTO_C, PONTO_D};

using namespace std;

typedef pair<double,double> pdd;
typedef pair<pdd, pdd> pp;//pair points'

//definindo variaveis globais
vector<pdd> mainPoints;
vector<pdd> grade;
pdd *ponto;
int tamX, tamY;
bool arrastando = false;
bool pontosCriados = false;
bool atualizaPontos = false;

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



void Draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    //desenhaGrade();

    glLoadIdentity();
    glColor3f(1.0, 1.0, 1.0);

    glutSwapBuffers();
}

void Initialize() {
    glViewport(0, 0, WIDTH, HEIGHT);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearColor(0.f, .0f, 0.f, 1.0f);
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}

bool isInScreen(pdd p)
{
    return !(p.first > WIDTH || p.first < 0 || p.second > HEIGHT || p.second < 0);
}

pdd lerp(pdd A, pdd B, double j)
{
    double x, y;
    x = ((1 - j)*A.first) + (j*B.first);
    y = ((1 - j)*A.second) + (j*B.second);

    return make_pair(x, y);
}

pp findLineSize(pdd p1, pdd p2)
{
    pdd previous1, previous2;
    for (double i = 1.2;; i+=0.2)
    {
        pdd actual = lerp(p1, p2, i);
//        printf("%lf %lf %lf\n", actual.first, actual.second, i);
        if(isInScreen(actual)) 
            previous1 = actual;
        else break;       
    }

    for (double i = -0.2;; i-=0.2)
    {
        pdd actual = lerp(p1, p2, i);
        if(isInScreen(actual)) 
            previous2 = actual;
        else break;       
    }

    return make_pair(previous1, previous2);
}


void drawLine(pdd pp1, pdd pp2)
{
    //glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(1, 0, 0);
    int tam = mainPoints.size();
    
    pp pontos = findLineSize(pp1, pp2);
    pdd p1 = pontos.first;
    pdd p2 = pontos.second;
    glBegin(GL_LINES);
        glVertex2f(p1.first, p1.second);
        glVertex2f(p2.first, p2.second);
    glEnd();

    glutSwapBuffers();   
}

bool insideForLinePoints(pdd A, pdd B, pdd C, pdd D, double j)
{
    pdd AB = lerp(A, B, j);
    pdd DC = lerp(D, C, j);
    pdd AD = lerp(A, D, j);
    pdd BC = lerp(B, C, j);

    if(!isInScreen(AB) || !isInScreen(DC)) return false;

    drawLine(AB, DC);
    drawLine(AD, BC);

    return true;
}


void makeMainLines()
{
    int tam = mainPoints.size();
    for (int i = 0; i < tam; ++i)
    {
        drawLine(mainPoints[i%tam], mainPoints[(i+1)%tam]);
    }
}

void makeLinePoints(pdd A, pdd B, pdd C, pdd D)
{
    double x, y;

    makeMainLines();

    for (double j = -0.2;; j-= 0.2)
    {
        if(insideForLinePoints(A, B, C, D, j) == false) break;
    }
    
    for (double j = 1.2;; j+= 0.2)
    {
        if(insideForLinePoints(A, B, C, D,  j) == false) break;
    }
}


void makeGrid()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1, 1, 1);
    for (double i = 8.5; i < 1000.0; i+=10.0)
    {
        glBegin(GL_LINES);
            glVertex2f(i, 500.0);
            glVertex2f(i, 0.500000);
        glEnd();
    }

    for (double i = 8.5; i < 1000.0; i+=10.0)
    {
        glBegin(GL_LINES);
            glVertex2f(5.5, i);
            glVertex2f(500.5, i);
        glEnd();
    }

    glutSwapBuffers();
}

void point()
{
        //add point to screen
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();    
       if (mainPoints.size() < 4) makeGrid();

        //acionada caso um dos pontos seja trocado de lugar
        if (atualizaPontos)
        {
            grade.erase(grade.begin(), grade.end());
        }
        
        //Gera pontos a partir dos pontos iniciais
        if ((mainPoints.size() == 4 && !pontosCriados) || atualizaPontos)
        {
            makeLinePoints(mainPoints[PONTO_A], mainPoints[PONTO_B], 
                mainPoints[PONTO_C], mainPoints[PONTO_D]);
     
            pontosCriados = true;
            atualizaPontos = true;
        } 

        glColor3f(0, 0, 1);

    glPointSize(POINT_SIZE);
        glBegin(GL_POINTS);
            glColor3f(1, 0, 0);
            for (vector<pdd>::iterator it = mainPoints.begin(); it != mainPoints.end(); ++it)
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
              atualizaPontos = true;
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
    glutCreateWindow("Interpolacao Bilinear Interativa");
    glutDisplayFunc(makeGrid);
    glutIdleFunc(point);
    //glutDisplayFunc(point);
    glutMouseFunc(onCLick);
    glutPassiveMotionFunc(dragPoint);
    Initialize();
    glutMainLoop();
    return 0;
}