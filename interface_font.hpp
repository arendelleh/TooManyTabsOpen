#pragma once
// Use the Windows UI font at runtime; no font file is copied or redistributed.
struct InterfaceFont {
 GLuint texture=0;
 float advances[95]{};
 bool attempted=false;
 bool prepare(){
  if(attempted)return texture!=0;
  attempted=true;
#ifdef _WIN32
  constexpr int width=1024,height=512,cell=64;
  HDC dc=CreateCompatibleDC(nullptr);
  if(!dc)return false;
  BITMAPINFO info{};info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
  info.bmiHeader.biWidth=width;info.bmiHeader.biHeight=-height;
  info.bmiHeader.biPlanes=1;info.bmiHeader.biBitCount=32;info.bmiHeader.biCompression=BI_RGB;
  void* bits=nullptr;HBITMAP bitmap=CreateDIBSection(dc,&info,DIB_RGB_COLORS,&bits,nullptr,0);
  HFONT font=CreateFontA(-42,0,0,0,FW_SEMIBOLD,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,DEFAULT_PITCH|FF_SWISS,"Segoe UI");
  if(!bitmap||!font){if(bitmap)DeleteObject(bitmap);if(font)DeleteObject(font);DeleteDC(dc);return false;}
  HGDIOBJ oldBitmap=SelectObject(dc,bitmap),oldFont=SelectObject(dc,font);
  PatBlt(dc,0,0,width,height,BLACKNESS);SetBkMode(dc,TRANSPARENT);SetTextColor(dc,RGB(255,255,255));
  TEXTMETRICA metrics{};GetTextMetricsA(dc,&metrics);
  for(int i=0;i<95;++i){char ch=char(i+32);SIZE extent{};GetTextExtentPoint32A(dc,&ch,1,&extent);advances[i]=float(extent.cx)/42.f;
   TextOutA(dc,(i%16)*cell+2,(i/16)*cell+2,&ch,1);
  }
  unsigned char* source=static_cast<unsigned char*>(bits);
  std::vector<unsigned char> alpha(width*height);
  for(int i=0;i<width*height;++i)alpha[i]=source[i*4];
  glGenTextures(1,&texture);glBindTexture(GL_TEXTURE_2D,texture);
  glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,width,height,0,GL_ALPHA,GL_UNSIGNED_BYTE,alpha.data());
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  ascent=float(metrics.tmAscent)/42.f;
  SelectObject(dc,oldFont);SelectObject(dc,oldBitmap);DeleteObject(font);DeleteObject(bitmap);DeleteDC(dc);
#endif
  return texture!=0;
 }
 float ascent=1;
 void draw(float x,float y,const string&s,Color c,float size){
  glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT|GL_TEXTURE_BIT|GL_CURRENT_BIT);
  glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);color(c);
  if(prepare()){
   glEnable(GL_TEXTURE_2D);glBindTexture(GL_TEXTURE_2D,texture);glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   const float quad=64.f/42.f*size;float top=y-ascent*size-2.f/42.f*size;
   glBegin(GL_QUADS);
   for(unsigned char ch:s){if(ch<32||ch>126)continue;int i=ch-32;
    float u=float(i%16)/16,v=float(i/16)/8,xx=x-2.f/42.f*size;
    glTexCoord2f(u,v);glVertex2f(xx,top);glTexCoord2f(u+1.f/16,v);glVertex2f(xx+quad,top);
    glTexCoord2f(u+1.f/16,v+1.f/8);glVertex2f(xx+quad,top+quad);glTexCoord2f(u,v+1.f/8);glVertex2f(xx,top+quad);
    x+=advances[i]*size;
   }glEnd();
  }else{
   // GLUT remains available on non-Windows builds without a platform font dependency.
   glDisable(GL_TEXTURE_2D);glRasterPos2f(x,y);for(unsigned char ch:s)glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,ch);
  }
  glPopAttrib();
 }
} interfaceFont;
void uiText(float x,float y,const string&s,Color c=ink,float size=14){interfaceFont.draw(x,y,s,c,size);}
