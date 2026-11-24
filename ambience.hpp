#pragma once
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <mmsystem.h>
#endif

struct Ambience {
 bool playing=false;
 void stop(){
#ifdef _WIN32
  PlaySoundA(nullptr,nullptr,0);
#endif
  playing=false;
 }
 bool toggle(){
  if(playing){stop();return true;}
#ifdef _WIN32
  playing=PlaySoundA("assets/audio/forest_ambience.wav",nullptr,SND_FILENAME|SND_ASYNC|SND_LOOP|SND_NODEFAULT)!=0;
  return playing;
#else
  return false;
#endif
 }
} ambience;
