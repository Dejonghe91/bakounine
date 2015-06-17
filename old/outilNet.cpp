#include "outilNet.h"
#include "./outilParsage.h"
#include "./tout.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

int codeRetour; // defini en extern dans le .h

string cookie;
string referer;

vector <LienCompteur> lienDejaVisites;

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

void ecrireLiens(string nomFichier, int min) {
    ofstream traceMots (nomFichier.c_str());
    for(int i=0; i<(int)lienDejaVisites.size(); i++) {
        if(lienDejaVisites[i].compteur>=min) {
            traceMots<<lienDejaVisites[i].lien<<" ( "<<lienDejaVisites[i].compteur<<" ) "<<endl;
        }
    }
}

void ecrireBonsLiens(string nomFichier) {
    int it;
    ofstream traceMots (nomFichier.c_str());
    string motTemp;
    for(int i=0; i<(int)lienDejaVisites.size(); i++) {
        it=0;
        if(lienDejaVisites[i].compteur>=(sqrt(lienDejaVisites.size())/4) && !lireMot(&it, &(lienDejaVisites[i].lien), "Wiki") && lienDejaVisites[i].lien.size()>2) {
            it=0;
            if(!lireChar(&it, &(lienDejaVisites[i].lien), ':') && maz(&it)) {
                traceMots<<lienDejaVisites[i].lien<<" ( "<<lienDejaVisites[i].compteur<<" ) "<<endl;
            } else {
                it=0;
                lireChar(&it, &(lienDejaVisites[i].lien), ':');
                it++;
                recopieFin(&it,&(lienDejaVisites[i].lien), &motTemp );
                traceMots<<motTemp<<" ( "<<lienDejaVisites[i].compteur<<" ) "<<endl;
                motTemp.clear();
            }
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
        cout<<"cookie changé!"<<endl;
    }
    Request.setField("cookie", cookieC);
    Request.setField("referer", refererC);
    std::cout << "Sending a request to "<<domaine  <<"..." << std::endl;
    std::cout << "page à ouvrir : "<<page<<endl;
    std::cout << "parametres : "<<param<<endl;
    std::cout << "Cookie : "<<cookieC<<endl;
    std::cout << "Referer : "<<refererC<<endl;
    sf::Http::Response Page = Http.sendRequest(Request);
    cout<<"parametres : "<<param<<endl;
    std::cout << "Status code (should be 200 on success) : " << Page.getStatus() << std::endl
              << "Response received from "<<domaine << std::endl
              << "HTTP version: " << Page.getMajorHttpVersion() << "." << Page.getMinorHttpVersion() << std::endl;
    //std::cout<< "Returned message : " << Page.getBody() << std::endl << std::endl;
    //    cookie = Page.getField("Set-Cookie");
    //    referer = domaine+page;
    string reponse = Page.getBody();
    if(Page.getStatus()!=200) {
        cout<<"F1"<<endl;
        cout<<"adresse posant problème : "<<domaine<<", "<<page<<endl;
        cout<<"réponse reçut après erreur : "<<reponse<<endl;
    }

/*
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
*/
    return reponse;
}

string ouvrirPage(string domaine, string page,  vector <vector <string> > var ) {
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
    cout<<"param : "<<parametre<<endl;
    //pause("verifions tout ca...");
    Request.setHttpVersion(1, 0);
    std::cout << "Sending a request to "<<domaine  <<"..." << std::endl;
    std::cout << "page à ouvrir : "<<page<<endl;
    sf::Http::Response Page = Http.sendRequest(Request);
    std::cout << "Status code (should be 200 on success) : " << Page.getStatus() << std::endl
              << "Response received from "<<domaine << std::endl
              << "HTTP version: " << Page.getMajorHttpVersion() << "." << Page.getMinorHttpVersion() << std::endl;
    //<< "Returned message : " << Page.getBody() << std::endl << std::endl;
    cookie = Page.getField("Set-Cookie");
    referer = domaine+page;
    string reponse = Page.getBody();
    if(Page.getStatus()!=200) {
        cout<<"F2"<<endl;
        cout<<"adresse posant problème : "<<domaine<<", "<<page<<endl;
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
        reponse = ouvrirPage(temp);
    }

    return reponse;
}


string ouvrirPage(string domaine, string page, string param) {
    sf::Http Http;
    Http.setHost(domaine);
    sf::Http::Request Request;
    Request.setMethod(sf::Http::Request::Post);
    Request.setUri(page);
    Request.setBody(param);
    Request.setHttpVersion(1, 0);
    //TEST!!!
    if(cookie.size()>0){
        cout<<"cookie envoyé : "<<cookie<<endl;
        Request.setField("cookie", cookie);
    }
/*
    if(referer.size()>0){
        Request.setField("referer", referer);
    }
*/
    std::cout << "Sending a request to "<<domaine  <<"..." << std::endl;
    std::cout << "page à ouvrir : "<<page<<endl;
    std::cout << "parametres : "<<param<<endl;
    std::cout << "Cookie : "<<cookie<<endl;
    std::cout << "Referer : "<<referer<<endl;
    sf::Http::Response Page = Http.sendRequest(Request);
    cout<<"parametres : "<<param<<endl;
    std::cout << "Status code (should be 200 on success): " << Page.getStatus() << std::endl
              << "Response received from "<<domaine << std::endl
              << "HTTP version: " << Page.getMajorHttpVersion() << "." << Page.getMinorHttpVersion() << std::endl;
    //std::cout<< "Returned message : " << Page.getBody() << std::endl << std::endl;
    cookie = Page.getField("Set-Cookie");
    referer = domaine+page;
    string reponse = Page.getBody();
    if(Page.getStatus()!=200) {
        cout<<"F3"<<endl;
        cout<<"adresse posant problème : "<<domaine<<", "<<page<<endl;
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

    return reponse;
}


string ouvrirPage(string domaine, string page) {
    sf::Http Http;
    Http.setHost(domaine);
    sf::Http::Request Request;
    Request.setMethod(sf::Http::Request::Get);
    //Request.setMethod(sf::Http::Request::Post); //A CHANGER
    Request.setUri(page);
    Request.setBody("");
    Request.setHttpVersion(1, 0);
    //AJOUT
    /*
    Request.setField("Connection", "keep-alive");
    Request.setField("Keep-Alive","115");
    */
    //FIN AJOUT


    if(cookie.size()>0){
        //cout<<"cookie envoyé : "<<cookie<<endl;
        Request.setField("cookie", cookie);
    } else {
        cout<<"pas de cookies!"<<endl;
    }

//    std::cout << "Sending a request to "<<domaine  <<"..." << std::endl;
//    std::cout << "page à ouvrir : "<<page<<endl;
    sf::Http::Response Page = Http.sendRequest(Request);
/*    std::cout << "Status code (should be 200 on success): " << Page.getStatus() << std::endl
              << "Response received from "<<domaine << std::endl
              << "HTTP version: " << Page.getMajorHttpVersion() << "." << Page.getMinorHttpVersion() << std::endl;
    //std::cout<< "Returned message : " << Page.getBody() << std::endl << std::endl;
*/
    cookie = Page.getField("Set-Cookie");
//    std::cout << "Cookie récupéré : \""<<cookie<<"\""<<endl;
    referer = domaine+page;
    codeRetour = Page.getStatus();
    string reponse = Page.getBody();
    if(Page.getStatus()!=200) {
        //pause("erreur suspecté");
        for(int i=0; i < page.size(); i++){
            cout<<page[i]<<" - "<<(int)page[i]<<endl;
        }
        cout<<"F4"<<endl;
        cout<<"adresse posant problème : "<<domaine<<", "<<page<<endl;
        cout<<"réponse reçut après erreur : "<<reponse<<endl;
        //pause("/erreur");
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
        cout<<"reponse : "<<reponse<<endl;
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

string ouvrirPage(string addresse) { //ATTENTION : NECESSITE UNE ADRESSE CONTERNANT LE DOMAINE
    string domaine;
    string page;
    decoupeLien(addresse, &domaine, &page);
    return ouvrirPage(domaine, page);
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
    cout<<"trouverLien!(string *, int *, string)"<<endl;
    cout<<contenu->size()<<endl;
    cout<<*it<<endl;
    cout<<nomDuLien<<endl;
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
        // bp();
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
        if((nom =="" && ((*page)[*it]=='>' || (*page)[*it]==' ')) || lireMot(it,page,suite)==nom ) {
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
            /*cout<<"2... :"<<endl;
            cout<<balises[0]<<endl;
            cout<<balises[1]<<endl;
            cout<<compteur<<endl;
            cout<<*it<<endl;
            cout<<page->size()<<endl;
            */
            if(compteur>0){
                //cout<<"récupéré : "<<retour<<endl;
                 //bp();

            }
            if(*it>=(int)page->size()){
                //cout<<"morceau étrange : "<<page<<endl;
                //cout<<"récupéré : "<<retour<<endl;
                // bp();
                compteur = -2;
            }
            if(lireMot(it, page, &retour, balises)[1]=='/') {
                //cout<<"trouve balise fermante"<<endl;
                compteur--;
            } else {
                cout<<"trouve balise ouvrante!!"<<endl;
                 //bp();
                compteur++;
            }
            (*it)++;
        }
        //On est ressorti de la zone à récuperer.
        retour+=">";
    } else {
        //la balise recherché n'existe pas!
        //cout<<"balise non trouvé!"<<endl;
        //cerr<<"balise non trouvé : "<<nom<<" dans le texte :"<<endl<<*page<<endl;
        /*
        cerr<<"balise non trouvé ! "<<endl;
        cout<<"retour!!!! '"<<retour<<"'"<<endl;
        retour="! BALISE NON TROUVE... ... ...";
         pause("ERREUR DE BALISE");
         bp();
        */
        retour="";
    }
    return retour;
}

string decouperPage(string * page, string balise, string nom) {
//prend le nom d'une balise et récupère le contenu texte présent entre la balise d'ouverture et celle de fermeture.
    if(page->size()<3) {
        cout<<"ERREUR : DECOUPAGE D'UNE PAGE VIDE !!!!"<<endl;
        int pause;
        cin>>pause;
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
        if(lireMot(&it,page,suite)==nom) {
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
        //cerr<<"balise non trouvé : "<<nom<<" dans le texte :"<<endl<<*page<<endl;
        retour=""; // ! BALISE NON TROUVE... ... ...";
        //pause("ERREUR DE BALISE 2");
         //bp();
    }
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
        //cerr<<"balise non trouvé : "<<nom<<" dans le texte :"<<endl<<*page<<endl;
        retour="";
         //bp();

    }


    return retour;
}


vector <string> trouverToutLesLiens(string * page) {
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
        if(lien[0]!='#' && lireMot(&itTemp, &lien, extImages)=="") {
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


    /*
    pipe(inPipe1);
    pid_t pid1;
    pid1=fork();
    if (pid1) {
        // Processus Pere
        close(inPipe1[1]);
        char * sTemp = (char *) malloc(1000*sizeof(char));
        read(inPipe1[0], sTemp, 1000);

    } else {
        //processus fils
        dup2(inPipe2[1],1);
        //execl("killall","-9","html2text",NULL);
        execl("html2text",,"--mode","gtp","--chinese-rules",NULL);

    }
    */


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

