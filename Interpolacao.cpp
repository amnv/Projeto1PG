#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <GL/glut.h> 

#define WIDTH 500
#define HEIGHT 500
#define POINT_SIZE 10

enum PontosCombinacao {PONTOS_AB, PONTOS_CD, PONTOS_AD, PONTOS_BC, PONTOS_AB_CD, PONTOS_AD_BC};
enum pontoInit {PONTO_A, PONTO_B, PONTO_C, PONTO_D};

using namespace std;

typedef pair<double,double> pdd;

//definindo variaveis globais
vector<pdd> mainPoints;
vector<vector<pdd>> grade(6);
pdd *ponto;
int tamX, tamY;
bool arrastando = false;
bool pontosCriados = false;

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

void makeLinePoints(pdd p1, pdd p2, int pos)
{
    double x, y;
    for (double j = 1;; j+= 0.2)
    {
        printf("adiconando pontos\n");
        printf("adiconando na posicao %d\n", pos);
        x = ((1 - j)*p1.first) + (j*p2.first);
        y = ((1 - j)*p1.second) + (j*p2.second);
        if(x > WIDTH || x < 0 || y > HEIGHT || y < 0) break;
        grade[pos].push_back(make_pair(x,y));
    }
    for (double j = 1;; j-= 0.2)
    {
        x = ((1 - j)*p1.first) + (j*p2.first);
        y = ((1 - j)*p1.second) + (j*p2.second);
        if(x > WIDTH || x < 0 || y > HEIGHT || y < 0) break;
     grade[pos].push_back(make_pair(x,y));
    }
}

void makePointsFromVector(int posVector1, int posVector2, int posPontos)
{
    int tam1 = grade[posVector1].size();
    int tam2 = grade[posVector2].size();
    for (int i = 0; i < tam1 && i < tam2 ; i++)
    {
        makeLinePoints(grade[posVector1][i],
            grade[posVector2][i],posPontos);
    }
}

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
        }

        
        //Gera pontos a partir dos pontos iniciais
        if (mainPoints.size() > 3 && !pontosCriados)
        {
            makeLinePoints(mainPoints[PONTO_A], mainPoints[PONTO_B], PONTOS_AB);
            makeLinePoints(mainPoints[PONTO_A], mainPoints[PONTO_D], PONTOS_AD);
            makeLinePoints(mainPoints[PONTO_B], mainPoints[PONTO_C], PONTOS_BC);
            makeLinePoints(mainPoints[PONTO_C], mainPoints[PONTO_D], PONTOS_CD);
            pontosCriados = true;
        
            //  printf("vai gerar pontos\n");
            //gera pontos a partir dos pontos computados
            //tem de estar na mesm ordem que os colocado a cima
            makePointsFromVector(PONTOS_AB, PONTOS_CD, PONTOS_AB_CD);
            makePointsFromVector(PONTOS_AD, PONTOS_BC, PONTOS_AD_BC);
        } 

        //printf("%ld\n", grade[0].size());

        //printf("gerou\n");
        glColor3f(1, 1  , 0);
        for(int i = 0; i < grade.size(); i++)
        {
            for (vector<pdd>::iterator it = grade[i].begin(); it != grade[i].end(); ++it)
            {
                printf("gerando\n");
                glVertex2f(it->first, it->second); 
            }
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