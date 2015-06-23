#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string
#include <fstream>
#include "./json.h"
#include "./outilNet.h"
#include "./outilParsage.h"
#include "./tout.h"
#include <curl/curl.h>

ofstream ofs("arbreJson.txt");
ofstream ofs2("relJson.txt");

vector<string> vcourant;
string ccourant;

map<string, vector<string> > rel;


string espace(int n){
    string retour;
    for(int i=0; i<n; i++){
        retour+="    ";
    }
    return retour;
}

void afficheA(int decalage, Value::ConstMemberIterator itr, Value& sousArbre){
    ofs<<"noeud : "<<espace(decalage)<<itr->name.GetString()<<endl;
    /*for(int i=0; i<strlen(itr->name.GetString()); i++){
        cout<<i<<":"<<itr->name.GetString()[i]<<"-";
    }
    cout<<endl;
    */
    if(itr->name.GetString()[0]=='P'){
        if(vcourant.size()>0){
            rel[ccourant]=vcourant;
            vcourant.clear();
            ccourant=itr->name.GetString();
        }
        ofs2<<itr->name.GetString()<<endl;
    }
    if(itr->value.GetType()==5){
        ofs<<"string : "<<espace(decalage+1)<<sousArbre[itr->name.GetString()].GetString()<<endl;
    }

    if(itr->value.GetType()==4){
        for (SizeType i = 0; i < sousArbre[itr->name.GetString()].Size(); i++){
            //afficheA(decalage+1, sousArbre[itr->name.GetString()][i], sousArbre);
            ofs<<" t : "<<sousArbre[itr->name.GetString()][i].GetType()<<endl;
            if(sousArbre[itr->name.GetString()][i].GetType()==5){
                ofs<<"ssstring : "<<espace(decalage+1)<<sousArbre[itr->name.GetString()][i].GetString()<<endl;
            }
            if(sousArbre[itr->name.GetString()][i].GetType()==6){
                ofs<<"ssNombre : "<<espace(decalage+1)<<sousArbre[itr->name.GetString()][i].GetInt()<<endl;
            }
            if(sousArbre[itr->name.GetString()][i].GetType()==3){
                recurtest(decalage+1, sousArbre[itr->name.GetString()][i]);
            }
        }
    }
    if(itr->value.GetType()==6){
        string cmp = "numeric-id";
        string cmp2 (itr->name.GetString());
        cout<<"nombre : "<<espace(decalage+1)<<"'"<<itr->name.GetString()<<endl;
        cout<<"nombre : "<<itr->value.GetInt()<<endl;
        ofs<<"nombre : "<<espace(decalage+1)<<"'"<<itr->name.GetString()<<"' : "<<itr->value.GetInt()<<endl;
        if(cmp2==cmp){
            ofs<<"trouvé"<<endl;
            ofs2<<" - "<<sousArbre[itr->name.GetString()].GetInt()<<" : "<<getNom(sousArbre[itr->name.GetString()].GetInt())<<endl;
            vcourant.push_back(getNom(sousArbre[itr->name.GetString()].GetInt()));
        }
    }

}


void recurRel(Value& sousArbre){
    for (Value::ConstMemberIterator itr = sousArbre.MemberBegin(); itr != sousArbre.MemberEnd(); ++itr){
        //cout<<espace(decalage)<<itr->name.GetString()<<kTypeNames[itr->value.GetType()]<<endl;
        RetourneRel(itr, sousArbre);
        if(itr->value.GetType()==3){
            recurRel(sousArbre[itr->name.GetString()]);
        }
        //printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }
}

void RetourneRel(Value::ConstMemberIterator itr, Value& sousArbre){
    if(itr->name.GetString()[0]=='P'){
        if(vcourant.size()>0){
            rel[ccourant]=vcourant;
            vcourant.clear();
            ccourant=itr->name.GetString();
        }
    }
    if(itr->value.GetType()==4){
        for (SizeType i = 0; i < sousArbre[itr->name.GetString()].Size(); i++){
            if(sousArbre[itr->name.GetString()][i].GetType()==3){
                recurRel(sousArbre[itr->name.GetString()][i]);
            }
        }
    }
    if(itr->value.GetType()==6){
        string cmp = "numeric-id";
        string cmp2 (itr->name.GetString());
        if(cmp2==cmp){
            vcourant.push_back(getNom(sousArbre[itr->name.GetString()].GetInt()));
        }
    }
}

void recurtest(int decalage, Value& sousArbre){
    for (Value::ConstMemberIterator itr = sousArbre.MemberBegin(); itr != sousArbre.MemberEnd(); ++itr){
        //cout<<espace(decalage)<<itr->name.GetString()<<kTypeNames[itr->value.GetType()]<<endl;
        afficheA(decalage, itr, sousArbre);
        if(itr->value.GetType()==3){
            recurtest(decalage+1, sousArbre[itr->name.GetString()]);
        }
        //printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }
}

void fils(Value& sousArbre){
    cout<<"noeuds fils : "<<endl;
    for (Value::ConstMemberIterator itr = sousArbre.MemberBegin(); itr != sousArbre.MemberEnd(); ++itr){
        cout<<itr->name.GetString()<<kTypeNames[itr->value.GetType()]<<endl;
        //afficheA(1, itr, sousArbre);
        //printf("Type of member %s is %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
    }
    cout<<endl<<endl<<endl;
}


string getNom(int id){
    cout<<id<<endl;
    string page="https://www.wikidata.org/wiki/Special:EntityData/Q";
    page+=to_string(id);
    page+=".json";
    /*string sid="Q";
    sid+=to_string(id);*/
    string json = ouvrirPageHttps(page);
    Document document;
    document.Parse(json.c_str());
    assert(document.IsObject());
    string sid = document["entities"].MemberBegin()->name.GetString();
    if(document["entities"][sid.c_str()]["labels"].HasMember("fr")){
        return document["entities"][sid.c_str()]["labels"]["fr"]["value"].GetString();
    } else {
        return "erreur";
    }
}


void afficherel(){
    map<string, vector<string> >::iterator iterTrace;
    for(iterTrace=rel.begin(); iterTrace!=rel.end(); iterTrace++){
        cout<<iterTrace->first<<endl;
        for(int i=0; i<iterTrace->second.size(); i++){
            cout<<"  "<<iterTrace->second[i]<<endl;
        }
    }
}

map<string, vector<string> > getRelWD(string id){
    rel.clear();
    string url = "https://www.wikidata.org/wiki/Special:EntityData/";
    url+=id;
    url+=".json";
    CURL *curl = curl_easy_init();
    string page = ouvrirPageHttps(url);
    Document document;
    document.Parse(page.c_str());
    assert(document.IsObject());
    recurRel( document);
    return rel;
}


string qid(string nom){//Renvoi l'id wikidata du mot donné en entrée.
    //https://www.wikidata.org/w/api.php?action=wbsearchentities&search=Philosophie%20politique&language=fr&format=json
    string url = "https://www.wikidata.org/w/api.php?action=wbsearchentities&search=";
    CURL *curl = curl_easy_init();
    url += curl_easy_escape(curl, nom.c_str(), nom.size());
    url+="&language=fr&format=json";
    url = transformer(&url, "_", "%20");
    string page = ouvrirPageHttps(url);
    Document document;
    document.Parse(page.c_str());
    assert(document.IsObject());
    //fils(document);
    assert(document["search"].IsArray());
    //fils(document["search"][0]);
    if(document["search"].Size()>0){
        return document["search"][0]["id"].GetString();
    } else {
        cout<<"pas de page wikidata"<<endl;
        return "";
    }
}


int test(string json){
    Document document;
    document.Parse(json.c_str());
    assert(document.IsObject());
    cout<<document.IsObject()<<endl;
    cout<<":-)"<<endl;
    cout<<1<<endl;
    assert(document.IsObject());
    cout<<2<<endl;
    assert(document["search"].IsObject());
    assert(document["search"]["id"].IsObject());

    //cout<<document["entities"].IsObject()<<endl;
    //pause("nom");
    //const Value& arbre = document["entities"];
    recurtest(1, document);
    afficherel();
}

int testOld(string json){
    Document document;
    document.Parse(json.c_str());
    assert(document.IsObject());
    cout<<document["entities"].IsObject()<<endl;
    cout<<":-)"<<endl;
    //cout<<document["entities"]["Q27645"]["labels"]["fr"]["value"].GetString()<<endl;
    //pause("nom");
    //const Value& arbre = document["entities"];
    recurtest(1, document["entities"]);
    afficherel();
}
