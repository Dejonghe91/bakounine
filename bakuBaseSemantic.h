#ifndef BAKUBASESEMANTIC_H
#define BAKUBASESEMANTIC_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>


using namespace std;


/**
 * Structure représentant une relation sémantique du réseau jeuxDeMots
 * composé d'un id (int) et d'un nom (string).
 */
struct RelSem {
    int id;
    string name;
};

/**
 *  Permet de charger la base de connaissance sémantique de bakounine
 */
map<string, vector<RelSem>> getBakuSemanticBase();

/**
 *  Permet de mettre à jour la base de connaissance sémantique de bakounine
 */
void writeBakuSemanticBase();

/**
 *  Permet de récupérer les relations sémantiques d'un terme
 */
vector<RelSem> getRel(string terme);

/**
 *
 */
bool isRelExist(string terme);

/**
 *
 */
bool addRel(string terme, int id, string relname);


// tools //
int Split(vector<string>& vecteur, string chaine, char separateur);

#endif // BAKUBASESEMANTIC_H
