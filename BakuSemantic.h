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
 * Structure représentant une relation sémantique du réseau jeuxDeMots (jdm)
 * composée d'un id (int), d'un nom (string) et d'un poids (définit par la base de connaissances).
 */
struct RelSem{
    int id;
    float weight;
    string name;
};

/**
 * Classe représentant la base de connaissance sémantique de bakounine, elle permet notament de faire des accés contrôlés
 * à la structure de données (ajout, suppréssion, modification et test de présence). Cette classe doit obligatoirement
 * être initilisée par la méthodes getBakuSemanticBase. 
 */
class BakuSemantic
{
    public:
        const string base_file = "./ressources/base.txt";
        const string stat_file = "./ressources/motStat.txt";

        BakuSemantic();
        virtual ~BakuSemantic();

        /**
         * Permet de charger la base de connaissance sémantique de bakounine
         * @return une map<string, vector<RelSem>> avec un mot comme cléf et un vecteur de relations associées 
         */
        map<string, vector<RelSem> > getBakuSemanticBase();

        /**
         * Permet de mettre à jour la base de connaissance sémantique de bakounine aprés ajout d'éléments
         */
        void writeBakuSemanticBase();

        /**
         *  Permet de récupérer les relations sémantiques d'un terme
         * @param terme , le terme portant des relations
         * @return un vecteur de relations associés au termes, un vecteur vide si le terme n'est pas dans la base
         */
        vector<RelSem> getRel(string terme);

        /**
         * vérifie qu'il existe des relations associés à un terme
         * @param terme, un terme portant des relations
         * @return true si le terme est présent dans la base, false sinon
         */
        bool isRelExist(string terme);

        /**
         * Permet d'associer un terme à une relation en fournissant son id(jdm) et son nom(jdm)
         * @param terme, le terme à associer
         * @param id, l'ID de la relation (jdm)
         * @param relname, le nom de la relation (jdm)
         * @return true si la relation à été ajouté, false sinon
         */
        bool addRel(string terme, int id, string relname);

        /**
         * Permet d'associer un terme à une relation en fournissant son nom(jdm)
         * @param terme, le terme à associer
         * @param relname, le nom de la relation (jdm)
         * @return true si la relation à été ajouté, false sinon
         */
        bool addRel(string terme, string relname);

        /**
         * Permet d'associer un terme et une relation
         * @param terme, le terme à associer à la relation
         * @param rel, la relation définit dans une structure RelSem
         * @return true si la relation à été ajouté, false sinon
         */
        bool addRel(string terme, RelSem rel);

        /**
         *  Permet de récupérer l'ID jdm d'une relation
         */
        /**
         * Permet de récupérer l'ID d'une relation (jdm) en indiquant son nom (jdm)
         * @param relname, le nom de la relation (jdm)
         * @return , un entier représentant l'ID de la relation 
         */
        int getrelIdWithName(string relname);

        /**
         * Permet d'ajouter les termes obtenues de manières statistique à la base de connaissance.
         * C'est données sont présentent dans le fichier './ressources/motStats.txt' .
         */
        void addStatRels();

    protected:
    private:
        /**
         * Base de connaissance lexicale de Bakounine
         */
        map<string, vector<RelSem> > base;
        
        /**
         * Permet de découper une chaîne de caractères, selon un identificateur, et de retourner les éléments découpés
         * dans un vecteur de chaîne de caractères
         * @param vecteur, un pointeur sur un vecteur vide qui contiendras les éléments découpés 
         * @param chaine, la chaîne de caractères à découper
         * @param separateur, le séparateur utilisé pour la découpe
         * @return un entier représentant le nombre d'éléments découpés
         */
        int Split(vector<string>& vecteur, string chaine, char separateur);
};

#endif // BAKUSEMANTIC_H
