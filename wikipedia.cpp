#include "wikipedia.h"



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


//lire toutes les catégories possibles dans wikipedia et les comparer aux relations sémantiques dans JDM (en français d'abord)
//1 : ouvrir 100 pages wikipedia
map<string, string> bakoustatlearn()
{
    ofstream ofs ("./ressources/relationsMots.txt",  ios::out | ios::app);                 // écriture des résultats
    map<string, string> retour;                         // map de retour
    map<string, map<string, int> > MotRelation;         // clé : un mot infobox : valeur, une liste de relations avec leur score.
    map<string, map<string, int> >::iterator iterMR;    // itérateur sur la map mot relations
    map<string, int>::iterator iter;                    // itérateur sur relation score
    map<string, string>::iterator iterTrace;            // itérateur sur la map return
    ifstream ifs ("./ressources/liensavisiter.txt");                 // fichier pages à visité
    string ligne;                                       // un mot du top100
    string mot;                                         // version JDM du mot
    string lien;                                        // le lien correspondant
    string page;                                        // une page du top100
    map<string,string> infos;                           // le contenu de l'infobox d'une page...
    vector <string> relations;                          // les relations de JDM pour le mot donné.
    int cpt=0;


    // parcours de tous les mots à visiter sur wikipédia
    while(getline(ifs, ligne) && cpt<4600) {
        if((int)ligne[ligne.size()-1]==13) {
            ligne.resize(ligne.size()-1);
        }

        cpt++;
        cout<<endl<<"------------------"<<endl<<endl<<"ligne : "<<ligne<<endl;
        mot=ligne;
        string jdme = jdmExiste(mot);

        if(jdme=="") {
            mot[0]+=32;
            jdme=jdmExiste(mot);
        }

        if(jdme!="") //Si le mot est présent dans JDM
        {
            cout<<"mot existant"<<endl;
            lien = "http://fr.wikipedia.org/wiki/";
            lien +=ligne;
            transformer(&lien, ' ','_');
            page = ouvrirPage(lien);
            infos = ouvririnfobox(page);
            for(map<string, string>::iterator i=infos.begin(); i!=infos.end(); i++){
                if(i->first.size()>2 && i->second.size()>2 && mot!=i->second){
                    ofs << jdme + "|" << i->first + "|" << i->second << endl;
                    relations = jdmRel(mot, i->second);
                    MotRelation[i->first]["R"]++;
                    for(int j=0; j<relations.size(); j++){
                        MotRelation[i->first][relations[j]]++;//On incremente la relation  courante pour l'info courante.
                    }
                }
            }
        }
    } //fin du while

    int maxi;
    string maxs;

    for(iterMR=MotRelation.begin(); iterMR!=MotRelation.end(); iterMR++)
    {
        maxi=0;
        maxs="";
        for(iter=iterMR->second.begin(); iter!=iterMR->second.end(); iter++) {
            if(iter->first!="R" && iter->first!="wiki" && iter->first!="associated" && iter->first!="aki" && iter->second>maxi) {
                maxi=iter->second;
                maxs=iter->first;
            }
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

void bakoustatplay(){
    string cible;
    string source;
    ofstream ofs ("./ressources/relationsTrouve.txt");
    map<string, string> relation = bakoustatlearn();
    cout<<"ce que l'on a appris : "<<endl;
    ifstream ifs ("./ressources/liensavisiter.txt");
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
