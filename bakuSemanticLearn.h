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

/**
 * Permet de remplir la base de connaissance sémantique de Bakounine en phase d'apprentissage (learn) et de produire 
 * des données à contribuer dans la phase de jeux (Play).
 */


/**
 * Permet d'apprendre des relations sur une liste de mots, tirées des fichiers './ressources/RelationsMots.txt' 
 * et './ressources/relationsMotsWD.txt' en utilisant le réseau lexico-sémantique JeuxDeMots (jdm)
 */
void bakouSemLearn();

/**
 * Permet d'apprendre des relations sur une liste de mots, tirées le fichier './ressources/RelationsMots.txt' 
 * en utilisant le réseau lexico-sémantique JeuxDeMots (jdm)
 */
void bakuSemanticLearn();

/**
 * Permet d'apprendre des relations sur une liste de mots, tirées le fichier './ressources/relationsMotsWD.txt' 
 * en utilisant le réseau lexico-sémantique JeuxDeMots (jdm)
 */
void bakuSemanticLearnWD();

/**
 * Permet de créer les fichiers mot1-relation-mot2 sur les données issuent de wikipédia, en utilisant l'infobox contenant
 * des clefs <-> valeurs
 */
void bakuSemanticPlay();

/**
 * Permet de créer les fichiers mot1-relation-mot2 sur les données issuent de wikidata contenant des couple cléfs <-> valeurs.
 */
void bakuSemanticPlayWD();

/**
 * Permet d'ajouter un mot à la base de connaissance 
 * @param baseSem, la base de connaissance
 * @param mot, le mot ciblé
 * @param rels, le vecteur de relations à associer au mot
 */
void addWord(BakuSemantic *baseSem, string mot, vector<RelSem> rels);

/**
 * Permet d'ajouter un mot à la base de connaissance 
 * @param baseSem, la base de connaissance
 * @param mot, le mot ciblé
 * @param rel, la relation à associer au mot
 */
void addWord(BakuSemantic *baseSem, string mot, RelSem rel);

#endif
