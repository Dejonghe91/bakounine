#include <iostream>
#include <map>
#include <string>
#include "./outilNet.h"
#include "./outilParsage.h"
#include "./jdm.cpp"
#include <curl/curl.h>
#include "json.h"

using namespace std;

map<string,string> ouvririnfobox(string s){
    cout<<"ouvrirInfoBox"<<endl;
    map<string,string> retour;
    string c= decouperPage(&s, "div","infobox");
    if(c==""){
        c= decouperPage(&s, "table","infobox");
    }
    if(c==""){
        ofstream ofs ("temp.html");
        ofs<<s<<endl;
        //system("chromium ./temp.html");
        //pause("problème");
    }
    int it=0;
    int it2=0;
    string cleT; //avant nétoyage
    string cle;
    string valeurT;
    string valeur;
    do{
        cle="";
        valeur="";
        cleT = decouperPage(&c,"th","",&it);
        valeurT = decouperPage(&c,"td","",&it);
        it2=0;
        while(cleT[it2]=='<'){
            lireChar(&it2,&cleT,'>');
            it2++;
            cout<<"cleTemp : "<<cleT<<"/"<<it2<<endl;
        }
        lireChar(&it2,&cleT,'<',&cle);

        it2=0;
        while(valeurT[it2]=='<'){
            lireChar(&it2,&valeurT,'>');
            it2++;
            cout<<"valeurTemp : "<<valeurT<<"/"<<it2<<endl;
        }
        lireChar(&it2,&valeurT,'<',&valeur);
        retour[cle]=valeur;
    }while(cleT!="");
    map<string,string>::iterator iter;
    for(iter = retour.begin(); iter != retour.end(); iter++){
        cout <<"cle : "<< iter->first <<endl<<" | valeur :"<< iter->second << endl<<endl;
    }
    cout<<" FIN ouvrirInfoBox"<<endl;
    return retour;
}





map<string, string> bakoulearn(){
    //lire toutes les catégories possibles dans wikipedia et les comparer aux relations sémantiques dans JDM (en français d'abord)
    //1 : ouvrir 100 pages wikipedia
    ofstream ofs ("relationsMots.txt");
    map<string, string> retour;
    map<string, map<string, int> > MotRelation; // clé : un mot dans une infobox de wp : valeur, une liste de relations avec leur score.
    map<string, map<string, int> >::iterator iterMR;
    map<string, int>::iterator iter;
    map<string, string>::iterator iterTrace;
    ifstream ifs ("liensavisiter.txt");
    string ligne; //un mot du top100
    string mot; //Version JDM du mot
    string lien; //Le lien correspondant
    string page; //une page du top100
    map<string,string> infos;  // Le contenu de l'infobox d'une page...
    vector <string> relations; //Les relations de JDM pour le mot donné.
    int cpt=0;
    while(getline(ifs, ligne) && cpt<4600){
        if((int)ligne[ligne.size()-1]==13){
            ligne.resize(ligne.size()-1);
        }
        cpt++;
        cout<<endl<<"------------------"<<endl<<endl<<"ligne : "<<ligne<<endl;
        mot=ligne;
        string jdme = jdmExiste(mot);
        if(jdme==""){
            mot[0]+=32;
            jdme=jdmExiste(mot);
        }
        if(jdme!=""){  //Si le mot est présent dans JDM
            cout<<"mot existant"<<endl;
            lien = "http://fr.wikipedia.org/wiki/";
            lien +=ligne;
            transformer(&lien, ' ','_');
            page = ouvrirPage(lien);
            ofs<<"page wikipedia : "<<lien<<endl;
            infos = ouvririnfobox(page);
            for(map<string, string>::iterator i=infos.begin(); i!=infos.end(); i++){
                if(i->first.size()>2 && i->second.size()>2 && mot!=i->second){
                    ofs<<" => "<<i->first<<" - "<<i->second<<endl;
                    relations = jdmRel(mot, i->second);
                    MotRelation[i->first]["R"]++;
                    for(int j=0; j<relations.size(); j++){
                        MotRelation[i->first][relations[j]]++;//On incremente la relation  courante pour l'info courante.
                        ofs<<endl<<i->first<<"-"<<relations[j]<<" : "<<MotRelation[i->first][relations[j]]<<endl<<endl;
                    }
                }
            }
        }
    }
    int maxi;
    string maxs;
    for(iterMR=MotRelation.begin(); iterMR!=MotRelation.end(); iterMR++){
        ofs<<iterMR->first<<endl;
        maxi=0;
        maxs="";
        for(iter=iterMR->second.begin(); iter!=iterMR->second.end(); iter++){
            if(iter->first!="R" && iter->first!="wiki" && iter->first!="associated" && iter->first!="aki" && iter->second>maxi){
                maxi=iter->second;
                maxs=iter->first;
            }
            ofs<<"    "<<iter->first<<" : "<<iter->second<<endl;
        }
        if(maxi>0){
            retour[iterMR->first]=maxs;
        }
    }

    cout<<"------------Mot->relation-----------"<<endl;
    for(iterTrace=retour.begin(); iterTrace!=retour.end(); iterTrace++){
        cout<<iterTrace->first<<" : "<<iterTrace->second<<endl;
    }
    return retour;
}


map<string, string> bakoulearnWD(){ //SUR WIKIDATA
    //lire toutes les catégories possibles dans wikipedia et les comparer aux relations sémantiques dans JDM (en français d'abord)
    //1 : ouvrir 100 pages wikipedia
    ofstream ofs ("relationsMots.txt");
    map<string, string> retour;
    map<string, map<string, int> > MotRelation; // clé : un mot dans une infobox de wp : valeur, une liste de relations avec leur score.
    map<string, map<string, int> >::iterator iterMR;
    map<string, int>::iterator iter;
    map<string, string>::iterator iterTrace;
    ifstream ifs ("liensavisiter.txt");
    string ligne; //un mot du top100
    string mot; //Version JDM du mot
    string lien; //Le lien correspondant
    string page; //une page du top100
    map<string,string> infos;  // Le contenu de l'infobox d'une page...
    vector <string> relations; //Les relations de JDM pour le mot donné.
    int cpt=0;
    while(getline(ifs, ligne) && cpt<4600){
        if((int)ligne[ligne.size()-1]==13){
            ligne.resize(ligne.size()-1);
        }
        cpt++;
        cout<<endl<<"------------------"<<endl<<endl<<"ligne : "<<ligne<<endl;
        mot=ligne;
        string jdme = jdmExiste(mot);
        if(jdme==""){
            mot[0]+=32;
            jdme=jdmExiste(mot);
        }
        if(jdme!=""){  //Si le mot est présent dans JDM
            cout<<"mot existant"<<endl;
            lien = "http://fr.wikipedia.org/wiki/";
            lien +=ligne;
            transformer(&lien, ' ','_');
            page = ouvrirPage(lien);
            ofs<<"page wikipedia : "<<lien<<endl;
            infos = ouvririnfobox(page);
            for(map<string, string>::iterator i=infos.begin(); i!=infos.end(); i++){
                if(i->first.size()>2 && i->second.size()>2 && mot!=i->second){
                    ofs<<" => "<<i->first<<" - "<<i->second<<endl;
                    relations = jdmRel(mot, i->second);
                    MotRelation[i->first]["R"]++;
                    for(int j=0; j<relations.size(); j++){
                        MotRelation[i->first][relations[j]]++;//On incremente la relation  courante pour l'info courante.
                        ofs<<endl<<i->first<<"-"<<relations[j]<<" : "<<MotRelation[i->first][relations[j]]<<endl<<endl;
                    }
                }
            }
        }
    }
    int maxi;
    string maxs;
    for(iterMR=MotRelation.begin(); iterMR!=MotRelation.end(); iterMR++){
        ofs<<iterMR->first<<endl;
        maxi=0;
        maxs="";
        for(iter=iterMR->second.begin(); iter!=iterMR->second.end(); iter++){
            if(iter->first!="R" && iter->first!="wiki" && iter->first!="associated" && iter->first!="aki" && iter->second>maxi){
                maxi=iter->second;
                maxs=iter->first;
            }
            ofs<<"    "<<iter->first<<" : "<<iter->second<<endl;
        }
        if(maxi>0){
            retour[iterMR->first]=maxs;
        }
    }

    cout<<"------------Mot->relation-----------"<<endl;
    for(iterTrace=retour.begin(); iterTrace!=retour.end(); iterTrace++){
        cout<<iterTrace->first<<" : "<<iterTrace->second<<endl;
    }
    return retour;
}



//http://htmlcxx.sourceforge.net/

bool existeRel(string mot1, string mot2, string rel){
    vector <string> lRel= jdmRel(mot1, mot2);
    for(int i=0; i<lRel.size(); i++){
        if(rel==lRel[i]){
            return true;
        }
    }
    return false;
}


void bakouplay(){ //V1
    string cible;
    string source;
    ofstream ofs ("relationsTrouve.txt");
    map<string, string> relation = bakoulearn();
    cout<<"ce que l'on a appris : "<<endl;
    ifstream ifs ("liensavisiter.txt");
    string ligne; //un mot du top100
    string mot; //un mot du top100 version JDM
    string lien; //Le lien correspondant
    string page; //une page du top100
    map<string,string> infos;  // Le contenu de l'infobox d'une page...
    vector <string> relations; //Les relations de JDM pour le mot donné.
    int cpt=0;
    while(getline(ifs, ligne) && cpt<4600){
        if((int)ligne[ligne.size()-1]==13){
            ligne.resize(ligne.size()-1);
        }
        cpt++;
        cout<<endl<<"------------------"<<endl<<endl<<"ligne : "<<ligne<<endl;
        mot=ligne;
        string jdme = jdmExiste(mot);
        if(jdme==""){
            mot[0]+=32;
            jdme=jdmExiste(mot);
        }
        if(jdme!=""){  //Si le mot est présent dans JDM
            cout<<"mot existant"<<endl;
            lien = "http://fr.wikipedia.org/wiki/";
            lien +=ligne;
            transformer(&lien, ' ','_');
            page = ouvrirPage(lien);
            //cout<<"page wikipedia : "<<lien<<endl;
            infos = ouvririnfobox(page);
            for(map<string, string>::iterator i=infos.begin(); i!=infos.end(); i++){
                if(i->first.size()>2 && i->second.size()>2 && mot!=i->second  ){
                    //cout<<" => "<<i->first<<" - "<<i->second<<endl;
                    if(relation[i->first].size()>1){
                        cible = jdmExiste(i->second);
                        source =  jdmExiste(mot);
                        if(relation[i->first]=="locution"){
                            relation[i->first]="syn";
                        }
                        if(cible!=""){
                            ofs<<source<<" -- "<<relation[i->first]<<" --> "<<cible;
                            if(existeRel(mot, i->second, relation[i->first])){
                                ofs<<" | JDM"<<endl;
                            } else {
                                ofs<<" | Nouvelle relation"<<endl;
                            }
                        } else {
                            ofs<<mot<<" -- "<<relation[i->first]<<" --> "<<transformer(&(i->second), "_", " ")<<" | nouveau mot"<<endl;
                        }
                        //<<endl;
                    }
                }
            }
        }
    }
}


void bakoucontribue(){
    map<string, string> relJDM=relationJDM();
    map<string, string>::iterator itertrace;
    for(itertrace=relJDM.begin(); itertrace!=relJDM.end(); itertrace++){
        cout<<itertrace->first<<" - \""<<itertrace->second<<"\""<<endl;
    }
    //pause("b");
    map<string, map<string, vector<string> > > relations;
    ifstream ifs("relationsTrouve.txt");
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
        if(!lireMot(&ligne, "JDM")){
            lireMot(&i, &ligne, &source, " -- " );
            lireMot(&i, &ligne, &rel, " --> " );
            lireMot(&i, &ligne, &cible, " | " );
            relations[source][rel].push_back(cible);
        }
    }
    map<string, map<string, vector<string> > >::iterator it;
    map<string, vector<string> > ::iterator it2;
    for(it = relations.begin(); it != relations.end(); it++  ){
        cout<<it->first<<endl;
        string urldelete="http://www.jeuxdemots.org/intern_interpretor.php?s=deleteall&p=Bakounine&t=";
        urldelete+=it->first;

        ouvrirPageForce(urldelete);
        for(it2 = it->second.begin(); it2 != it->second.end(); it2++){
            cout<<"  "<<it2->first<<endl;
            string urlContrib = "http://www.jeuxdemots.org/intern_interpretor.php?s=makecontrib&p=Bakounine&t=";
            urlContrib += it->first;
            urlContrib += "&r=";
            cout<<"relation : \""<<it2->first<<"\" : \""<<relJDM[it2->first]<<"\""<<endl;
            //pause("r");
            urlContrib += relJDM[it2->first];
            urlContrib += "&prop=";
            for(int i=0; i<it2->second.size(); i++){
                cout<<"    "<<it2->second[i]<<endl;
                urlContrib +=it2->second[i];
                urlContrib +="|";
            }
            urlContrib.resize(urlContrib.size()-1);
            int curlit;
            urlContrib = transformer(&urlContrib, " ", "_");
            cout<<curl_easy_escape(curl, urlContrib.c_str(), urlContrib.size())<<endl;
            cout<<urlContrib<<endl;
            ouvrirPageForce(urlContrib);
        }
    }
}




void bakou(string mot){

//1 : Trouver des termes candidats.
//1.1 : Trouver tout les mots en rapport avec le mot source dans les 10 langues majeurs de wp grace aux liens hypertexte.
//1.2 :

//2 : Filtre statistique exogène

//3 :

}





int main()
{
/*	string s0 = "rosettacode";
        string s1 = "raisethysword";
	cout << "distance between " << s0 << " and " << s1 << " : "
	     << LevenshteinDistance(s0,s1) << std::endl;
    pause("levenstein");
*/
/*
    CURL *curl = curl_easy_init();
    string page= ouvrirPage("http://fr.wikipedia.org/wiki/Cin%C3%A9ma");
    vector <string> retour = trouverToutLesLiensInterne(&page);
    int it;
    for(int i=0; i<retour.size(); i++){
        cout<<curl_easy_unescape(curl, retour[i].c_str(), retour[i].size(), &it)<<endl;
    }
    pause("liens");
*/

    string page= ouvrirPageHttps("https://www.wikidata.org/w/api.php?action=wbsearchentities&search=Philosophie&language=fr&format=json");
    //string page= ouvrirPageHttps("https://www.wikidata.org/wiki/Special:EntityData/Q40116.json");

    //string page= ouvrirPage("https://www.wikidata.org/wiki/Special:EntityData/Q40116.json");
    pause("1");
    cout<<"page : "<<page<<endl;
    pause("2");
    qid(page);
    pause("id");
    test(page);
    pause("json");
    //bakoucontribue();
    //bakouplay();
    return 0;

}


