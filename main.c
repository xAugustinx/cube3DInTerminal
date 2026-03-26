//if ((int)b < wyokosc && (int)b >= 0 && i+x1 < szerokosc && i+x1 >= 0)
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef unsigned char byte;
#define TRUE 1
#define FALSE 0

#define szerokosc 150
#define wyokosc 100

char display[wyokosc][szerokosc];
int fov = 50,i;

typedef struct {float x; float y; float z;} Vec3;


Vec3 cam = {0,0,0};

Vec3 cubeE[] = {
    {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1},
    {-1,-1, 1}, {1,-1, 1}, {1,1, 1}, {-1,1, 1}
};
int edges[12][2] = {{0,1}, {1,2}, {2,3}, {3,0}, {4,5}, {5,6}, {6,7}, {7,4}, {0,4}, {1,5}, {2,6}, {3,7}};


char colorScale[] = ".,~;=/@#";
byte scaleAmount = 8;


void drawLine(int x0, int y0, int x1, int y1) {
    int punkty[4] = {x0, y0, x1, y1};
    if (x1 < x0) {
        punkty[0] = x1; punkty[1] = y1;
        punkty[2] = x0; punkty[3] = y0;
    }

    int dlugosci[] = {punkty[2]-punkty[0], punkty[3]-punkty[1] };
    float oneStep = (float)dlugosci[1]/(float)dlugosci[0];
    float y = punkty[1];

    for (int x =0; x < dlugosci[0]; x++ ) {
        
        int mnoznik = (oneStep > 0) ? 1 : -1;
        for (float i = 0; i < fabs(oneStep); i++) display[(int)y+(int)i*mnoznik ][punkty[0]+x] = '=';
        
        y+=oneStep;
    }


    if (x0 == x1) {
        for (int i = y0; i<y1; i++) display[i][x0] = '=';
        for (int i = y1; i<y0; i++) display[i][x0] = '=';
    }

}

void mnozenieMaciezyNya() {
    for (i = 0; i < 8; i++) {

        float pz = cubeE[i].z;
        cubeE[i].z= cubeE[i].z * cos(0.05f) - cubeE[i].x  * sin(0.05f);
        cubeE[i].x= pz * sin(0.05f)+cubeE[i].x * cos(0.05f);
        pz=cubeE[i].y;
        cubeE[i].y= cubeE[i].y * cos(0.03f) - cubeE[i].x  * sin(0.03f);
        cubeE[i].x=pz * sin(0.03f)+cubeE[i].x * cos(0.03f);
    }
}

Vec3 projections(Vec3 b){

    Vec3 a = {b.x-cam.x,b.y-cam.y,b.z-cam.z  };


    Vec3 point = {
        (szerokosc/2.0f + (a.x * 60 / a.z)),
        (wyokosc/2.0f + (a.y * 40 / a.z)), 0
    };

    return point;
}

int main()
{
    while (TRUE)
    {
        for(i=0;i<szerokosc;i++)for(int y=0;y<wyokosc;y++)display[y][i]=32;
        Vec3 cubeCopy[8];
        for (i=0;i<8;i++) {
            cubeCopy[i] = cubeE[i];
            cubeCopy[i].x+=1; cubeCopy[i].z+=4; cubeCopy[i].y-=1;
        };

        for (i = 0; i < 12; i++){

            Vec3 points[] = {projections(cubeCopy[edges[i][0]]),projections(cubeCopy[edges[i][1]])};
            byte czyP[] = {0,0};
            drawLine(points[0].x,points[0].y,points[1].x,points[1].y);
        }

        for (i=0;i<wyokosc;i++) {
            for (int j=0; j<szerokosc;j++) {printf("%c",display[i][j]); }
            printf("\n");
        }

        mnozenieMaciezyNya();

        usleep(32000);
        //printf("\033[H");
        system("cls");
    }
}
