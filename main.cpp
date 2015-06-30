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


using namespace std;


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


int main()
{
    BakuSemantic baseSem;
    baseSem.getBakuSemanticBase();
    baseSem.addStatRels();

    return 0;
}
