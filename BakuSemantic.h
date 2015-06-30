#ifndef BAKUSEMANTIC_H
#define BAKUSEMANTIC_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "jdm.h"


using namespace std;

/**
 * Structure représentant une relation sémantique du réseau jeuxDeMots
 * composé d'un id (int) et d'un nom (string).
 */
struct RelSem{
    int id;
    float weight;
    string name;
};

class BakuSemantic
{
    public:
        const string base_file = "./ressources/base.txt";
        const string stat_file = "./ressources/motStat.txt";

        BakuSemantic();
        virtual ~BakuSemantic();

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
         *  vérifie qu'il existe des relations associés à un terme
         */
        bool isRelExist(string terme);

        /**
         * Permet d'associer une relation sur un terme en fournissant son id et son nom
         */
        bool addRel(string terme, int id, string relname);

        /**
         * Permet d'associer une relation sur un terme en fournissant son nom
         */
        bool addRel(string terme, string relname);

        /**
         * Permet d'associer une relation sur un terme
         */
        bool addRel(string terme, RelSem rel);

        /**
         *  Permet de récupérer l'ID jdm d'une relation
         */
        int getrelIdWithName(string relname);

        /**
         *  Permet d'ajouter les termes obtenues de manières statistique à la base de connaissance
         */
        void addStatRels();

    protected:
    private:
        map<string, vector<RelSem>> base;
        int Split(vector<string>& vecteur, string chaine, char separateur);
        //int getrelIdWithName(string relname);
};


void bakuSemanticTest();

#endif // BAKUSEMANTIC_H
