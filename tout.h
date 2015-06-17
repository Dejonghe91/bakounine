#ifndef ALL_H
#define ALL_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "./outilParsage.h"

#define HOME "/home/dworkin/travail"
#define SORTIE "Sortie3"

using namespace std;



struct Relation{
    string mot1;
    string mot2;
    char r;
    int date;
};

ostream & operator<<(ostream& os, const  Relation & r1);


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


#endif
