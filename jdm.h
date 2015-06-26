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

struct relfind {
    string rel;
    string cible;
    int w;
    bool sens;      // 0 pour les relation entrantes et 1 pour les relations sortantes
};

string latin1(string UTF);

string jdmExiste(string s);

vector<string> jdmRel(string mot1, string mot2);

vector<relfind> getNeightboors(string mot);

vector<relfind> getNeightboors(string mot, vector<string> relToFind);

map <string, string> relationJDM();

map <string, string> relationJDMTrue();

bool jdmEquivalent(string s); //TRUE uniquement si le premier mot de l('autocomplétion est strictement identique.


#endif
