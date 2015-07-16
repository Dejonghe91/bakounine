#include "outilNet.h"
#include "./outilParsage.h"
#include "./tout.h"


#include <string>
#include <iostream>
#include <vector>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <curl/curl.h>

#define AUTOKILL 0

using namespace std;

int codeRetour; // defini en extern dans le .h

string cookie;
string referer;
//string dernierePageVisite="";
string lastPageVisite="";

vector <LienCompteur> lienDejaVisites;

void savePage(string nom, string * contenu){ //nom doit être une adresse de site, ce sera le nom du fichier.
    /*
    if(lireChar(&nom, '?')){
        cout<<"L'adresse contient des parametres : pas de sauvegarde!"<<endl;
    }
    */
    if(nom.size()<128){
        int it=0;
        string nom2; //nom sans http ou https
        string nomDossier="./pages/";
        lireMot(&it, &nom, "//");
        recopieFin(&it, &nom, &nom2);
        if(nom2==""){
            nom2=nom;
        }
        it=0;
        while(lireChar(&it, &nom2, '/', &nomDossier)){
            cout<<nomDossier<<endl;
            if(!DirectoryExists(nomDossier.c_str())){
                boost::filesystem::create_directory(nomDossier.c_str());
            }
            nomDossier+='/';
            it++;
        }
        //cout<<"save page : "<<nom<<endl;
        int i = system("pwd");
        //cout<<REPSAVE<<endl;
        //cout<<nom2<<endl;
        string fichier = REPSAVE;
        fichier += nom2;
        //cout<<fichier<<endl;
        if(!is_readable(fichier) ){
            //cout<<"ecriture!"<<endl;
            ofstream ofs(fichier.c_str());
            //cout<<"Open? "<<ofs.is_open()<<endl;
            ofs<<*contenu<<endl;
        } else {
            //cout<<"le fichier existe déjà"<<endl;
        }
        //pause("save page!");
    }
}

string loadPage(string nom){
    //cout<<"load page : "<<nom<<endl;

    int it=0;
    string nom2; //nom sans http ou https
    lireMot(&it, &nom, "//");
    recopieFin(&it, &nom, &nom2);
    if(nom2==""){
        nom2=nom;
    }
    string retour;
    string fichier = REPSAVE;
    fichier += nom2;
    //cout<<fichier<<endl;
    //cout<<is_readable(fichier)<<endl;
    if(is_readable(fichier) ){
        copierContenuFichier(fichier, &retour);
    } else {
        retour = "";
    }
    //cout<<"retour : "<<retour<<endl;
    //pause("load page");
    return retour;
}


bool ajouterLien(string lien) {
    int i;
    for( i=0; i<(int)lienDejaVisites.size() && lienDejaVisites[i].lien!=lien; i++) {
    }
    if(i<(int)lienDejaVisites.size()) {
        lienDejaVisites[i].compteur++;
        return true;
    } else {
        LienCompteur temp;
        temp.lien = lien;
        temp.compteur=1;
        lienDejaVisites.push_back(temp);
        return false;
    }
}

void majLiens(string nomFichier, int mini) {
    ifstream ifs (nomFichier.c_str());
    vector <LienCompteur> vs;
    string ligne;
    int it;
    string motTemp;
    string cptTemp;
    LienCompteur lcTemp;
    while(getline(ifs, ligne)){
        it=0;
        motTemp.clear();
        cptTemp.clear();
        lireMot(&it, &ligne, &motTemp , " ( ");
        lireMot(&it, &ligne, &cptTemp , " ) ");
        lcTemp.lien=motTemp;
        lcTemp.compteur = SToI(cptTemp);
        vs.push_back(lcTemp);
    }
    bool trouve=false;
    for(int j=0; j<lienDejaVisites.size(); j++){
        for(int i =0; i<vs.size() && trouve==false; i++){
            if(vs[i].lien==lienDejaVisites[j].lien){
                vs[i].compteur+=lienDejaVisites[j].compteur;
                trouve= true;
            }
        }
        if(trouve==false){
            vs.push_back(lienDejaVisites[j]);
        }
        trouve=false;
    }
    ifs.close();
    ofstream traceMots (nomFichier.c_str());
    for(int i=0; i<(int)vs.size(); i++) {
        if(vs[i].compteur>=mini) {
            traceMots<<vs[i].lien<<" ( "<<vs[i].compteur<<" ) "<<endl;
        }
    }
}


void ecrireLiens(string nomFichier, int min) {
    ofstream traceMots (nomFichier.c_str());
    for(int i=0; i<(int)lienDejaVisites.size(); i++) {
        if(lienDejaVisites[i].compteur>=min) {
            traceMots<<lienDejaVisites[i].lien<<" ( "<<lienDejaVisites[i].compteur<<" ) "<<endl;
        }
    }
}


void decoupeLien(string lien, string * domaine, string * page) {

    int it=0;
    if((lien[0]=='/' && lien[1]!='/' ) || lien[0]=='#') {
        page->clear();
        *page=lien;
        //domaine->clear();
    } else {

        //cout<<"découpage du lien : "<<lien<<endl;
        domaine->clear();
        page->clear();
        if(lien[0]=='/') {
            lien.erase(lien.begin());
            lien.erase(lien.begin());
        }
        if(!lireMot(&it, &lien, "http://")) {
            it=0;
        }
        lireChar(&it, &lien, '.', domaine);
        lireChar(&it, &lien, '/', domaine); // après un . puis un / : fin du nom de domaine.
        page->resize(lien.size()-it);
        for(int i = 0; i<(int)page->size(); i++) {
            (*page)[i]=lien[i+it];
        }
    }
}


string ouvrirPageManuel(string domaine, string page, string cookieC, string param, string refererC){
    //cout<<"OUVRIR PAGE : domaine et pas et string param et string refererC)"<<endl;
    sf::Http Http;
    Http.setHost(domaine);
    sf::Http::Request Request;
    Request.setMethod(sf::Http::Request::Post);
    Request.setUri(page);
    Request.setBody(param);
    Request.setHttpVersion(1, 0);
    //TEST!!!
    if(cookieC.size()<2){
        cookieC=cookie;
        //cout<<"cookie changé!"<<endl;
    }
    Request.setField("cookie", cookieC);
    Request.setField("referer", refererC);
    sf::Http::Response Page = Http.sendRequest(Request);

    string reponse = Page.getBody();
    if(Page.getStatus()!=200) {
        cout<<"réponse reçut après erreur : "<<reponse<<endl;
    }


    return reponse;
}

string ouvrirPage(string domaine, string page,  vector <vector <string> > var ) {
    //cout<<"OUVRIR PAGE : domaine et pas et vector param)"<<endl;
    sf::Http Http;
    page+='?';
    string parametre;
    for(int i=0; i<(int)var.size(); i++) {
        parametre+=var[i][0];
        parametre+='=';
        parametre+=var[i][1];
        if(i<(int)var.size()-1) {
            parametre+='&';
        }
    }
    Http.setHost(domaine);
    sf::Http::Request Request;
    Request.setMethod(sf::Http::Request::Post);
    Request.setUri(page);
    Request.setBody(parametre);
    //cout<<"param : "<<parametre<<endl;
    //pause("verifions tout ca...");
    Request.setHttpVersion(1, 0);
    //std::cout << "Sending a request to "<<domaine  <<"..." << std::endl;
    //std::cout << "page à ouvrir : "<<page<<endl;
    sf::Http::Response Page = Http.sendRequest(Request);
    //std::cout << "Status code (should be 200 on success) : " << Page.getStatus() << std::endl
    //          << "Response received from "<<domaine << std::endl
    //          << "HTTP version: " << Page.getMajorHttpVersion() << "." << Page.getMinorHttpVersion() << std::endl;

    cookie = Page.getField("Set-Cookie");
    referer = domaine+page;
    string reponse = Page.getBody();
    if(Page.getStatus()!=200 || Page.getStatus()!=204) {
        //cout<<"réponse reçut après erreur : "<<reponse<<endl;
    }
    if(Page.getStatus()==301) {
        cout<<"ERREUR 301 : "<<endl<<reponse<<endl;
        string nouveauLien = Page.getField("Location");
        cout<<"Location : "<<nouveauLien<<endl;
        if(nouveauLien.size()>2) {
            reponse = ouvrirPage(nouveauLien);
        }
    }
    if(Page.getStatus()==302) {
        int it=0;
        lireMot(&it, &reponse, "HREF");
        it+=2;
        string temp;
        lireChar(&it, &reponse, '"',&temp);
        reponse = ouvrirPage(temp);
    }

    return reponse;
}


string ouvrirPage(string domaine, string page, string param) {
    //cout<<"OUVRIR PAGE : domaine et pas et string param)"<<endl;
    sf::Http Http;
    Http.setHost(domaine);
//    Http.setPort(8080);

    sf::Http::Request Request;
    Request.setMethod(sf::Http::Request::Post);
    Request.setUri(page);
    Request.setBody(param);
    Request.setHttpVersion(1, 0);
    //TEST!!!
    if(cookie.size()>0){
        //cout<<"cookie envoyé : "<<cookie<<endl;
        Request.setField("cookie", cookie);
    }

    sf::Http::Response Page = Http.sendRequest(Request);
    cookie = Page.getField("Set-Cookie");
    referer = domaine+page;
    string reponse = Page.getBody();
    if(Page.getStatus()!=200 || Page.getStatus()!=204) {
        cout<<"réponse reçut après erreur : "<<reponse<<endl;
    }
    if(Page.getStatus()==301) {
        cout<<"ERREUR 301 : "<<endl<<reponse<<endl;
        string nouveauLien = Page.getField("Location");
        cout<<"Location : "<<nouveauLien<<endl;
        if(nouveauLien.size()>2) {
            reponse = ouvrirPage(nouveauLien);
        }
    }
    if(Page.getStatus()==302) {
        int it=0;
        lireMot(&it, &reponse, "HREF");
        it+=2;
        string temp;
        lireChar(&it, &reponse, '"',&temp);
        cerr<<"nouvelle addresse trouve : "<<temp<<endl;
        reponse = ouvrirPage(temp);
    }
    codeRetour =  Page.getStatus();
    return reponse;
}


string ouvrirPage(string domaine, string page) {

    sf::Http Http;
    Http.setHost(domaine);
    sf::Http::Request Request;
    Request.setMethod(sf::Http::Request::Get);
    Request.setUri(page);
    Request.setBody("");
    Request.setHttpVersion(1, 0);



    if(cookie.size()>0){
       // cout<<"cookie envoyé : "<<cookie<<endl;
        Request.setField("cookie", cookie);
    } else {
        //cout<<"pas de cookies!"<<endl;
    }

    sf::Http::Response Page = Http.sendRequest(Request);
    cookie = Page.getField("Set-Cookie");
    //std::cout << "Cookie récupéré : \""<<cookie<<"\""<<endl;
    referer = domaine+page;
    codeRetour = Page.getStatus();
    string reponse = Page.getBody();
    if(Page.getStatus()!=200 || Page.getStatus()!=204) {
        //pause("erreur suspecté");
        //cout<<"réponse reçut après erreur : "<<reponse<<endl;
        ofstream ofspage("./traces/pageerreur.html");
        ofspage<<"//"<<Page.getStatus()<<endl<<"//"<<domaine<<endl<<"//"<<page<<endl<<reponse<<endl;
        //pause("/erreur");
    }
    if(Page.getStatus()==301) {
        //cout<<"ERREUR 301 : "<<endl<<reponse<<endl;
        string nouveauLien = Page.getField("Location");
        //cout<<"Location : "<<nouveauLien<<endl;
        if(nouveauLien.size()>2) {
            reponse = ouvrirPage(nouveauLien);
        }
    }
    if(Page.getStatus()==302) {
        //cout<<"reponse : "<<reponse<<endl;
        int it=0;
        lireMot(&it, &reponse, "HREF");
        it+=2;
        string temp;
        lireChar(&it, &reponse, '"',&temp);
        cerr<<"nouvelle addresse trouve : "<<temp<<endl;
        reponse = ouvrirPage(temp);
    }

    return reponse;
}


string ouvrirPageForce(string addresse){
    string domaine;
    string page;
    decoupeLien(addresse, &domaine, &page);
    string resultat = ouvrirPage(domaine, page);
    return resultat;
}


string ouvrirPageHttps(string adresse) { //ATTENTION : NECESSITE UNE ADRESSE CONTERNANT LE DOMAINE
    string resultat = loadPage(adresse);
    if(resultat==""){
        //cout<<"pas trouvé"<<endl;
        resultat = ouvrirPageHttpsForce(adresse);
        if(MODESAUVEGARDE && codeRetour==200){
            savePage(adresse, &resultat);
            lastPageVisite=adresse;
        }
        if(codeRetour!=200){
            cout << "ERREUR !!" << endl;
            cout<<adresse<<endl;
            cout<<codeRetour<<endl;
        }
    } else  {
        //cout<<"lecture d'une page sauvegardée"<<endl;
        lastPageVisite=adresse;
        codeRetour=200;
    }
    return resultat;
}


string ouvrirPage(string addresse) { //ATTENTION : NECESSITE UNE ADRESSE CONTERNANT LE DOMAINE
    //cout<<"ouvrir page : "<<addresse<<endl;
    string domaine;
    string page;
    string resultat = loadPage(addresse);
    //cout<<"résultat : "<<endl<<resultat<<endl;
    //pause("o p 1");
    if(resultat==""){
        //pause("o p 2");
        decoupeLien(addresse, &domaine, &page);
        resultat = ouvrirPage(domaine, page);
        //cout<<"MS : "<<MODESAUVEGARDE<<" / CR : "<<codeRetour<<endl;
        if(MODESAUVEGARDE && codeRetour==200){
            //pause("o p 3");
            savePage(addresse, &resultat);
            lastPageVisite=addresse;
        }
        if(codeRetour!=200){
            //pause("o p 4");
            //cout<<addresse<<endl;
            //cout<<codeRetour<<endl;
            //pause("probleme code retour!");
        }
    } else  {
        //pause("o p 5");
        lastPageVisite=addresse;
        codeRetour=200;
    }
    return resultat;
}



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string ouvrirPageHttpsForce(string addresse) {
    CURL *curl;
    CURLcode res;
    std::string retour;
    //long cr=1;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, addresse.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &retour);
    res = curl_easy_perform(curl);
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &codeRetour);
    curl_easy_cleanup(curl);


    //std::cout <<"adresse : "<< addresse << std::endl;
    //std::cout <<"page : "<< retour << std::endl;
    ofstream ofspage("./traces/pageerreur.html");
    ofspage<<retour<<endl;
    //std::cout <<"res : "<< res << std::endl;
    //std::cout <<"codeRetour : "<< codeRetour << std::endl;



    //pause("https");
    return retour;
}

void ecrireDansFichier(string chemin, string texte, string nomFichier) {
    if(dernierCaractere(chemin)!='/') {
        chemin+='/';
    }
    chemin +=nomFichier;
    ecrireDansFichier(texte, chemin);

}


void ecrireDansFichier(string texte, string nomFichier) {
    /*
    cout<<"ecrire dans fichier : "<<nomFichier<<endl;
    system("pwd");
    */
    ofstream fichier (nomFichier.c_str());
    fichier<<texte<<endl;
    fichier.close();
}

string trouverLien( string * contenu, int depart, string nomDuLien ) { //renvoi l'addresse du lien que pointe le mot nomDuLien
    int it =depart;
    string lienTemp;
    //bool trouve= false;
    vector <string> recherche;
    recherche.push_back("</a>");
    recherche.push_back(nomDuLien);
    string renvoi = "</a>"; //prochain mot trouvé.
    while(renvoi=="</a>") {
        if(lireMot(&it, contenu, "href")) { //1 : trouver href.
            lireChar(&it,contenu, '"');
            it++;
            lienTemp.clear();
            lireChar(&it, contenu, '"', &lienTemp); // 2 : copier l'adresse.
            //cout<<"lien Temp : "<<lienTemp<<endl;
            renvoi = lireMot(&it, contenu, recherche); //3 regarder si le mot est présent avant le </a>
        } else {
            renvoi="";
        }
    }
    if(renvoi==nomDuLien) {
        return lienTemp;
    } else {
        /*
        int pause;
        cout<<"lien : "<<nomDuLien<<" inexistant dans le texte "<<*contenu<<endl;
        cin>>pause;
        */
        return ""; //lien inexistant
    }

}

string trouverLien( string * contenu, int *it, string nomDuLien ) { //renvoi l'addresse du lien que pointe le mot nomDuLien
   // cout<<"trouverLien!(string *, int *, string)"<<endl;
   // cout<<contenu->size()<<endl;
   // cout<<*it<<endl;
   // cout<<nomDuLien<<endl;
    string lienTemp;
    //bool trouve= false;
    vector <string> recherche;
    recherche.push_back("</a>");
    recherche.push_back(nomDuLien);
    string renvoi = "</a>"; //prochain mot trouvé.
    while(renvoi=="</a>") {
        if(lireMot(it, contenu, "href")) { //1 : trouver href.
            lireChar(it,contenu, '"');
            (*it)++;
            lienTemp.clear();
            //afficherMorceau(contenu, *it-20, 40);
            lireChar(it, contenu, '"', &lienTemp); // 2 : copier l'adresse.
            renvoi = lireMot(it, contenu, recherche); //3 regarder si le mot est présent avant le </a>
        } else {
            renvoi="";
        }
    }
    if(renvoi==nomDuLien) {
        return lienTemp;
    } else {
        /*
        int pause;
        cout<<"lien : "<<nomDuLien<<" inexistant dans le texte "<<*contenu<<endl;
        cin>>pause;
        */
        return ""; //lien inexistant
    }

}



void afficherMorceau(string * s, int it, int nbCharALire) {
    cout<<"aficher morceau!"<<endl;
    for(int i=it; i<nbCharALire; i++) {
        cout<<(*s)[i];
    }
    if((int)s->size()>it) {
        cout<<endl;
    }
}


string decouperPageEntiere(string * page, string balise, string nom){
    int it=0;
    string retour;
    string temp;
    do{
        temp = decouperPage(page, balise, nom, &it);
        retour+=temp;
        retour+='.';
        retour+='\n';

    }while(temp!="");
    //cout<<"fin de la page!"<<endl;
    return retour;


}

string decouperPage(string * page, string balise, string nom, int * it) {
    //prend le nom d'une balise et récupère le contenu texte présent entre la balise d'ouverture et celle de fermeture.
    if(page->size()<3) {
        cout<<"ERREUR : DECOUPAGE D'UNE PAGE VIDE !!!!"<<endl;
        // //bp();
        return "";
    }
    //cout<<"découper Page!"<<endl;
    string retour;
    vector <string> debut(2,"<"); //contient '< balise' et '<balise'.
    debut[0]+=balise;
    debut[1]+=' ';
    debut[1]+=balise;
    vector <string> suite; //contient '>' et le nom cherché.
    suite.push_back(">");
    suite.push_back(nom);
    bool trouve = false;
    while(trouve==false && *it<(page->size()-2)) {
        //cout<<"1..."<<endl;
        //cout<<"pre : "<<*it<<" / "<<page->size()<<endl;
        lireMot(it, page, debut); //On trouve la balise recherché.
        (*it)++;
        //afficherMorceau(page, *it, *it+10 );
        // pause("Morceau ^");
        //cout<<"post : "<<*it<<" / "<<page->size()<<endl;
        if((nom =="" && ((*page)[*it]=='>' || (*page)[*it]==' ')) || lireMot(it, page,suite)==nom ) {
            //cout<<"trouve!"<<endl;
            // pause("trouve :-)");
            trouve=true;
            (*it)-=2;
            lireChar(it, page, '>');
        }
    }
    //cout<<*it<<" - "<<page->size()-1<<endl;
    // pause("... pab");
    if(*it<(page->size()-1)) {
        //On est à l'endroit du texte qu'il faut commencer à aspirer.
        retour+="<";
        retour+=balise;
        balise+='>';
        int compteur=0;
        vector <string> balises(2,"<");
        balises[0]+=balise;
        balises[1]+='/';
        balises[1]+=balise;
        while(compteur>=0) {

            if(compteur>0){
                //cout<<"récupéré : "<<retour<<endl;
                 //bp();

            }
            if(*it>=(int)page->size()){
                //cout<<"morceau étrange : "<<page<<endl;
                //cout<<"récupéré : "<<retour<<endl;
                // //bp();
                compteur = -2;
            }
            if(lireMot(it, page, &retour, balises)[1]=='/') {
                //cout<<"trouve balise fermante"<<endl;
                compteur--;
            } else {
               // cout<<"trouve balise ouvrante!!"<<endl;
                 //bp();
                compteur++;
            }
            (*it)++;
        }
        //On est ressorti de la zone à récuperer.
        retour+=">";
    } else {

        retour="";
    }
    return retour;
}

string decouperPage(string * page, string balise, string nom) {
//prend le nom d'une balise et récupère le contenu texte présent entre la balise d'ouverture et celle de fermeture.
    if(page->size()<3) {
        cout<<"ERREUR : DECOUPAGE D'UNE PAGE VIDE !!!!"<<endl;
        return "";
    }
    cout<<"découper Page!"<<endl;
    string retour;
    int it =0;
    //int itMem; //mémorise une ancienne valeur de it;
    vector <string> debut(2,"<"); //contient '< balise' et '<balise'.
    debut[0]+=balise;
    debut[1]+=' ';
    debut[1]+=balise;
    vector <string> suite; //contient '>' et le nom cherché.
    suite.push_back(">");
    suite.push_back(nom);
    bool trouve= false;
    while(trouve==false && it<(page->size()-2)) {
        lireMot(&it, page, debut); //On trouve la balise recherché.
        //afficherMorceau(page, it-2, it+40 );
        if(lireMot(&it,page, suite)==nom) {
            trouve=true;
            lireChar(&it, page, '>');
        }
    }
    if(it<(page->size()-1)) {
        //On est à l'endroit du texte qu'il faut commencer à aspirer.
        retour+="<";
        retour+=balise;
        int compteur=0;
        vector <string> balises(2,"<");
        balises[0]+=balise;
        balises[1]+='/';
        balises[1]+=balise;
        while(compteur>=0) {
            if(lireMot(&it, page, &retour, balises)[1]=='/') {
                compteur--;
            } else {
                compteur++;
            }
            it++;
        }
        //On est ressorti de la zone à récuperer.
        retour+=">";
    } else {
        //la balise recherché n'existe pas!
        cerr<<"balise de type "<<balise<<" non trouvé : "<<nom<<" dans le texte se trouvant dans erreur.html"<<endl;
        ofstream erreur ("./traces/erreur.html");
        erreur<<*page<<endl;
        retour="! BALISE NON TROUVE... ... ...";
         //pause("ERREUR DE BALISE 2");
         if(AUTOKILL==1){
            cout<<"erreur balise 2"<<endl;
            exit(1);
         }
         //bp();
    }
    return retour;
}



string enleverPage(string * page, string balise, string nom) {
//prend le nom d'une balise et enlève le contenu texte présent entre la balise d'ouverture et celle de fermeture.
    bool victoire = false;
    if(page->size()<3) {
        cout<<"ERREUR : DECOUPAGE D'UNE PAGE VIDE !!!!"<<endl;
        return "";
    }
    cout<<"découper Page!"<<endl;
    string retour;
    int it =0;
    //int itMem; //mémorise une ancienne valeur de it;
    vector <string> debut(2,"<"); //contient '< balise' et '<balise'.
    debut[0]+=balise;
    debut[1]+=' ';
    debut[1]+=balise;
    vector <string> suite; //contient '>' et le nom cherché.
    suite.push_back(">");
    suite.push_back(nom);
    bool trouve= false;
    while(trouve==false && it<(page->size()-2)) {
        lireMot(&it, page, &retour, debut); //On trouve la balise recherché.
        //afficherMorceau(page, it-2, it+40 );
        if(lireMot(&it,page, &retour, suite)==nom) {
            trouve=true;
            lireChar(&it, page, '>', &retour);
        }
        cout<<"boucle, it ="<<it<<endl;
    }
    cout<<"sortie"<<endl;
    ofstream erreur ("./traces/erreur.html");
    erreur<<*page<<endl;
    //retour="! BALISE NON TROUVE... ... ...";
    //pause("ERREUR DE BALISE 3");
     //bp();
    if(it<(page->size()-1)) {
        //On est à l'endroit du texte qu'il faut commencer à aspirer.
        string retour2;
        retour2+="<";
        retour2+=balise;
        int compteur=0;
        vector <string> balises(2,"<");
        balises[0]+=balise;
        balises[1]+='/';
        balises[1]+=balise;
        while(compteur>=0) {
            if(lireMot(&it, page, &retour2, balises)[1]=='/') {
                compteur--;
            } else {
                compteur++;
            }
            it++;
        }
        victoire = true;
        //On est ressorti de la zone à récuperer.
        retour2+=">";
    } else {
        //la balise recherché n'existe pas!
        /*
        cerr<<"balise non trouvé : "<<nom<<" dans le texte se trouvant dans erreur.html"<<endl;
        ofstream erreur ("erreur.html");
        erreur<<*page<<endl;
        retour="! BALISE NON TROUVE... ... ...";
         pause("ERREUR DE BALISE 3");
         //bp();
        */
    }
    recopieFin(&it, page, &retour);
    return retour;
}


string decouperPageInv(string * page, string balise, string nom) {
//prend le nom d'une balise et renvoi le contenu de la page en enlevant le texte présent entre la balise d'ouverture et celle de fermeture.
    string retour;
    int it =0;
    //int itMem; //mémorise une ancienne valeur de it;
    vector <string> debut(2,"<"); //contient '< balise' et '<balise'.
    debut[0]+=balise;
    debut[1]+=' ';
    debut[1]+=balise;
    vector <string> suite; //contient '>' et le nom cherché.
    suite.push_back(">");
    suite.push_back(nom);
    bool trouve= false;
    while(trouve==false && it<(page->size()-1)) {
        lireMot(&it, page,&retour , debut); //On trouve la balise recherché.
        //afficherMorceau(page, it-2, it+40 );
        if(lireMot(&it,page,&retour, suite)==nom) {
            trouve=true;
            lireChar(&it, page, '>', &retour);
        }
    }
    if(it<(page->size()-1)) {
        //On est à l'endroit du texte qu'il faut commencer à ne pas recopier.
        retour+="<";
        retour+=balise;
        int compteur=0;
        vector <string> balises(2,"<");
        balises[0]+=balise;
        balises[1]+='/';
        balises[1]+=balise;
        while(compteur>=0) {
            if(lireMot(&it, page, balises)[1]=='/') {
                compteur--;
            } else {
                compteur++;
            }
            it++;
        }
        //On est ressorti de la zone à ne pas récuperer.
        //retour+=">";
        recopieFin(&it, page, &retour);
    } else {
        //la balise recherché n'existe pas!
        cerr<<"balise non trouvé : "<<nom<<" dans le texte :"<<endl<<*page<<endl;
        retour="! BALISE NON TROUVE ...";
         //bp();

    }
    return retour;
}


vector <string> trouverToutLesLiens(string * page){
    return trouverToutLesLiens(page, 0);
}


vector <string> trouverToutLesLiensInterne(string * page){
    vector <string> retour = trouverToutLesLiens(page, 1);

    return retour;
}

vector <string> trouverToutLesLiens(string * page, bool interne) {
    int it=0;
    string lien;
    vector <string> retour;
    int itTemp;
    vector <string> extImages;
    extImages.push_back(".jpg");
    extImages.push_back(".svg");
    extImages.push_back(".png");
    extImages.push_back(".jpeg");
    while(lireMot(&it, page, "href=\"")) {
        itTemp=0;
        lireChar(&it, page, '"',&lien);
        if(lien[0]!='#' && lireMot(&itTemp, &lien, extImages)=="" && !lireMot( &lien, "Aide:") && !lireMot( &lien, "action=edit") && (!lireMot( &lien, "//") || !interne)  && !lireMot(&lien, "commons") && !lireMot(&lien, "index.php") &&  !lireMot(&lien, ":") &&  !lireMot(&lien, "API")  &&  !lireMot(&lien, "International_Standard")){
            retour.push_back(lien);
        }
        lien.clear();
    }
    return retour;
}


string htmlToText(string * texteHTML) {
    string retour;
    //string retour2;
    int it=0;
    vector <string> listeMots;
    listeMots.push_back(">"); //Normal, les suivants sont les mots pour lesquels il faut enlever le contenu entre les deux balises.
    listeMots.push_back("script");
    string motRetour, motCherche;
    while(lireChar(&it, texteHTML, '<', &retour)) {
        motRetour = lireMot(&it, texteHTML, listeMots);
        //cout<<"mot retour : "<<motRetour;
        if(motRetour != ">") {
            motCherche = "/";
            motCherche+=motRetour;
            lireMot(&it, texteHTML, motCherche);
            lireChar(&it, texteHTML, '>');
            it++;
        } else {
            lireChar(&it, texteHTML, '>');
            it++;
        }
    }
    return retour;

}

string sansEspace(string s) {
    //cout<<"SANS ESPACES!"<<endl;
    string retour;
    int it=0;
    while (lireChar(&it, &s, ' ', &retour)) {
        retour+="%20";
        it++;
    }
    //cout<<"Sans espaces : "<<endl<<s<<endl<<" donne : "<<endl<<retour<<endl;
    return retour;
}

string htmlToXML(string * texte){
    string texte2 = transformer(texte, "&lt;","<");
    string texte3 = transformer(&texte2, "&gt;",">");

    return texte3;
}

int getCodeRetour(){
    return codeRetour;
}


string Url_encodeR (string url){

    if(url[url.size()-1] == ' ')
        url.resize(url.size()-1);

    url = transformer(&url, "€","%80");
    url = transformer(&url, "%A0","%20");
    url = transformer(&url, "%20%20","%20");
    url = transformer(&url, "%20%20%20","%20");
    url = transformer(&url, "=+","=");
    url = transformer(&url, "","%81");
    url = transformer(&url, "‚","%82");
    url = transformer(&url, "ƒ","%83");
    url = transformer(&url, "„","%84");
    url = transformer(&url, "…","%85");
    url = transformer(&url, "†","%86");
    url = transformer(&url, "‡","%87");
    url = transformer(&url, "ˆ","%88");
    url = transformer(&url, "‰","%89");
    url = transformer(&url, "Š","%8A");
    url = transformer(&url, "‹","%8B");
    url = transformer(&url, "Œ","Oe");
    url = transformer(&url, "","%8D");
    url = transformer(&url, "Ž","%8E");
    url = transformer(&url, "","%8F");
    url = transformer(&url, "","%90");
    url = transformer(&url, "‘","%91");
    url = transformer(&url, "’","%92");
    url = transformer(&url, "“","%93");
    url = transformer(&url, "”","%94");
    url = transformer(&url, "•","%95");
    url = transformer(&url, "–","%96");
    url = transformer(&url, "—","%97");
    url = transformer(&url, "˜","%98");
    url = transformer(&url, "™","%99");
    url = transformer(&url, "š","%9A");
    url = transformer(&url, "›","%9B");
    url = transformer(&url, "","%9D");
    url = transformer(&url, "ž","%9E");
    url = transformer(&url, "Ÿ","%9F");
    url = transformer(&url, "¡","%A1");
    url = transformer(&url, "¢","%A2");
    url = transformer(&url, "£","%A3");
    url = transformer(&url, "¤","%A4");
    url = transformer(&url, "¥","%A5");
    url = transformer(&url, "¦","%A6");
    url = transformer(&url, "§","%A7");
    url = transformer(&url, "¨","%A8");
    url = transformer(&url, "©","%A9");
    url = transformer(&url, "ª","%AA");
    url = transformer(&url, "«","%AB");
    url = transformer(&url, "¬","%AC");
    url = transformer(&url, "­","%AD");
    url = transformer(&url, "®","%AE");
    url = transformer(&url, "¯","%AF");
    url = transformer(&url, "°","%B0");
    url = transformer(&url, "±","%B1");
    url = transformer(&url, "²","%B2");
    url = transformer(&url, "³","%B3");
    url = transformer(&url, "´","%B4");
    url = transformer(&url, "µ","%B5");
    url = transformer(&url, "¶","%B6");
    url = transformer(&url, "·","%B7");
    url = transformer(&url, "¸","%B8");
    url = transformer(&url, "¹","%B9");
    url = transformer(&url, "º","%BA");
    url = transformer(&url, "»","%BB");
    url = transformer(&url, "¼","%BC");
    url = transformer(&url, "½","%BD");
    url = transformer(&url, "¾","%BE");
    url = transformer(&url, "¿","%BF");
    url = transformer(&url, "À","%C0");
    url = transformer(&url, "Á","%C1");
    url = transformer(&url, "Â","%C2");
    url = transformer(&url, "Ã","%C3");
    url = transformer(&url, "Ä","%C4");
    url = transformer(&url, "Å","%C5");
    url = transformer(&url, "Æ","%C6");
    url = transformer(&url, "Ç","%C7");
    url = transformer(&url, "È","%C8");
    url = transformer(&url, "É","%C9");
    url = transformer(&url, "Ê","%CA");
    url = transformer(&url, "Ë","%CB");
    url = transformer(&url, "Ì","%CC");
    url = transformer(&url, "Í","%CD");
    url = transformer(&url, "Î","%CE");
    url = transformer(&url, "Ï","%CF");
    url = transformer(&url, "Ð","%D0");
    url = transformer(&url, "Ñ","%D1");
    url = transformer(&url, "Ò","%D2");
    url = transformer(&url, "Ó","%D3");
    url = transformer(&url, "Ô","%D4");
    url = transformer(&url, "Õ","%D5");
    url = transformer(&url, "Ö","%D6");
    url = transformer(&url, "×","%D7");
    url = transformer(&url, "Ø","%D8");
    url = transformer(&url, "Ù","%D9");
    url = transformer(&url, "Ú","%DA");
    url = transformer(&url, "Û","%DB");
    url = transformer(&url, "Ü","%DC");
    url = transformer(&url, "Ý","%DD");
    url = transformer(&url, "Þ","%DE");
    url = transformer(&url, "ß","%DF");
    url = transformer(&url, "á", "%E1");
    url = transformer(&url, "â", "%E2");
    url = transformer(&url, "ã", "%E3");
    url = transformer(&url, "ä", "%E4");
    url = transformer(&url, "å", "%E5");
    url = transformer(&url, "ç", "%E7");
    url = transformer(&url, "ê", "%EA");
    url = transformer(&url, "ë", "%EB");
    url = transformer(&url, "ì", "%EC");
    url = transformer(&url, "í", "%ED");
    url = transformer(&url, "î", "%EE");
    url = transformer(&url, "ï", "%EF");
    url = transformer(&url, "à", "%E0");
    url = transformer(&url, "œ","oe");
    url = transformer(&url, "é","%E9");
    url = transformer(&url, "É","%E9");
    url = transformer(&url, "(","%28");
    url = transformer(&url, ")","%29");
    url = transformer(&url, ",","%2C");
    url = transformer(&url, "è","%E8");
    url = transformer(&url, "\"","%22");
    url = transformer(&url, "\'","%27");
    url = transformer(&url, "#","%23");
    url = transformer(&url, "vIH","VIH");
    url = transformer(&url, "ñ","%F1");
    url = transformer(&url, "ò","%F2");
    url = transformer(&url, "ó","%F3");
    url = transformer(&url, "ô","%F4");
    url = transformer(&url, "õ","%F5");
    url = transformer(&url, "ö","%F6");
    url = transformer(&url, "`","%60");
    transformer(&url, ' ','+');

    return url;

}


