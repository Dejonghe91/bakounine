#include "./outilParsage.h"

#include "./tout.h"

#define trace if (true) ; else cout

//using namespace perso;

int SToI(string s){
    istringstream buffer(s);
    int i;
    buffer >> i;
    return i;

}


float SToF(string s){
    return atof( s.c_str() );
}

int nombreDeMots(string s){
    int i=0;
    int cpt=1;
    while(lireChar(&i, &s, ' ')){
        cpt++;
        i++;
    }
    return cpt;
}

int distanceL(const std::string source, const std::string target) {

  // Step 1

  const int n = source.length();
  const int m = target.length();
  if (n == 0) {
    return m;
  }
  if (m == 0) {
    return n;
  }

  // Good form to declare a TYPEDEF

  //typedef std::vector< std::vector<int> > Tmatrix;

  std::vector< std::vector<int> > matrix(n+1);

  // Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
  // allow for allocation on declaration of 2.nd dimension of vec of vec

  for (int i = 0; i <= n; i++) {
    matrix[i].resize(m+1);
  }

  // Step 2

  for (int i = 0; i <= n; i++) {
    matrix[i][0]=i;
  }

  for (int j = 0; j <= m; j++) {
    matrix[0][j]=j;
  }

  // Step 3

  for (int i = 1; i <= n; i++) {

    const char s_i = source[i-1];

    // Step 4

    for (int j = 1; j <= m; j++) {

      const char t_j = target[j-1];

      // Step 5

      int cost;
      if (s_i == t_j) {
        cost = 0;
      }
      else {
        cost = 1;
      }

      // Step 6

      const int above = matrix[i-1][j];
      const int left = matrix[i][j-1];
      const int diag = matrix[i-1][j-1];
      int cell = min( above + 1, min(left + 1, diag + cost));

      // Step 6A: Cover transposition, in addition to deletion,
      // insertion and substitution. This step is taken from:
      // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's
      // Enhanced Dynamic Programming ASM Algorithm"
      // (http://www.acm.org/~hlb/publications/asm/asm.html)

      if (i>2 && j>2) {
        int trans=matrix[i-2][j-2]+1;
        if (source[i-2]!=t_j) trans++;
        if (s_i!=target[j-2]) trans++;
        if (cell>trans) cell=trans;
      }

      matrix[i][j]=cell;
    }
  }

  // Step 7

  return (int)matrix[n][m];
}




string IToSTroisChiffres(int n){
    string s;
    if(n<100){
        s+='0';
    }
    if(n<10){
        s+='0';
    }
    std::ostringstream oss;
    oss<<n;
    s+=oss.str();
    return s;
}

string IToS(int i){
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

int max(int a, int b){
    if(a>b)
        return a;
    else
        return b;
}

int min(int a, int b){
    if(a<b)
        return a;
    else
        return b;
}



void transformer (string * s, char x, char v){ //remplace tout les x par des v dans s.
    for(int i =0; i<(int)s->size(); i++){
        if((*s)[i]==x){
            (*s)[i]=v;
        }
    }
}

string transformer (string * s, string x, string v){//remplace tout les x par des v dans s.
    //trace<<"transformer (string * s, string x, string v)"<<endl;
    //trace<<"entrée : "<<*s<<endl;
    int i=0;
    string retour;
    while(lireMot(&i, s, &retour, x)){
        //trace<<retour<<endl;
        retour+=v;
        //trace<<retour<<endl;
    }
    //trace<<retour<<endl;
    // pause("fin transfo!");
    return retour;
}



bool estUnChiffre(char i){
    return (i>47 && i<58);
}

bool estUneLettre(char i){
    if((i>64 && i<91) || (i>96 && i<122)){
        //trace<<"est une lettre : "<<i<<endl;
        return true;
    } else {
    /*return ((i>64 && i<91) || (i>96 && i<122));
    */
        return false;
    }
}

char autre(char couleur){
    if(couleur=='B')
        return 'W';
    else
        return 'B';
}

bool lireChar( string * ligne, char caractere){
    int i=0;
    return lireChar(&i, ligne, caractere);
}

//avance i un caractere apres le symbole recherché.
bool lireChar(int * i, string * ligne, char caractere){
    while(*i< (int)ligne->size() && (*ligne)[*i]!=caractere ){
        (*i)++;
    }
    //(*i)++;
    return (*i< (int)ligne->size());

}

//re'copie ligne dans f de i jusqu'au caractère c, avance i.
void lireChar(int * i, string * ligne, char c, ofstream * f){
    //(*i)--;
    //(*f)<<" lire char : "<<endl;
    while((*ligne)[*i]!=c){
        (*f)<<(*ligne)[*i];
        (*i)++;
    }
    //(*i)++;
}

bool estDsLaListe(char c,vector<char> vc){
    int i=0;
    while(i<(int)vc.size() && vc[i]!=c){
        i++;
    }
    return i<(int)vc.size();
}



char lireChar(int * i, string  *ligne, vector <char> vc, string * s){
    //trace<<"trace : lireChar (vc) : "<<*i<<" ( "<<(*ligne)[*i]<<" ) : "<<*ligne<<" : "<<endl;
    while((*i)<ligne->size() && !estDsLaListe((*ligne)[*i], vc)){
        *s+=(*ligne)[*i];
        (*i)++;
    }
    //trace<<"recopié : "<<*s<<endl;
    //trace<<" i : "<<*i<<endl;
    if((*i)<ligne->size() && estDsLaListe((*ligne)[*i], vc)){
        trace<< (*ligne)[*i]<<endl;
        // pause("trouve!");
        return (*ligne)[*i];
    } else {
        // pause("pas trouve!");
        return 0;
    }
}


//recopie ligne dans s de i jusqu'au caractère c, avance i.
bool lireChar(int * i, string * ligne, char c, string * s){
    trace<<"trace : lireChar : "<<*i<<" ( "<<(*ligne)[*i]<<" ) : "<<*ligne<<" : "<<c<<endl;
    while( *i<(int)ligne->size() && (*ligne)[*i]!=c){
        *s+=(*ligne)[*i];
        (*i)++;
    }
    trace<<"recopié : "<<*s<<endl;
    trace<<" i : "<<*i<<endl;
    return (*i<(int)ligne->size() && (*ligne)[*i]==c);
}

//recopie source dans dest de i jusqu'à la fin, avance i.
void recopieFin(int * i, string * source, string * dest){
    trace<<"recopieFin : ("<<*i<<" - "<<*source  <<endl;
    while(*i<(int)source->size()){
        trace<<(*source)[*i];
        *dest+=(*source)[*i];
        (*i)++;
    }
    //(*i)++;
    trace<<endl;
}

void recopieMorceau(int i, int j, string * source, string * dest){
    trace<<"recopieMorceau : ("<<i<<" à "<<j<<" : "<<*source  <<endl;
    while(i<j){
        //trace<<(*source)[*i];
        *dest+=(*source)[i];
        i++;
    }
    //(*i)++;
    trace<<endl;

}


bool lireMot( string * ligne, string motRecherche){
    int it=0;
    return lireMot(&it, ligne, motRecherche);

}
bool lireMot(int * i, string * ligne, string motRecherche){
    trace<<"lire mot : "<<motRecherche<<endl;
    //trace<<"dans ligne : "<<*ligne<<endl;
    //int ite;
    //trace<<" : "<<ite<<"("<<motRecherche.size()<<")"<<endl;
    trace<<" ligne : "<<*ligne<<endl;
    bool trouve = false;
    int it;
    while(!trouve && *i<(int)ligne->size()){
        while(!egal((*ligne)[*i],motRecherche[0]) && *i<(int)ligne->size()){
            (*i)++;
        }
        for(it =0; it<(int)motRecherche.size() && egal(motRecherche[it],(*ligne)[*i]) && *i<(int)ligne->size(); it++){
            trace<<"."<<(*ligne)[*i];
            (*i)++;
        }
        trace<<endl;
        trace<<it<<" = "<<motRecherche.size()<<" ? "<<endl;
        if(it==(int)motRecherche.size()){
            trouve= true;
        }
    }
    trace<<endl<<"trouve : "<<trouve<<endl;
    //(*i)++;
    return trouve;
}

bool lireMot(int * i, string * ligne, string * retour, string motRecherche){ //parcours la ligne jusqu'au mot recherché. Recopie le résultat dans retour.
    trace<<"lire mot : "<<motRecherche<<endl;
    trace<<" ligne : "<<*ligne<<endl;

    bool trouve = false;
    int it;
    while(!trouve && *i<(int)ligne->size()){
        while((*ligne)[*i]!=motRecherche[0] && *i<(int)ligne->size()){
            *retour+=(*ligne)[*i];
            (*i)++;
        }
        for(it =0; it<(int)motRecherche.size() && motRecherche[it]==(*ligne)[*i] && *i<(int)ligne->size(); it++){
            trace<<"l[i] : "<<(*ligne)[*i]<<endl;
            *retour+=(*ligne)[*i];
            (*i)++;
        }
        trace<<endl;
        if(it==(int)motRecherche.size()){
            trouve= true;
        }
    }
    trace<<endl<<"trouve : "<<trouve<<endl;
    if(trouve)
        retour->resize(retour->size()-motRecherche.size());
    //(*i)++;
    return trouve;
}

bool lireMot(int * i, string * ligne, string motRecherche, int deplacementMax){
    //trace<<"lire mot : "<<motRecherche<<endl;
    //trace<<" ligne : "<<*ligne<<endl;
    //A SUPPRIMER!!!!
    /*
    cout<<" -100 : ";
    for(int iter = *i-100; iter< *i; iter++){
        cout<<(*ligne)[iter];
    }
    cout<<endl;
    cout<<" A analyser : ";
    for(int iter = *i; iter< deplacementMax; iter++){
        cout<<(*ligne)[iter];
    }
    cout<<endl;
    cout<<" Fin : ";
    for(int iter = *i+deplacementMax; iter< deplacementMax+100; iter++){
        cout<<(*ligne)[iter];
    }
    cout<<endl;
    */

    int emplacementMax = min(*i+deplacementMax, ligne->size());
    bool trouve = false;
    int it;
    while(!trouve && *i<emplacementMax ){
        while((*ligne)[*i]!=motRecherche[0] && *i<emplacementMax){
            cout<<(*ligne)[*i];
            (*i)++;
        }
        cout<<endl;
        for(it =0; it<(int)motRecherche.size() && motRecherche[it]==(*ligne)[*i] && *i<emplacementMax; it++){
            cout<<(*ligne)[*i];
            (*i)++;
        }
        trace<<endl;
        if(it==(int)motRecherche.size()){
            trouve= true;
        } else {
            cout<<endl;
            cout<<"pas le bon mot..."<<endl;
        }
    }
    trace<<endl<<"trouve : "<<trouve<<endl;
    //(*i)++;
    if(trouve==false){
        (*i)-=deplacementMax;
    }
    bp();
    return trouve;
}


string lireMot(int * i, string * ligne, vector <string> motRecherche){ //retourne le mot trouvé.
    trace<<"l m : " <<(*i)<<"<"<<ligne->size()<<endl;
    trace<<*ligne<<endl;
    // afficher(motRecherche);
    for(int j=0; j<(int)motRecherche.size(); j++){
        //trace<<"lire mot : "<<motRecherche[j]<<endl;
    }
    //trace<<" ligne : "<<*ligne<<endl;
    bool trouve = false;
    vector <string> motTemp; //les mots eligibles.
    motTemp.resize(motRecherche.size());
    for(int j =0; j<(int)motRecherche.size(); j++){
        motTemp[j]=motRecherche[j];
    }
    vector <int> place;
    place.resize(motTemp.size(),0);
    //int it;
    int tailleMT;

    while(!trouve && *i<(int)ligne->size()){
        tailleMT = motTemp.size();
        trace<<*i<<" - " <<(*ligne)[*i]<<" : "<<tailleMT<<endl;
        for(int j=0; j<tailleMT; j++){
            //trace<<j<<"ieme mot scanné : "<<motTemp[j]<<"(lettre "<<place[j]<<")"<<endl;
            if(egal((*ligne)[*i],motTemp[j][place[j]]) && *i<(int)ligne->size()){
                place[j]++;
                //trace<<"j'ai "<<place[j]<<" lettres du mot "<<motTemp[j]<<endl;
                if(place[j]==(int)motTemp[j].size()){
                    trace<<"mot trouvé!"<<motTemp[j][place[j]]<<endl;
                    return (motTemp[j]);
                }
                motTemp.push_back(motTemp[j]);
                place.push_back(0);
            } else if(j<(int)motRecherche.size()){
                place[j]=0;
            } else {
                //trace<<"on ecrase : "<<motTemp[j]<<" : "<<j<<"ieme mot."<<endl;
                place.erase(place.begin()+j);
                motTemp.erase(motTemp.begin()+j);
                tailleMT--;
            }
        }
        (*i)++;
    }
    trace<<endl<<"trouve : "<<trouve<<endl;
    return "";
}


bool egal(string a, string b){//gestion des majuscules...
    /*
    cout<<"egal : "<<a<<" : "<<b<<endl;
    cout<<"taille:"<<a.size()<<" / "<< b.size()<<endl;
    cout<<"mm :"<<min(a.size(), b.size())<<" / "<< max(a.size(), b.size())<<endl;
    */
    int i;
    for( i =0; i<min(a.size(), b.size()) && egal(a[i], b[i]); i++){

    }
    if(i==min(a.size(), b.size())){
        if(min(a.size(), b.size())==max(a.size(), b.size())){
            return true;
        } else if (min(a.size(), b.size())==max(a.size(), b.size())-1 && (a.size()==max(a.size(), b.size()) && (int)a[a.size()-1]==32) || (b.size()==max(a.size(), b.size()) && (int)b[b.size()-1]==32) ){
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}


bool egal(char a, char b){//gestion des majuscules...
    if (a==b){
        return true;
    } else if(64<a && a<91 && 97<b && b<123 && a+32==b){
        return true;
    } else if(64<b && b<91 && 97<a && a<123 && b+32==a){
        return true;
    } else {
        return false;
    }
}


string lireMot(int * i, string * ligne, string * retour, vector <string> motRecherche){ //retourne le mot trouvé. Recopie dans retour la chaine de i au mot trouvé.
    trace<<"lire Mot : "<<*i<<*ligne<<endl;
    //     afficher(motRecherche);
    bool trouve = false;
    vector <string> motTemp; //les mots eligibles.
    motTemp.resize(motRecherche.size());
    for(int j =0; j<(int)motRecherche.size(); j++){
        motTemp[j]=motRecherche[j];
    }
    vector <int> place;
    place.resize(motTemp.size(),0);
    //int it;
    int tailleMT;
    while(!trouve && *i<(int)ligne->size()){
        tailleMT = motTemp.size();
        //trace<<*i<<" - " <<(*ligne)[*i]<<" : "<<tailleMT<<endl;
        *retour+=(*ligne)[*i];
        for(int j=0; j<tailleMT; j++){
            //trace<<j<<"ieme mot scanné : "<<motTemp[j]<<"(lettre "<<place[j]<<")"<<endl;
            if(egal((*ligne)[*i],motTemp[j][place[j]]) && *i<(int)ligne->size()){
                place[j]++;
                //trace<<"j'ai "<<place[j]<<" lettres du mot "<<motTemp[j]<<endl;
                if(place[j]==(int)motTemp[j].size()){
                    trace<<"mot trouvé!"<<motTemp[j][place[j]]<<endl;
                    return (motTemp[j]);
                }
                motTemp.push_back(motTemp[j]);
                place.push_back(0);
            } else if(j<(int)motRecherche.size()){
                place[j]=0;
            } else {
                //trace<<"on ecrase : "<<motTemp[j]<<" : "<<j<<"ieme mot."<<endl;
                place.erase(place.begin()+j);
                motTemp.erase(motTemp.begin()+j);
                tailleMT--;
            }
        }
        (*i)++;
    }
    trace<<endl<<"trouve : "<<trouve<<endl;
    return "";
}

bool enleverMot(vector <string> * v, string mot){
    bool trouve = false;
    for(int i=0; i<(int)v->size() && trouve==false; i++){
        if((*v)[i]==mot){
            trouve= true;
            v->erase(v->begin()+i);
        }
    }
    return trouve;
}

void enleverMot(string * phrase, string aEnlever){
    string phrase2 = transformer(phrase, aEnlever, "");
    (*phrase) = phrase2;
}

string lireDeuxMots(int * i, string * ligne, vector <string> motRecherche, int tailleMax){ //renvoi la ligne entre les deux mots crées.
    /*cout<<"lireDeuxMots : "<<* i<<" : "<<* ligne<<endl;
    for(int i=0; i<motRecherche.size(); i++){
        cout<<motRecherche[i]<<endl;
    }
    */
    cout<<"LireDeuxMots : "<<endl;
    // afficher(motRecherche);
    trace<<*ligne<<endl<<endl;

    string retour;
    //trace<<"------------AVANT Lire Mot?------------ "<<endl;
    retour = lireMot(i,ligne, motRecherche);
    //trace<<"------------APRES Lire Mot?------------ "<<endl;
    if(retour == ""){
        //cout<<"vide"<<endl;
        return "";
    } else {
        //retour = ""; // pour que le premier mot n'aparaisse pas.
        enleverMot(&motRecherche, retour);
        cout<<"premier mot trouvé, " <<retour <<", on passe à la suite!"<<endl;
        int depart = (*i);
        (*i)++;
        string temp;
        cout<<"trouver le(s) mot(s) : "<<endl;
        afficher(motRecherche);
        cout<<"dans la phrase : "<<*ligne<<" à partir de la"<<*i<<"eme lettre"<<endl;
        retour = "";
        temp = lireMot(i,ligne, &retour, motRecherche);
        // pause("l2m");
        if((*i)-depart+ (int)temp.size()<tailleMax){
        //retour+=temp;
            if(temp== ""){
                cout<<"pas de deuxieme mot trouvé :s"<<endl;
                //pause("???");
                return "";
            } else {
                cout<<" deuxieme mot trouvé : "<<temp<<endl;
                (*i) -=(temp.size()+1);
                //(*i)--;
                retour.resize(retour.size()-temp.size()); //pour que le deuxième mot n'aparaisse pas non plus.
                return retour;
            }
        } else {
            cout<<"taille depassé ( "<<(*i)-depart+ temp.size() <<" )"<<endl;
            return lireDeuxMots(i, ligne, motRecherche, tailleMax);
        }
    }
}



string lireDeuxMots(int * i, string * ligne, vector <string> motRecherche){ //renvoi une phrase complète contenant les deux mots.
    string phrase;
    int it=0;
    trace<<"lire deux mots dans une meme phrase : "<<endl;
    // afficher(motRecherche);
    vector <char> chararret;
    chararret.push_back('.');
    chararret.push_back('\n');
    //while(lireChar(i, ligne, '.',&phrase) &&  lireDeuxMots(&it, &phrase, motRecherche, 1000)==""){
    while(lireChar(i, ligne, chararret,&phrase) &&  lireDeuxMots(&it, &phrase, motRecherche, 1000)==""){
        (*i)++;
        it=0; //remetre it à 0.
        trace<<"phrase : "<<phrase<<endl;
        phrase.clear(); //A REMETRE!!!
    }
    trace<<"mot retenu : "<<endl;
    // afficher(motRecherche);
    it=0;
    if(lireDeuxMots(&it, &phrase, motRecherche, 1000)!=""){
        trace<<"phrase retenu : "<<phrase<<endl;
        return phrase;
    } else {
        return "";
    }
}

string lireDeuxMots( string * ligne, vector <string> motRecherche){ //renvoi une ligne complète contenant les deux mots, ne découpe pas en fonction des points...
    //cout<<"LIRE DEUX MOTS!!!"<<endl;
    //pause("ldm");
    string phrase;
    int it=0;
    int i;
    /*
    cout<<"lire deux mots dans une meme ligne : "<<endl;
    cout<<ligne<<endl;
    afficher(motRecherche);
    */
    vector <char> chararret;
    //chararret.push_back('.');
    chararret.push_back('\n');
    //while(lireChar(i, ligne, '.',&phrase) &&  lireDeuxMots(&it, &phrase, motRecherche, 1000)==""){
    while(lireChar(&i, ligne, chararret,&phrase) &&  lireDeuxMots(&it, &phrase, motRecherche, 2000)==""){
        i++;
        it=0; //remetre it à 0.
        //cout<<"phrase : "<<phrase<<endl;
        phrase.clear(); //A REMETRE!!!
    }
    trace<<"mot retenu : "<<endl;
    // afficher(motRecherche);
    it=0;
    string phraseCourte = lireDeuxMots(&it, &phrase, motRecherche, 500);
    if(phraseCourte!=""){
        trace<<"phrase retenu : "<<phrase<<" : "<<phraseCourte<<endl;
        return phraseCourte;
    } else {
        return "";
    }
}


bool lireDeuxMots(int * i, string * ligne, vector <string> motRecherche, int tailleMax, vector<string> retour){
    //renvoie vrai si deux mots trouvés proposés dans motRecherche sont trouvés dans ligne à une distance l'un de 'lautre inferieur à tailleMax.
    //retour récupère un tableau de 4 strings : 1 : mot 1. 2 : Mot 2. 3 : mocreau de phrase entre les mots.  4 : phrases competes contenant les mots.
    //vector<string> retour(4,"");
    motRecherche.push_back(".");
    string phrase;
    string demiPhrase;
    string mot;

    do{
        phrase="";
        mot = lireMot(i,ligne, &phrase ,motRecherche);
    } while(mot==".");
    if(mot == ""){
        return 0;
    } else {
        trace<<"premier mot trouvé, " <<mot <<", on passe à la suite!"<<endl;
        int depart = (*i);
        (*i)++;
        string temp;
        //motRecherche.pop_back();
        trace<<"liste des mots recherches : "<<endl;
        // afficher(motRecherche);
        trace<<"dans le morceau de phrase : "<<phrase<<endl;
        temp = lireMot(i,ligne, &phrase, motRecherche);
        if((*i)-depart+ (int)temp.size()<tailleMax){ //Si le deuxieme mot ets pas trop loin du premier.
            if(temp== ""){
                trace<<"pas de deuxieme mot trouvé :s"<<endl;
                return 0;
            } else if(mot== temp){ //RAJOUT RECENT!!! TEST
                /*if(*i<ligne->size()-10){
                } */
                *i=*i-mot.size()-1;
                return lireDeuxMots(i, ligne, motRecherche, tailleMax, retour);

            } else { //FIN RAJOUT RECENT!
                trace<<" deuxieme mot trouvé : "<<temp<<endl;
                retour[0]=mot;
                retour[2]=mot;
                retour[1]=temp;
                retour[2]+=demiPhrase;
                phrase+=demiPhrase;
                lireChar(i, ligne, '.', &phrase);
                retour[3]=phrase;
                (*i) -=(temp.size()+1);
                trace<<mot<<" - "<<temp<<endl;
                 pause("LireDeuxMots!!!");
                return 1;
            }
        } else {
            trace<<"taille depassé ( "<<(*i)-depart+ temp.size() <<" )"<<endl;
            return 0;
        }
    }
}



//cherche un charactere parmi une liste dans la ligne, avance i au caractere suivant, et renvoi le caractere trouvé.
char lireChar(int * i, string * ligne, char * caractere, int taille){
    bool test = 1;
    while(test){
        for(int j=0; j<taille && test; j++){
            if((*ligne)[*i]==caractere[j])
                test = 0;
        }
        (*i)++;
    }
    char retour;
    (*i)--;
    if(test)
        //retour = NULL;
        retour =0;
    else
        retour = (*ligne)[(*i)];
    //(*i)++;
    return retour;
}

int lireNombre(int * i, string * ligne){
    bool neg = false;
    while(!estUnChiffre((*ligne)[*i]) /* && !estUneLettre((*ligne)[*i]) */){
        if(!neg && (*ligne)[*i]=='-'){
            neg=true;
        } else {
            neg=false;
        }
        (*i)++;
    }
    string temp;
    if(estUnChiffre((*ligne)[*i])){
        while(estUnChiffre((*ligne)[*i])){
            temp+=(*ligne)[*i];
            (*i)++;
        }
    } else {
        temp='0';
    }
    std::istringstream iss(temp);

    int nombre;
    iss >> nombre;
    if(neg){
        nombre = 0 - nombre;
    }
    //trace<<"nombre : "<<nombre<<endl;
    return nombre;
}

float lireNombreAVirgule(int *it, string * ligne){
    int nb1; //, nb2;
    float nb2;
    float retour;
    nb1 = lireNombre(it, ligne);
    vector<char> suite;
    string rien;
    suite.push_back('.');
    suite.push_back(' ');
    suite.push_back(',');
    if(lireChar(it, ligne, suite, &rien)!=' '){
        trace<<"c'est un nombre à virgule!"<<endl;
        nb2 = lireNombre(it, ligne);
        int nbTemp = log10((float)nb2)+1;
        trace<<"nbt : "<<nbTemp<<endl;
        trace<<"... :"<<(nb2 / pow(10,nbTemp))<<endl;
        retour = (float) nb1;
        //trace<<(nb2 / pow(10, log10((float)nb2)))<<endl;
        retour += (nb2 / pow(10,nbTemp));
    } else {
        //nb2 = 0;
        retour = (float)nb1;
    }
    trace<<" v : "<<retour<<endl;
    return retour;
}

char nombreToLettre(int i){
    if(i>=0 && i<=26){
        return (char)(i+97);
    } else {
        return ' ';
    }
}

void supprimeCaractere(string * s, char c){ //supprime le caractère c dans s.
    for(int i=0; i<(int)s->size(); i++){
        if((*s)[i]==c){
            s->erase(s->begin()+ i);
        }
    }
}


bool maz(int * i){
    *i=0;
    return true;
}

string contenu(string s){
    string retour;
    string temp;
    ifstream stream (s.c_str());
    while(getline(stream, temp)){
        retour+=temp;
        retour+='\n';
    }
    return retour;
}

char dernierCaractere(string s){
    return s[s.size()-1];
}



bool memeMots(string mot1, string mot2){
    //trace<<"'"<<mot1<<"', '"<<mot2<<"'"<<endl;
    int it=0;
    int it2 = 0;
    string m11;
    string m12;
    string m21;
    string m22;
    if(lireChar(&it, &mot1, ' ', &m11)){
        it++;
        if(lireChar(&it2, &mot2, ' ', &m21)){
            it2++;
            lireChar(&it, &mot1, '>', &m12);
            lireChar(&it2, &mot2, '>', &m22);
            //trace<<m11<<" - "<<m12<<" - "<<m21<<" - "<<m22<<endl;
            return ((memeMots(m11,m21) && memeMots(m12,m22)) || (memeMots(m12,m21) && memeMots(m11,m22)));
        } else {
            return false;
        }
    } else {
        if(lireChar(&it2, &mot2, ' ', &m21)){
            return false;
        } else {
            //trace<<distanceL(mot1, mot2)<<endl;
            return distanceL(mot1, mot2)<3;
        }
    }
}

map<string, string> ponctuation;

void initPoncutation(){
    ponctuation["("]="parentheseOuvrante";
    ponctuation[")"]="parentheseFermante";
    ponctuation[","]="virgule";
    ponctuation[";"]="pointVirgule";
    ponctuation["."]="point";
    ponctuation["-"]="tiret";
    ponctuation["/"]="slash";
    ponctuation["\\"]="antiSlash";
    ponctuation["±"] = "plusOuMoins";
}


string transfoPoncutation(string p){
    if(ponctuation[p]!=""){
        return ponctuation[p];
    } else {
        return p;
    }

}


string ajouterEspace(string phrase){
    trace<<"ajouterEspace : \""<<phrase<<"\""<<endl;
    char cTemp;
    char cTemp2;
    string retour;
    phrase+=' ';


    for(int i =0; i<(int)phrase.size()-1; i++){
        cTemp = phrase[i];
        cTemp2 = phrase[i+1];
        trace<<cTemp<<" "<<(int)cTemp<<" - "<<cTemp2<<" "<<(int)cTemp2<<endl;
        retour+=cTemp;
        if(cTemp!=' ' && cTemp2!=' '){
            if((cTemp2>90 && cTemp2<97) || cTemp2>122 || (cTemp2<64 && cTemp2>57) || cTemp2< 48){
                trace<<" - "<<endl;
                retour+=' ';
            }
            if((cTemp>90 && cTemp<97) || cTemp>122 || (cTemp<64 && cTemp>57) || cTemp< 48){
                trace<<" - "<<endl;
                retour+=' ';
            }
        }
    }
    retour+=phrase[phrase.size()-1];
    string retour2;

    //On supprime les duos d'espaces collés.
    for(int i =0; i<(int)retour.size()-1; i++){
        if(retour[i]!=' ' || retour[i+1]!=' '){
            retour2+=retour[i];
        }
    }
    if(retour2[retour2.size()-1]!=' ')
        retour2+=' ';
    return retour2;
}

void copierContenuFichier(string s, string * temp){
    string ligne;
    //system("pwd");
    trace<<s<<endl;
    ifstream ifs (s.c_str());
    while(getline(ifs, ligne)){
        trace<<"ccf : l : "<<ligne<<endl;
        (*temp) +=ligne;
        (*temp) +='\n';
    }

    // pause("Copier Contenu Fichier");
}


bool DirectoryExists( const char* pzPath ){
    if ( pzPath == NULL)
        return false;

    DIR *pDir;
    bool bExists = false;

    pDir = opendir (pzPath);

    if (pDir != NULL){
        bExists = true;
        (void) closedir (pDir);
    }

    return bExists;
}

bool prefixe(string mot, string debut){
    bool retour= true;
    for(int i=0; i<(int)debut.size() && retour; i++){
        if(mot[i]!=debut[i]){
            retour= false;
        }
    }
    return retour;
}


bool suffixe(string mot, string fin){
    bool retour= true;
    for(int i=0; i<(int)fin.size() && retour; i++){
        if(mot[i+mot.size()-fin.size()]!=fin[i]){
            retour= false;
        }
    }
    return retour;
}

bool estUnePonctuation(char c){
    if (c=='.' || c=='?' || c=='!' || c==';'){
        return true;
    } else {
        return false;
    }
}


int distance(const std::string source, const std::string target) {

  // Step 1

  const int n = source.length();
  const int m = target.length();
  if (n == 0) {
    return m;
  }
  if (m == 0) {
    return n;
  }

  // Good form to declare a TYPEDEF

  typedef std::vector< std::vector<int> > Tmatrix;

  Tmatrix matrix(n+1);

  // Size the vectors in the 2.nd dimension. Unfortunately C++ doesn't
  // allow for allocation on declaration of 2.nd dimension of vec of vec

  for (int i = 0; i <= n; i++) {
    matrix[i].resize(m+1);
  }

  // Step 2

  for (int i = 0; i <= n; i++) {
    matrix[i][0]=i;
  }

  for (int j = 0; j <= m; j++) {
    matrix[0][j]=j;
  }

  // Step 3

  for (int i = 1; i <= n; i++) {

    const char s_i = source[i-1];

    // Step 4

    for (int j = 1; j <= m; j++) {

      const char t_j = target[j-1];

      // Step 5

      int cost;
      if (s_i == t_j) {
        cost = 0;
      }
      else {
        cost = 1;
      }

      // Step 6

      const int above = matrix[i-1][j];
      const int left = matrix[i][j-1];
      const int diag = matrix[i-1][j-1];
      /*const*/ int cell = min( above + 1, min(left + 1, diag + cost));

      // Step 6A: Cover transposition, in addition to deletion,
      // insertion and substitution. This step is taken from:
      // Berghel, Hal ; Roach, David : "An Extension of Ukkonen's
      // Enhanced Dynamic Programming ASM Algorithm"
      // (http://www.acm.org/~hlb/publications/asm/asm.html)

      if (i>2 && j>2) {
        int trans=matrix[i-2][j-2]+1;
        if (source[i-2]!=t_j) trans++;
        if (s_i!=target[j-2]) trans++;
        if (cell>trans) cell=trans;
      }

      matrix[i][j]=cell;
    }
  }

  // Step 7

  return matrix[n][m];
}

void dire(string fichier){ //ecrit via cout la totalité du contenu du fichier.
    string s;
    copierContenuFichier(fichier, &s);
    cout<<s<<endl;
}

void propre(string source, string dest, vector<string> aEnlever){ //recopie le fichier source dans le fichier dest, mais avec les aEnlever en moins.
    ifstream ifs(source.c_str());
    ofstream ofs (dest.c_str());
    string ligne;
    while(getline(ifs, ligne)){
        for(int i=0; i<aEnlever.size(); i++){
            enleverMot(&ligne, aEnlever[i]);
        }
        ofs<<ligne<<endl;
    }
}

void compterLigne(string source, string dest, vector<string> mot){ //ecrit dans dest le nombre de ligne pour chaque mot trouvé...
    ifstream ifs(source.c_str());
    ofstream ofs (dest.c_str());
    string ligne;
    vector<int> compteur(mot.size(),0);
    int compteurlignes =0;
    //compteur.resize(mot.size());
    bool trouverMot;
    while(getline(ifs, ligne)){
        compteurlignes++;
        trouverMot=false;
        for(int i=0; i<mot.size(); i++){
            if(lireMot(&ligne, mot[i] )){
                compteur[i]++;
            }
        }
        //ofs<<ligne<<endl;
    }
    for(int i=0; i<mot.size(); i++){
        ofs<<mot[i]<<" : "<<compteur[i]<<endl;
    }
    ofs<<"le fichier contenait en tout "<<compteurlignes<<" lignes."<<endl;
}


void enleverligne(string source, string dest, vector<string> aEnlever){ //recopie le fichier source dans le fichier dest, mais sans les ligne ccontenant des aEnlever
    ifstream ifs(source.c_str());
    ofstream ofs (dest.c_str());
    string ligne;
    bool trouverMot;
    while(getline(ifs, ligne)){
        trouverMot=false;
        for(int i=0; i<aEnlever.size() && !trouverMot; i++){
            if(lireMot(&ligne, aEnlever[i] )){
                trouverMot=true;
            }
        }
        if(!trouverMot){
            ofs<<ligne<<endl;
        }
        //ofs<<ligne<<endl;
    }
}

string lireLigne(int * i,  string *ligne){
    string retour;
    lireChar(i, ligne, '\n', &retour);
    return retour;
}


bool is_readable( string file )
{
    ifstream fichier( file.c_str() );
    return !fichier.fail();
}


Couple ligneToCouple(string ligne){
    int i = 1;
    string mot1;
    string mot2;
    string type;
    string nb;
    lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '>', &mot1);
    i++;
    lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '>', &mot2);
    i++;
    lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '>', &type);
    i++;
    lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '>', &nb);
    i++;

    cout<<mot1<<" - "<<mot2<<endl;
    Couple retour;
    if(mot1[mot1.size()-1]==' '){
        mot1.resize(mot1.size()-1);
    }
    if(mot2[mot2.size()-1]==' '){
        mot2.resize(mot2.size()-1);
    }
    retour.mot1=mot1;
    retour.mot2=mot2;
    retour.type=type[0];
    retour.nb=atoi(nb.c_str());

    return retour;
}


vector <string> ligneToListe(string ligne){
    vector <string> retour;
    string motTemp;

    string traceI;

    int i = 1;

    string mot1;
    //string mot2;
    string type;
    string nb;
    lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '>', &mot1);
    i++;
    retour.push_back(mot1);
    lireChar(&i, &ligne, '<');
    i++;
    //Liste à récupérer!

    while(lireChar(&i, &ligne, '<', &traceI) && i++ && !lireChar(&traceI, '>') &&  lireChar(&i, &ligne, '>', &motTemp)){
        retour.push_back(motTemp);
        trace.clear();
        motTemp.clear();
        i++;
    }
    //lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '>', &type);
    i++;
    lireChar(&i, &ligne, '<');
    i++;
    lireChar(&i, &ligne, '>', &nb);
    i++;
/*    Couple retour;
    if(mot1[mot1.size()-1]==' '){
        mot1.resize(mot1.size()-1);
    }
    if(mot2[mot2.size()-1]==' '){
        mot2.resize(mot2.size()-1);
    }

    retour.mot1=mot1;
    retour.mot2=mot2;
    retour.type=type[0];
    retour.nb=atoi(nb.c_str());
*/
    return retour;
}

string retireAccent(string message)
{
    string accent("ÀÁÂÃÄÅàáâãäåÒÓÔÕÖØòóôõöøÈÉÊËèéêëÌÍÎÏìíîïÙÚÛÜùúûüÿÑñÇç");
    string sansAccent("AAAAAAaaaaaaOOOOOOooooooEEEEeeeeIIIIiiiiUUUUuuuuyNnCc");
    string messageCourt;
    int i=0,j=0,k=0,taille;

    taille=message.size();

    for (i=0;i<=taille;i++)
    {
        for(j=0;j<=104;j=j+2)
        {
            if((message[i]==accent[j])&&(message[i+1]==accent[j+1]))
            {
                message[i]=sansAccent[j/2];
                for(k=i+1;k<taille;k++)
                {
                    message[k]=message[k+1];
                }
                message=message.substr(0,taille-1);
                taille=message.size();
            }
        }
    }

    return message;
}

void retireAccentFichier(string fEntree, string fSortie){
    string c = contenu(fEntree);
    c = retireAccent(c);
    ofstream ofs(fSortie.c_str());
    ofs<<c<<endl;
}

void copierFichier(string fichierE, string fichierS){
    string s;
    copierContenuFichier(fichierE, &s);
    ofstream f(fichierS.c_str());
    f<<s<<endl;
}

