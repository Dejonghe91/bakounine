#include <iostream>
#include <map>
#include <string>
#include "outilNet.h"
#include "outilParsage.h"
#include "jdm.h"
#include <curl/curl.h>
#include "json.h"
#include "BakuSemantic.h"
#include "wikipedia.h"
#include "bakuSemanticLearn.h"

using namespace std;

/*
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
*/



/*
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
*/
/*
map <string, string> getDuo(map <string, vector <string> > liste){
    string cible;
    string source;
    map <string, string> retour;
    for(map <string, vector <string> >::iterator it=liste.begin(); it!=liste.end(); it++ ){
        for(int i=0; i<it->second.size(); i++){
            retour[it->first].push_back(it->second[i]);
        }
    }
}
*/

map<string, string> bakoulearnFichier(string s){ //SUR un fichier avec mot - r
    ifstream ifs(s);
    ofstream ofs("motR.txt");
    string ligne;
    map<string, string> retour;
    map<string, map<string, int> > MotRelation;
    string relCourante;
    string motCourant;
    map<string, map<string, int> >::iterator iterMR;
    int i = 0;
    while(getline(ifs, ligne)){
        i=0;
        relCourante.clear();
        motCourant.clear();
        lireMot(&i, &ligne, &relCourante, " - ");
        lireMot(&i, &ligne, &motCourant, " : ");
        MotRelation[relCourante][motCourant]++;
    }
    map<string, int>::iterator iter;
    map<string, string>::iterator iterTrace;
    for(iterMR=MotRelation.begin(); iterMR!=MotRelation.end(); iterMR++){
        ofs<<iterMR->first<<endl;
        int maxi=1;
        int total=1;
        string maxs="";
        for(iter=iterMR->second.begin(); iter!=iterMR->second.end(); iter++){
            if(iter->first!="R" && iter->first!="wiki" && iter->first!="associated" && iter->first!="aki" && iter->second>maxi){
                maxi=iter->second;
                maxs=iter->first;
            }
            if(iter->first=="R"){
                total = iter->second;
            }
            ofs<<"    "<<iter->first<<" : "<<iter->second<<endl;
        }
        if(maxi>1 && maxi*5>total){
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

    string id;
    map<string, vector<string> > relationsT;


    int cpt=0;
    while(getline(ifs, ligne) && cpt<1000){
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
            id = qid(mot);
            if(id!=""){
                relationsT = getRelWD(id);
                for(map<string, vector <string> >::iterator i=relationsT.begin(); i!=relationsT.end(); i++){
                    if(i->first.size()>2 ){
                        for(int i2=0; i2<i->second.size(); i2++){
                            if(i->second[i2].size()>2 && mot!=i->second[i2]){
                                relations = jdmRel(mot, i->second[i2]);
                                MotRelation[i->first]["R"]++;
                                for(int j=0; j<relations.size(); j++){
                                    MotRelation[i->first][relations[j]]++;//On incremente la relation  courante pour l'info courante.
                                    ofs<<i->first<<" - "<<relations[j]<<" : "<<MotRelation[i->first][relations[j]]<<endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    bakoulearnFichier("relationsMots.txt");
    /*
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
    pause("p");
    return retour;
    */
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



void bakouplayWD(){ //V1
    string cible;
    string source;
    ofstream ofs ("relationsTrouveWD.txt");
    map<string, string> relation = bakoulearnFichier("relationsMotsWD.txt");
    cout<<"ce que l'on a appris : "<<endl;
    ifstream ifs ("liensavisiter.txt");
    string ligne; //un mot du top100
    string mot; //un mot du top100 version JDM
    string lien; //Le lien correspondant
    string page; //une page du top100
    map<string,string> infos;  // Le contenu de l'infobox d'une page...
    vector <string> relations; //Les relations de JDM pour le mot donné.
    int cpt=0;
    while(getline(ifs, ligne) && cpt<1000){
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
/*            lien = "http://fr.wikipedia.org/wiki/";
            lien +=ligne;
            transformer(&lien, ' ','_');
            page = ouvrirPage(lien);
            //cout<<"page wikipedia : "<<lien<<endl;
            infos = ouvririnfobox(page);
            */

            string id = qid(mot);
            if(id!=""){
                map<string, vector<string> > relationsT = getRelWD(id);
                for(map<string, vector <string> >::iterator i=relationsT.begin(); i!=relationsT.end(); i++){
                    if(i->first.size()>2 ){
                        for(int i2=0; i2<i->second.size(); i2++){
                            if(i->second[i2].size()>2 && mot!=i->second[i2]){
                //for(map<string, string>::iterator i=infos.begin(); i!=infos.end(); i++){
                    //if(i->first.size()>2 && i->second.size()>2 && mot!=i->second  ){
                        //cout<<" => "<<i->first<<" - "<<i->second<<endl;
                        //if(relation[i->first].size()>1){
                                cible = jdmExiste(i->second[i2]);
                                source =  jdmExiste(mot);
                                if(relation[i->first]=="locution"){
                                    relation[i->first]="syn";
                                }
                                if(cible!=""){
                                    ofs<<source<<" -- "<<relation[i->first]<<" --> "<<cible;
                                    if(existeRel(mot, i->second[i2], relation[i->first])){
                                        ofs<<" | JDM"<<endl;
                                    } else {
                                        ofs<<" | Nouvelle relation"<<endl;
                                    }
                                } else {
                                    ofs<<mot<<" -- "<<relation[i->first]<<" --> "<<transformer(&(i->second[i2]), "_", " ")<<" | nouveau mot"<<endl;
                                }
                            }
                            //<<endl;
                        }
                    }
                }
            }
        }
    }
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
            for(int i=0; i<it2->second.size(); i++){
                cout<<"    "<<it2->second[i]<<endl;
                urlContrib +=curl_easy_escape(curl, it2->second[i].c_str(), it2->second[i].size());
                urlContrib +="|";
            }
            urlContrib.resize(urlContrib.size()-1);
            int curlit;
            cout<<urlContrib<<endl;
            ouvrirPageForce(urlContrib);
        }
    }
}


void bakuSemanticTest(){
    BakuSemantic sem;
    sem.getBakuSemanticBase();
    vector<RelSem> rel = sem.getRel("");
    if(rel.empty())
        cout << "OK" << endl;

    rel = sem.getRel("composer");
    if(!rel.empty())
        cout << "OK" << endl;

    bool res = sem.addRel("fait partie de", "has_part");

    cout << "res "+ res << endl;

    sem.writeBakuSemanticBase();
}


void bakuSemanticLearnTest() {

    ifstream fichier("relationsMots.txt", ios::in);  // on ouvre le fichier en lecture
    vector <string> mrs;

    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        string word;
        int i=0;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            i=0;
            word = "";
            if(lireMot(&i, &ligne, "|")){
                lireMot(&i, &ligne, &word, "|");
                cout << "mot récupéré = " + word << endl;
                mrs.push_back(word);
            }
        }

        fichier.close();  // on referme le fichier
    }
    else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }

    if(!mrs.empty()){
        bakouSemanticLearn(mrs);
    }
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

/*
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
    */

    //for(int i=0; i< 10; i++)
        bakuSemanticLearnTest();

//   bakoucontribue();

    return 0;

}
