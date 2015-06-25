#ifndef BAKUSEMANTICLEANR_H
#define BAKUSEMANTICLEARN_H

#include <string>
#include <map>
#include <fstream>

#include "BakuSemantic.h"
#include "wikipedia.h"
#include "outilNet.h"
#include "outilParsage.h"
#include "jdm.h"


using namespace  std;

void bakouSemanticLearn(vector<string> words);

void addWord(BakuSemantic *baseSem, string mot, vector<RelSem> rels);

void addWord(BakuSemantic *baseSem, string mot, RelSem rel);

#endif
