#ifndef JDM_H
#define JDM_H

#include <iostream>
#include <string>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/locale.hpp>
#include "tout.h"
#include "outilNet.h"

using namespace std;

string latin1(string UTF);

string jdmExiste(string s);

vector<string> jdmRel(string mot1, string mot2);

vector<string> getNeightboors(string mot);

map <string, string> relationJDM();

#endif
