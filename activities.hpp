#pragma once
// Concrete in-world actions replace the old "acknowledge a step" interaction.
const Vec3 pageSources[3]={{-7,0,-5},{-4,0,-5.5f},{-6.5f,0,-1.5f}};
const Vec3 plankSources[3]={{7,0,-5},{4,0,-5.5f},{6.5f,0,-1.5f}};
const Vec3 plantBeds[3]={{-5.7f,0,3},{-4.5f,0,3.5f},{-5.2f,0,4.5f}};
Vec3 activityTarget(int task){
 int step=std::min(tasks[task].done,2);
 if(task==0)return wood.carrying[task]?gardens[task]:pageSources[step];
 if(task==1)return wood.carrying[task]?gardens[task]:plankSources[step];
 if(task==2)return wood.carrying[task]?plantBeds[step]:Vec3{-7,0,4.8f};
 return step==0?Vec3{6.5f,0,5}:step==1?Vec3{-7,0,.2f}:Vec3{-7,0,2.7f};
}
string activityVerb(int task){
 if(tasks[task].later)return "Bring this tab back first [L]";
 if(tasks[task].done==3)return "This activity is complete";
 if(task==0)return wood.carrying[task]?"E: place page in the study book":"E: collect the lost page";
 if(task==1)return wood.carrying[task]?"E: place a bridge plank":"E: pick up a bridge plank";
 if(task==2)return wood.carrying[task]?"E: plant a seed in the flower bed":"E: take a seed from the pouch";
 return tasks[task].done==0?"E: gather tea leaves":tasks[task].done==1?"E: fill the cup at the pond":"E: place your tea on the bench";
}
void performActivity(){
 if(tasks[selected].later){feedback="This tab is set aside. Press L to bring it back when you want.";return;}
 if(tasks[selected].done==3){feedback="This activity is complete. You can explore or take a rest.";return;}
 Vec3 p=activityTarget(selected);
 if(!closeTo(p.x,p.z)){feedback="Walk to the marked object. "+activityVerb(selected)+".";return;}
 if(selected<3&&!wood.carrying[selected]){
  wood.carrying[selected]=true;wood.taskPanel=true;
  feedback=selected==0?"Page collected. Carry it to the study book.":selected==1?"Plank collected. Carry it to the broken bridge.":"Seed collected. Walk to the marked flower bed.";
  return;
 }
 wood.activityApproved=true;completeStep();wood.activityApproved=false;
 wood.carrying[selected]=false;
 if(selected==0)feedback="The page is back in the book. One part of the task is clearer.";
 else if(selected==1)feedback="A bridge plank is in place. The path reconnects a little at a time.";
 else if(selected==2)feedback="A flower has grown. Small habits can begin with one action.";
 else feedback=tasks[3].done==1?"Tea leaves gathered. Now visit the pond.":tasks[3].done==2?"Your cup is filled. Carry it to the bench.":"Tea is ready on the bench. You can take a quiet pause.";
}
string nearbyPrompt(){
 if(wood.lights>=3&&closeTo(0,6,2))return "E: enter the sanctuary and finish this chapter";
 Vec3 p=activityTarget(selected);
 if(closeTo(p.x,p.z)&&tasks[selected].done<3)return activityVerb(selected);
 if(closeTo(-7,.2f,1.6f))return "B: rest beside the pond";
 return "Click a marked object to walk to it. "+activityVerb(selected);
}
void enterRest(){
 wood.returnX=wood.x;wood.returnZ=wood.z;wood.restVisit=true;
 wood.x=-6.8f;wood.z=1.8f;wood.navigating=false;
 screen=Screen::Break;breathTime=0;paused=false;
}
