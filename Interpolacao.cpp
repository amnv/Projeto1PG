#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <GL/glut.h> 

#define WIDTH 500
#define HEIGHT 500
#define POINT_SIZE 5

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

bool insideForPoint(pdd A, pdd B, pdd C, pdd D, int pos, double j)
{
    pdd AB = lerp(A,B, j);
    pdd DC = lerp(D, C, j);

    if(!isInScreen(AB) && !isInScreen(DC)) return false;

    grade[pos].push_back(AB);
    grade[pos].push_back(DC);

    for (double i = -0.2; ; i-=0.2)
    {
        pdd AB_DC = lerp(AB, DC, i);
        if(!isInScreen(AB_DC)) break;
        grade[pos].push_back(AB_DC);
    }

    for (double i = 1.2; ; i+=0.2)
    {
        pdd AB_DC = lerp(AB, DC, i);
        if(!isInScreen(AB_DC)) break;
        grade[pos].push_back(AB_DC);
    }
    return true;
}

void makeLinePoints(pdd A, pdd B, pdd C, pdd D, int pos)
{
    double x, y;

    for (double j = -0.02;; j-= 0.02)
    {
        if(insideForPoint(A, B, C, D, pos, j) == false) break;
    }
    
    for (double j = 1.02;; j+= 0.02)
    {
        if(insideForPoint(A, B, C, D, pos, j) == false) break;
    }
}

void point()
{
        //add point to screen
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();    


        //acionada caso um dos pontos seja trocado de lugar
        if (atualizaPontos)
        {
            for (int i = 0; i < grade.size(); ++i)
            {
                grade[i].erase(grade[i].begin(), grade[i].end());
            }
        }
        
        //Gera pontos a partir dos pontos iniciais
        if ((mainPoints.size() == 4 && !pontosCriados) || atualizaPontos)
        {
            makeLinePoints(mainPoints[PONTO_A], mainPoints[PONTO_B], 
                mainPoints[PONTO_C], mainPoints[PONTO_D], 0);
     
            //gera pontos a partir dos pontos computados
            //tem de estar na mesm ordem que os colocado a cima
            //makePointsFromVector(PONTOS_AB, PONTOS_CD, PONTOS_AB_CD);
            //makePointsFromVector(PONTOS_AD, PONTOS_BC, PONTOS_AD_BC);
            pontosCriados = true;
            atualizaPontos = false;
        } 

        //printf("%ld\n", grade[0].size());

        //printf("gerou\n");
        glColor3f(0, 0, 1);
        bool flag = false;
        /*for(int i = 0; i < grade.size(); i++)
        {
            glBegin(GL_LINE_STRIP);
            for (vector<pdd>::iterator it = grade[i].begin(); it != grade[i].end(); ++it)
            {
                glVertex2f(it->first, it->second); 
            }
            glEnd();
        }*/
   glBegin(GL_POINTS);
            for(int i = 0; i < grade.size(); i++)
        {
         
            for (vector<pdd>::iterator it = grade[i].begin(); it != grade[i].end(); ++it)
            {
                glVertex2f(it->first, it->second); 
            }
            
        }
glEnd();
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
    //glutDisplayFunc(point);
    glutIdleFunc(point);
    //glutDisplayFunc(point);
    glutMouseFunc(onCLick);
    glutPassiveMotionFunc(dragPoint);
    Initialize();
    glutMainLoop();
    return 0;
}