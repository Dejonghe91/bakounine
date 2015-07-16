#ifndef ALL_H
#define ALL_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "jdm.h"
#include <time.h>
#include <curl/curl.h>
#include "./outilParsage.h"

#define HOME "/home/dworkin/travail"
#define SORTIE "Sortie3"

using namespace std;

extern vector<string> termesavisiter;
extern vector<string> newtermesavisiter;

struct Relation{
    string mot1;
    string mot2;
    char r;
    int date;
};


ostream & operator<<(ostream& os, const  Relation & r1);

string majusculeW(string s);
//namespace perso{
    //void pause();
    string repertoireCourant();

    bool raz(int * it);

    bool raz(string * s);


    void pause(string message);

    void bp();

    void bpE(string erreur);

    void afficher(vector<string> s);

    void init();
    int alea(int nombre);

    void afficher(vector<Relation> vr);

    void vider(vector <int> * v);

    bool dansVecteur(string mot, vector<string> vTemp);

char * Utf8ToLatin1String (char*s);





//}

//int levenshtein_distance(const std::string &s1, const std::string &s2);

struct score{
    string mot;
    int valeur;
};


size_t LevenshteinDistance(const std::string &s1, const std::string &s2);


bool isMajuscule(string s); //renvoie vrai si la première lettre est une majuscule.

string majuscule(string s); //renvoie la string avec sa première lettre en majuscule.

string minuscule(string s); //renvoie la string avec sa première lettre en minuscule.

string invMajuscule(string s); //renvoie la string avec sa première lettre en majuscule si elle est minuscule ou en minuscule si elle est majuscule.

#endif
