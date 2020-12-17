#include<stdio.h>
#include<GL/glut.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>


float width=800,height=600,bat_length=200,bat_height=20,ball_radius=8;
float batCenterCoordinate=width/2.0,ballCenterCoordinatex=width/2.0,ballCenterCoordinatey=height/2.0,ballspeedx=0,ballspeedy=8,batspeed=0,blockwidth=width/10,blockheight=height/10;
int gameover=0,blockx=(int)(width/blockwidth),blocky=(int)(height/blockheight)/2;
int gamenotstarted=1;
bool batchange=0;
int score=0;
int allblocks=0;

float timebonus=10000;

int specialx=rand()%10,specialy=rand()%5;

bool blockarray[800][600];

bool is_all_done(){
    for(int i=0;i<blockx;i++)
        for(int j=0;j<blocky;j++)
            if(blockarray[i][j])
                return false;
    return true;
}

float max(float a,float b){
    return a>b?a:b;
}

float min(float a,float b){
    return a<b?a:b;
}

void myinit(){
    glClearColor(1,1,1,1);
    gluOrtho2D(0,800,0,600);
    for(int i=0;i<blockx;i++)
        for(int j=0;j<blocky;j++)
            blockarray[i][j]=1;
    specialx=rand()%10;
    specialy=rand()%5;
}

void drawbat(){
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
        glVertex2f(batCenterCoordinate-bat_length/2.0,0.0);
        glVertex2f(batCenterCoordinate+bat_length/2.0,0.0);
        glVertex2f(batCenterCoordinate+bat_length/2.0,bat_height);
        glVertex2f(batCenterCoordinate-bat_length/2.0,bat_height);
    glEnd();
}

void drawblock(float x,float y,float r,float g,float b){
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+blockwidth,y);
        glVertex2f(x+blockwidth,y+blockheight);
        glVertex2f(x,y+blockheight);
    glEnd();
    int modify=10;
    x+=modify;
    y+=modify;
    glBegin(GL_LINE_LOOP);
        glVertex2f(x,y);
        glVertex2f(x+blockwidth-2*modify,y);
        glVertex2f(x+blockwidth-2*modify,y+blockheight-2*modify);
        glVertex2f(x,y+blockheight-2*modify);
    glEnd();
}

void drawblocks(){
    for(int i=0;i<blockx;i++){
        for(int j=0;j<blocky;j++){
            if(i==specialx&&j==specialy&&blockarray[i][j]){
                drawblock(i*blockwidth,j*blockheight+height/2,0,0,0);
                continue;
            }
            if(blockarray[i][j])
                drawblock(i*blockwidth,j*blockheight+height/2,(rand()%100)/100.0,(rand()%100)/100.0,(rand()%100)/100.0);
        }
    }
}

void boundary_check(){
    if(ballCenterCoordinatex+ball_radius>=width){
        ballspeedx*=-1;
        ballCenterCoordinatex=width-ball_radius-2;
    }
    if(ballCenterCoordinatex-ball_radius<=0){
        ballspeedx*=-1;
        ballCenterCoordinatex=ball_radius+2;
    }
    if(ballCenterCoordinatey+ball_radius>=height){
        ballspeedy*=-1;
        ballCenterCoordinatey=height-ball_radius-2;
    }
    if(ballCenterCoordinatey-ball_radius<=5){
        gameover=1;
        ballspeedy*=-1;
        ballspeedx=ballspeedy=0;
    }
}

void bat_check(){
    if(ballCenterCoordinatey-ball_radius<=bat_height&&(ballCenterCoordinatex<=batCenterCoordinate+bat_length/2.0+ball_radius&&ballCenterCoordinatex>=batCenterCoordinate-bat_length/2.0-ball_radius)){
        // ballspeedx*=-1;
        ballspeedy*=-1;
        if(ballspeedx*batspeed>0)
            ballspeedx+=batspeed;
        else
            ballspeedx-=batspeed;
    }
    if(ballspeedx>8)
        ballspeedx-=1;
    if(ballspeedy>8)
        ballspeedy-=1;
}

void getpos(int x,int y,int *a,int *b){
    *a=(int)(x/blockwidth);
    *b=(int)(y/blockheight)-(int)(height/blockheight)/2;
}

void block_check(){
    if(ballCenterCoordinatey+ball_radius<height/2)
        return;
    int a,b;
    int ch=0;
    getpos((int)(ballCenterCoordinatex),(int)(ballCenterCoordinatey-ball_radius),&a,&b);
    if(blockarray[specialx][specialy]==false&&batchange==false){
        bat_length*=1.5;
        batchange=true;
    }
    getpos((int)(ballCenterCoordinatex+ball_radius),(int)(ballCenterCoordinatey),&a,&b);
    if(blockarray[a][b]==true){
        ballspeedx*=-1;
        score+=200;
    }
    blockarray[a][b]=false;
    getpos((int)(ballCenterCoordinatex-ball_radius),(int)(ballCenterCoordinatey),&a,&b);
    if(blockarray[a][b]==true){
        ballspeedx*=-1;
        score+=200;
    }
    blockarray[a][b]=false;
    getpos((int)(ballCenterCoordinatex),(int)(ballCenterCoordinatey+ball_radius),&a,&b);
    if(blockarray[a][b]==true){
        ballspeedy*=-1;
        score+=200;
    }
    blockarray[a][b]=false;
    getpos((int)(ballCenterCoordinatex),(int)(ballCenterCoordinatey-ball_radius),&a,&b);
    if(blockarray[a][b]==true){
        ballspeedy*=-1;
        score+=200;
    }
    blockarray[a][b]=false;
}

void moveball(){
    boundary_check();
    bat_check();
    block_check();
    ballCenterCoordinatex+=ballspeedx;
    ballCenterCoordinatey+=ballspeedy;
}

void drawball(){
    glColor3f(0,0,1);
    glBegin(GL_POLYGON);
    float thet=0.0;
        for(int i=0;i<360;i+=20){
            thet=3.14159*i/180.0;
            glVertex2f(ballCenterCoordinatex+ball_radius*cos(thet),ballCenterCoordinatey+ball_radius*sin(thet));
        }
    glEnd();
}

void drawscore(){
    glRasterPos2f(9,70);
    char str[20];
    sprintf(str,"Score: %05d",score);
    for(int i=0;str[i]!=0;i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    glRasterPos2f(8,50);
    timebonus-=0.02;
    timebonus=max(timebonus,0);
    sprintf(str,"Time Bonus: %d",(int)timebonus);
    for(int i=0;str[i]!=0;i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
    glRasterPos2f(10,30);
    bat_length==200?sprintf(str,"Power Up: %s","Unused"):sprintf(str,"Power Up: %s","In Use");
    for(int i=0;str[i]!=0;i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str[i]);
}

void prinString(float x,float y,void *font,char *string) {
  char *c;
  glRasterPos2f(x, y);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void continue_game(){
        drawbat();
        moveball();
        drawblocks();
        drawball();
        drawscore();
}

void start_game(){
    char str[100];
		strcpy(str,"DX-BALL");
        glColor3f(1,0,0);
		prinString(350,500,GLUT_BITMAP_HELVETICA_18,str);
		strcpy(str,"Menu");
		prinString(300,450,GLUT_BITMAP_HELVETICA_18,str);
		strcpy(str,"S Start Game");
		prinString(290,430,GLUT_BITMAP_HELVETICA_18,str);
		strcpy(str,"Q Quit");
		prinString(290,410,GLUT_BITMAP_HELVETICA_18,str);
}

void game_over(){
    char str[100];
        glColor3f(1,0,0);
		strcpy(str,"Game Over");
		prinString(350,500,GLUT_BITMAP_HELVETICA_18,str);
        score=is_all_done()?score+(int)timebonus:score;
        sprintf(str,"Score: %d",score);
		prinString(300,450,GLUT_BITMAP_HELVETICA_18,str);
		strcpy(str,"Q Quit Game");
		prinString(290,430,GLUT_BITMAP_HELVETICA_18,str);
}

void mydisplay(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    if(gameover){
        game_over();
    }
    else{
        if(gamenotstarted)
            start_game();
        else
            continue_game();
    }
    glutSwapBuffers();
    glutPostRedisplay();
}

void motionfunc(int x,int y){
    float temp=batCenterCoordinate;
    batCenterCoordinate=min(width-bat_length/2.0,(float)x);
    batCenterCoordinate=max(bat_length/2.0,batCenterCoordinate);
    batspeed=batCenterCoordinate-temp;
}

void keyboardfunc(unsigned char key,int x,int y){
    if(gamenotstarted){
        if(key=='s')
            gamenotstarted=0;
        if(key=='q')
            exit(0);
    }
    if(gameover){
        if(key=='q')
            exit(0);
    }
}

int main(int argc,char *argv[]){
    srand(time(0));
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Mydx");
    glutDisplayFunc(mydisplay);
    glutPassiveMotionFunc(motionfunc);
    glutKeyboardFunc(keyboardfunc);
    myinit();
    glutMainLoop();
}