#pragma once
#include <string>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <mmsystem.h>
#endif

struct Ambience {
 bool playing=false,chosen=false,failed=false;
 void stop(){
#ifdef _WIN32
  PlaySoundW(nullptr,nullptr,0);
#endif
  playing=false;
 }
 bool start(){
#ifdef _WIN32
  // Resolve assets beside the executable, independent of the launch directory.
  wchar_t executable[32768]{};
  DWORD length=GetModuleFileNameW(nullptr,executable,32768);
  std::wstring path(executable,length);
  auto slash=path.find_last_of(L"\\/");
  if(!length||length>=32768||slash==std::wstring::npos){failed=true;return false;}
  path.resize(slash+1);path+=L"assets\\audio\\forest_ambience.wav";
  playing=PlaySoundW(path.c_str(),nullptr,SND_FILENAME|SND_ASYNC|SND_LOOP|SND_NODEFAULT)!=0;
  failed=!playing;
  return playing;
#else
  failed=true;return false;
#endif
 }
 bool toggle(){
  chosen=true;
  if(playing){stop();failed=false;return true;}
  return start();
 }
 void enterForest(){if(!chosen&&!playing)start();}
} ambience;
