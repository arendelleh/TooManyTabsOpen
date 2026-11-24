#pragma once
const char* characterName(int n){return n==0?"Petal Fairy":n==1?"Woodland Elf":"Moon Mage";}
const char* seasonName(){return wood.season==0?"Summer":wood.season==1?"Autumn":"Snowfall";}
void drawAlternateKeeper(){
 glPushMatrix();glTranslatef(wood.x,.15f,wood.z);glRotatef(wood.heading*360/TAU,0,1,0);
 bool mage=wood.character==2;Color outfit=mage?Color{.42f,.34f,.66f}:Color{.29f,.52f,.35f};
 cone(0,.08f,0,mage?.39f:.27f,mage?.9f:.65f,outfit);
 ball(0,.80f,0,.23f,.3f,.18f,outfit);ball(0,1.2f,0,.25f,.27f,.23f,{.95f,.76f,.60f});
 for(float side:{-1.f,1.f}){ball(side*.28f,.76f,0,.08f,.25f,.08f,outfit);ball(side*.11f,1.25f,.21f,.025f,.035f,.015f,{.13f,.11f,.17f});
  if(!mage)ball(side*.27f,1.23f,0,.14f,.065f,.06f,{.95f,.76f,.60f});
 }
 if(mage){cone(0,1.40f,0,.32f,.60f,outfit);ring3(0,1.43f,0,.34f,{.78f,.70f,.91f});branchBetween({.38f,.05f,0},{.38f,1.55f,0},.025f,{.56f,.38f,.29f});ball(.38f,1.6f,0,.10f,.13f,.10f,{.72f,.80f,1},wood.day?0:.6f);}
 else{cone(0,1.40f,0,.27f,.38f,{.35f,.57f,.35f});for(float side:{-1.f,1.f})ball(side*.13f,.13f,.08f,.10f,.13f,.18f,{.42f,.28f,.18f});}
 glPopMatrix();
}
void fairy(){if(wood.character==0)drawPetalFairy();else drawAlternateKeeper();}
void wardrobePanel(){
 Color card{.10f,.12f,.19f},gold{1,.82f,.52f};rounded(778,85,305,485,card);
 uiText(800,117,"YOUR STORYBOOK",gold,18);
 uiText(800,147,"Choose a season",ink,13);
 for(int n=0;n<3;++n)uiActionButton(795+n*92,160,86,n==0?"Summer":n==1?"Autumn":"Snow",wood.season==n);
 uiText(800,229,"Choose your keeper",ink,13);
 for(int n=0;n<3;++n)uiActionButton(800,250+n*58,260,characterName(n),wood.character==n);
 wrapText(800,454,"Different appearances, the same abilities. Your progress stays when you change a character or season.",36,muted);
 uiActionButton(800,520,260,"Close storybook [C]");
}
bool wardrobeClick(int x,int y){
 if(!wood.wardrobe)return false;
 for(int n=0;n<3;++n){
  if(hit(x,y,795+n*92,160,86,33)){wood.season=n;feedback=string(seasonName())+" has arrived in the forest.";return true;}
  if(hit(x,y,800,250+n*58,260,33)){wood.character=n;feedback=string("You are now the ")+characterName(n)+".";return true;}
 }
 if(hit(x,y,800,520,260,33)){wood.wardrobe=false;return true;}
 return hit(x,y,778,85,305,485);
}
