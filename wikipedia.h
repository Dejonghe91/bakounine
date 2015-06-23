#ifndef WIKIPEDIA_H
#define WIKIPEDIA_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include "outilNet.h"
#include "outilParsage.h"
#include "jdm.h"
#include <curl/curl.h>


using namespace std;


/**
 *  Ouvre une infobox d'une page wikipedia
 */
map<string,string> ouvririnfobox(string s);


/**
 * Renvoi une map string string avec comme clef : mot relation et comme valeur le nom de la relation jdm correspondante
 */
map<string,string> bakoulearn();

string majuscule(string s); //renvoie le mot avec une minuscule ou une majuscule en fonction de l'opinion de Wikipedia sur le sujet...


#endif
