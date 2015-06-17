#ifndef OUTILPARSAGE_H
#define OUTILPARSAGE_H
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>




using namespace std;
//using namespace perso;


struct Couple
{
    string mot1;
    string mot2;
    char type;
    int nb;
};

int distanceL(const std::string source, const std::string target);


string IToSTroisChiffres(int n);
string IToS(int i);

float SToF(string s);
int SToI(string s);

bool estUnChiffre(char i);
bool estUneLettre(char i);

char autre(char couleur);

bool lireChar(string * ligne, char caractere);

//avance i un caractere apres le symbole recherché.
bool lireChar(int * i, string * ligne, char caractere);

//re'copie ligne dans f de i jusqu'au caractère c, avance i.
void lireChar(int * i, string * ligne, char c, ofstream * f);

//recopie ligne dans s de i jusqu'au caractère c, avance i.
bool lireChar(int * i, string * ligne, char c, string * s);

//Recopie la phrase dans s de i jusqu'à un des caractère de c, avance i.
char lireChar(int * i, string  *ligne, vector <char> vc, string * s);

//recopie source dans dest de i jusqu'à la fin, avance i.
void recopieFin(int * i, string * source, string * dest);

//recopie source dans dest de i jusqu'à j.
void recopieMorceau(int i, int j, string * source, string * dest);


bool lireMot(string * ligne, string motRecherche); //renvoi vrai si le mot est présent dans la ligne, faux sinon.
bool lireMot(int * i, string * ligne, string motRecherche); //parcours la ligne jusqu'au mot recherché.
bool lireMot(int * i, string * ligne, string * retour, string motRecherche); //parcours la ligne jusqu'au mot recherché. Recopie le résultat dans retour.
bool lireMot(int * i, string * ligne, string motRecherche, int depacementMax); //parcours la ligne jusqu'au mot recherché si il se trouve avant déplacement max, sinon remet i à son niveau d'entrée.

string  lireMot(int * i, string * ligne, vector <string> motRecherche); //retourne le mot trouvé.
string lireDeuxMots(int * i, string * ligne, vector <string> motRecherche, int tailleMax); //renvoi la ligne entre les deux mots crées. TailleMax indique la distance Max entre les deux mots. 0 pour infini
string lireDeuxMots(int * i, string * ligne, vector <string> motRecherche); //Renvoi une phrase contenant les deux mots, bouge i.
string lireDeuxMots( string * ligne, vector <string> motRecherche); //renvoi une ligne complète contenant les deux mots, ne découpe pas en fonction des points...
string lireMot(int * i, string * ligne, string * retour, vector <string> motRecherche); //retourne le mot trouvé. Recopie dans retour la chaine de i au mot trouvé.

string lireLigne(int * i,  string *ligne);

//cherche un charactere parmi une liste dans la ligne, avance i au caractere suivant, et renvoi le caractere trouvé.
char lireChar(int * i, string * ligne, char * caractere, int taille);
int lireNombre(int * i, string * ligne);
char nombreToLettre(int i);

void recupererTableau(string source, vector<vector<int> > * retour);

void transformer (string * s, char x, char v); //remplace tout les x par des v dans s.
string transformer (string * s, string x, string v); //remplace tout les x par des v et renvoi la chaine resultat sans modifier la chaine source.
void enleverMot(string * phrase, string aEnlever); //enleve les aEnlever de la phrase
void propre(string source, string dest, vector<string> aEnlever); //recopie le fichier source dans le fichier dest, mais avec les aEnlever en moins.
void enleverligne(string source, string dest, vector<string> aEnlever); //recopie le fichier source dans le fichier dest, mais sans les ligne ccontenant des aEnlever
void compterLigne(string source, string dest, vector<string> mot); //ecrit dans dest le nombre de ligne pour chaque mot trouvé...

bool egal(string a, string b);//Utilise egal(char a, char b) et gere les derniers caractères vides

bool egal(char a, char b);//gestion des majuscules...

void supprimeCaractere(string * s, char c); //supprime le caractère c dans s.

bool maz(int * i); //met i à 0 et renvoi true (s''utilise pour réinitialiser i dans une condition.

bool enleverMot(vector <string> * v, string mot);

string contenu(string s); //renvoi l'intregralité du contenu du fichier.

char dernierCaractere(string s);

bool memeMots(string mot1, string mot2);

string transfoPoncutation(string p);
void initPoncutation();

string ajouterEspace(string phrase);

int nombreDeMots(string s);

bool DirectoryExists( const char* pzPath );

void copierContenuFichier(string s, string * temp); //copie dans temp la totalité du contenu de ifs.

void dire(string fichier); //ecrit via cout la totalité du contenu du fichier.

bool suffixe(string mot, string fin); //renvoi vrai si fin est suffixe de mot.
bool prefixe(string mot, string debut); //renvoi vrai si début est un préfixe de mot.

int distance(const std::string source, const std::string target);

bool estUnePonctuation(char c); //retourne vrai si le charactère est un caractère de ponctuation, faux sinon.

bool is_readable( string file );

Couple ligneToCouple(string ligne);
vector <string> ligneToListe(string ligne); //Comme ligneToCouple mais pour des fichier contenant des rapports mot -> liste. le premier élement de la liste est le mot au coeur de la relation.

string retireAccent(string message); //Enleve les accents : http://www.siteduzero.com/forum-83-613710-p1-retirer-les-accents-d-un-string.html

void retireAccentFichier(string fEntree, string fSortie); //ecrit dans le fichier fSortie le contenu de fEntree minore des accents.

void copierFichier(string fichierE, string fichierS);

#endif // OUTIL_PARSAGE

