#pragma once
// All persistent HUD text has an opaque backing. Drawers are mutually exclusive.
const Color paper{.96f,.91f,.79f}, forestInk{.16f,.25f,.23f}, moss{.18f,.29f,.25f}, gilt{.76f,.61f,.35f};
void storyPlate(float x,float y,float w,float h){
 rounded(x,y,w,h,gilt);rounded(x+2,y+2,w-4,h-4,moss);
 // Tiny leaf ornaments frame the storybook without covering the view.
 for(float side:{0.f,1.f}){float xx=x+10+side*(w-20);line(xx,y+10,xx,y+25,gilt);circle(xx-3,y+15,2.5f,gilt);circle(xx+3,y+20,2.5f,gilt);}
}
void storyButton(int x,int y,int w,const string&label,bool active=false){
 rounded(float(x),float(y),float(w),34,active?paper:Color{.25f,.37f,.30f});
 uiText(x+12.f,y+23.f,label,active?forestInk:paper,12);
}
string tabState(int i){return tasks[i].done==3?"Settled":tasks[i].later?"Safely set aside":std::to_string(tasks[i].done)+" / 3 small steps";}
string actionHint(){
 if(wood.lights>=3&&closeTo(0,6,2))return "Enter the sanctuary";
 if(tasks[selected].later)return "This tab can wait. Bring it back in Tabs.";
 if(tasks[selected].done==3)return "A little room made. Choose a tab or rest.";
 string hint=activityVerb(selected);if(hint.substr(0,3)=="E: ")hint=hint.substr(3);
 if(!hint.empty())hint[0]=char(std::toupper(static_cast<unsigned char>(hint[0])));
 return hint;
}
bool actionReady(){Vec3 p=activityTarget(selected);return (wood.lights>=3&&closeTo(0,6,2))||(!tasks[selected].later&&tasks[selected].done<3&&closeTo(p.x,p.z));}
void tabsDrawer(){
 storyPlate(20,108,292,477);
 uiText(40,140,"THE OPEN TABS",paper,18);
 uiText(40,165,"Choose one. The others can wait.",paper,12);
 for(int i=0;i<4;++i){int y=185+i*55;storyButton(34,y,264,std::to_string(i+1)+"  "+tasks[i].title,i==selected);uiText(48,y+48,tabState(i),paper,11);}
 storyButton(34,423,126,focus?"Unfocus [F]":"Focus [F]",focus);
 storyButton(168,423,130,tasks[selected].later?"Restore [L]":"Later [L]",tasks[selected].later);
 uiText(40,485,focus?"Only this tab asks for attention.":"Focus softens the other spaces.",paper,11);
 storyButton(34,510,126,quiet?"Quiet: on [M]":"Quiet [M]",quiet);
 storyButton(168,510,130,"Rest [B]");
}
void guideDrawer(){
 storyPlate(744,108,336,464);
 uiText(767,142,"FOREST GUIDE",paper,19);
 storyButton(764,163,294,ambience.playing?"Sound: on [V]":"Sound: off [V]",ambience.playing);
 storyButton(764,207,294,wood.day?"Visit the night forest [N]":"Visit the day forest [N]");
 storyButton(764,251,294,"Character & season [C]");
 uiText(767,315,"Move at your own pace",paper,15);
 wrapText(767,344,"Click the ground to walk. Near the gold marker, E performs the action shown below.",36,paper);
 uiText(767,420,"WASD walk   Drag right to orbit",paper,12);
 uiText(767,446,"Wheel zoom   F11 window size",paper,12);
 uiText(767,472,"J journal   R restart   Esc exit",paper,12);
 storyButton(764,511,294,"Close guide [H]");
}
bool handleWorldClick(int x,int y){
 if(hit(x,y,20,20,292,73))return true;
 if(hit(x,y,20,108,140,34)&&!wood.taskPanel){openDrawer(0);return true;}
 if(hit(x,y,944,20,136,34)){openDrawer(1);return true;}
 if(wood.wardrobe){if(wardrobeClick(x,y))return true;return true;}
 if(wood.help){
  if(hit(x,y,764,163,294,34)){ambience.toggle();return true;}
  if(hit(x,y,764,207,294,34)){toggleDay();return true;}
  if(hit(x,y,764,251,294,34)){openDrawer(2);return true;}
  if(hit(x,y,764,511,294,34)){openDrawer(1);return true;}
  return true;
 }
 if(wood.taskPanel){
  for(int i=0;i<4;++i)if(hit(x,y,34,185+i*55,264,34)){selectTab(i);Vec3 p=nextSpark(i);if(!tasks[i].later&&tasks[i].done<3)walkToward(p.x,p.z);return true;}
  if(hit(x,y,34,423,126,34)){focus=!focus;return true;}
  if(hit(x,y,168,423,130,34)){toggleLater();return true;}
  if(hit(x,y,34,510,126,34)){toggleQuiet();return true;}
  if(hit(x,y,168,510,130,34)){wood.taskPanel=false;enterRest();return true;}
  if(hit(x,y,20,108,292,477))return true;
  wood.taskPanel=false;return true;
 }
 if(hit(x,y,300,635,500,68)){
  if(actionReady())interactWood();
  else if(!tasks[selected].later&&tasks[selected].done<3){Vec3 p=activityTarget(selected);walkToward(p.x,p.z);}
  else openDrawer(0);
  return true;
 }
 return sceneClick(x,y);
}
void gameHUD(){
 storyPlate(20,20,292,73);
 uiText(40,47,"TOO MANY TABS OPEN",paper,16);
 uiText(40,73,string(seasonName())+" / "+(wood.day?"Daydream Grove":"Lanternwood"),paper,12);
 if(screen==Screen::World){
  storyButton(944,20,136,"Menu [H]",wood.help);
  if(!wood.taskPanel)storyButton(20,108,140,"Tabs [T]  "+std::to_string(openTabCount()));
  storyPlate(395,20,310,59);
  uiText(413,43,std::to_string(openTabCount())+" open / "+std::to_string(asideTabCount())+" set aside",paper,12);
  uiText(413,64,std::to_string(wood.lights)+" / 3 story lanterns",paper,12);
  // One action, one location cue; no world-space text or automatic popups.
  storyPlate(300,635,500,68);
  uiText(320,659,actionHint(),paper,14);
  string cue=actionReady()?"E / click to interact":tasks[selected].later||tasks[selected].done==3?"Open Tabs [T] to choose what comes next":"Click here to walk to the gold marker";
  uiText(320,684,cue,paper,12);
  if(wood.taskPanel)tabsDrawer();
  if(wood.help)guideDrawer();
  if(wood.wardrobe)wardrobePanel();
 }else if(screen==Screen::Break){
  storyPlate(300,555,500,132);
  uiText(322,586,"THE QUIET GLADE",paper,20);
  uiText(322,613,"Nothing needs finishing here. Watch the ripple.",paper,12);
  storyButton(320,635,190,paused?"Resume [Space]":"Pause [Space]");
  storyButton(525,635,255,"Return to forest [B / E]");
 }else{
  storyPlate(300,155,500,405);
  bool end=screen==Screen::Ending;
  uiText(328,200,end?"A LITTLE MORE ROOM":"Once upon an open tab...",paper,23);
  wrapText(328,241,end?"You made room through small choices. Unfinished tabs can stay safely here.":"A student's open tabs became a forest. Each scroll is a thought asking for attention.",52,paper);
  wrapText(328,315,end?"A step, a boundary, quiet, and rest all belong in your story.":"Choose one tab. Follow its gold marker to collect, carry, or grow something. Or choose Later, Quiet, or Rest.",52,paper);
  wrapText(328,395,end?"There is no need to clear every tab.":"Make room for three lanterns, then visit the sanctuary. There is no timer.",52,paper);
  storyButton(328,458,444,end?"Keep exploring [Enter]":"Enter the forest [Enter]",true);
  uiText(328,531,"A reflection game, not a wellbeing score.",paper,12);
 }
}
