#ifndef OUTILNET_H
#define OUTILNET_H


#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <math.h>

#define MODESAUVEGARDE 1
//1 pour enregistré les pages visités, 0 pour ne pas le faire

#define REPSAVE "./pages/"
//repertoire ou les pages seront sauvegardés.

using namespace std;

struct LienCompteur{
    string lien;
    int compteur;
};

extern int codeRetour;
extern string lastPageVisite;
//extern string dernierePageVisite;

int getCodeRetour();

void savePage(string nom, string contenu); //nom doit être une adresse de site, ce sera le nom du fichier.
string loadPage(string nom);

string enleverPage(string * page, string balise, string nom); //prend le nom d'une balise et enlève le contenu texte présent entre la balise d'ouverture et celle de fermeture.


bool ajouterLien(string lien); //Ajoute un  lien à la liste de liens maintenu.
void ecrireLiens(string nomFichier, int min); //Ecrit les liens de la liste dans un fichier.
void majLiens(string nomFichier, int min); //Comme ecrireLiens, mais met à jour le fichier plutôt que l'écraser
//void ecrireBonsLiens(string nomFichier); //Ecrit uniquement les liens ayant été visités plus d'un certain nombre de fois.
//vector <string> bonsLiens(); //renvoi uniquement les liens ayant été visités plus d'un certain nombre de fois.

void decoupeLien(string lien, string * domaine, string * page); //Prend un lien en entrée et change les valeurs de domaine et page.

string ouvrirPage(string addresse); //Ouvre la page présente à l'adresse addresse. /!\ NECESSITE UNE ADRESSE ABSOLU
string ouvrirPageForce(string addresse); //Ouvre la page à l'adresse sans passer par une éventuelle sauvegarde locale.
string ouvrirPage(string domaine, string page); //Ouvre la page page du domaine domaine.
string ouvrirPage(string domaine, string page,  vector <vector <string> > var ); //Ouvre la page page dans le domaine domaine avec les variables en entrée (PAS TESTE!!!)
string ouvrirPage(string domaine, string page, string param); //Ouvre la page avec param en post
string ouvrirPageHttps(string addresse);
string ouvrirPageHttpsForce(string addresse);

void ecrireDansFichier(string texte, string nomFichier); //Ecrit le texte texte dans un fichier nommé nomFichier. /!\ Ecrase le contenu du fichier si celui ci existe!
void ecrireDansFichier(string chemin, string texte, string nomFichier); //Ecrit le texte texte dans un fichier nommé nomFichier. /!\ Ecrase le contenu du fichier si celui ci existe!


string trouverLien( string * contenu, int depart, string nomDuLien ); //renvoi l'addresse du lien que pointe le mot nomDuLien
string trouverLien( string * contenu, int * it, string nomDuLien ); //renvoi l'addresse du lien que pointe le mot nomDuLien, déplace it.

void afficherMorceau(string * s, int it, int nbCharALire); //Affiche nbCharALire caractères présents en partant de it.

string decouperPageEntiere(string * page, string balise, string nom); //Récupère tout les morceaux de texte présent entre des balises du types donnée.


string decouperPage(string * page, string balise, string nom); //prend le nom d'une balise et récupère le contenu texte présent entre la balise d'ouverture et celle de fermeture.
string decouperPage(string * page, string balise, string nom, int *it); //prend le nom d'une balise et récupère le contenu texte présent entre la balise d'ouverture et celle de fermeture, mais part d'it, et le déplace.
string decouperPageInv(string * page, string balise, string nom);
//prend le nom d'une balise et renvoi le contenu de la page en enlevant le texte présent entre la balise d'ouverture et celle de fermeture.

vector <string> trouverToutLesLiens(string * page, bool interne); //Renvoi un vecteur de string contenant tout les liens présents dans le mocreau de code envoyé. /!\ Cette version ne renvoi pas les liens vers les images. Si interne vaut 1, alors la fonction ne renvoi pas les liens pointant vers d'autre domaines.
vector <string> trouverToutLesLiens(string * page); //Renvoi un vecteur de string contenant tout les liens présents dans le mocreau de code envoyé. /!\ Cette version ne renvoi pas les liens vers les images
vector <string> trouverToutLesLiensInterne(string * page); //Renvoi un vecteur de string contenant tout les liens présents dans le mocreau de code envoyé. /!\ Cette version ne renvoi pas les liens vers les images. Ne renvoi pas les liens pointant vers d'autre domaines.

string htmlToText(string * texteHTML); //Renvoi une version du texte epuré des balises html.

string sansEspace(string s); //Renvoi la chaine avec des %20 à la place des espaces

string htmlToXML(string * texte);

string ouvrirPageManuel(string domaine, string page, string cookieC, string param, string refererC);

string Url_encodeR (string url);


#endif
