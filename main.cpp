#include <iostream>
#include <map>
#include <curl/curl.h>
#include <string>
#include "tout.h"
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

const int seuilVisite = 10000;
ofstream traceFile("./traces/trace_url.txt");

bool bakoutrie(string *m1, string *m2){
    //cout<<"BAKOUTRIE : "<<*m1<<" - "<<*m2<<endl;
    if(*m2=="BakouErreur"){
        //cout<<"bakouErreur!"<<endl;
        return false;
    }
    int i =0;
    string temp;
    if(lireMot(&i, m2,"Catégorie:") ){
        //cout<<"trie : Catégorie:"<<endl;
        recopieFin(&i, m2, &temp);
        *m2=temp;
        //return true;
    }
    i=0;
    if(lireMot(&i, m2, &temp, "(") ){
        //cout<<"trie : (!"<<endl;
        *m2=temp;
        //return true;
    }

    i =0;
    temp.clear();
    if(lireMot(&i, m2, &temp, ",") ){
        *m2=temp;
        //return true;
    }
    i =0;
    temp.clear();
    if(lireMot(&i, m2, &temp, " /") ){
        *m2=temp;
        //return true;
    }
    //cout<<"M2 : \""<<*m2<<"\""<<endl;
    //cout<<"M1 : \""<<*m1<<"\""<<endl;


    i=0;
    temp.clear();
    if(lireMot(&i, m2, &temp, " \\") ){
        *m2=temp;
        return true;
    }

    if(egal(*m1,*m2)){
        //cout<<"trie : identique!"<<endl;
        return false;
    } else {
        return true;
    }
}

void bakoucontribue(string fichier){
    map<string, string> relJDM=relationJDMTrue();
    map<string, string> relJDMTemp=relationJDM();

    for(map<string, string>::iterator it= relJDMTemp.begin(); it!=relJDMTemp.end(); it++ ){
        relJDM[it->first]=it->second;
    }

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
    //CURL *curl = curl_easy_init();
    while(getline(ifs, ligne)) {
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
        urldelete+=Url_encodeR(it->first);
        ouvrirPageForce(urldelete);
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++){
            cout<<"  "<<it2->first<<endl;
            string urlContrib = "http://www.jeuxdemots.org/intern_interpretor.php?s=makecontrib&p=Bakounine&t=";
            urlContrib += Url_encodeR(majusculeW(it->first));
            urlContrib += "&r=";
            cout<<"relation : \""<<it2->first<<"\" : \""<<relJDM[it2->first]<<"\""<<endl;
            //pause("r");
            urlContrib += relJDM[it2->first];
            urlContrib += "&prop=";
            taille=urlContrib.size();
            for(int i=0; i<it2->second.size(); i++) {
                string mot1 = it->first;
                string mot2 = it2->second[i];
                if(bakoutrie(&mot1 , &mot2 )){
                    cout<<"    "<<mot2<<endl;
                    urlContrib +=Url_encodeR(majusculeW(mot2));
                    urlContrib +="|";
                }
            }
            if(urlContrib.size()>taille) {
                urlContrib.resize(urlContrib.size()-1);
                cout<<urlContrib<<endl;
                traceFile << urlContrib << endl;
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
    bakouSemLearn();
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

void loadWords(string file){
    ifstream fichier (file);
    string ligne;
    while(getline(fichier, ligne)){
        termesavisiter.push_back(ligne);
    }
}

void rechargerMot(){
    int i=0, j=0;

    //on dépile les termes en trop
    while(termesvisite.size() > seuilVisite){
        string t = termesvisite[0];
        termesvisite.erase(termesvisite.begin()+0);
        m_termesvisite.erase(t);
        cout << "nombre de termes atteins, on pop : " << t << endl;
    }

    termesavisiter.clear();
    map<string,int>::iterator it;
    for(it = m_newtermesavisiter.begin(); it != m_newtermesavisiter.end(); it++) {
        // on pop le terme le plus ancien
        if(termesvisite.size() > seuilVisite){
            string t = termesvisite[0];
            termesvisite.erase(termesvisite.begin()+0);
            m_termesvisite.erase(t);
            cout << "nombre de termes atteins, on pop : " << t << endl;
        }

        //copie colle valeur dans la m_termevisite, dans le vecteur termesvisite et dans le vecteur termesavisite
        string terme = it->first;
        termesavisiter.push_back(terme);
        m_termesvisite[terme] = 1;
        termesvisite.push_back(terme);

    }
    m_newtermesavisiter.clear();
}


int main()
{
    // 
    if(termesavisiter.empty()){
        loadWords("./ressources/liensavisiter.txt");
        termesvisite = termesavisiter;
    }

    while(!termesavisiter.empty()) {
        //bakoulearn();
        //bakouplay();
        //bakoucontribue();
        //cout << "Recharge des mots " << endl;
        //cout << "-------------------------------" << endl;
        //rechargerMot();
        
        vector<string> result = getPhrase();
        ofstream trace ("./traces/phrases_extraites.txt");
        for(int i=0; i<result.size(); i++){
            trace << result[i] << endl;
            trace << "-----------------------------------------" << endl;
            cout << result[i] << endl;
            cout << "-----------------------------------------" << endl;
        }
        trace.close();
        termesavisiter.clear();
    }
}