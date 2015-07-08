#include <iostream>
#include <map>
#include <curl/curl.h>
#include <string>
#include "outilNet.h"
#include "outilParsage.h"
#include "jdm.h"
#include "json.h"
#include "BakuSemantic.h"
#include "wikipedia.h"
#include "wikidata.h"
#include "bakuSemanticLearn.h"
#include "FileMining.h"


using namespace std;


bool bakoutrie(string *m1, string *m2){
    if(*m1==*m2){
        return false;
    }
    if(*m2=="BakouErreur"){
        return false;
    }
    int i =0;
    string temp;
    if(lireMot(&i, m2,"Catégorie:") ){
        recopieFin(&i, m2, &temp);
        *m2=temp;
        return true;
    }
    i=0;
    if(lireMot(&i, m2, &temp, "(") ){
        *m2=temp;
        return true;
    }
    i =0;
    temp.clear();
    if(lireMot(&i, m2, &temp, ",") ){
        *m2=temp;
        return true;
    }
    i =0;
    temp.clear();
    if(lireMot(&i, m2, &temp, "/") ){
        *m2=temp;
        return true;
    }
    i=0;
    temp.clear();
    if(lireMot(&i, m2, &temp, "\\") ){
        *m2=temp;
        return true;
    }
    return true;
}



void bakoucontribue(string fichier){
    map<string, string> relJDM=relationJDM();
    map<string, string>::iterator itertrace;
    for(itertrace=relJDM.begin(); itertrace!=relJDM.end(); itertrace++){
        cout<<itertrace->first<<" - \""<<itertrace->second<<"\""<<endl;
    }
    //pause("b");
    map<string, map<string, vector<string> > > relations;
    ifstream ifs(fichier);
    string ligne;
    int i;
    string source;
    string rel;
    string cible;
    CURL *curl = curl_easy_init();
    while(getline(ifs, ligne)){
        source.clear();
        rel.clear();
        cible.clear();
        i=0;
        //if(!lireMot(&ligne, "JDM")){
            lireMot(&i, &ligne, &source, " -- " );
            lireMot(&i, &ligne, &rel, " --> " );
            lireMot(&i, &ligne, &cible, " | " );
            relations[source][rel].push_back(cible);
        //}
    }
    map<string, map<string, vector<string> > >::iterator it;
    map<string, vector<string> > ::iterator it2;
    int taille;//Taille URL avant les ajouts.
    for(it = relations.begin(); it != relations.end(); it++  ){
        cout<<it->first<<endl;
        string urldelete="http://www.jeuxdemots.org/intern_interpretor.php?s=deleteall&p=Bakounine&t=";
        urldelete+=curl_easy_escape(curl, it->first.c_str(), it->first.size());
        ouvrirPageForce(urldelete);
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++){
            cout<<"  "<<it2->first<<endl;
            string urlContrib = "http://www.jeuxdemots.org/intern_interpretor.php?s=makecontrib&p=Bakounine&t=";
            urlContrib += curl_easy_escape(curl, it->first.c_str(), it->first.size());
            urlContrib += "&r=";
            cout<<"relation : \""<<it2->first<<"\" : \""<<relJDM[it2->first]<<"\""<<endl;
            //pause("r");
            urlContrib += relJDM[it2->first];
            urlContrib += "&prop=";
            taille=urlContrib.size();
            for(int i=0; i<it2->second.size(); i++){
                string mot1 = it2->first;
                string mot2 = it2->second[i];
                if(bakoutrie(&mot1 , &mot2 )){
                    cout<<"    "<<it2->second[i]<<endl;
                    urlContrib +=curl_easy_escape(curl, it2->second[i].c_str(), it2->second[i].size());
                    urlContrib +="|";
                }
            }
            if(urlContrib.size()>taille){
                urlContrib.resize(urlContrib.size()-1);
                cout<<urlContrib<<endl;
                ouvrirPageForce(urlContrib);
            }
        }
    }
}

/**
 * Prends tous les mots dans liensavisiter et récupères les informations de différentes sources d'informations
 * Afin d'effectuer un apprentissage par une méthode statistique et une méthode sémantique
 */
void bakoulearn() {
    // On charge la base de connaissances/bakuSemanticPlay();
    BakuSemantic base;
    base.getBakuSemanticBase();

    // On entraine sur le fichier liensavisiter  par le biais de plusieurs ressources
    bakoustatlearnWD();
    base.addStatRels(); // on ajoute les résultats obtenues par stats à la base de connaissances

    // On finis par l'apprentissage par propagation dans le réseau jdm
    bakoustatlearn();
    bakuSemanticLearnTest();
}

void bakouplay(){
    bakoustatplay();
    bakoustatplayWD();
    bakuSemanticPlay();
    bakuSemanticPlayWD();
}

void bakoucontribue() {
    bakoucontribue("./ressources/relationsTrouve.txt");
    bakoucontribue("./ressources/relationsTrouveWD.txt");
    bakoucontribue("./ressources/relationsTrouveSem.txt");
    bakoucontribue("./ressources/relationsTrouveSemWD.txt");
}


int main()
{
    bakoulearn();
    bakouplay();
    bakoucontribue();
}
