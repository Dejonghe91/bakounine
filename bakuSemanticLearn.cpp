#include "bakuSemanticLearn.h"


void bakouSemanticLearn(vector<string> words){

    BakuSemantic baseSem;
    ofstream fichier("test.txt", ios::out | ios::app);
    map<string, vector<RelSem>> base = baseSem.getBakuSemanticBase();

    if(base.empty()){
        base = baseSem.getBakuSemanticBase();
    }

    // parcours de tous les termes de la liste words
    for (long i=0; i<(long)words.size(); ++i)
    {
        cout << "Pour le mot : " << words.at(i) << endl;
        string mr = jdmExiste(words.at(i));
        string mr2 = "";

        if(mr == "") {
            vector<string> locution;
            string mot="";
            int dep=0;
            while(lireMot(&dep, &words.at(i),&mot," ")){
                locution.push_back(mot);
                mot = "";
            }

            if(locution.size() > 1){
                mr = jdmExiste(locution[0]);
                cout << "mot non trouvé : " << words.at(i) << ", remplaçant = " << locution[0] << endl;
                mr2 = words.at(i);
            }
        }
        else{
            mr2 = mr;
        }


        cout << "Traitement de : " << mr << endl;

        if(mr != "" && !baseSem.isRelExist(mr)) {
            vector<relfind> voisins = getNeightboors(mr);

            for (long j=0; j<(long)voisins.size(); ++j)
            {
                relfind voisin = voisins.at(j);

                if(baseSem.isRelExist(voisin.cible) )
                {
                    // voisin = tlist
                    if(voisin.rel == "r_syn" || voisin.rel == "r_locution" || voisin.rel == "r_syn_strict")
                    {
                        cout << "Voisin trouvé pour syn ou locution : " << voisin.cible << endl;
                        vector<RelSem> rels = baseSem.getRel(voisin.cible);
                        int pmt = voisin.w;
                        for(int k=0; k<rels.size(); k++){
                            int prt = rels[k].weight;
                            if(pmt*prt > 50){
                                RelSem r;
                                r.id = rels[k].id;
                                r.name = rels[k].name;
                                r.weight = pmt/2;
                                addWord(&baseSem, mr, r);


                                if(voisin.sens){
                                    fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
                                }else{
                                    fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
                                }
                                fichier << "pmt =" << pmt << ", prt = "<< prt << ", pmt*prt = " << pmt*prt << ", et pmt/2 = " << pmt/2 << endl;
                                fichier << "--------------------------------------------" << endl;
                            }
                        }
                    }
                   /* else if(voisin.rel == "r_locution")
                    {

                        // cas de la locution




                    }*/
                    else if ((voisin.rel == "r_isa" && voisin.sens == 1)|| (voisin.rel == "r_hypo" && voisin.sens == 0) || (voisin.rel == "r_instance" && voisin.sens == 0))
                    {
                        cout << "Voisin trouvé pour hypo ou isa : " << voisin.cible << endl;
                        if ( (voisin.rel == "r_isa" && voisin.sens == 1) && (mr.find(voisin.cible) != std::string::npos))
                        {
                            vector<RelSem> rels = baseSem.getRel(voisin.cible);
                            addWord(&baseSem, mr, rels);

                            if(voisin.sens){
                                fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
                            }else{
                                fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
                            }
                            fichier << "--------------------------------------------" << endl;
                        }
                        else if((voisin.rel == "r_hypo" && voisin.sens == 0) && (voisin.cible.find(mr) != std::string::npos))
                        {
                            vector<RelSem> rels = baseSem.getRel(voisin.cible);
                            addWord(&baseSem, mr, rels);

                            if(voisin.sens){
                                fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
                            }else{
                                fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
                            }
                            fichier << "--------------------------------------------" << endl;
                        }
                        else
                        {

                            vector<string> filtre;
                            filtre.push_back("r_isa");
                            filtre.push_back("r_hypo");
                            filtre.push_back("r_instance");

                            string tlist = voisin.cible;

                            vector<relfind> matchVoisins = getNeightboors(voisin.cible, filtre);


                            for(long k=0; k<(long)matchVoisins.size(); ++k) { // parcours des voisins de tlist
                                for(long l=0; l<(long) voisins.size(); ++l){ // parcours des voisins

                                    if(matchVoisins.at(k).cible == voisins.at(l).cible) {
                                        relfind vtlist_tlist = matchVoisins.at(k);
                                        relfind vtlist_mr = voisins.at(l);

                                        if( ( (vtlist_tlist.rel == "r_instance" && vtlist_tlist.sens == 1)||
                                            (vtlist_tlist.rel == "r_isa" && vtlist_tlist.sens == 0) ||
                                            (vtlist_tlist.rel == "r_hypo" && vtlist_tlist.sens == 1))
                                            &&
                                            ( (vtlist_mr.rel == "r_instance" && vtlist_mr.sens == 1)||
                                            (vtlist_mr.rel == "r_isa" && vtlist_mr.sens == 0) ||
                                            (vtlist_mr.rel == "r_hypo" && vtlist_mr.sens == 1)) ) {

                                            vector<RelSem> rels = baseSem.getRel(jdmExiste(voisin.cible));
                                            addWord(&baseSem, mr, rels);


                                            if(voisin.sens){
                                                fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
                                            }else{
                                                fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
                                            }
                                            fichier << vtlist_tlist.rel << " (" << vtlist_tlist.w << "/" << vtlist_tlist.sens << ")            " << vtlist_mr.rel << " (" << vtlist_mr.w << "/" << vtlist_mr.sens << ") " << endl;
                                            fichier << "               " << vtlist_mr.cible << endl;
                                            fichier << "--------------------------------------------" << endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        cout << "---------------------------------------" << endl;
    }
    fichier.close();
}


void addWord(BakuSemantic *baseSem, string mot, vector<RelSem> rels){
    cout << "Ajout dans la base sémantique avec les relations suivantes : " << endl;
    // pour cout c'est ici avec les plus ou moins dans fichier
    for(long i=0; i<(long)rels.size(); ++i) {
        cout << "\t " +mot +" -> "+ rels.at(i).name << ", "<< rels.at(i).weight << endl;
        baseSem->addRel(mot, rels.at(i));
    }
    baseSem->writeBakuSemanticBase();
    baseSem->getBakuSemanticBase();
}


void addWord(BakuSemantic *baseSem, string mot, RelSem rel){
    cout << "Ajout dans la base sémantique avec les relations suivantes : " << endl;
    // pour cout c'est ici avec les plus ou moins dans fichier
    cout << "\t " +mot +" -> "+ rel.name << ", "<< rel.weight << endl;
    baseSem->addRel(mot, rel);

    baseSem->writeBakuSemanticBase();
    baseSem->getBakuSemanticBase();
}

