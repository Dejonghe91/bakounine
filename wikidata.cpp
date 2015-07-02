#include "wikidata.h"


void bakouplayWD(){ //V1
    string cible;
    string source;
    ofstream ofs ("./ressources/relationsTrouveWD.txt");
    map<string, string> relation = bakoulearnFichier("relationsMotsWD.txt");
    cout<<"ce que l'on a appris : "<<endl;
    ifstream ifs ("./ressources/liensavisiter.txt");
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
            string id = qid(mot);
            if(id!=""){
                map<string, vector<string> > relationsT = getRelWD(id);
                for(map<string, vector <string> >::iterator i=relationsT.begin(); i!=relationsT.end(); i++){
                    if(i->first.size()>1 ){
                        for(int i2=0; i2<i->second.size(); i2++){
                            string mot2 = i->second[i2];
                            if(i->second[i2].size()>2 && mot!=i->second[i2] && !lireMot(&mot2, ":") && !lireMot(&mot2,"wiki") ){
                //for(map<string, string>::iterator i=infos.begin(); i!=infos.end(); i++){
                    //if(i->first.size()>2 && i->second.size()>2 && mot!=i->second  ){
                        //cout<<" => "<<i->first<<" - "<<i->second<<endl;
                        //if(relation[i->first].size()>1){
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
                                    } else {
                                        source= majusculeW(mot);
                                        cible = majusculeW(i->second[i2]);
                                        ofs<<transformer(&source,"_", " ")<<" -- "<<relation[nomRelC]<<" --> "<<transformer(&cible, "_", " ")<<" | nouveau mot"<<endl;
                                    }
                                }
                            }
                            if(lireMot(&mot2, ":")){
                                //A faire : les catégories...
                            }
                            //<<endl;
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
    pause("ben...");
    return retour;
}

map<string, string> bakoustatlearnWD(){ //SUR WIKIDATA
    //lire toutes les catégories possibles dans wikipedia et les comparer aux relations sémantiques dans JDM (en français d'abord)
    //1 : ouvrir 100 pages wikipedia
    ofstream ofs ("./ressources/relationsMotsWD.txt", ios::out | ios::app);
    map<string, string> retour;
    map<string, map<string, int> > MotRelation; // clé : un mot dans une infobox de wp : valeur, une liste de relations avec leur score.
    map<string, map<string, int> >::iterator iterMR;
    map<string, int>::iterator iter;
    map<string, string>::iterator iterTrace;
    ifstream ifs ("./ressources/liensavisiter.txt");
    string ligne; //un mot du top100
    string mot; //Version JDM du mot
    string lien; //Le lien correspondant
    string page; //une page du top100
    map<string,string> infos;  // Le contenu de l'infobox d'une page...
    vector <string> relations; //Les relations de JDM pour le mot donné.

    string id;
    map<string, vector<string> > relationsT;


    int cpt=0;
    while(getline(ifs, ligne) && cpt<2000){
        if((int)ligne[ligne.size()-1]==13){
            ligne.resize(ligne.size()-1);
        }
        cpt++;
        cout<<endl<<"------------------"<<endl<<endl<<"ligne : "<<ligne<<endl;
        mot=transformer(&ligne,"_", " ");

        string jdme = jdmExiste(mot);
        if(jdme==""){
            mot[0]+=32;
            jdme=jdmExiste(mot);
        }
        if(jdme!=""){  //Si le mot est présent dans JDM
            ofs<<jdme<<" : "<<endl;
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
                                ofs<<getNomRel(i->first)<<" - R : "<<MotRelation[i->first]["R"]<<" ( "<<i->second[i2]<<" ) "<<endl;
                                for(int j=0; j<relations.size(); j++){
                                    MotRelation[i->first][relations[j]]++;//On incremente la relation  courante pour l'info courante.
                                    ofs<<getNomRel(i->first)<<" - "<<relations[j]<<" : "<<MotRelation[i->first][relations[j]]<<" ( "<<i->second[i2]<<" ) "<<endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    bakoulearnFichier("relationsMotsWD.txt");
}
