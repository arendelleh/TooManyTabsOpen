
#include <GL/freeglut.h>
#include <array>
#include "ambience.hpp"
#include <algorithm>
#include <cmath>
#include <cctype>
#include <vector>
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

#include "journal_ui.hpp"

void display(){if(screen==Screen::Desktop)displayJournal();else displayGame();}

void keyboard(unsigned char key,int,int){
 wood.keys[key]=true;
 if(key=='v'||key=='V'){if(!ambience.toggle())feedback="Sound could not start. Check the audio asset and system audio device.";glutPostRedisplay();return;}
 if(key=='n'||key=='N'){toggleDay();glutPostRedisplay();return;}
 if(key=='h'||key=='H'){openDrawer(1);glutPostRedisplay();return;}
 if(key=='t'||key=='T'||key==9){openDrawer(0);glutPostRedisplay();return;}
 if(key==27){glutLeaveMainLoop();return;}
 if(key=='r'||key=='R'){reset();glutPostRedisplay();return;}
 if((key=='c'||key=='C')&&screen==Screen::World){openDrawer(2);glutPostRedisplay();return;}
 if(key=='k'||key=='K'){wood.season=(wood.season+1)%3;glutPostRedisplay();return;}
 if(key=='j'||key=='J'){if(screen==Screen::World)screen=Screen::Desktop;else if(screen==Screen::Desktop)screen=Screen::World;glutPostRedisplay();return;}
 if(screen==Screen::Ending){if(key==13)screen=Screen::World;glutPostRedisplay();return;}
 if(screen==Screen::World){
  if(key>='1'&&key<='4'){selectTab(key-'1');Vec3 p=nextSpark(selected);if(!tasks[selected].later&&tasks[selected].done<3)walkToward(p.x,p.z);}
  switch(key){
   case 'e':case 'E':interactWood();break;
   case 'l':case 'L':toggleLater();break;
   case 'f':case 'F':focus=!focus;break;
   case 'm':case 'M':toggleQuiet();break;
   case 'b':case 'B':enterRest();break;
   default:break;
  }
  glutPostRedisplay();return;
 }
 if(screen==Screen::Welcome){if(key==13){ambience.enterForest();screen=Screen::World;}}
 else if(screen==Screen::Break){if(key=='b'||key=='B'||key=='e'||key=='E')leaveRest();else if(key==' ')paused=!paused;}
 else{
  if(key>='1'&&key<='4'){selected=key-'1';feedback="Choose a small step, or leave this task for later.";}
  switch(key){case 'c':case 'C':visitJournalActivity();break;case 'l':case 'L':toggleLater();break;
   case 'f':case 'F':focus=!focus;break;case 'm':case 'M':toggleQuiet();break;
   case 'b':case 'B':enterRest();break;default:break;}
 }

 glutPostRedisplay();
}

void mouse(int buttonId,int state,int px,int py){
 if(buttonId==GLUT_RIGHT_BUTTON){wood.dragging=state==GLUT_DOWN;wood.mouseX=px;return;}
 if((buttonId==3||buttonId==4)&&state==GLUT_DOWN){wood.cameraDistance=std::clamp(wood.cameraDistance+(buttonId==3?-1.f:1.f),16.f,29.f);return;}
 if(buttonId!=GLUT_LEFT_BUTTON||state!=GLUT_DOWN)return;
 int top=winH-vy-vh;if(px<vx||px>=vx+vw||py<top||py>=top+vh)return;
 int x=(px-vx)*W/vw,y=(py-top)*H/vh;
 if(screen==Screen::Welcome||screen==Screen::Ending){if(hit(x,y,328,458,444,34))keyboard(13,0,0);return;}
 if(screen==Screen::Desktop){journalClick(x,y);glutPostRedisplay();return;}
 if(screen==Screen::World){handleWorldClick(x,y);glutPostRedisplay();return;}
 if(screen==Screen::Break){if(hit(x,y,525,635,255,34))keyboard('b',0,0);else if(hit(x,y,320,635,190,34))keyboard(' ',0,0);return;}
 for(int i=0;i<4;++i)if((!focus||i==selected)&&hit(x,y,30+i*265,115,250,65))keyboard('1'+i,0,0);
 if(hit(x,y,55,495,245,42))keyboard('c',0,0);
 else if(hit(x,y,315,495,245,42))keyboard('l',0,0);
 else if(hit(x,y,767,263,280,42))keyboard('f',0,0);
 else if(hit(x,y,767,318,280,42))keyboard('m',0,0);
 else if(hit(x,y,767,373,280,42))keyboard('b',0,0);
}

void reshape(int w,int h){
 winW=std::max(1,w);winH=std::max(1,h);
 vw=winW;vh=winH;vx=0;vy=0;
 glViewport(vx,vy,vw,vh);glMatrixMode(GL_PROJECTION);glLoadIdentity();glOrtho(0,W,H,0,-1,1);glMatrixMode(GL_MODELVIEW);
}

void timer(int){int now=glutGet(GLUT_ELAPSED_TIME);float dt=std::min((now-lastTick)/1000.f,.1f);lastTick=now;worldTick(dt);if(screen==Screen::Break&&!paused)breathTime+=dt;glutPostRedisplay();glutTimerFunc(16,timer,0);}
int main(int argc,char**argv){
 reset();
 if(argc>1&&std::string(argv[1])=="--audio-check"){bool ok=ambience.start();ambience.stop();std::cout<<(ok?"PASS: audio asset found beside executable and playback accepted\n":"FAIL: audio asset missing or playback unavailable\n");return ok?0:1;}
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
  wood.x=spark.x;wood.z=spark.z;interactWood();assert(tasks[0].done==0&&wood.carrying[0]);
  spark=activityTarget(0);wood.x=spark.x;wood.z=spark.z;interactWood();assert(tasks[0].done==1&&wood.lights==1&&!wood.carrying[0]);
  toggleLater();assert(wood.lights==2);toggleLater();toggleLater();assert(wood.lights==2);
  toggleQuiet();assert(wood.lights==3);toggleQuiet();toggleQuiet();assert(wood.lights==3);
  wood.x=0;wood.z=0;interactWood();assert(screen==Screen::World);
  wood.x=0;wood.z=6;interactWood();assert(screen==Screen::Ending);
  reset();leaveRest();assert(wood.lights==1);leaveRest();assert(wood.lights==1);
  reset();screen=Screen::World;wood.keys['w']=true;worldTick(100);assert(std::hypot(wood.x,wood.z)<9.201f);
  reset();assert(wood.lights==0&&!wood.rest&&!wood.boundary&&!wood.silence);
  screen=Screen::World;assert(wood.day);assert(decorativeEmission(2.f)==0.f);
  openDrawer(1);handleWorldClick(800,220);assert(!wood.day&&wood.lights==0);assert(decorativeEmission(2.f)==2.f);
  handleWorldClick(800,220);assert(wood.day&&wood.lights==0);
  handleWorldClick(980,30);assert(!wood.help);
  handleWorldClick(50,120);assert(wood.taskPanel);
  handleWorldClick(50,195);assert(selected==0&&wood.navigating&&!wood.taskPanel);
  openDrawer(0);openDrawer(2);assert(wood.wardrobe&&!wood.taskPanel&&!wood.help);
  openDrawer(1);assert(wood.help&&!wood.wardrobe&&!wood.taskPanel);
  openDrawer(1);assert(!wood.help);
  assert(openTabCount()==4&&asideTabCount()==0);
  toggleLater();assert(openTabCount()==3&&asideTabCount()==1);toggleLater();
  focus=true;assert(motionBudget(90)==18);focus=false;assert(motionBudget(90)==90);
  wood.x=0;wood.z=0;walkToward(1,1);worldTick(.1f);assert(wood.x>0&&wood.z>0);
  for(int task=0;task<4;++task){
   reset();screen=Screen::World;selected=task;
   for(int stage=0;stage<3;++stage){
    Vec3 target=activityTarget(task);wood.x=target.x;wood.z=target.z;interactWood();
    if(task<3){assert(tasks[task].done==stage&&wood.carrying[task]);target=activityTarget(task);wood.x=target.x;wood.z=target.z;interactWood();}
    assert(tasks[task].done==stage+1);
   }
  }
  reset();screen=Screen::World;wood.wardrobe=true;wardrobeClick(820,325);assert(wood.character==1);
  wardrobeClick(900,175);assert(wood.season==1&&wood.lights==0);
  wood.x=2;wood.z=3;enterRest();assert(screen==Screen::Break);leaveRest();assert(wood.x==2&&wood.z==3);
  reset();screen=Screen::Desktop;
  journalClick(150,370);assert(selected==2&&screen==Screen::Desktop);
  journalClick(450,510);assert(screen==Screen::World&&wood.navigating&&tasks[2].done==0);
  screen=Screen::Desktop;journalClick(730,510);assert(tasks[2].later);
  journalClick(730,510);assert(!tasks[2].later&&tasks[2].done==0);
  reset();
  std::cout<<"PASS: 3D proximity, story lanterns, boundary and quiet limits, sanctuary, rest, movement, and reset;  step limits, Later guard, restore, reset, hit boundaries\n";return 0;
 }

 glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH|GLUT_MULTISAMPLE);glutInitWindowSize(W,H);
 glutCreateWindow("Too Many Tabs Open | The Lanternwood - 3D");glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
 forestShader.init();
 glutCloseFunc([](){ambience.stop();});
 glutIgnoreKeyRepeat(1);glutKeyboardUpFunc(keyUp);glutSpecialFunc(specialKey);glutMotionFunc(worldMotion);
 glutDisplayFunc(display);glutReshapeFunc(reshape);glutKeyboardFunc(keyboard);glutMouseFunc(mouse);
 glutFullScreen();
 lastTick=glutGet(GLUT_ELAPSED_TIME);glutTimerFunc(16,timer,0);glutMainLoop();return 0;
}

