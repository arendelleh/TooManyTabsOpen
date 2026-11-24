#pragma once
// Camera-facing additive halos use a tiny procedurally generated RGBA texture.
GLuint glowTexture=0;
float decorativeEmission(float strength){return wood.day?0.f:strength;}
void prepareGlow(){
 if(glowTexture)return;
 unsigned char pixels[64*64*4];
 for(int y=0;y<64;++y)for(int x=0;x<64;++x){float u=(x-31.5f)/31.5f,v=(y-31.5f)/31.5f;
  float d=u*u+v*v,a=std::exp(-d*6.f)*std::max(0.f,1.f-d);
  int i=(y*64+x)*4;pixels[i]=pixels[i+1]=pixels[i+2]=255;pixels[i+3]=static_cast<unsigned char>(255*a);
 }
 glGenTextures(1,&glowTexture);glBindTexture(GL_TEXTURE_2D,glowTexture);glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,64,64,0,GL_RGBA,GL_UNSIGNED_BYTE,pixels);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}
void halo(Vec3 p,float size,Color c,float alpha){
 Vec3 r{float(wood.model[0])*size,float(wood.model[4])*size,float(wood.model[8])*size};
 Vec3 u{float(wood.model[1])*size,float(wood.model[5])*size,float(wood.model[9])*size};
 glColor4f(c.r,c.g,c.b,alpha);glBegin(GL_QUADS);
 glTexCoord2f(0,0);glVertex3f(p.x-r.x-u.x,p.y-r.y-u.y,p.z-r.z-u.z);
 glTexCoord2f(1,0);glVertex3f(p.x+r.x-u.x,p.y+r.y-u.y,p.z+r.z-u.z);
 glTexCoord2f(1,1);glVertex3f(p.x+r.x+u.x,p.y+r.y+u.y,p.z+r.z+u.z);
 glTexCoord2f(0,1);glVertex3f(p.x-r.x+u.x,p.y-r.y+u.y,p.z-r.z+u.z);glEnd();
}
Vec3 festoon(float t,int row){return {-8.f+16*t,4.5f+row*.6f-1.15f*std::sin(t*TAU*.5f),row==0?-4.7f:3.2f+.6f*std::sin(t*TAU*.5f)};}
void lantern(Vec3 p,float scale){
 glPushMatrix();glTranslatef(p.x,p.y,p.z);glScalef(scale,scale,scale);
 Color brass{.75f,.47f,.18f};
 branchBetween({0,.8f,0},{0,1.25f,0},.022f,brass);
 cone(0,.60f,0,.30f,.23f,brass);cube(0,-.10f,0,.46f,.10f,.46f,brass);
 for(float x:{-.20f,.20f})for(float z:{-.20f,.20f})cube(x,.25f,z,.028f,.65f,.028f,brass);
 ball(0,.22f,0,.13f,.25f,.13f,wood.day?Color{.70f,.65f,.53f}:Color{1,.75f,.26f},decorativeEmission(1.8f));
 glPopMatrix();
}
void fairyLightGeometry(){
 for(int row=0;row<2;++row){
  for(int i=0;i<32;++i){Vec3 a=festoon(i/32.f,row),b=festoon((i+1)/32.f,row);branchBetween(a,b,.012f,{.44f,.33f,.20f});}
  for(int i=0;i<=16;++i){Vec3 p=festoon(i/16.f,row);ball(p.x,p.y-.06f,p.z,.055f,.09f,.055f,wood.day?Color{.69f,.65f,.53f}:Color{1,.83f,.44f},decorativeEmission(1.4f));
   if(i%4==0)lantern({p.x,p.y-.65f,p.z},.48f);
  }
 }
 // Tall lantern posts mark the sanctuary and each task garden.
 for(int i=0;i<4;++i){Vec3 p=gardens[i];branchBetween({p.x+1.8f,0,p.z},{p.x+1.8f,2.2f,p.z},.055f,{.41f,.27f,.19f});lantern({p.x+1.8f,1.65f,p.z},.55f);}
}
void fairyGlowPass(){
 prepareGlow();forestShader.stop();glDisable(GL_LIGHTING);glDisable(GL_FOG);glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE);glDepthMask(GL_FALSE);
 glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,glowTexture);glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
 float power=wood.day?0.f:.85f;
 for(int row=0;row<2;++row)for(int i=0;i<=16;++i){Vec3 p=festoon(i/16.f,row);p.y-=.06f;halo(p,.42f,{1,.69f,.25f},power);
  if(i%4==0){p.y-=.48f;halo(p,.72f,{1,.63f,.22f},power);}
 }
 for(int i=0;i<4;++i){Vec3 p=gardens[i];halo({p.x+1.8f,1.78f,p.z},.78f,{1,.67f,.25f},power);}
 for(int i=0;i<3;++i)if(i<wood.lights)halo({(i-1)*.63f,.7f,6},.65f,{1,.76f,.32f},power);
 // Floating shimmers form an animated trail around the keeper and across the clearing.
 for(int i=0;i<motionBudget(wood.day?20:85);++i){float a=i*2.399f+wood.time*.1f,r=.8f+(i%13)*.65f;
  Vec3 p{r*std::cos(a),.5f+(i%11)*.25f+.12f*std::sin(wood.time+i),r*.7f*std::sin(a)};
  float pulse=std::pow(.5f+.5f*std::sin(wood.time*(1.1f+(i%4)*.2f)+i*2.1f),5.f);
  halo(p,.12f+pulse*.18f,i%3==0?Color{.77f,.58f,1}:Color{1,.85f,.47f},(wood.day?.16f:1.f)*(.25f+pulse*.75f));
 }
 if(!wood.day)halo({wood.x,.95f,wood.z},.65f,{.68f,.67f,1},.20f);
 glDisable(GL_TEXTURE_2D);
 // Four-point glints remain crisp inside the diffuse halos.
 glBegin(GL_LINES);
 for(int i=0;i<motionBudget(wood.day?8:28);++i){float a=i*2.399f+wood.time*.05f,r=1+(i%9)*.8f;float x=r*std::cos(a),z=r*.7f*std::sin(a),y=.7f+(i%7)*.34f;
  float pulse=std::pow(std::max(0.f,std::sin(wood.time*1.3f+i*3.2f)),10.f);glColor4f(1,.85f,.55f,pulse);
  glVertex3f(x-.10f*pulse,y,z);glVertex3f(x+.10f*pulse,y,z);glVertex3f(x,y-.14f*pulse,z);glVertex3f(x,y+.14f*pulse,z);
 }glEnd();
 glDepthMask(GL_TRUE);glDisable(GL_BLEND);forestShader.begin(wood.day?1.f:0.f);glEnable(GL_LIGHTING);
}
