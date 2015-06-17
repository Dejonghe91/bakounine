#include <iostream>
#include <string>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/locale.hpp>


#include "tout.h"

using namespace std;

string latin1(string UTF){
    string copieS;
    copieS+=UTF;
    char * sLatin = (char*) copieS.c_str();
    sLatin = Utf8ToLatin1String(sLatin);
    return sLatin;
}


//http://www.jeuxdemots.org/autocompletion/autocompletion.php?completionarg=proposition&proposition=saloperie
string jdmExiste(string s){ //false si on appelle la fonction la première fois, true si on l'a appelé en minuscule et qu'on la rappele en majuscule.
    cout<<" existe ?"<<s<<endl;
    string url="http://www.jeuxdemots.org/autocompletion/autocompletion.php?completionarg=proposition&proposition=";
    url+=s;
    cout<<url<<endl;
    url = transformer(&url," ","%20");
    cout<<"url : "<<url<<endl;
    //pause("url");
    string result=ouvrirPage(url);
    /*
    if(lireMot(&result,s) || lireMot(&result,latin1(s))){
        cout<<"le mot existe dans JDM!!!"<<endl;
        //pause("existe!");
        return s;
        //cout<<"le mot n'existe pas dans JDM!!!"<<endl;
    } else {
    */
        cout<<result<<endl;
        //cout<<"le mot n'existe pas dans JDM!!!"<<endl;
        int i=0;
        string candidat;
        lireMot(&i, &result, "[\"");
        lireChar(&i, &result, '"', &candidat);
        if(LevenshteinDistance(s, candidat)<4 || LevenshteinDistance(latin1(s), candidat)<4){
            return boost::locale::conv::to_utf<char>(candidat,"Latin1");;
        } else {
            return "";
        }
    //}
}



vector <string> jdmRel(string mot1, string mot2){
    //mot1.resize(mot1.size()-1);
    mot1=transformer(&mot1," ","%20");
    mot2=transformer(&mot2," ","%20");

    //http://www.jeuxdemots.org/intern_lexical_signature.php?affichrel_submit=Afficher&term_1=Pulp%20fiction&term_2=film
    vector <string> retour;
    string adresse = "http://www.jeuxdemots.org/intern_lexical_signature.php?affichrel_submit=Afficher&term_1=";
    adresse+=latin1(mot1);
    adresse+="&term_2=";
    adresse+=latin1(mot2);
    cout<<"adresse : "<<adresse<<endl;
    //pause("adresse");
    string page = ouvrirPage(adresse);
    string relTexte = decouperPage(&page,"div","relations");
    int it=0;
    string rel;
    while(lireMot(&it, &relTexte, "<br>")){

        it++;
        lireMot(&it, &relTexte, " -- "); //mot1;
        it++;
        lireMot(&it, &relTexte, "_");
        lireMot(&it, &relTexte, &rel , " ");
        retour.push_back(rel);
        cout<<rel<<endl;
        rel.clear();
        //pause("relation");
    }
    return retour;

    //string = decouperPage(&c,"div","relations",&it);



}
