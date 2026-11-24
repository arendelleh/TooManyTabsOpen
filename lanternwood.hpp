#pragma once
// Full 3D scene component. The original task data and desktop remain the journal.
#include "forest_shader.hpp"
constexpr float TAU=6.28318530718f;
struct Vec3 { float x,y,z; };
struct Woodland {
 float x=0,z=5,heading=0,time=0,cameraYaw=0,cameraDistance=20;
 bool day=true,help=false,taskPanel=false,navigating=false;
 float targetX=0,targetZ=0;
 double model[16]{},projection[16]{}; int viewport[4]{};
 int lights=0; bool boundary=false,silence=false,rest=false,dragging=false;
 bool keys[256]{}; int mouseX=0;
} wood;
const Vec3 gardens[4]={{-5,0,-3.5f},{5,0,-3.5f},{-5,0,3.5f},{5,0,3.5f}};
const Color accents[4]={{.72f,.58f,1},{1,.69f,.43f},{.43f,.84f,.75f},{.97f,.58f,.73f}};
bool logicTest=false;
Vec3 nextSpark(int i) {
 float a=tasks[i].done*TAU/3;
 return {gardens[i].x+std::cos(a)*1.4f,0,gardens[i].z+std::sin(a)*1.4f};
}
bool closeTo(float x,float z,float radius=1.35f){return std::hypot(wood.x-x,wood.z-z)<radius;}
void gainLight(){wood.lights=std::min(3,wood.lights+1);}
void resetWood(){wood=Woodland{};}
bool canAcknowledge(){
 if(logicTest)return true;
 Vec3 p=nextSpark(selected);
 if(!closeTo(p.x,p.z)){feedback="Walk to the gold spark for this tab, then press E.";return false;}
 return true;
}
void noteStep(){gainLight();}
void noteBoundary(){if(!wood.boundary&&tasks[selected].later){wood.boundary=true;gainLight();}}
void toggleQuiet(){quiet=!quiet;if(quiet&&!wood.silence){wood.silence=true;gainLight();}feedback=quiet?"The message moth is quiet. You choose when to check in.":"Messages are visible again. You can quiet them whenever you wish.";}
void leaveRest(){screen=Screen::World;if(!wood.rest){wood.rest=true;gainLight();}feedback="Rest belongs in the story too. Return whenever you need.";}
void interactWood(){
 if(wood.lights>=3&&closeTo(0,6,2)){screen=Screen::Ending;return;}
 if(tasks[selected].done==3){feedback="This tab is settled. Choose another, rest, or return to the sanctuary.";return;}
 completeStep();
}

// Immediate-mode helpers use independent matrix stacks for each model.
void material(Color c,float glow=0){
 glColor3f(c.r,c.g,c.b); GLfloat em[]={c.r*glow,c.g*glow,c.b*glow,1};
 glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,em);
}
void ball(float x,float y,float z,float sx,float sy,float sz,Color c,float glow=0){
 glPushMatrix();glTranslatef(x,y,z);glScalef(sx,sy,sz);material(c,glow);glutSolidSphere(1,18,12);glPopMatrix();
}
void cube(float x,float y,float z,float sx,float sy,float sz,Color c){
 glPushMatrix();glTranslatef(x,y,z);glScalef(sx,sy,sz);material(c);glutSolidCube(1);glPopMatrix();
}
void cone(float x,float y,float z,float radius,float height,Color c){
 glPushMatrix();glTranslatef(x,y,z);glRotatef(-90,1,0,0);material(c);glutSolidCone(radius,height,20,4);glPopMatrix();
}
void ring3(float x,float y,float z,float radius,Color c){
 glPushMatrix();glTranslatef(x,y,z);glRotatef(90,1,0,0);material(c,wood.day?0.f:.15f);glutSolidTorus(.035,radius,8,48);glPopMatrix();
}
void label3(float x,float y,float z,const string&s,Color c){
 forestShader.stop();glDisable(GL_LIGHTING);color(c);glRasterPos3f(x,y,z);for(unsigned char ch:s)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,ch);glEnable(GL_LIGHTING);forestShader.begin(wood.day?1.f:0.f);
}
void orthoUI(){
 forestShader.stop();
 glDisable(GL_LIGHTING);glDisable(GL_FOG);glDisable(GL_DEPTH_TEST);glDepthMask(GL_FALSE);
 glMatrixMode(GL_PROJECTION);glLoadIdentity();glOrtho(0,W,H,0,-1,1);glMatrixMode(GL_MODELVIEW);glLoadIdentity();
}
void rounded(float x,float y,float w,float h,Color c){
 float r=14; color(c);glBegin(GL_TRIANGLE_FAN);glVertex2f(x+w/2,y+h/2);
 const float cx[4]={x+r,x+w-r,x+w-r,x+r},cy[4]={y+r,y+r,y+h-r,y+h-r};
 for(int k=0;k<4;++k)for(int j=0;j<=8;++j){float a=(k*90+180+j*90.f/8)*TAU/360;glVertex2f(cx[k]+r*std::cos(a),cy[k]+r*std::sin(a));}
 glVertex2f(x,y+r);glEnd();
}
void wrapText(float x,float y,const string&s,int limit=53,Color c=ink){
 string line,word;auto flush=[&](){text(x,y,line,c,GLUT_BITMAP_HELVETICA_12);y+=19;line.clear();};
 for(size_t k=0;k<=s.size();++k){char ch=k<s.size()?s[k]:' ';if(ch==' '){if(line.size()+word.size()+1>size_t(limit))flush();if(!line.empty())line+=' ';line+=word;word.clear();}else word+=ch;}
 if(!line.empty())flush();
}
void skyBackdrop(){
 orthoUI();glBegin(GL_QUADS);
 if(wood.day)glColor3f(.52f,.68f,.78f);else glColor3f(.025f,.045f,.12f);
 glVertex2f(0,0);glVertex2f(W,0);
 if(wood.day)glColor3f(.88f,.80f,.79f);else glColor3f(.19f,.21f,.33f);
 glVertex2f(W,H);glVertex2f(0,H);glEnd();
 if(wood.day){circle(865,110,43,{1,.91f,.65f});circle(865,110,33,{1,.97f,.80f});}
 else{
  for(int i=0;i<90;++i){float x=float((i*173+31)%W),y=float((i*97+17)%430);float tw=.6f+.25f*std::sin(wood.time*(.4f+i*.005f)+i);circle(x,y,i%13==0?2:1,{tw,tw*.92f,tw*.74f});}
  circle(930,100,35,{.91f,.85f,.69f});circle(942,90,32,{.04f,.06f,.14f});
 }
}
#include "forest_detail.hpp"
#include "fairy_lights.hpp"
void tree(float x,float z,int seed){broadleaf(x,z,seed);}
void fairy(){
 glPushMatrix();glTranslatef(wood.x,.2f+.07f*std::sin(wood.time*3),wood.z);glRotatef(wood.heading*360/TAU,0,1,0);
 // Petal dress, hair, face, arms, and four translucent-looking solid wing lobes.
 float flutter=.15f*std::sin(wood.time*9);
 for(float side:{-1.f,1.f}){
  ball(side*(.44f+flutter*.2f),.95f,-.10f,.40f,.52f,.055f,{.60f,.87f,.83f},.28f);
  ball(side*.36f,.55f,-.10f,.29f,.31f,.055f,{.65f,.64f,.95f},.23f);
  ball(side*.14f,.13f,.04f,.07f,.18f,.10f,{.91f,.69f,.57f});
  ball(side*.29f,.70f,.04f,.07f,.25f,.08f,{.96f,.77f,.62f});
 }
 cone(0,.20f,0,.42f,.68f,{.80f,.40f,.64f});
 ball(0,.83f,0,.24f,.29f,.19f,{.80f,.43f,.67f});
 ball(0,1.2f,0,.27f,.29f,.25f,{.98f,.79f,.64f});
 ball(0,1.37f,-.04f,.29f,.16f,.24f,{.90f,.69f,.35f});
 ball(-.20f,1.2f,-.03f,.11f,.22f,.20f,{.90f,.69f,.35f});
 for(float side:{-1.f,1.f}){ball(side*.09f,1.24f,.232f,.025f,.04f,.02f,{.13f,.12f,.22f});ball(side*.12f,1.15f,.23f,.045f,.022f,.01f,{.91f,.5f,.5f});}
 ring3(0,1.56f,0,.2f,{1,.78f,.35f});
 ball(.36f,.86f,.16f,.09f,.09f,.09f,{1,.84f,.43f},.7f);
 glPopMatrix();
}
void scrollModel(int i){
 const Vec3 p=gardens[i];Color ac=accents[i];float dim=focus&&i!=selected?.28f:1;
 Color stone{.32f*dim,.36f*dim,.40f*dim};
 ball(p.x,-.12f,p.z,2.3f,.32f,2.3f,stone);
 ring3(p.x,.12f,p.z,2.15f,{ac.r*dim,ac.g*dim,ac.b*dim});
 for(int n=0;n<3;++n){float a=n*TAU/3;Color lc=n<tasks[i].done?Color{1,.8f,.3f}:Color{.3f,.35f,.4f};ball(p.x+std::cos(a)*1.4f,.25f,p.z+std::sin(a)*1.4f,.10f,.15f,.10f,lc,n<tasks[i].done?.7f:0);}
 glPushMatrix();glTranslatef(p.x,2.0f+.10f*std::sin(wood.time+i),p.z);glRotatef(-8+3*std::sin(wood.time*.5f+i),0,0,1);
 cube(0,0,0,2.4f,1.35f,.10f,tasks[i].later?Color{.37f,.38f,.45f}:Color{.92f*dim,.84f*dim,.69f*dim});
 cube(-1.21f,0,0,.15f,1.5f,.19f,ac);cube(1.21f,0,0,.15f,1.5f,.19f,ac);
 cube(-.74f,.33f,.08f,.60f,.08f,.06f,ac);cube(0,.07f,.08f,1.65f,.045f,.05f,{.45f,.39f,.38f});cube(-.2f,-.16f,.08f,1.25f,.045f,.05f,{.50f,.43f,.4f});
 glPopMatrix();
 label3(p.x-1.35f,3.05f,p.z,tasks[i].title,i==selected?Color{1,.84f,.48f}:Color{.75f,.8f,.85f});
 if(i==selected&&tasks[i].done<3&&!tasks[i].later){
  Vec3 s=nextSpark(i);float yy=.8f+.12f*std::sin(wood.time*3);
  ball(s.x,yy,s.z,.16f,.24f,.16f,{1,.8f,.3f},1);
  ring3(s.x,.17f,s.z,.43f,{1,.76f,.3f});
  label3(s.x-.45f,1.5f,s.z,"E: small step",{1,.87f,.58f});
 }
}
void sanctuary(){
 for(float side:{-1.f,1.f}){cube(side*1.1f,1.25f,6,.18f,2.6f,.24f,{.52f,.4f,.41f});ball(side*1.1f,2.65f,6,.22f,.25f,.22f,{1,.8f,.35f},decorativeEmission(.4f));}
 cube(0,2.6f,6,2.4f,.15f,.24f,{.69f,.52f,.43f});
 ring3(0,.18f,6,1.7f,wood.lights>=3?Color{1,.8f,.37f}:Color{.42f,.47f,.5f});
 for(int i=0;i<3;++i){float x=(i-1)*.63f;cube(x,1.5f,6,.025f,1.8f,.025f,{.52f,.4f,.4f});ball(x,.7f+.07f*std::sin(wood.time+i),6,.18f,.26f,.18f,i<wood.lights?Color{1,.76f,.32f}:Color{.23f,.28f,.36f},i<wood.lights?decorativeEmission(.8f):0);}
 label3(-1.05f,3.25f,6,wood.lights>=3?"SANCTUARY - E to rest":"THE PATH HOME",{1,.87f,.6f});
}
void scene3D(){
 skyBackdrop();glDepthMask(GL_TRUE);glEnable(GL_DEPTH_TEST);glClear(GL_DEPTH_BUFFER_BIT);
 glMatrixMode(GL_PROJECTION);glLoadIdentity();gluPerspective(48,double(vw)/vh,.1,100);
 glMatrixMode(GL_MODELVIEW);glLoadIdentity();
 float a=wood.cameraYaw;gluLookAt(std::sin(a)*wood.cameraDistance,wood.cameraDistance*.50f,std::cos(a)*wood.cameraDistance,0,.4f,0,0,1,0);
 glGetDoublev(GL_MODELVIEW_MATRIX,wood.model);glGetDoublev(GL_PROJECTION_MATRIX,wood.projection);glGetIntegerv(GL_VIEWPORT,wood.viewport);
 forestShader.begin(wood.day?1.f:0.f);
 glEnable(GL_LIGHTING);glEnable(GL_LIGHT0);glEnable(GL_COLOR_MATERIAL);glEnable(GL_NORMALIZE);
 glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
 GLfloat pos[]={-5,14,10,1},amb[]={.47f,.43f,.53f,1},dif[]={.85f,.78f,.66f,1};
 glLightfv(GL_LIGHT0,GL_POSITION,pos);glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);glLightModelfv(GL_LIGHT_MODEL_AMBIENT,amb);
 glEnable(GL_FOG);GLfloat fog[]={.16f,.16f,.25f,1};glFogfv(GL_FOG_COLOR,fog);glFogi(GL_FOG_MODE,GL_LINEAR);glFogf(GL_FOG_START,30);glFogf(GL_FOG_END,62);
 ball(0,-.65f,0,12,.9f,11,{.38f,.51f,.40f});
 ball(0,-2,0,8,2.2f,7,{.17f,.15f,.24f});
 // Winding stepping-stone paths connect each task garden to the central clearing.
 for(int k=0;k<4;++k)for(int j=1;j<8;++j){float u=j/8.f;ball(gardens[k].x*u,.06f,gardens[k].z*u,.31f,.09f,.25f,{.50f,.47f,.48f});}
 for(int i=0;i<18;++i){float aa=i*TAU/18;if(std::sin(aa)<.65f)tree(11.f*std::cos(aa),10.f*std::sin(aa),i);}
 forestDetails();
 fairyLightGeometry();
 for(int i=0;i<28;++i){float aa=i*2.399f;float r=7.8f+float(i%4)*.45f;float x=r*std::cos(aa),z=r*.84f*std::sin(aa);cube(x,.2f,z,.06f,.38f,.06f,{.72f,.68f,.5f});ball(x,.4f,z,.24f,.12f,.24f,i%2?Color{.73f,.42f,.55f}:Color{.58f,.56f,.81f});}
 for(int i=0;i<4;++i)scrollModel(i);
 sanctuary();
 // Fireflies are time-driven 3D points. Focus quiets the visual motion.
 forestShader.stop();glDisable(GL_LIGHTING);glPointSize(3);glBegin(GL_POINTS);
 for(int i=0;i<(wood.day?0:90);++i){float t=wood.time*(focus?.12f:.30f),aa=i*2.399f+t;float r=2+float(i%11)*.72f;color({.85f,.79f,.45f});glVertex3f(r*std::cos(aa),.7f+float(i%9)*.35f+.2f*std::sin(t+i),r*.8f*std::sin(aa));}glEnd();glEnable(GL_LIGHTING);forestShader.begin(wood.day?1.f:0.f);
 // The notification is a drifting message moth, quieted by the original M action.
 if(!quiet){float mx=3.2f*std::sin(wood.time*.4f);ball(mx,4.6f,1,.2f,.10f,.10f,{.87f,.63f,.7f});label3(mx-.8f,4.95f,1,"Any updates?  [M] quiet",{.95f,.72f,.79f});}
 fairy();
 fairyGlowPass();
 orthoUI();
}
bool projectedNear(Vec3 p,int x,int y,float radius){
 double sx,sy,sz;if(!gluProject(p.x,p.y,p.z,wood.model,wood.projection,wood.viewport,&sx,&sy,&sz)||sz>1)return false;
 float xx=float((sx-vx)/vw*W),yy=H-float((sy-vy)/vh*H);
 return std::hypot(xx-x,yy-y)<radius;
}
void walkToward(float x,float z){wood.targetX=x;wood.targetZ=z;wood.navigating=true;}
void toggleDay(){wood.day=!wood.day;feedback=wood.day?"The Daydream Grove - a warmer kind of space.":"The Lanternwood - let the little lights guide you.";}
bool sceneClick(int x,int y){
 if(projectedNear({-7,.4f,.2f},x,y,35)){screen=Screen::Break;breathTime=0;paused=false;wood.navigating=false;return true;}
 if(projectedNear({7,1,-.7f},x,y,28)){feedback="A quiet companion. You can pause here without finishing another task.";return true;}
 if(wood.lights>=3&&projectedNear({0,1.3f,6},x,y,42)){if(closeTo(0,6,2))interactWood();else walkToward(0,6);return true;}
 for(int i=0;i<4;++i)if(projectedNear({gardens[i].x,2,gardens[i].z},x,y,48)||projectedNear(nextSpark(i),x,y,24)){
  selected=i;wood.taskPanel=true;Vec3 p=nextSpark(i);walkToward(p.x,p.z);feedback="Walking to this scroll. Read the step, then choose what feels useful.";return true;
 }
 double ax,ay,az,bx,by,bz;double wx=vx+double(x)*vw/W,wy=vy+double(H-y)*vh/H;
 if(gluUnProject(wx,wy,0,wood.model,wood.projection,wood.viewport,&ax,&ay,&az)&&gluUnProject(wx,wy,1,wood.model,wood.projection,wood.viewport,&bx,&by,&bz)&&std::fabs(by-ay)>.001){
  double t=-ay/(by-ay);float xx=float(ax+(bx-ax)*t),zz=float(az+(bz-az)*t);
  if(t>0&&std::hypot(xx,zz)<9.2f){walkToward(xx,zz);return true;}
 }
 return false;
}
void uiActionButton(int x,int y,int w,const string&label,bool active=false){
 rounded(float(x),float(y),float(w),33,active?Color{.26f,.40f,.35f}:Color{.18f,.23f,.25f});
 text(x+10.f,y+22.f,label,{.96f,.91f,.78f},GLUT_BITMAP_HELVETICA_12);
}
bool handleWorldClick(int x,int y){
 if(hit(x,y,820,20,133,33)){toggleDay();return true;}
 if(hit(x,y,965,20,110,33)){wood.help=!wood.help;return true;}
 for(int i=0;i<4;++i)if(hit(x,y,20,90+i*38,175,32)){selected=i;wood.taskPanel=true;Vec3 p=nextSpark(i);walkToward(p.x,p.z);return true;}
 if(hit(x,y,831,425,244,38)&&wood.taskPanel){wood.taskPanel=false;return true;}
 if(hit(x,y,173,626,110,33)){interactWood();return true;}
 if(hit(x,y,293,626,110,33)){toggleLater();return true;}
 if(hit(x,y,413,626,110,33)){focus=!focus;return true;}
 if(hit(x,y,533,626,110,33)){toggleQuiet();return true;}
 if(hit(x,y,653,626,110,33)){screen=Screen::Break;breathTime=0;paused=false;wood.navigating=false;return true;}
 if(hit(x,y,773,626,145,33)){wood.taskPanel=!wood.taskPanel;return true;}
 if(wood.taskPanel&&hit(x,y,807,265,275,200))return true;
 if(wood.help&&hit(x,y,280,170,540,230))return true;
 return sceneClick(x,y);
}
void gameHUD(){
 const Color card{.085f,.10f,.16f},gold{1,.81f,.49f},cream{.96f,.90f,.80f};
 text(30,39,"TOO MANY TABS OPEN",cream,GLUT_BITMAP_HELVETICA_18);
 text(30,61,wood.day?"THE DAYDREAM GROVE":"THE LANTERNWOOD",gold,GLUT_BITMAP_HELVETICA_12);
 text(490,42,std::to_string(wood.lights)+" / 3 story lanterns",gold,GLUT_BITMAP_HELVETICA_12);
 uiActionButton(680,20,125,ambience.playing?"Sound: on [V]":"Sound: off [V]",ambience.playing);
 uiActionButton(820,20,133,wood.day?"Night [N]":"Day [N]");if(screen==Screen::World)uiActionButton(965,20,110,"Help [H]",wood.help);
 if(screen==Screen::Welcome||screen==Screen::Ending){
  rounded(65,120,530,454,card);
  text(95,168,screen==Screen::Ending?"A LITTLE MORE ROOM":"Once upon an open tab...",cream,GLUT_BITMAP_TIMES_ROMAN_24);
  wrapText(95,210,screen==Screen::Ending?"The lanterns are lit. Your unfinished tasks can wait here. You do not have to finish everything to deserve a pause.":"A student opened so many tabs that the thoughts became a forest. You are its little keeper. Find a little room among the branches.",65,cream);
  text(95,290,screen==Screen::Ending?"You can keep exploring, or close this chapter.":"Light three story lanterns, then return to the sanctuary.",gold,GLUT_BITMAP_HELVETICA_12);
  wrapText(95,325,"Take a small step, set a tab aside, quiet the message moth, or rest. Each is a valid way forward. There is no timer and nothing to lose.",65);
  button(95,390,315,screen==Screen::Ending?"Keep exploring [Enter]":"Enter the forest [Enter]",true);
  text(95,460,"WASD walk   Right-drag orbit   E interact",cream,GLUT_BITMAP_HELVETICA_12);
  text(95,485,"Fictional reflection game, not a mental-health assessment.",muted,GLUT_BITMAP_HELVETICA_12);
  text(95,505,"If you need support, a trusted person or campus counselor",muted,GLUT_BITMAP_HELVETICA_12);
  text(95,523,"can be a place to start.",muted,GLUT_BITMAP_HELVETICA_12);
 }else if(screen==Screen::Break){
  rounded(300,135,500,435,card);text(350,180,"The quiet glade",cream,GLUT_BITMAP_TIMES_ROMAN_24);
  float phase=std::fmod(breathTime,8.f),amount=(1-std::cos(phase*TAU/8))/2;
  circle(550,340,110,{.11f,.23f,.27f});circle(550,340,52+40*amount,{.45f,.72f,.66f});
  text(482,345,paused?"A quiet pause":"Let the glow drift",card,GLUT_BITMAP_HELVETICA_12);
  text(370,490,"Follow gently if comfortable, or simply rest here.",cream,GLUT_BITMAP_HELVETICA_12);
  text(370,520,"Space pause / resume    B or E return",gold,GLUT_BITMAP_HELVETICA_12);
 }else{
  // Small, actual buttons replace the large instructions covering the forest.
  for(int i=0;i<4;++i){int yy=90+i*38;uiActionButton(20,yy,175,std::to_string(i+1)+"  "+tasks[i].title,i==selected);}
  rounded(161,615,770,57,card);
  uiActionButton(173,626,110,"Small step [E]");
  uiActionButton(293,626,110,tasks[selected].later?"Bring back [L]":"Later [L]");
  uiActionButton(413,626,110,"Focus [F]",focus);
  uiActionButton(533,626,110,quiet?"Quiet: on [M]":"Quiet [M]",quiet);
  uiActionButton(653,626,110,"Rest [B]");
  uiActionButton(773,626,145,"Task details [T]",wood.taskPanel);
  if(wood.taskPanel){
   rounded(807,265,275,200,card);text(825,292,tasks[selected].title,accents[selected],GLUT_BITMAP_HELVETICA_12);
   text(825,316,tasks[selected].later?"SET ASIDE":tasks[selected].done==3?"SETTLED":"YOUR NEXT SMALL STEP",gold,GLUT_BITMAP_HELVETICA_10);
   wrapText(825,344,tasks[selected].done==3?"You can stop here or choose another tab.":tasks[selected].steps[tasks[selected].done],32,cream);
   text(825,397,"Approach the gold spark, then E.",muted,GLUT_BITMAP_HELVETICA_10);
   uiActionButton(831,425,244,"Close task details [T]");
  }
  if(wood.help){
   rounded(280,170,540,230,card);text(308,205,"A LITTLE GUIDE",gold);
   text(308,242,"Click the ground to walk. Click a scroll to visit its space.",cream,GLUT_BITMAP_HELVETICA_12);
   text(308,270,"WASD walk | Right-drag orbit | Wheel zoom | J journal",cream,GLUT_BITMAP_HELVETICA_12);
   text(308,298,"Light three lanterns through small choices, then return home.",cream,GLUT_BITMAP_HELVETICA_12);
   text(308,326,"A step, a boundary, quiet, or rest are all valid choices.",cream,GLUT_BITMAP_HELVETICA_12);
   text(308,363,"H close help | F11 full-screen | R restart | Esc exit",muted,GLUT_BITMAP_HELVETICA_12);
  }
  text(24,698,wood.lights>=3?"The path is lit. Click the sanctuary arch to head home.":feedback,gold,GLUT_BITMAP_HELVETICA_12);
 }

}
void displayGame(){
 static int lastTheme=-1;
 if(lastTheme!=int(wood.day)){glutSetWindowTitle(wood.day?"Too Many Tabs Open | The Daydream Grove - 3D":"Too Many Tabs Open | The Lanternwood - 3D");lastTheme=int(wood.day);}
 glClearColor(.07f,.08f,.15f,1);glDepthMask(GL_TRUE);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);scene3D();gameHUD();glDepthMask(GL_TRUE);glutSwapBuffers();
}
void worldTick(float dt){
 wood.time+=dt;
 if(screen!=Screen::World)return;
 float dx=float(wood.keys['d']||wood.keys['D'])-float(wood.keys['a']||wood.keys['A']);
 float dz=float(wood.keys['s']||wood.keys['S'])-float(wood.keys['w']||wood.keys['W']);
 float len=std::hypot(dx,dz);
 if(len>0)wood.navigating=false;
 if(len==0&&wood.navigating){float xx=wood.targetX-wood.x,zz=wood.targetZ-wood.z,d=std::hypot(xx,zz);
  if(d<.18f)wood.navigating=false;
  else{float step=std::min(d,dt*3.5f);wood.x+=xx/d*step;wood.z+=zz/d*step;wood.heading=std::atan2(xx,zz);}
 }
 if(len>0){dx/=len;dz/=len;float a=wood.cameraYaw;float wx=dx*std::cos(a)+dz*std::sin(a),wz=-dx*std::sin(a)+dz*std::cos(a);wood.x+=wx*dt*3.5f;wood.z+=wz*dt*3.5f;wood.heading=std::atan2(wx,wz);float r=std::hypot(wood.x,wood.z);if(r>9.2f){wood.x*=9.2f/r;wood.z*=9.2f/r;}}
}
void keyUp(unsigned char key,int,int){wood.keys[key]=false;}
void toggleFull(){static bool full=true;full=!full;if(full)glutFullScreen();else{glutReshapeWindow(W,H);glutPositionWindow(80,60);}}
void specialKey(int key,int,int){if(key==GLUT_KEY_F11)toggleFull();}
void worldMotion(int x,int){if(wood.dragging){wood.cameraYaw+=(x-wood.mouseX)*.008f;wood.mouseX=x;}}

