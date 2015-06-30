#include "bakuSemanticLearn.h"


void getMots(string *mr, string *mr2){
    string mrTest = jdmExiste(*mr);
    *mr2 = *mr;
    if(mrTest == "") {
        vector<string> locution;
        string mot="";
        int dep=0;
        while(lireMot(&dep, mr, &mot," ")) {
            locution.push_back(mot);
            mot = "";
        }
        if(locution.size() >= 1){
            cout << "mot non trouvé : " << *mr << ", remplaçant = " << locution[0] << endl;
            *mr = jdmExiste(locution[0]);
        }
    }
    else{
        mr2 = &mrTest;
        mr = &mrTest;
    }
}


void casSynAndLocution(BakuSemantic baseSem, string mr, string mr2, relfind voisin, ofstream *fichier){
    cout << "Voisin trouvé pour " << voisin.rel << ": " << voisin.cible << endl;
    vector<RelSem> rels = baseSem.getRel(voisin.cible);
    int pmt = voisin.w;
    for(int k=0; k<rels.size(); k++)
    {
        int prt = rels[k].weight;
        cout << "pmt =" << pmt << ", prt = "<< prt << ", pmt*prt = " << pmt*prt << ", et prt/2 = " << (float)prt/(float)2 << endl;
        if(pmt*prt > 24){
            RelSem r;
            r.id = rels[k].id;
            r.name = rels[k].name;
            r.weight = (float)prt/(float)2;

            if(mr != mr2){
                addWord(&baseSem, mr, r);
                addWord(&baseSem, mr2, r);
            }else{
                addWord(&baseSem, mr, r);
            }

            if(voisin.sens){
                *fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
            }else{
                *fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
            }
            *fichier << "pmt =" << pmt << ", prt = "<< prt << ", pmt*prt = " << pmt*prt << ", et prt/2 = " << (float)prt/(float)2 << endl;
            *fichier << "--------------------------------------------" << endl;
        }
    }
}


void casIsaANdHypo(BakuSemantic baseSem, string mr, vector<relfind> voisins ,relfind voisin, ofstream *fichier){
    cout << "Voisin trouvé pour " << voisin.rel << ": " << voisin.cible << endl;


    string m = mr.size()>voisin.cible.size() ? mr : voisin.cible;
    bool test = false;

    if(m == mr)
        test = mr.find(voisin.cible) != std::string::npos;
    else
        test = voisin.cible.find(mr) != std::string::npos;

    if(test){
        vector<RelSem> rels = baseSem.getRel(voisin.cible);
        addWord(&baseSem, mr, rels);

        if(voisin.sens){
            *fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
        }else{
            *fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
        }
        *fichier << "--------------------------------------------" << endl;
    }
    // OK
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
                            *fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
                        }else{
                            *fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
                        }
                        *fichier << vtlist_tlist.rel << " (" << vtlist_tlist.w << "/" << vtlist_tlist.sens << ")            " << vtlist_mr.rel << " (" << vtlist_mr.w << "/" << vtlist_mr.sens << ") " << endl;
                        *fichier << "               " << vtlist_mr.cible << endl;
                        *fichier << "--------------------------------------------" << endl;
                    }
                }
            }
        }
    }

}


void bakouSemanticLearn(vector<string> words){

    BakuSemantic baseSem;
    ofstream fichier("./traces/test.txt", ios::out | ios::app);
    map<string, vector<RelSem>> base = baseSem.getBakuSemanticBase();

    if(base.empty()){
        base = baseSem.getBakuSemanticBase();
    }

    // parcours de tous les termes de la liste words
    for (long i=0; i<(long)words.size(); ++i)
    {
        string mr = words.at(i);
        string mr2 = "";

        getMots(&mr, &mr2);

        cout << "Traitement de : " << mr << endl;

        // si le mot n'est pas présent dans la base de connaissances
        if(mr != "" && !baseSem.isRelExist(mr)) {

            vector<relfind> voisins = getNeightboors(mr);

            // on regarde si un voisins de ce mot dans jeuxDeMot est présent dans la base de connaissances
            // par les relation syn, locution, isa et hypo
            for (long j=0; j<(long)voisins.size(); ++j)
            {
                relfind voisin = voisins.at(j); // voisin = tlist

                if(baseSem.isRelExist(voisin.cible) )
                {

                    if(voisin.rel == "r_syn" || voisin.rel == "r_syn_strict" || voisin.rel == "r_locution" )
                    {
                        casSynAndLocution(baseSem, mr, mr2, voisin, &fichier);
                    }
                    else if ((voisin.rel == "r_isa" && voisin.sens == 1)|| (voisin.rel == "r_hypo" && voisin.sens == 0) || (voisin.rel == "r_instance" && voisin.sens == 0))
                    {
                        casIsaANdHypo(baseSem, mr, voisins , voisin, &fichier);
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



void bakuSemanticLearnTest() {

    ifstream fichier("./ressources/relationsMots.txt", ios::in);  // on ouvre le fichier en lecture
    vector <string> mrs;

    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        string word;
        int i=0;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            i=0;
            word = "";
            if(lireMot(&i, &ligne, "|")){
                lireMot(&i, &ligne, &word, "|");
                mrs.push_back(word);
            }
        }

        fichier.close();  // on referme le fichier
    }
    else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }

    if(!mrs.empty()){
        bakouSemanticLearn(mrs);
    }
}


void bakuSemanticLearnTestWD() {

    ifstream fichier("./ressources/relationsMotsWD.txt", ios::in);  // on ouvre le fichier en lecture
    vector <string> mrs;

    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        string word;
        int i=0;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            i=0;
            word = "";
            if(lireMot(&ligne, "-")){
                lireMot(&i, &ligne, &word, " -");
                mrs.push_back(word);
            }
        }

        fichier.close();  // on referme le fichier
    }
    else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }

    if(!mrs.empty()){
        bakouSemanticLearn(mrs);
    }
}

void bakuSemanticLearnTestMed(){

    ifstream fichier("./ressources/mot_lionel.txt", ios::in);  // on ouvre le fichier en lecture
    vector <string> mrs;

    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            mrs.push_back(ligne);
        }

        fichier.close();  // on referme le fichier
    }
    else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }

    if(!mrs.empty()){
        bakouSemanticLearn(mrs);
    }

}

