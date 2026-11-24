#pragma once
void pageModel(Vec3 p,float size=.5f){
 glPushMatrix();glTranslatef(p.x,p.y,p.z);glRotatef(-12,0,0,1);
 cube(0,.15f,0,size,.035f,size*.7f,{.95f,.87f,.67f});
 for(int i=0;i<3;++i){cube(-size*.05f,.172f,(i-1)*size*.13f,size*.6f,.005f,.013f,{.48f,.39f,.32f});}
 glPopMatrix();
}
void activityModels(){
 for(int k=0;k<3;++k){
  if(k>=tasks[0].done&&!(k==tasks[0].done&&wood.carrying[0]))pageModel(pageSources[k]);
  if(k>=tasks[1].done&&!(k==tasks[1].done&&wood.carrying[1])){Vec3 p=plankSources[k];cube(p.x,.16f,p.z,.8f,.13f,.21f,{.61f,.39f,.23f});}
  Vec3 bed=plantBeds[k];ball(bed.x,.1f,bed.z,.40f,.1f,.38f,{.34f,.24f,.21f});
  if(k<tasks[2].done){branchBetween({bed.x,.15f,bed.z},{bed.x,.85f,bed.z},.025f,{.30f,.55f,.29f});
   for(int n=0;n<5;++n){float a=n*TAU/5;ball(bed.x+.15f*std::cos(a),.88f,bed.z+.15f*std::sin(a),.15f,.06f,.12f,{.93f,.56f,.72f});}ball(bed.x,.91f,bed.z,.09f,.05f,.09f,{1,.82f,.31f});}
 }
 // The study book gains real, visible pages when a delivery succeeds.
 cube(-5,.35f,-3.5f,1.35f,.13f,.85f,{.38f,.28f,.47f});
 for(int n=0;n<tasks[0].done;++n)pageModel({-5,.30f+n*.05f,-3.5f},1.15f);
 // A short stream and the three repaired sections of a support bridge.
 ball(5,.12f,-3.5f,1.3f,.025f,.6f,wood.season==2?Color{.66f,.82f,.91f}:Color{.31f,.59f,.64f});
 for(int n=0;n<tasks[1].done;++n)cube(4.3f+n*.65f,.28f,-3.5f,.58f,.10f,.85f,{.62f,.42f,.25f});
 ball(-7,.22f,4.8f,.25f,.30f,.22f,{.68f,.49f,.25f});label3(-7.5f,.95f,4.8f,"SEED POUCH",{.96f,.83f,.50f});
 if(tasks[3].done<1){ball(6.5f,.30f,5,.25f,.32f,.25f,{.29f,.58f,.32f});label3(6.0f,1.05f,5,"TEA LEAVES",{.88f,.91f,.65f});}
 if(tasks[3].done==3){cone(-7,.58f,2.7f,.12f,.17f,{.91f,.78f,.61f});ring3(-7,.77f,2.7f,.11f,{.87f,.73f,.51f});}
 // The objective marker is a navigation cue, not a decorative lamp.
 if(tasks[selected].done<3&&!tasks[selected].later){Vec3 p=activityTarget(selected);ring3(p.x,.20f,p.z,.48f,{1,.79f,.34f});label3(p.x-.75f,1.2f,p.z,activityVerb(selected),{1,.88f,.62f});}
 if(wood.carrying[selected]|| (selected==3&&tasks[3].done>0&&tasks[3].done<3)){
  Vec3 p{wood.x,1.4f,wood.z};
  if(selected==0)pageModel(p,.44f);
  else if(selected==1)cube(p.x,p.y,p.z,.7f,.12f,.18f,{.65f,.43f,.25f});
  else ball(p.x,p.y,p.z,.12f,.13f,.12f,selected==2?Color{.91f,.76f,.33f}:Color{.87f,.76f,.62f});
 }
}
