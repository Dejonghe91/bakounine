#include "wikidata.h"

ofstream ofsRelM ("./ressources/relationsMotsWD.txt");



void bakoustatplayWD(){ //V1
    string cible;
    string source;
    ofstream ofs ("./ressources/relationsTrouveWD.txt");
    map<string, string> relation = bakoulearnFichier("./ressources/relationsMotsWD.txt");
    cout<<"ce que l'on a appris : "<<endl;
    string ligne; //un mot du top100
    string mot; //un mot du top100 version JDM
    string lien; //Le lien correspondant
    string page; //une page du top100
    map<string,string> infos;  // Le contenu de l'infobox d'une page...
    vector <string> relations; //Les relations de JDM pour le mot donné.
    int cpt=0, itmot=0;
    while(itmot < termesavisiter.size() && cpt<1000){
        ligne = termesavisiter[itmot];
        if((int)ligne[ligne.size()-1]==13){
            ligne.resize(ligne.size()-1);
        }
        cpt++;
        itmot++;
        cout<<endl<<"------------------"<<endl<<endl<<"ligne : "<<ligne<<endl;
        mot=ligne;
        string jdme = jdmExiste(mot);
        if(jdme==""){
            mot[0]+=32;
            jdme=jdmExiste(mot);
        }
        if(jdme!=""){  //Si le mot est présent dans JDM
            cout<<"mot existant"<<endl;
            string id = qid(mot);
            if(id!=""){
                map<string, vector<string> > relationsT = getRelWD(id);
                for(map<string, vector <string> >::iterator i=relationsT.begin(); i!=relationsT.end(); i++){
                    if(i->first.size()>1 ){
                        for(int i2=0; i2<i->second.size(); i2++){
                            string mot2 = i->second[i2];
                            if(i->second[i2].size()>2 && mot!=i->second[i2] && !lireMot(&mot2, ":") && !lireMot(&mot2,"wiki") ){

                                string nomRelC = getNomRel(i->first);
                                cible = majusculeW(jdmExiste(i->second[i2]));
                                source = majusculeW(jdmExiste(mot));
                                if(relation[nomRelC]!=""){ //si on a trouvé une relation
                                    if(relation[nomRelC]=="locution"){
                                        relation[nomRelC]="syn";
                                    }
                                    if(cible!=""){
                                        ofs<<transformer(&source,"_", " ")<<" -- "<<relation[nomRelC]<<" --> "<<transformer(&cible,"_", " ");
                                        if(existeRel(mot, i->second[i2], relation[nomRelC])){
                                            ofs<<" | JDM"<<endl;
                                        } else {
                                            ofs<<" | Nouvelle relation"<<endl;
                                        }

                                        addnewtermeavisiter(transformer(&cible,"_", " "));

                                    } else {
                                        source= majusculeW(mot);
                                        cible = majusculeW(i->second[i2]);
                                        ofs<<transformer(&source,"_", " ")<<" -- "<<relation[nomRelC]<<" --> "<<transformer(&cible, "_", " ")<<" | nouveau mot"<<endl;

                                        addnewtermeavisiter(transformer(&cible,"_", " "));
                                    }
                                }
                            }
                            if(lireMot(&mot2, ":")){
                                //A faire : les catégories...
                            }
                        }
                    }
                }
            }
        }
    }
}



map<string, string> bakoulearnFichier(string s){ //SUR un fichier avec mot - r
    ifstream ifs(s);
    ofstream ofs("./ressources/motR.txt");
    ofstream fichier("./ressources/motStat.txt");
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
        if(lireMot(&i, &ligne, &relCourante, " - ")){
            lireMot(&i, &ligne, &motCourant, " : ");
            MotRelation[relCourante][motCourant]++;
        }
    }
    map<string, int>::iterator iter;
    map<string, string>::iterator iterTrace;
    for(iterMR=MotRelation.begin(); iterMR!=MotRelation.end(); iterMR++){
        ofs<<iterMR->first<<endl;
        int maxi=2;
        int total=0;
        string maxs="";
        for(iter=iterMR->second.begin(); iter!=iterMR->second.end(); iter++) {
            if(iter->first!="R" && iter->first!="wiki" && iter->first!="associated" && iter->first!="aki" && iter->second>maxi){
                maxi=iter->second;
                maxs=iter->first;
            }
            total += iter->second;
            ofs<<"    "<<iter->first<<" : "<<iter->second<<endl;
        }

        if(maxs != "" &&maxi>1 && (maxi*20)>=total )
        {
            fichier << iterMR->first << "|" << maxs << "|" << (float)maxi/(float)total<<endl;
            retour[iterMR->first]=maxs;
        }
    }
    cout<<"------------Mot->relation-----------"<<endl;
    for(iterTrace=retour.begin(); iterTrace!=retour.end(); iterTrace++) {
        cout<<iterTrace->first<<" : "<<iterTrace->second<<endl;
    }
    return retour;
}






map<string, string> bakoustatlearnWD(){ //SUR WIKIDATA
    //lire toutes les catégories possibles dans wikipedia et les comparer aux relations sémantiques dans JDM (en français d'abord)
    //1 : ouvrir 100 pages wikipedia
    //string fileRes = "./ressources/relationsMotsWD.txt";
    //
    map<string, string> retour;
    map<string, map<string, int> > MotRelation; // clé : un mot dans une infobox de wp : valeur, une liste de relations avec leur score.
    map<string, map<string, int> >::iterator iterMR;
    map<string, int>::iterator iter;
    map<string, string>::iterator iterTrace;
    //ifstream ifs ("./ressources/liensavisiter.txt");
    string ligne; //un mot du top100
    string mot; //Version JDM du mot
    string lien; //Le lien correspondant
    string page; //une page du top100
    map<string,string> infos;  // Le contenu de l'infobox d'une page...
    vector <string> relations; //Les relations de JDM pour le mot donné.

    string id;
    map<string, vector<string> > relationsT;


    int cpt=0, itmot=0;
    while(itmot < termesavisiter.size() && cpt<800){
        ligne = termesavisiter[itmot];
        if((int)ligne[ligne.size()-1]==13){
            ligne.resize(ligne.size()-1);
        }
        cpt++;
        itmot++;
        cout<<endl<<"------------------"<<endl<<endl<<"ligne : "<<ligne<<endl;
        mot=transformer(&ligne,"_", " ");

        string jdme = jdmExiste(mot);
        if(jdme==""){
            mot[0]+=32;
            jdme=jdmExiste(mot);
        }
        if(jdme!=""){  //Si le mot est présent dans JDM
            ofsRelM<<jdme<<" : "<<endl;
            cout<<"mot existant"<<endl;
            id = qid(mot);
            if(id!=""){
                cout<<"page trouvé"<<endl;
                relationsT = getRelWD(id);
                for(map<string, vector <string> >::iterator i=relationsT.begin(); i!=relationsT.end(); i++){
                    if(i->first.size()>2 ){
                        cout<<"Et 1 : "<<i->first<<endl;
                        for(int i2=0; i2<i->second.size(); i2++){
                            if(i->second[i2].size()>2 && mot!=i->second[i2]){
                                cout<<"Et 2 : "<<i->second[i2]<<endl;
                                relations = jdmRel(mot, i->second[i2]);
                                MotRelation[i->first]["R"]++;
                                ofsRelM<<getNomRel(i->first)<<" - R : "<<MotRelation[i->first]["R"]<<" ( "<<i->second[i2]<<" ) "<<endl;
                                for(int j=0; j<relations.size(); j++){
                                    cout<<"Et 3 : "<<relations[j]<<endl;
                                    MotRelation[i->first][relations[j]]++;//On incremente la relation  courante pour l'info courante.
                                    ofsRelM<<getNomRel(i->first)<<" - "<<relations[j]<<" : "<<MotRelation[i->first][relations[j]]<<" ( "<<i->second[i2]<<" ) "<<endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    retour = bakoulearnFichier("./ressources/relationsMotsWD.txt");
    return retour;
}
