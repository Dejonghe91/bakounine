#include "./tout.h"
#include "./outilParsage.h"
//#include "./param.h"
#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <string>
#include <iostream>

using namespace std;

/*
int TYPENUM = TYPENUM_H;
int MOTPRESENTS = MOTPRESENTS_H;
int NBMOTS = NBMOTS_H;
int NBMAXMOTS = NBMAXMOTS_H;
int COURANT = COURANT_H;
string TEXTEINFO;// = "générés depuis les données du .h";
//TEXTEINFO += TEXTEINFO_H;
int  VTRACE = VTRACE_H;
string NOMFICHIERWEKA = NOMFICHIERWEKA_H;
 //TESTCOURANT = TESTCOURANT_H;
int  SAVEDESCRIPT = SAVEDESCRIPT_H;
int  CASECOURANTE = CASECOURANTE_H;
float  SEUILCOURANT = SEUILCOURANT_H;
string SORTIE = SORTIE_H;
*/


//namespace perso {

vector<string> termesavisiter;
map<string,int> m_newtermesavisiter;
vector<string> termesvisite;
map<string,int> m_termesvisite;

 string boolToTex(bool b){
    if(b){
        return "vrai";
    } else {
        return "false";
    }
}

void persoPause(){
    int pause;
    cout<<"pause"<<endl;
    cin>>pause;
}

bool raz(int * it){
    *it=0;
    return true;
}

bool raz(string *s){
    s->clear();
    return true;
}

string repertoireCourant()
{
    char temp[200];
    return ( getcwd(temp, 200) ? string( temp ) : string("") );
}

int alea(int nombre){
    return rand() % nombre;
}

void pause (string message){
    cout<<"pause : "<<message<<endl;
    persoPause();

}

void bp(){
    cout<<"break!"<<endl;
}

void init(){
  srand ( time(NULL) );
  initPoncutation();
}

void bpE(string erreur){
    std::cout<<erreur<<std::endl;
    bp();

}

void afficher(vector<string> s){
    for(int i=0; i<(int)s.size(); i++){
        cout<<s[i]<<endl;
    }

}

bool dansVecteur(string mot, vector<string> vTemp){
    int i;
    for( i =0; i<vTemp.size() && vTemp[i]!=mot; i++){
    }
    return i<vTemp.size();
}

void afficher(vector<Relation> vr){
    cout<<"affichage du vecteur (taille  "<<vr.size()<<") : "<<endl;
    for(int i=0; i<(int)vr.size(); i++){
        cout<<vr[i]<<endl;
    }
}


void vider(vector <int> * v){
    for(int i=0; i<(int)v->size(); i++){
        (*v)[i]=0;
    }
}


bool isMajuscule(string s){ //renvoie vrai si la première lettre est une majuscule.
    return !((int)s[0]>96 || ((int)s[0]<0 && (int)s[1]>-99));
}

string majuscule(string s){ //renvoie la string avec sa première lettre en majuscule.
    if((int)s[0]>96){
        s[0]-=32;
    }
    if((int)s[0]<0){
        if((int)s[1]>-99){ //MAJUSCULE ACCENTUEE
            s[1]-=32;
        }
    }
    return s;
}

string invMajuscule(string s){ //renvoie la string avec sa première lettre en majuscule si elle est minuscule ou en minuscule si elle est majuscule.
    if(isMajuscule(s)){
        return minuscule(s);
    } else {
        return majuscule(s);
    }
}


string minuscule(string s){ //renvoie la string avec sa première lettre en minuscule.

    if((int)s[0]<96 && (int)s[0]>0){
        s[0]+=32;
    }
    if((int)s[0]<0){
        if((int)s[1]<-99){ //MAJUSCULE ACCENTUEE
            s[1]+=32;
        }
    }
    return s;
}


ostream& operator<<(ostream& os, const  Relation & r1){
    os << r1.mot1 << " - " << r1.mot2 << " ( "<< r1.r<<" , "<<r1.date<<" ) "<<endl;
    return os;
}


int GetUtf8CharacterLength( unsigned char utf8Char )
{
    if ( utf8Char < 0x80 ) return 1;
    else if ( ( utf8Char & 0x20 ) == 0 ) return 2;
    else if ( ( utf8Char & 0x10 ) == 0 ) return 3;
    else if ( ( utf8Char & 0x08 ) == 0 ) return 4;
    else if ( ( utf8Char & 0x04 ) == 0 ) return 5;

    return 6;
}

char Utf8ToLatin1Character( char *s, int *readIndex )
{
    int len = GetUtf8CharacterLength( static_cast<unsigned char>( s[ *readIndex ] ) );
    if ( len == 1 )
    {
        char c = s[ *readIndex ];
        (*readIndex)++;

        return c;
    }

    unsigned int v = ( s[ *readIndex ] & ( 0xff >> ( len + 1 ) ) ) << ( ( len - 1 ) * 6 );
    (*readIndex)++;
    for ( len-- ; len > 0 ; len-- )
    {
        v |= ( static_cast<unsigned char>( s[ *readIndex ] ) - 0x80 ) << ( ( len - 1 ) * 6 );
        (*readIndex)++;
    }

    return ( v > 0xff ) ? 0 : (char)v;
}

char *Utf8ToLatin1String( char *s )
{
    for ( int readIndex = 0, writeIndex = 0 ; ; writeIndex++ )
    {
        if ( s[ readIndex ] == 0 )
        {
            s[ writeIndex ] = 0;
            break;
        }

        char c = Utf8ToLatin1Character( s, &readIndex );
        if ( c == 0 )
        {
            c = '_';
        }

        s[ writeIndex ] = c;
    }

    return s;
}


// Compute Levenshtein Distance
// Martin Ettl, 2012-10-05
size_t LevenshteinDistance(const std::string &s1, const std::string &s2)
{
  const size_t m(s1.size());
  const size_t n(s2.size());

  if( m==0 ) return n;
  if( n==0 ) return m;

  size_t *costs = new size_t[n + 1];

  for( size_t k=0; k<=n; k++ ) costs[k] = k;

  size_t i = 0;
  for ( std::string::const_iterator it1 = s1.begin(); it1 != s1.end(); ++it1, ++i )
  {
    costs[0] = i+1;
    size_t corner = i;

    size_t j = 0;
    for ( std::string::const_iterator it2 = s2.begin(); it2 != s2.end(); ++it2, ++j )
    {
      size_t upper = costs[j+1];
      if( *it1 == *it2 )
      {
		  costs[j+1] = corner;
	  }
      else
	  {
		size_t t(upper<corner?upper:corner);
        costs[j+1] = (costs[j]<t?costs[j]:t)+1;
	  }

      corner = upper;
    }
  }

  size_t result = costs[n];
  delete [] costs;

  return result;
}


string majusculeW(string s){ //renvoie le mot avec une minuscule ou une majuscule en fonction de l'opinion de Wikipedia sur le sujet...

    if(jdmEquivalent(s) and not jdmEquivalent(invMajuscule(s) ) ){
        return s;
    }
    if((!jdmEquivalent(s)) and  jdmEquivalent(invMajuscule(s) ) ){
        return invMajuscule(s);
    }
    cout<<"S1 : "<<s<<endl;
    cout<<"int : "<<(int)s[0]<<(int)s[1]<<endl;
    bool maj=0;
    if(!isMajuscule(s)){
        s=majuscule(s);
    }

    string s2= transformer(&s," ","_");
    //CURL *curl = curl_easy_init();
    //s2= curl_easy_escape(curl, s2.c_str(), s2.size());
    string adresse =  "fr.wikipedia.org/wiki/";
    adresse+=s2;
    string page = ouvrirPageHttps(adresse);

    s=minuscule(s);

    cout<<"S2 : "<<s<<endl;
    cout<<"int : "<<(int)s[0]<<(int)s[1]<<endl;
    vector <string> recherche;
    recherche.push_back(" ");
    recherche.push_back(" ");
    recherche.push_back(" ");
    recherche.push_back(">");
    recherche[0]+=s;
    recherche[1]+=s;
    recherche[2]+=s;
    recherche[3]+=s;
    recherche[0]+=" ";
    recherche[1]+=",";
    recherche[2]+="&";
    recherche[3]+="<";

    //cout<<"recherche : '"<<recherche<<"'"<<endl;
    bool trouve=false;
    for(int i=0; i<recherche.size(); i++){
        cout<<i<<" : "<<recherche[i]<<" : "<<lireMot(&page, recherche[i])<<endl;
        trouve+=lireMot(&page, recherche[i]);
    }
    if( codeRetour!=200 || !trouve){ //Si on ne le trouve pas en minscule A INSERER :
        cout<<"pas trouvé"<<endl;
        cout<<codeRetour<<endl;
        s=majuscule(s);
        //On lui rend sa majuscule
        cout<<"S3 : "<<s<<endl;
    } else {
        cout<<"trouvé"<<endl;
    }
    cout<<s<<endl;
    //pause("majuscule?");
    return s;
}

void addnewtermeavisiter(string mot){
    if(m_termesvisite[mot] == 0 && m_newtermesavisiter[mot] == 0){
        m_newtermesavisiter[mot] = 1;
    }
}
