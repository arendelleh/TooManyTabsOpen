#pragma once
#include <GL/glext.h>
#include <fstream>
#include <sstream>

// Load only the shader entry points we need. Older drivers retain the fixed pipeline.
struct ForestShader {
 using CS=GLuint(APIENTRY*)(GLenum);
 using SS=void(APIENTRY*)(GLuint,GLsizei,const char*const*,const GLint*);
 using One=void(APIENTRY*)(GLuint);
 using Get=void(APIENTRY*)(GLuint,GLenum,GLint*);
 using Log=void(APIENTRY*)(GLuint,GLsizei,GLsizei*,char*);
 using CP=GLuint(APIENTRY*)();
 using Attach=void(APIENTRY*)(GLuint,GLuint);
 using Loc=GLint(APIENTRY*)(GLuint,const char*);
 using Float=void(APIENTRY*)(GLint,GLfloat);
 CS createShader=nullptr; SS source=nullptr; One compile=nullptr,link=nullptr,use=nullptr,deleteShader=nullptr;
 Get shaderStatus=nullptr,programStatus=nullptr; Log shaderLog=nullptr;
 CP createProgram=nullptr; Attach attach=nullptr; Loc location=nullptr; Float uniformFloat=nullptr;
 GLuint program=0; GLint dayLocation=-1; bool enabled=false;
 template<class T> static T load(const char* name){return reinterpret_cast<T>(glutGetProcAddress(name));}
 static string read(const char* path){std::ifstream f(path);std::ostringstream s;s<<f.rdbuf();return s.str();}
 GLuint stage(GLenum kind,const string& code){
  GLuint s=createShader(kind);const char* p=code.c_str();source(s,1,&p,nullptr);compile(s);
  GLint ok=0;shaderStatus(s,GL_COMPILE_STATUS,&ok);
  if(!ok){char log[2048]={};shaderLog(s,2047,nullptr,log);std::cerr<<"Forest shader: "<<log<<'\n';deleteShader(s);return 0;}
  return s;
 }
 void init(){
  createShader=load<CS>("glCreateShader");source=load<SS>("glShaderSource");compile=load<One>("glCompileShader");
  shaderStatus=load<Get>("glGetShaderiv");shaderLog=load<Log>("glGetShaderInfoLog");
  createProgram=load<CP>("glCreateProgram");attach=load<Attach>("glAttachShader");link=load<One>("glLinkProgram");
  programStatus=load<Get>("glGetProgramiv");use=load<One>("glUseProgram");
  deleteShader=load<One>("glDeleteShader");location=load<Loc>("glGetUniformLocation");uniformFloat=load<Float>("glUniform1f");
  if(!createShader||!source||!compile||!shaderStatus||!shaderLog||!createProgram||!attach||!link||!programStatus||!use||!deleteShader||!location||!uniformFloat)return;
  string v=read("assets/shaders/forest.vert"),f=read("assets/shaders/forest.frag");
  if(v.empty()||f.empty()){std::cerr<<"Forest shader assets missing; using standard OpenGL lighting.\n";return;}
  GLuint vs=stage(GL_VERTEX_SHADER,v),fs=stage(GL_FRAGMENT_SHADER,f);
  if(!vs||!fs){if(vs)deleteShader(vs);if(fs)deleteShader(fs);return;}
  program=createProgram();attach(program,vs);attach(program,fs);link(program);deleteShader(vs);deleteShader(fs);
  GLint ok=0;programStatus(program,GL_LINK_STATUS,&ok);if(!ok){program=0;std::cerr<<"Forest shader link failed.\n";return;}
  dayLocation=location(program,"daylight");enabled=true;std::cout<<"Forest GLSL lighting ready.\n";
 }
 void begin(float day){if(enabled){use(program);uniformFloat(dayLocation,day);}}
 void stop(){if(use)use(0);}
} forestShader;
