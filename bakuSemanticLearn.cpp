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
        string mr = words.at(i);
        if(!baseSem.isRelExist(mr)) {
            cout << "le mot "+ mr + "n'existe pas, on récupère les voisins dans jeux de mots" << endl;
            vector<relfind> voisins = getNeightboors(mr);
            cout << "Il y a " << voisins.size() << "voisins" << endl;

            for (long j=0; j<(long)voisins.size(); ++j)
            {
                relfind voisin = voisins.at(j);

                cout << "\t voisin : "+voisin.cible+" avec relation (e/s) " << voisin.sens << " nommée " + voisin.rel << endl;

                if( baseSem.isRelExist(voisin.cible) )
                {
                    if(voisin.rel == "syn" || voisin.rel == "syn_strict"){
                        vector<RelSem> rels = baseSem.getRel(voisin.cible);
                        addWord(baseSem, mr, rels);
                    }
                    else if (voisin.rel == "isa" || voisin.rel == "hypo"){

                        vector<string> filtre;
                        filtre.push_back("isa");
                        filtre.push_back("hypo");
                        filtre.push_back("instance");
                        vector<relfind> matchVoisins = getNeightboors(voisin.cible, filtre);

                        for(int k=0; k<matchVoisins.size(); k++) {
                            for(long l=0; l<(long)voisins.size(); ++l) {
                                if(matchVoisins.at(k).cible == voisins.at(l).cible){

                                    if(  (matchVoisins.at(k).rel == "isa" && matchVoisins.at(k).sens == 1) ||
                                          (matchVoisins.at(k).rel == "hypo" && matchVoisins.at(k).sens == 0) ||
                                          (matchVoisins.at(k).rel == "instance" && matchVoisins.at(k).sens == 0)
                                    )
                                    {
                                        vector<RelSem> rels = baseSem.getRel(voisin.cible);
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




