// Copyright �2018 Black Sphere Studios
// For conditions of distribution and use, see copyright notice in "bss_util.h"

#ifndef __INIPARSE_H__BSS__
#define __INIPARSE_H__BSS__

#include "defines.h"
#include <wchar.h>
#include <stdio.h>

#ifdef  __cplusplus
extern "C" {
#endif

#define MAXLINELENGTH 1024

  typedef struct {
    //const char* file;
    //const char* curloc;
    //const char* end;
    //unsigned int length;
    FILE* file;
    char* curvalue;
    char* curkey;
    char* cursection;
    char newsection; //set to 1 if a new section was found during parsing
    char buf[MAXLINELENGTH];
  } INIParser;

  typedef struct {
    const void* start;
    const void* end;
  } INICHUNK;

  extern char bssInitINI(INIParser* init, FILE* stream);
  extern char bssDestroyINI(INIParser* destroy);
  extern char bssParseLine(INIParser* parse);
  extern INICHUNK bssFindINISection(const void* data, size_t length, const char* section, size_t instance);
  extern INICHUNK bssFindINIEntry(INICHUNK section, const char* key, size_t instance);

#ifdef  __cplusplus
}
#endif

#endif
