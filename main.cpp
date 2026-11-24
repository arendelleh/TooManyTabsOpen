
#include <GL/freeglut.h>
#include <array>
#include <algorithm>
#include <cmath>
#include <string>
#include <cassert>
#include <iostream>
using namespace std;


constexpr int W=1100, H=720;
struct Color { float r,g,b; };
const Color bg{.055f,.075f,.13f}, panel{.10f,.13f,.21f}, ink{.91f,.94f,1}, muted{.61f,.68f,.79f}, mint{.40f,.88f,.73f}, blue{.48f,.66f,1};
enum class Screen { Welcome, Desktop, Break, World, Ending };
struct Task {string title, subtitle; array<string,3> steps; int done=0; bool later=false; };
#include "task_data.hpp"
array<Task,4> tasks;
Screen screen=Screen::Welcome;
int selected=0, winW=W,winH=H, vx=0,vy=0,vw=W,vh=H,lastTick=0;
bool focus=false,quiet=false,paused=false;
float breathTime=0;
string feedback="You do not have to do everything at once.";


void resetWood();
bool canAcknowledge();
void noteStep();
void noteBoundary();

void reset(){
 resetWood();
 tasks=makeTasks();
 selected=0;focus=quiet=paused=false;breathTime=0;screen=Screen::Welcome;
 feedback="You do not have to do everything at once.";
}


void completeStep(){
 Task &t=tasks[selected];
 if(t.later){feedback="Bring this task back from Later before working on it.";return;}
 if(!canAcknowledge())return;
 if(t.done<3){++t.done;noteStep();feedback=t.done==3?"This example task is complete. It is okay to stop here.":"One small step acknowledged. Choose your own pace.";}
 else feedback="All three steps are already complete.";
}


void toggleLater(){tasks[selected].later=!tasks[selected].later;noteBoundary();feedback=tasks[selected].later?"Set aside for later. Your progress stays for this session.":"Back on your desk. Start when you are ready.";}
void color(Color c){glColor3f(c.r,c.g,c.b);}
void rect(float x,float y,float w,float h,Color c){color(c);glBegin(GL_QUADS);glVertex2f(x,y);glVertex2f(x+w,y);glVertex2f(x+w,y+h);glVertex2f(x,y+h);glEnd();}
void line(float x,float y,float a,float b,Color c){color(c);glBegin(GL_LINES);glVertex2f(x,y);glVertex2f(a,b);glEnd();}
void text(float x,float y,const std::string&s,Color c=ink,void *font=GLUT_BITMAP_HELVETICA_18){color(c);glRasterPos2f(x,y);for(unsigned char ch:s)glutBitmapCharacter(font,ch);}
void circle(float x,float y,float r,Color c){color(c);glBegin(GL_TRIANGLE_FAN);glVertex2f(x,y);for(int i=0;i<=64;++i){float a=i*6.2831853f/64;glVertex2f(x+std::cos(a)*r,y+std::sin(a)*r);}glEnd();}
void diamond(float x,float y,Color c){
 static const GLfloat vertices[]={0,-12,12,0,0,12,-12,0};
 static const GLubyte indices[]={0,1,2,0,2,3};
 glPushMatrix();glTranslatef(x,y,0);color(c);
 glEnableClientState(GL_VERTEX_ARRAY);glVertexPointer(2,GL_FLOAT,0,vertices);
 glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_BYTE,indices);
 glDisableClientState(GL_VERTEX_ARRAY);glPopMatrix();
}



void button(float x,float y,float w,const std::string&label,bool active=false){rect(x,y,w,42,active?mint:Color{.19f,.25f,.36f});text(x+14,y+27,label,active?bg:ink);}
bool hit(int x,int y,int a,int b,int w,int h){return x>=a&&x<a+w&&y>=b&&y<b+h;}
void header(){
 diamond(40,40,mint);text(64,47,"TOO MANY TABS OPEN",ink,GLUT_BITMAP_HELVETICA_18);
 text(64,73,"A little space for your next small step",muted,GLUT_BITMAP_HELVETICA_12);
 line(30,92,1070,92,Color{.21f,.26f,.36f});
}



#include "lanternwood.hpp"

void displayLegacy(){
 orthoUI();
 glClearColor(bg.r,bg.g,bg.b,1);glClear(GL_COLOR_BUFFER_BIT);glMatrixMode(GL_MODELVIEW);glLoadIdentity();header();
 if(screen==Screen::Welcome){
  rect(120,145,860,475,panel);text(175,208,"College can feel like too many tabs open.",ink,GLUT_BITMAP_HELVETICA_18);
  text(175,254,"Assignments. Group chats. Deadlines. Making time for yourself.",muted);
  text(175,303,"Explore a fictional student desktop at your own pace.");
  text(175,339,"Choose one task, set another aside, or take a quiet break.");
  text(175,392,"There is no countdown and no perfect score.",mint);
  button(175,431,250,"Open my desktop [Enter]",true);
  text(175,515,"An educational reflection activity, not a mental-health assessment.",muted,GLUT_BITMAP_HELVETICA_12);
  text(175,540,"If you need support, consider a trusted person or your campus counselor.",muted,GLUT_BITMAP_HELVETICA_12);
 }else if(screen==Screen::Break){
  text(360,142,"A moment away from the tabs",mint);
  float phase=std::fmod(breathTime,8.f);float amount=(1-std::cos(phase*6.2831853f/8))/2;
  circle(550,340,135,Color{.10f,.22f,.28f});circle(550,340,65+55*amount,Color{.28f,.65f,.59f});
  text(515,346,paused?"Paused":phase<4?"Breathe in":"Breathe out",bg);
  text(317,510,"Follow gently if comfortable, or simply rest here.",muted);
  button(330,550,205,paused?"Resume [Space]":"Pause [Space]");button(555,550,215,"Back to desktop [B]");
 }
 
 else{
  for(int i=0;i<4;++i){
   if(focus&&i!=selected)continue;
   int x=30+i*265;rect(x,115,250,65,i==selected?Color{.20f,.32f,.39f}:panel);
   diamond(x+22,140,i==selected?mint:muted);text(x+44,145,tasks[i].title);
   text(x+44,165,tasks[i].later?"LATER":tasks[i].done==3?"COMPLETE":"READY FOR A SMALL STEP",muted,GLUT_BITMAP_HELVETICA_10);
  }


  const Task&t=tasks[selected];rect(30,202,695,367,panel);
  text(55,244,t.title,ink,GLUT_BITMAP_HELVETICA_18);text(55,273,t.subtitle,muted);
  for(int i=0;i<3;++i){
   int y=300+i*57;rect(55,y,28,28,i<t.done?mint:Color{.20f,.26f,.36f});
   if(i<t.done){line(62,y+14,68,y+21,bg);line(68,y+21,78,y+7,bg);}
   text(100,y+21,t.steps[i],i<t.done?mint:ink);
  }

  button(55,495,245,t.done==3?"Steps complete":"Complete next step [C]",t.done<3&&!t.later);
  button(315,495,245,t.later?"Bring back [L]":"Set aside for later [L]");
  rect(745,202,325,367,panel);text(767,240,"MAKE SOME SPACE",mint);
  button(767,263,280,focus?"Focus mode: ON [F]":"Focus mode: OFF [F]",focus);
  button(767,318,280,quiet?"Notifications: muted [M]":"Mute notifications [M]",quiet);
  button(767,373,280,"Take a breathing break [B]");
  rect(767,439,280,105,Color{.28f,.28f,.24f});
  color(Color{.46f,.44f,.32f});glBegin(GL_TRIANGLES);glVertex2f(1027,439);glVertex2f(1047,459);glVertex2f(1027,459);glEnd();
  text(783,466,quiet?"Quiet space enabled":"A fictional group chat says:",muted,GLUT_BITMAP_HELVETICA_12);
  text(783,493,quiet?"You choose when to check in.":"Any updates on our project?",ink,GLUT_BITMAP_HELVETICA_12);
  text(783,520,"You can ask someone for support.",mint,GLUT_BITMAP_HELVETICA_12);
  rect(30,589,1040,49,Color{.10f,.22f,.24f});text(48,620,feedback,mint);
 }

 text(30,675,"1-4 Select tab | C Step | L Later | F Focus | M Mute | B Break | R Reset | Esc Exit",muted,GLUT_BITMAP_HELVETICA_12);
 text(30,697,"Fictional tasks. Completing a step acknowledges an action; it does not measure your well-being.",muted,GLUT_BITMAP_HELVETICA_10);
 glutSwapBuffers();
}

void display(){if(screen==Screen::Desktop)displayLegacy();else displayGame();}

void keyboard(unsigned char key,int,int){
 wood.keys[key]=true;
 if(key==27){glutLeaveMainLoop();return;}
 if(key=='r'||key=='R'){reset();glutPostRedisplay();return;}
 if(key=='j'||key=='J'){if(screen==Screen::World)screen=Screen::Desktop;else if(screen==Screen::Desktop)screen=Screen::World;glutPostRedisplay();return;}
 if(screen==Screen::Ending){if(key==13)screen=Screen::World;glutPostRedisplay();return;}
 if(screen==Screen::World){
  if(key>='1'&&key<='4'){selected=key-'1';feedback="Follow the gold spark to acknowledge one small step.";}
  switch(key){
   case 'e':case 'E':case 'c':case 'C':interactWood();break;
   case 'l':case 'L':toggleLater();break;
   case 'f':case 'F':focus=!focus;break;
   case 'm':case 'M':toggleQuiet();break;
   case 'b':case 'B':screen=Screen::Break;breathTime=0;paused=false;break;
   default:break;
  }
  glutPostRedisplay();return;
 }
 if(screen==Screen::Welcome){if(key==13)screen=Screen::World;}
 else if(screen==Screen::Break){if(key=='b'||key=='B'||key=='e'||key=='E')leaveRest();else if(key==' ')paused=!paused;}
 else{
  if(key>='1'&&key<='4'){selected=key-'1';feedback="Choose a small step, or leave this task for later.";}
  switch(key){case 'c':case 'C':completeStep();break;case 'l':case 'L':toggleLater();break;
   case 'f':case 'F':focus=!focus;break;case 'm':case 'M':toggleQuiet();break;
   case 'b':case 'B':screen=Screen::Break;breathTime=0;paused=false;break;default:break;}
 }

 glutPostRedisplay();
}

void mouse(int buttonId,int state,int px,int py){
 if(buttonId==GLUT_RIGHT_BUTTON){wood.dragging=state==GLUT_DOWN;wood.mouseX=px;return;}
 if((buttonId==3||buttonId==4)&&state==GLUT_DOWN){wood.cameraDistance=std::clamp(wood.cameraDistance+(buttonId==3?-1.f:1.f),16.f,29.f);return;}
 if(buttonId!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)return;
 int top=winH-vy-vh;if(px<vx||px>=vx+vw||py<top||py>=top+vh)return;
 int x=(px-vx)*W/vw,y=(py-top)*H/vh;
 if(screen==Screen::Welcome||screen==Screen::Ending){if(hit(x,y,95,390,315,42))keyboard(13,0,0);return;}
 if(screen==Screen::World){for(int i=0;i<4;++i)if((!focus||i==selected)&&hit(x,y,27+i*268,93,252,48))keyboard('1'+i,0,0);return;}
 if(screen==Screen::Break){if(hit(x,y,300,480,500,80))keyboard('b',0,0);else if(hit(x,y,440,230,220,220))keyboard(' ',0,0);return;}
 for(int i=0;i<4;++i)if((!focus||i==selected)&&hit(x,y,30+i*265,115,250,65))keyboard('1'+i,0,0);
 if(hit(x,y,55,495,245,42))keyboard('c',0,0);
 else if(hit(x,y,315,495,245,42))keyboard('l',0,0);
 else if(hit(x,y,767,263,280,42))keyboard('f',0,0);
 else if(hit(x,y,767,318,280,42))keyboard('m',0,0);
 else if(hit(x,y,767,373,280,42))keyboard('b',0,0);
}

void reshape(int w,int h){
 winW=std::max(1,w);winH=std::max(1,h);float scale=std::min(winW/float(W),winH/float(H));
 vw=std::max(1,int(W*scale));vh=std::max(1,int(H*scale));vx=(winW-vw)/2;vy=(winH-vh)/2;
 glViewport(vx,vy,vw,vh);glMatrixMode(GL_PROJECTION);glLoadIdentity();glOrtho(0,W,H,0,-1,1);glMatrixMode(GL_MODELVIEW);
}

void timer(int){int now=glutGet(GLUT_ELAPSED_TIME);float dt=std::min((now-lastTick)/1000.f,.1f);lastTick=now;worldTick(dt);if(screen==Screen::Break&&!paused)breathTime+=dt;glutPostRedisplay();glutTimerFunc(16,timer,0);}
int main(int argc,char**argv){
 reset();
 if(argc>1&&std::string(argv[1])=="--self-test"){
  logicTest=true;
  for(int i=0;i<8;++i){completeStep();}
  assert(tasks[0].done==3);
  selected=1;toggleLater();completeStep();assert(tasks[1].done==0);
  toggleLater();completeStep();assert(tasks[1].done==1);
  reset();assert(selected==0&&!focus&&!quiet&&!paused&&screen==Screen::Welcome);
  for(const auto&t:tasks)assert(t.done==0&&!t.later);
  assert(hit(0,0,0,0,10,10)&&!hit(10,10,0,0,10,10));
  logicTest=false;screen=Screen::World;
  Vec3 spark=nextSpark(0);wood.x=0;wood.z=0;completeStep();assert(tasks[0].done==0);
  wood.x=spark.x;wood.z=spark.z;completeStep();assert(tasks[0].done==1&&wood.lights==1);
  toggleLater();assert(wood.lights==2);toggleLater();toggleLater();assert(wood.lights==2);
  toggleQuiet();assert(wood.lights==3);toggleQuiet();toggleQuiet();assert(wood.lights==3);
  wood.x=0;wood.z=0;interactWood();assert(screen==Screen::World);
  wood.x=0;wood.z=6;interactWood();assert(screen==Screen::Ending);
  reset();leaveRest();assert(wood.lights==1);leaveRest();assert(wood.lights==1);
  reset();screen=Screen::World;wood.keys['w']=true;worldTick(100);assert(std::hypot(wood.x,wood.z)<9.201f);
  reset();assert(wood.lights==0&&!wood.rest&&!wood.boundary&&!wood.silence);
  std::cout<<"PASS: 3D proximity, story lanterns, boundary and quiet limits, sanctuary, rest, movement, and reset;  step limits, Later guard, restore, reset, hit boundaries\n";return 0;
 }

 glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_MULTISAMPLE);glutInitWindowSize(W,H);
 glutCreateWindow("Too Many Tabs Open | The Lanternwood - 3D");glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
 glutIgnoreKeyRepeat(1);glutKeyboardUpFunc(keyUp);glutSpecialFunc(specialKey);glutMotionFunc(worldMotion);
 glutDisplayFunc(display);glutReshapeFunc(reshape);glutKeyboardFunc(keyboard);glutMouseFunc(mouse);
 lastTick=glutGet(GLUT_ELAPSED_TIME);glutTimerFunc(16,timer,0);glutMainLoop();return 0;
}

