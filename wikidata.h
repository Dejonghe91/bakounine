#ifndef WIKIDATA_H
#define WIKIDATA_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "outilNet.h"
#include "outilParsage.h"
#include "jdm.h"
#include "json.h"
#include "tout.h"
#include <curl/curl.h>

using namespace std;


void bakouplayWD();

map<string, string> bakoulearnFichier(string s);

map<string, string> bakoustatlearnWD();

#endif
