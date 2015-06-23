#include "bakuSemanticLearn.h"


void bakouSemanticLearn(vector<string> words){

    BakuSemantic baseSem;
    ofstream fichier("test.txt", ios::out | ios::trunc);
    map<string, vector<RelSem>> base = baseSem.getBakuSemanticBase();
    cout << "Chargement de la base sémantic ...";

    if(base.empty()){
        base = baseSem.getBakuSemanticBase();
    }
    cout << "Chargée !" << endl;

    // parcours de tous les termes de la liste words
    for (long i=0; i<(long)words.size(); ++i)
    {
        string mr =  jdmExiste(words.at(i));
        if(mr != "" && !baseSem.isRelExist(mr)) {
            cout << "le mot ["+ mr + "] n'existe pas dans la base, on récupère les voisins dans jeux de mots" << endl;
            vector<relfind> voisins = getNeightboors(mr);
            cout << "Il y a " << voisins.size() << " voisins" << endl;

            for (long j=0; j<(long)voisins.size(); ++j)
            {
                relfind voisin = voisins.at(j);

                cout << "\t voisin : "+voisin.cible+" avec relation (e/s) " << voisin.sens << " nommée " + voisin.rel << endl;

                if( baseSem.isRelExist(voisin.cible) )
                {
                    // voisin = tlist
                    cout << "match dans la base avec tlist = ["+voisin.cible+"] avec rel = "+ voisin.rel << endl;

                    if(voisin.rel == "r_syn" || voisin.rel == "r_syn_strict") {
                        cout << "avec la relation syn" << endl;
                        vector<RelSem> rels = baseSem.getRel(voisin.cible);
                        addWord(baseSem, mr, rels);
                    }
                    else if (voisin.rel == "r_isa" || voisin.rel == "r_hypo") {
                        cout << "avec les relations hyp et syn" << endl;

                        vector<string> filtre;
                        filtre.push_back("r_isa");
                        filtre.push_back("r_hypo");
                        filtre.push_back("r_instance");

                        string tlist = voisin.cible;

                        vector<relfind> matchVoisins = getNeightboors(voisin.cible, filtre);


                        for(long k=0; k<(long)matchVoisins.size(); ++k) { // parcours des voisins de tlist
                            for(long l=0; l<(long) voisins.size(); ++l){ // parcours des voisins

                                cout << tlist+ " -> "+matchVoisins.at(k).cible << endl;
                                cout << mr+ " -> "+voisins.at(l).cible << endl;

                                if(matchVoisins.at(k).cible == voisins.at(l).cible){
                                    relfind vtlist_tlist = matchVoisins.at(k);
                                    relfind vtlist_mr = voisins.at(l);

                                    if( (vtlist_tlist.rel == "instance" ||
                                        (vtlist_tlist.rel == "isa" && vtlist_tlist.sens == 1) ||
                                        (vtlist_tlist.rel == "hypo" && vtlist_tlist.sens == 0))
                                        &&
                                        (vtlist_mr.rel == "instance" ||
                                        (vtlist_mr.rel == "isa" && vtlist_mr.sens == 1) ||
                                        (vtlist_mr.rel == "hypo" && vtlist_mr.sens == 0)) ) {

                                        vector<RelSem> rels = baseSem.getRel(jdmExiste(voisin.cible));
                                        addWord(baseSem, mr, rels);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // on sauvegarde le résultat dans la base sémantique
            baseSem.writeBakuSemanticBase();
        }
    }
}


void addWord(BakuSemantic baseSem, string mot, vector<RelSem> rels){
    cout << "Ajout dans la base sémantique avec les relations suivantes : " << endl;
    // pour cout c'est ici avec les plus ou moins dans fichier
    for(long i=0; i<(long)rels.size(); ++i) {
        cout << "\t " + rels.at(i).name << endl;
        baseSem.addRel(mot, rels.at(i).name);
    }
}




