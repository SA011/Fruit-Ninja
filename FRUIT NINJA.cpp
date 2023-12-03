# include "iGraphics.h"
#include<windows.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#define ROUND_OBJECT 0
#define BAD 1
#define GOOD 2
#define BONUS 3
#define IGNORE_COLOR 0xffffff

typedef struct
{
    double x,y;
}point;
typedef struct
{
    point p;
    double rad;
    double spd;
    double xspd;
    long long int t;
    int vis;
    int dir;
    char name[20];
}roundobjects;

typedef struct
{
    long long hgsc;
    char name[20];
}hg;

clock_t strt,endd;
const double err=0.000001;
const int goodrnd=14,badrnd=5,bonrnd=1,scrhg=800,scrwd=1000,framerate=8;
int pause=0,menu=0,nomove=0,miss=0,mouse_ind1=0,mouse_ind2=0,gameover=0,instruction=0,highscoreind=0,isnew=0,high_move_pos=0,boom_anim_start=0,menu_music=0,music_timer=0;
double appearance_timer=0,gap=0.2,gravity=0.1,in_game_timer2=0,crdt_ind=0;
long long int animation_timer=0,animation_existence=200,score=0;
point mouse_track[200000],scorepos,timerpos;
roundobjects goodr[goodrnd],badr[badrnd],bonusr[bonrnd],menuobject[4],chooseobjects[2];
char goodrc[goodrnd][20]= {"plum2.bmp","watermelon1.bmp","mango1.bmp","coconut1.bmp","apple2.bmp","orange1.bmp","lemon1.bmp"};                                           //PICTURE NAME ARRAYS
char badrc[badrnd][20]= {"bomb.bmp","bomb.bmp","bomb.bmp","bomb.bmp","bomb.bmp"};
char bonusrc[bonrnd][20]= {"bonusfruit.bmp"};
hg highscore[2][11];
int hg_ind=0,hg_ind1=0,high_score_height[5]={550,480,410,340,280},high_score_top=0,high_score_bottom=4,sorted=0,name_ind=0,time_trial=0,music_on=0,choose=0;
point hit;
void high_score_read()
{
    FILE *in=fopen("high_score.txt","r");
    FILE *in2=fopen("highscore_name.txt","r");
    hg_ind=0;
    while(fscanf(in,"%lld",&highscore[0][hg_ind].hgsc)!=EOF)hg_ind++;
    for(int i=0;i<hg_ind;i++)
        fscanf(in2,"%s",highscore[0][i].name);
    fclose(in);
    fclose(in2);
    in=fopen("high_score1.txt","r");
    in2=fopen("highscore_name1.txt","r");
    hg_ind1=0;
    while(fscanf(in,"%lld",&highscore[1][hg_ind1].hgsc)!=EOF)hg_ind1++;
    for(int i=0;i<hg_ind1;i++)
        fscanf(in2,"%s",highscore[1][i].name);
    fclose(in);
    fclose(in2);
}
void high_score_print()
{
    FILE *out=fopen("high_score.txt","w");
    FILE *out2=fopen("highscore_name.txt","w");
    for(int i=0;i<hg_ind;i++)
    {
        fprintf(out,"%lld\n",highscore[0][i].hgsc);
    }
    for(int i=0;i<hg_ind;i++)
    {
        fprintf(out2,"%s\n",highscore[0][i].name);
    }
    fclose(out);
    fclose(out2);
    out=fopen("high_score1.txt","w");
    out2=fopen("highscore_name1.txt","w");
    for(int i=0;i<hg_ind1;i++)
    {
        fprintf(out,"%lld\n",highscore[1][i].hgsc);
    }
    for(int i=0;i<hg_ind1;i++)
    {
        fprintf(out2,"%s\n",highscore[1][i].name);
    }
    fclose(out);
    fclose(out2);
}

int cmp(const void *a,const void *b)
{
    hg ta=*((hg *)a);
    hg tb=*((hg *)b);
    if(ta.hgsc < tb.hgsc)
        return 1;
    else if(ta.hgsc > tb.hgsc)
        return -1;
    else
        return 0;
}

void sort_high_score()
{
    highscore[time_trial][10].hgsc=score;
    qsort(highscore[time_trial],11,sizeof(hg),cmp);
    high_score_print();
    sorted=1;
}
void draw_credits()
{
    iShowBMP(0,0,"high_score_bg1.bmp");
    iSetColor(0,100,255);
    char s[][40]={"CSE 102 TEAM PROJECT","Created By:","MD. REDWANUL KARIM (1805111)","SK. SABIT BIN MOSADDEK (1805106)","Supervised by:","MD. SHARIFUL ISLAM BHUIYAN"};
    if(crdt_ind>=1&&crdt_ind<=390)iText(380,crdt_ind+200,s[0],GLUT_BITMAP_TIMES_ROMAN_24);
    if(crdt_ind>=80&&crdt_ind<=470)iText(450,crdt_ind+121,s[1],GLUT_BITMAP_TIMES_ROMAN_24);
    if(crdt_ind>=120&&crdt_ind<=510)iText(330,crdt_ind+81,s[3],GLUT_BITMAP_TIMES_ROMAN_24);
    if(crdt_ind>=155&&crdt_ind<=545)iText(340,crdt_ind+46,s[2],GLUT_BITMAP_TIMES_ROMAN_24);
    if(crdt_ind>=210&&crdt_ind<=600)iText(450,crdt_ind-9,s[4],GLUT_BITMAP_TIMES_ROMAN_24);
    if(crdt_ind>=240&&crdt_ind<=630)iText(340,crdt_ind-39,s[5],GLUT_BITMAP_TIMES_ROMAN_24);
    crdt_ind+=0.5;
    if(crdt_ind>630)crdt_ind=1;
}
void draw_hg_bg()
{
    highscoreind=1;
    iShowBMP(0,0,"high_score_bg1.bmp");
    if(time_trial)iText(450,700,"TIME TRIAL MODE",GLUT_BITMAP_TIMES_ROMAN_24);
    else iText(450,700,"CLASSIC MODE",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(100,100,100);
    iFilledRectangle(830,240,4,310);
    iSetColor(50,50,50);
    iFilledRectangle(820,240+(9-high_score_bottom)*31,24,155);
    iSetColor(100,100,100);
    iFilledRectangle(820,240+(9-high_score_bottom+1)*31,24,93);
    iShowBMP2(800,535,"arr1.bmp",0);
    iShowBMP2(800,200,"arr2.bmp",0);
    iSetColor(255,0,0);
    for(int i=high_score_top,j=0;i<=high_score_bottom;i++,j++)
    {
        char a[40],b[20];
        itoa(i+1,a,10);
        strcat(a," :  ");
        strcat(a,highscore[time_trial][i].name);
        iText(300,high_score_height[j],a,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(570,high_score_height[j],itoa(highscore[time_trial][i].hgsc,b,10),GLUT_BITMAP_TIMES_ROMAN_24);
    }
}
int instruction_x=870,instruction_y=740,instruction_l=60;

double dis(point a,point b)                                                  //DISTANCE BETWEEN TWO POINT
{
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
bool isinsideroundobject(roundobjects a,point p)                              //IF A POINT IS INSIDE THE ROUND OBJECT
{
    if(dis(a.p,p)<a.rad)return true;
    return false;
}
void drawobject(int typ,int ind)                                          //DRAWING  A SINGLE OBJECT AND ASSOCIATING ITS  OTHER CO-ORDINATES
{
    if(typ==BAD)
    {
        iShowBMP2(badr[ind].p.x-badr[ind].rad,badr[ind].p.y-badr[ind].rad,badr[ind].name,IGNORE_COLOR);
        if(badr[ind].p.y<-2*badr[ind].rad)badr[ind].vis=0,badr[ind].p.y=-30;

    }
    if(typ==GOOD)
    {
        iShowBMP2(goodr[ind].p.x-goodr[ind].rad,goodr[ind].p.y-goodr[ind].rad,goodr[ind].name,IGNORE_COLOR);
        if(goodr[ind].p.y<-2*goodr[ind].rad)goodr[ind].vis=0,goodr[ind].p.y=-30,miss++;

    }
    if(typ==BONUS)
    {

        iShowBMP2(bonusr[ind].p.x-bonusr[ind].rad,bonusr[ind].p.y-bonusr[ind].rad,bonusr[ind].name,IGNORE_COLOR);
        if(bonusr[ind].p.y<-2*bonusr[ind].rad)bonusr[ind].vis=0,bonusr[ind].p.y=-30,miss++;

    }
}

void drawall()                                                                              //DRAWING ALL THE OBJECTS
{
    for(int i=0; i<goodrnd; i++)
        if(goodr[i].vis)
            drawobject(GOOD,i);
    for(int i=0; i<badrnd; i++)
        if(badr[i].vis)
            drawobject(BAD,i);
    for(int i=0; i<bonrnd; i++)
        if(bonusr[i].vis)
            drawobject(BONUS,i);
}
void initonce()
{
    menu=1;
    highscoreind=0;
    score=0;
    scorepos.x=10;
    scorepos.y=750;
    animation_timer=1;
    music_timer=-1;
    animation_existence=300;
    timerpos.x=scrwd/2-10;
    timerpos.y=scrhg-20;
    for(int i=0; i<goodrnd; i++)
    {
        goodr[i].t=25*i+1;
        goodr[i].p.y=-30;
        goodr[i].vis=0;
        strcpy(goodr[i].name,goodrc[i%7]);
        if(i==3||i==10||i==2||i==9)goodr[i].rad=52;
        else goodr[i].rad=36;
    }
    for(int i=0; i<badrnd; i++)
    {
        badr[i].t=48*(i+1);
        badr[i].p.y=-30;
        badr[i].vis=0;
        strcpy(badr[i].name,badrc[i]);
        badr[i].rad=30;
    }
    for(int i=0; i<bonrnd; i++)
    {
        bonusr[i].t=150*(i+1);
        bonusr[i].p.y=-30;
        bonusr[i].vis=0;
        strcpy(bonusr[i].name, bonusrc[i]);
        bonusr[i].rad=38;
    }
    for(int i=0;i<4;i++)
    {
        strcpy(menuobject[i].name,goodrc[i]);
        menuobject[i].rad=goodr[i].rad;
    }
    strcpy(chooseobjects[0].name,goodrc[5]);
    chooseobjects[0].rad=goodr[5].rad;
    strcpy(chooseobjects[1].name,bonusrc[0]);
    chooseobjects[1].rad=bonusr[0].rad;
    menuobject[0].p.x=836;
    menuobject[0].p.y=126;
    menuobject[1].p.x=516;
    menuobject[1].p.y=216;
    menuobject[2].p.x=212;
    menuobject[2].p.y=192;
    menuobject[3].p.x=817;
    menuobject[3].p.y=377;
    chooseobjects[0].p.x=360;
    chooseobjects[0].p.y=402;
    chooseobjects[1].p.x=625;
    chooseobjects[1].p.y=455;
}
void initialize()
{
    menu=0;
    pause=0;
    score=0;
    highscoreind=0;
    miss=0;
    gap=0.2;
    menu_music=0;
    appearance_timer=0;
    animation_timer=1;
    animation_existence=100;
    music_timer=-1;
    in_game_timer2=0;
    strt=clock();
    sorted=0;

    boom_anim_start=0;
    for(int i=0; i<goodrnd; i++)
    {
        goodr[i].p.y=-goodr[i].rad+1;
        goodr[i].vis=0;
    }
    for(int i=0; i<badrnd; i++)
    {
        badr[i].p.y=-badr[i].rad+1;
        badr[i].vis=0;
    }
    for(int i=0; i<bonrnd; i++)
    {
        bonusr[i].p.y=-badr[i].rad+1;
        bonusr[i].vis=0;
    }
}
void drawmouse()
{
    for(int i=mouse_ind1;i<mouse_ind2-1;i++)
    {

        iSetColor(240-(mouse_ind2-i)*5,240-(mouse_ind2-i)*2,240-(mouse_ind2-i)*5);
        for(int j=8;j>=-7;j--)
        {
            iLine(mouse_track[i].x+j/2.0,mouse_track[i].y+j/2.0,mouse_track[i+1].x+j/2.0,mouse_track[i+1].y+j/2.0);
        }
    }
}
void printscore()
{
    char str[100],buf[100];
    iSetColor(255,255,255);
    strcpy(str,"SCORE: ");
    strcat(str,itoa(score,buf,10));
    iText(scorepos.x,scorepos.y,str,GLUT_BITMAP_TIMES_ROMAN_24);
    strcpy(str,"HIGH SCORE: ");
    if(score > highscore[time_trial][0].hgsc)
        strcat(str,itoa(score,buf,10));
    else
        strcat(str,itoa(highscore[time_trial][0].hgsc,buf,10));
    if(score>highscore[time_trial][9].hgsc)isnew=1;
    iText(scorepos.x,scorepos.y-50,str,GLUT_BITMAP_TIMES_ROMAN_24);

}
void drawmenu()
{
    iShowBMP(0,0,"menuanother.bmp");
    if(music_on)iShowBMP2(940,740,"music_icon_on.bmp",IGNORE_COLOR);
    else iShowBMP2(940,740,"music_icon_off.bmp",IGNORE_COLOR);
    for(int i=0;i<4;i++)
    {
        iShowBMP2(menuobject[i].p.x-menuobject[i].rad,menuobject[i].p.y-menuobject[i].rad,menuobject[i].name,IGNORE_COLOR);
    }
    iShowBMP2(instruction_x,instruction_y,"quesmark2.bmp",IGNORE_COLOR);
    high_score_bottom=4;
    high_score_top=0;
}
void boom_anim()
{
    char s[10];
      strcpy(s,"b//");
      s[3]=boom_anim_start/13+'0'+1;
      s[4]='\0';
      strcat(s,".bmp");
      iShowBMP2(hit.x-100,hit.y-100,s,IGNORE_COLOR);

    boom_anim_start++;
    if(boom_anim_start>100)
    {
        boom_anim_start=0;
                if(music_on)PlaySound((LPCSTR) "Game-over.wav", NULL, SND_FILENAME | SND_ASYNC);
        gameover=1;
    }
}
void drawtimer()
{
    endd=clock();
    double in_game_timer=in_game_timer2+(endd-strt)/CLOCKS_PER_SEC;
    if(time_trial)
    {
        in_game_timer=60-in_game_timer;

        if(in_game_timer<=0)
        {
            if(music_on)PlaySound((LPCSTR) "time-up.wav", NULL, SND_FILENAME | SND_ASYNC);
            gameover=1;
            return;
        }
    }
    int minute=(int)in_game_timer/60;
    int secnd=(int)in_game_timer%60;
    char showtime[6];
    showtime[0]='0'+minute/10;
    showtime[1]='0'+minute%10;
    showtime[2]=':';
    showtime[3]='0'+secnd/10;
    showtime[4]='0'+secnd%10;
    showtime[5]='\0';
    iText(timerpos.x,timerpos.y,showtime,GLUT_BITMAP_TIMES_ROMAN_24);
}
void drawpause()
{
    iShowBMP(0,0,"pausebg1.bmp");
    if(music_on)iShowBMP2(940,740,"music_icon_on.bmp",IGNORE_COLOR);
    else iShowBMP2(940,740,"music_icon_off.bmp",IGNORE_COLOR);
    for(int i=0;i<4;i++)
    {
        iShowBMP2(menuobject[i].p.x-menuobject[i].rad,menuobject[i].p.y-menuobject[i].rad,menuobject[i].name,IGNORE_COLOR);
    }
}
void drawanimation()
{
    animation_timer++;
    if(animation_timer>animation_existence)animation_timer=0;
    iShowBMP(0,0,"anim.bmp");
    iSetColor(173,10,6);
    iFilledRectangle(0,0,animation_timer*(1000.0/animation_existence),20);
}
void drawcross()
{
    for(int i=0;i<miss;i++)
    {
        iShowBMP2(scrwd-(3-i)*55,scrhg-50,"cross2.bmp",IGNORE_COLOR);
    }
}
void drawinstruction()
{
    iShowBMP(0,0,"INSTRUCTIONS.bmp");
    iShowBMP2(143,90,goodrc[0],0xffffff);
    iShowBMP2(90,90,goodrc[3],0Xffffff);
    iShowBMP2(115,160,goodrc[2],0xffffff);
    iShowBMP2(470,130,badrc[0],0xffffff);
    iShowBMP2(790,120,bonusrc[0],0xffffff);
}
void show_gameover()
{
    char str[100],buf[100];
    iSetColor(255,255,255);
    strcpy(str,"YOUR SCORE: ");
    strcat(str,itoa(score,buf,10));
    iShowBMP(0,0,"gameover.bmp");
    iText(400,350,str,GLUT_BITMAP_TIMES_ROMAN_24);
    if(!sorted&&isnew==0)
    {
        sort_high_score();
        highscore[time_trial][10].name[0]='\0';
    }
}
void show_take_name()
{
    iSetColor(0,0,0);
    iFilledRectangle(270,290,490,42);
    iSetColor(255,255,255);
    iText(275,302,"ENTER NAME: ",GLUT_BITMAP_TIMES_ROMAN_24);
    iFilledRectangle(470,295,280,32);
    iSetColor(0,0,0);
    iText(475,302,highscore[time_trial][10].name,GLUT_BITMAP_TIMES_ROMAN_24);
}
void draw_choose()
{
    if(music_on)iShowBMP(0,0,"choose1.bmp");
    else iShowBMP(0,0,"choose2.bmp");
    for(int i=0;i<2;i++)iShowBMP2(chooseobjects[i].p.x-chooseobjects[i].rad,chooseobjects[i].p.y-chooseobjects[i].rad,chooseobjects[i].name,IGNORE_COLOR);
}
void iDraw()
{
    iClear();
    if(animation_timer>0)
    {
        drawanimation();
        return;
    }
    if(menu)drawmenu(),drawmouse();
    else if(choose)draw_choose(),drawmouse();
    else if(instruction) drawinstruction();
    else if(pause) drawpause(),drawmouse();
    else if(gameover)
    {
        show_gameover();
        if(isnew)show_take_name();
    }
    else if(highscoreind)draw_hg_bg();
    else if(crdt_ind)draw_credits();
    else {
            iShowBMP(0,0,"bg25.bmp");
            if(miss>=3&&!time_trial)
            {
                if(music_on)PlaySound((LPCSTR) "Game-over.wav", NULL, SND_FILENAME | SND_ASYNC);
                gameover=1;
            }
        if(boom_anim_start)boom_anim();
        drawall();
        drawmouse();
        printscore();
        drawtimer();
        if(!time_trial)drawcross();
    }
    if(!music_on || instruction || highscoreind || crdt_ind || choose )
        music_timer=0;
    if(music_on && menu && music_timer%497==0 )
    {
        PlaySound((LPCSTR) "menumusic.wav", NULL, SND_FILENAME | SND_ASYNC);
        music_timer=0;

    }
}
void iMouseMove(int mx, int my)
{

    if(nomove||animation_timer||gameover||instruction||boom_anim_start||crdt_ind)return;
    point m;
    mouse_track[mouse_ind2].x=mx;
    mouse_track[mouse_ind2].y=my;
    mouse_ind2++;
    if(mouse_ind2-mouse_ind1>3)mouse_ind1++;
    m.x=mx,m.y=my;
    if(choose)
    {
        for(int i=0;i<2;i++)
        {
            if(isinsideroundobject(chooseobjects[i],m))
            {
            	if(music_on)PlaySound((LPCSTR) "fruit_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
                if(i==1)time_trial=1;
                else time_trial=0;
                choose=0;
                initialize();
                if(music_on)PlaySound((LPCSTR) "Game-start.wav", NULL, SND_FILENAME | SND_ASYNC);
            }
        }
    }
    if(menu)
    {
        for(int i=0;i<4;i++)
        {
            if(isinsideroundobject(menuobject[i],m))
            {
            	if(music_on)PlaySound((LPCSTR) "fruit_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
                if(i==0)exit(0);
                if(i==1)choose=1,menu=0;
                if(i==2)menu=0,highscoreind=1,time_trial=0;
                if(i==3)menu=0,crdt_ind=1;
            }
        }
    }
    if(pause)
    {
        for(int i=0;i<4;i++)
        {
            if(isinsideroundobject(menuobject[i],m))
            {
            	if(music_on)PlaySound((LPCSTR) "fruit_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
                if(i==0)exit(0);
                if(i==1)pause=0,iResumeTimer(0),strt=clock();
                if(i==3)pause=0,iResumeTimer(0),animation_timer++,animation_existence=100,initialize(),menu=1;
                if(i==2)iResumeTimer(0),initialize();
            }
        }
    }
    if(highscoreind)
    {
        if(mouse_track[0].x>=820&&mouse_track[0].x<=844&&high_move_pos)
        {
            if(m.y<240+(9-high_score_bottom)*31)
            {
                if(high_score_bottom<9)high_score_bottom++,high_score_top++;
            }
            if(m.y>240+(9-high_score_bottom)*31+155)
            {
                if(high_score_top>0)high_score_bottom--,high_score_top--;
            }
        }
    }
    if(menu||animation_timer||gameover||highscoreind||boom_anim_start||choose||pause||instruction||crdt_ind)return;
    for(int i=0; i<goodrnd; i++)
    {
       if(goodr[i].vis&&isinsideroundobject(goodr[i],m))
       	{
       		if(music_on)PlaySound((LPCSTR) "fruit_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
       		goodr[i].vis=0,goodr[i].p.y=-30,score+=10;
       	}
    }
    for(int i=0; i<badrnd; i++)
    {
       if(badr[i].vis&&isinsideroundobject(badr[i],m))
       	{

            if(music_on)PlaySound((LPCSTR) "bomb_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
            if(time_trial)score-=50,badr[i].vis=0,badr[i].p.y=-30;
            else boom_anim_start=1,hit=badr[i].p,badr[i].vis=0;
        }
    }
    for(int i=0; i<bonrnd; i++)
    {
       if(bonusr[i].vis&&isinsideroundobject(bonusr[i],m))
       	{
            if(music_on)PlaySound((LPCSTR) "bonus_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
            bonusr[i].vis=0,bonusr[i].p.y=-30,score+=25;
        }
    }
}


void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {

        point p;
        p.x=mx,p.y=my;
        if(menu)
        {
            if(p.x>= instruction_x && p.x<=instruction_x+instruction_l && p.y>=instruction_y && p.y<=instruction_y+instruction_l)
            {
                if(music_on)PlaySound((LPCSTR) "instruction_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
                menu=0;
                instruction=1;

            }
            if(p.x>=940&&p.x<=1000&&p.y>=740&&p.y<=800)
            {
                music_on^=1;
                if(!music_on)
                {
                    PlaySound((LPCSTR) "instruction_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
                    music_timer=0;
                }
            }
        }
        else if(pause)
        {
            if(p.x>=940&&p.x<=1000&&p.y>=740&&p.y<=800)
            {
                if(!music_on)
                    PlaySound((LPCSTR) "instruction_sound.wav", NULL, SND_FILENAME | SND_ASYNC);
                music_on^=1;
            }
        }
        else if(highscoreind)
        {
            point c;
            c.x=832,c.y=567;
            if(dis(p,c)<=32&&high_score_top>0)
            {
                high_score_bottom--,high_score_top--;
                return;
            }
            c.y=232;
             if(dis(p,c)<=32&&high_score_bottom<9)
            {
                high_score_bottom++,high_score_top++;
                return;
            }
            if(p.y>=240+(9-high_score_bottom)*31&&p.y<=240+(9-high_score_bottom)*31+155)high_move_pos=1;

            return;
        }
        mouse_track[mouse_ind2]=p;
        mouse_ind2++;
    }
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        mouse_ind1=0,mouse_ind2=0;
        high_move_pos=0;
    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        nomove=1;
        mouse_ind1=0,mouse_ind2=0;

    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
    {
        nomove=0;
    }
}

void iKeyboard(unsigned char key)
{
    if(key==27)
    {
        if(!(animation_timer||gameover||instruction||menu||highscoreind||boom_anim_start||crdt_ind||choose))
        {
            pause^=1;
            if(pause)
            {
                iPauseTimer(0);
                endd=clock();
                in_game_timer2+=(endd-strt)/CLOCKS_PER_SEC;
            }
            else
            {
                iResumeTimer(0);
                strt=clock();
            }
        }
    }
    if(gameover)
    {
        if(isnew)
        {
            if(key==13)
            {
                if(name_ind==0)
                {
                    printf("\a");
                    return;
                }
                highscore[time_trial][10].name[name_ind]='\0';
                name_ind=0;
                isnew=0;
            }
            else if(key==8)
            {
                if(name_ind<=0)printf("\a");
                else name_ind--,highscore[time_trial][10].name[name_ind]='\0';
            }
            else if(key==' '||name_ind>14)
            {
                printf("\a");
            }
            else
            {
                highscore[time_trial][10].name[name_ind]=key;
                name_ind++;

                highscore[time_trial][10].name[name_ind]='\0';
            }
        }
        else{
        if(key==13)
        {
            gameover=0;
            initialize();
        }
        if(key==27)
        {
            gameover=0;
            initialize();
            menu=1;
        }
        }
    }
    if(instruction || highscoreind||crdt_ind||choose)
    {
        if(key==27)
        {
            menu=1;
            crdt_ind=0;
            instruction=0;
            highscoreind=0;
            choose=0;
            menu_music=0;
        }
    }
}

void iSpecialKeyboard(unsigned char key)
{

    if(key == GLUT_KEY_END)
    {
        exit(0);
    }
    else if(isnew&&gameover)printf("\a");
    if(highscoreind)
    {
        if(key == GLUT_KEY_DOWN && high_score_bottom<9)
            high_score_bottom++,high_score_top++;
        if(key == GLUT_KEY_UP && high_score_top>0)
            high_score_bottom--,high_score_top--;
        if(key==GLUT_KEY_LEFT&&time_trial)time_trial=0;
        if(key==GLUT_KEY_RIGHT&&!time_trial)time_trial=1;
    }
}

void playmussic()
{
    music_timer++;
}

void fall()
{
    if(menu||animation_timer||gameover||highscoreind||boom_anim_start||choose||pause||instruction||crdt_ind)return;
    appearance_timer+=gap;
    gap+=0.0002;
    gap=(gap>1?1:gap);
//    in_game_timer+=(framerate+3.0)/1000.0;
    for(int i=0; i<goodrnd; i++)
    {
        if(goodr[i].vis)
        {
            goodr[i].p.y+=goodr[i].spd;
            goodr[i].spd-=gravity;
            if(goodr[i].dir)goodr[i].p.x+=goodr[i].xspd;
            else goodr[i].p.x-=goodr[i].xspd;
            if(goodr[i].dir&&goodr[i].p.x+goodr[i].rad>scrwd||goodr[i].p.x-goodr[i].rad<0&&goodr[i].dir==0)goodr[i].dir^=1;
        }else if((long long int)appearance_timer%200==goodr[i].t)
        {
            goodr[i].vis=1;
            goodr[i].p.x=rand()%scrwd;
            goodr[i].spd=9+rand()%4;
            goodr[i].xspd=1+rand()%4;
            if(goodr[i].p.x>scrwd/2)goodr[i].dir=0;
            else goodr[i].dir=1;
        }
    }
    for(int i=0; i<badrnd; i++)
    {
        if(badr[i].vis)
        {
            badr[i].p.y+=badr[i].spd;
            badr[i].spd-=gravity;
            if(badr[i].dir)badr[i].p.x+=badr[i].xspd;
            else badr[i].p.x-=badr[i].xspd;
            if(badr[i].dir&&badr[i].p.x+badr[i].rad>scrwd||badr[i].p.x-badr[i].rad<0&&badr[i].dir==0)badr[i].dir^=1;
        }else if((long long int)appearance_timer%200==badr[i].t)
        {
            badr[i].vis=1;
            badr[i].p.x=rand()%scrwd;
            badr[i].spd=9+rand()%4;
            badr[i].xspd=1+rand()%4;
            if(badr[i].p.x>scrwd/2)badr[i].dir=0;
            else badr[i].dir=1;
        }
    }
    for(int i=0; i<bonrnd; i++)
    {
        if(bonusr[i].vis)
        {
            bonusr[i].p.y+=bonusr[i].spd;
            bonusr[i].spd-=gravity;
            if(bonusr[i].dir)bonusr[i].p.x+=bonusr[i].xspd;
            else bonusr[i].p.x-=bonusr[i].xspd;
            if(bonusr[i].dir&&bonusr[i].p.x+bonusr[i].rad>scrwd||bonusr[i].p.x-bonusr[i].rad<0&&bonusr[i].dir==0)bonusr[i].dir^=1;
        }else if((long long int)appearance_timer%200==bonusr[i].t)
        {
            bonusr[i].vis=1;
            bonusr[i].p.x=rand()%scrwd;
            bonusr[i].spd=9+rand()%4;
            bonusr[i].xspd=1+rand()%4;
            if(bonusr[i].p.x>scrwd/2)bonusr[i].dir=0;
            else bonusr[i].dir=1;
        }
    }
}
int main()
{
    initonce();
    high_score_read();
    iSetTimer(framerate,fall);
    iSetTimer(1000,playmussic);
    iInitialize(scrwd, scrhg, "FRUIT NINJA");
    return 0;
}
