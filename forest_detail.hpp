#pragma once
// Procedural woodland details: layered broadleaf trees, a pond, flowers and a deer.
void branchBetween(Vec3 a,Vec3 b,float radius,Color bark){
 float dx=b.x-a.x,dy=b.y-a.y,dz=b.z-a.z,len=std::sqrt(dx*dx+dy*dy+dz*dz);
 if(len<.001f)return;
 glPushMatrix();glTranslatef(a.x,a.y,a.z);
 if(std::fabs(dx)+std::fabs(dy)>.001f)glRotatef(std::acos(std::clamp(dz/len,-1.f,1.f))*360/TAU,-dy,dx,0);
 else if(dz<0)glRotatef(180,1,0,0);
 material(bark);glutSolidCylinder(radius,len,9,2);glPopMatrix();
}
void broadleaf(float x,float z,int seed){
 float h=4.3f+(seed%4)*.6f;Color bark{.29f,.23f,.17f};
 branchBetween({x,0,z},{x+.12f,h,z},.24f,bark);
 for(int k=0;k<4;++k){float a=seed+k*TAU/4;float ex=x+1.3f*std::cos(a),ez=z+1.25f*std::sin(a);
  branchBetween({x,h*.60f,z},{ex,h+.15f*(k%2),ez},.09f,bark);
  Color leaf=seed%4==0?Color{.67f,.43f,.57f}:seed%4==1?Color{.49f,.47f,.66f}:k%2?Color{.29f,.44f,.34f}:Color{.43f,.57f,.40f};
  if(wood.season==1)leaf=k%2?Color{.78f,.39f,.16f}:Color{.85f,.59f,.23f};
  if(wood.season==2)leaf=k%2?Color{.49f,.64f,.68f}:Color{.65f,.76f,.81f};
  ball(ex,h+.5f*(k%2),ez,1.55f,.9f,1.35f,leaf);
  if(wood.season==2)ball(ex,h+.72f+.5f*(k%2),ez,1.40f,.25f,1.23f,{.90f,.94f,1});
  branchBetween({x,0,z},{x+.8f*std::cos(a),.05f,z+.8f*std::sin(a)},.10f,bark);
 }
 Color crown=wood.season==1?Color{.79f,.46f,.20f}:wood.season==2?Color{.65f,.77f,.83f}:seed%4==0?Color{.73f,.49f,.61f}:seed%4==1?Color{.56f,.51f,.72f}:Color{.37f,.52f,.37f};
 ball(x,h+.4f,z,1.7f,1.15f,1.5f,crown);
 if(wood.season==2)ball(x,h+1.32f,z,1.5f,.30f,1.3f,{.92f,.96f,1});
}
void flowerPatch(float x,float z,int seed){
 for(int n=0;n<5;++n){float a=n*2.399f+seed;float xx=x+std::cos(a)*.38f,zz=z+std::sin(a)*.38f;
  branchBetween({xx,.02f,zz},{xx,.28f,zz},.013f,{.31f,.48f,.26f});
  Color c=(seed+n)%2?Color{.88f,.48f,.60f}:Color{.74f,.63f,.89f};
  for(int p=0;p<4;++p){float b=p*TAU/4;ball(xx+.065f*std::cos(b),.3f,zz+.065f*std::sin(b),.065f,.027f,.065f,c);}
  ball(xx,.32f,zz,.035f,.025f,.035f,{1,.83f,.39f});
 }
}
void forestDeer(){
 glPushMatrix();glTranslatef(7,.1f,-.7f);glRotatef(-30,0,1,0);
 Color fur{.64f,.39f,.22f};
 ball(0,.9f,0,.65f,.38f,.27f,fur);ball(.46f,1.25f,0,.2f,.47f,.19f,fur);ball(.56f,1.62f,.01f,.29f,.20f,.17f,fur);
 for(float x:{-.40f,.37f})for(float z:{-.16f,.16f})branchBetween({x,.67f,z},{x,.05f,z},.043f,{.49f,.30f,.19f});
 for(float side:{-1.f,1.f}){ball(.45f,1.89f,side*.15f,.08f,.18f,.045f,fur);ball(.70f,1.68f,side*.15f,.022f,.027f,.017f,{.07f,.06f,.05f});}
 for(int j=0;j<5;++j)ball(-.30f+j*.13f,1.08f,.245f,.036f,.023f,.018f,{.95f,.87f,.65f});
 glPopMatrix();
}
void forestDetails(){
 // Cool distant silhouettes add real depth behind the playable clearing.
 for(int i=0;i<10;++i){float x=(i-4.5f)*4.3f,y=5.5f+(i%4)*1.1f,z=-23.f+(i%3)*2.f;cube(x,y/2,z,.3f,y,.3f,{.30f,.39f,.38f});ball(x,y,z,2.4f,1.8f,2,{.40f,.49f,.48f});}
 if(wood.season!=2)for(int i=0;i<24;++i){float a=i*2.399f;float r=8.8f+(i%3)*.45f;flowerPatch(r*std::cos(a),r*.8f*std::sin(a),i);}
 // Pond, pebbles and a wooden bench make the rest space recognizable.
 ball(-7,.08f,.2f,1.35f,.035f,1.6f,wood.season==2?Color{.69f,.84f,.92f}:Color{.31f,.58f,.58f},.05f);
 for(int i=0;i<12;++i){float a=i*TAU/12;ball(-7+1.4f*std::cos(a),.10f,.2f+1.65f*std::sin(a),.22f,.12f,.19f,{.43f,.45f,.34f});}
 cube(-7,.48f,2.7f,1.7f,.13f,.60f,{.45f,.29f,.19f});cube(-7,.84f,2.98f,1.7f,.45f,.1f,{.45f,.29f,.19f});
 for(float x:{-7.6f,-6.4f})cube(x,.22f,2.7f,.1f,.5f,.43f,{.32f,.24f,.18f});
 if(wood.day)forestDeer();
 else{
  ball(7,.32f,-.7f,.62f,.24f,.34f,{.64f,.39f,.22f});
  ball(7.4f,.40f,-.6f,.25f,.19f,.18f,{.64f,.39f,.22f});
  // Moonflowers open around the pond while the deer rests.
  for(int i=0;i<7;++i){float a=i*TAU/7;ball(-7+1.1f*std::cos(a),.15f,.2f+1.3f*std::sin(a),.12f,.05f,.12f,{.73f,.77f,1},.22f);}
 }
 label3(-8,1.1f,.2f,"QUIET GLADE - click to rest",{.95f,.87f,.64f});
 if(wood.day&&wood.season==0)for(int i=0;i<8;++i){float a=wood.time*.4f+i*1.7f;float x=5*std::sin(a),z=3*std::cos(a*.7f),y=1.7f+.5f*std::sin(a*2);
  float wing=.10f+.12f*std::fabs(std::sin(wood.time*8+i));
  ball(x-wing,y,z,wing,.07f,.11f,{.91f,.59f,.22f});ball(x+wing,y,z,wing,.07f,.11f,{.91f,.59f,.22f});
 }
 if(wood.season!=2)for(int i=0;i<18;++i){float t=wood.time*.15f+i*.7f;float y=4.f-std::fmod(t,3.6f);ball(8*std::sin(i*2.4f)+.3f*std::sin(t),y,6*std::cos(i*1.7f),.045f,.016f,.075f,wood.season==1?Color{.92f,.50f,.18f}:Color{.92f,.68f,.76f});}
}
