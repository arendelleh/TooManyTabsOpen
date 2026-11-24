#pragma once
void visitJournalActivity(){
 screen=Screen::World;wood.taskPanel=wood.help=wood.wardrobe=false;
 if(!tasks[selected].later&&tasks[selected].done<3){Vec3 p=activityTarget(selected);walkToward(p.x,p.z);}
}
void displayJournal(){
 glDepthMask(GL_TRUE);glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);scene3D();
 // The living forest remains behind a two-page field journal.
 glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);glColor4f(.03f,.09f,.07f,.68f);
 glBegin(GL_QUADS);glVertex2f(0,0);glVertex2f(W,0);glVertex2f(W,H);glVertex2f(0,H);glEnd();glDisable(GL_BLEND);
 uiText(110,94,"THE KEEPER'S JOURNAL",paper,24);
 uiText(110,120,"One thought at a time. Nothing here needs rushing.",paper,14);
 storyButton(823,76,177,"Return to forest [J]");
 rounded(100,158,910,430,gilt);rounded(104,162,296,422,moss);rounded(402,162,604,422,paper);
 rect(395,170,7,405,{.49f,.40f,.25f});
 uiText(125,196,"YOUR OPEN TABS",paper,17);
 for(int i=0;i<4;++i){int y=220+i*70;storyButton(120,y,260,tasks[i].title,i==selected);uiText(134,y+55,tabState(i),paper,12);}
 uiText(125,548,std::to_string(openTabCount())+" open / "+std::to_string(asideTabCount())+" set aside",paper,13);
 const Task&t=tasks[selected];uiText(430,203,t.title,forestInk,23);
 wrapText(430,234,t.subtitle,65,forestInk);
 for(int i=0;i<3;++i){int y=283+i*62;
  circle(442,y+7,11,i<t.done?moss:Color{.82f,.77f,.65f});
  if(i<t.done){line(437,y+7,441,y+11,paper);line(441,y+11,448,y+1,paper);}
  else uiText(438,y+12,std::to_string(i+1),forestInk,12);
  wrapText(467,y+11,t.steps[i],61,forestInk);
 }
 storyButton(430,492,260,t.done==3?"Explore the forest [C]":t.later?"Return to forest [C]":"Go to activity [C]",true);
 storyButton(707,492,270,t.later?"Bring this tab back [L]":"Set aside for later [L]");
 uiText(430,557,"Progress comes from actions in the forest.",forestInk,13);
 storyButton(110,610,210,focus?"Focus: on [F]":"Focus on this tab [F]",focus);
 storyButton(338,610,210,quiet?"Messages quiet [M]":"Quiet messages [M]",quiet);
 storyButton(566,610,210,"Visit quiet glade [B]");
 uiText(110,682,"Small steps, boundaries, and rest all make room.",paper,13);
 glDepthMask(GL_TRUE);glutSwapBuffers();
}
bool journalClick(int x,int y){
 if(hit(x,y,823,76,177,34)){screen=Screen::World;return true;}
 for(int i=0;i<4;++i)if(hit(x,y,120,220+i*70,260,34)){selected=i;return true;}
 if(hit(x,y,430,492,260,34)){visitJournalActivity();return true;}
 if(hit(x,y,707,492,270,34)){toggleLater();return true;}
 if(hit(x,y,110,610,210,34)){focus=!focus;return true;}
 if(hit(x,y,338,610,210,34)){toggleQuiet();return true;}
 if(hit(x,y,566,610,210,34)){enterRest();return true;}
 return false;
}
