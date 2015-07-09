#include "bakuSemanticLearn.h"


bool getMots(BakuSemantic *baseSem, string *mr, string *mr2){
    string mrTest = jdmExiste(*mr);
    *mr2 = *mr;
    if(mrTest == "") {
        // analyse syntaxique
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
            vector <RelSem> rels = baseSem->getRel(*mr);
            if(!rels.empty()){
                RelSem _max = rels[0];
                for(int i=1; i<rels.size(); i++){
                    if(_max.weight < rels[i].weight)
                        _max = rels[i];
                }

                addWord(baseSem, *mr, _max);
                return true;
            }

            return false;
        }
    }
    else{
        mr2 = &mrTest;
        mr = &mrTest;

        return false;
    }
}


void casSynAndLocution(BakuSemantic *baseSem, string mr, string mr2, relfind voisin, ofstream *fichier){
    cout << "Voisin trouvé pour " << voisin.rel << ": " << voisin.cible << endl;
    vector<RelSem> rels = baseSem->getRel(voisin.cible);
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
                addWord(baseSem, mr, r);
                addWord(baseSem, mr2, r);
            }else{
                addWord(baseSem, mr, r);
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


void casIsaANdHypo(BakuSemantic *baseSem, string mr, string mr2, vector<relfind> voisins ,relfind voisin, ofstream *fichier){
    cout << "Voisin trouvé pour " << voisin.rel << ": " << voisin.cible << endl;

    /*
    string m = mr.size()>voisin.cible.size() ? mr : voisin.cible;
    bool test = false;

    if(m == mr)
        test = mr.find(voisin.cible) != std::string::npos;
    else
        test = voisin.cible.find(mr) != std::string::npos;

    if(test){
        vector<RelSem> rels = baseSem->getRel(voisin.cible);
        addWord(baseSem, mr, rels);

        if(voisin.sens){
            *fichier << voisin.cible << " <-- " << voisin.rel << " (" << voisin.w << ") -- " << mr << endl;
        }else{
            *fichier << voisin.cible << " -- " << voisin.rel << " (" << voisin.w << ") --> " << mr << endl;
        }
        *fichier << "--------------------------------------------" << endl;
    }
    // OK
    else*/
    //{
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

                        vector<RelSem> rels = baseSem->getRel(jdmExiste(voisin.cible));

                        if(mr != mr2){
                            addWord(baseSem, mr, rels);
                            addWord(baseSem, mr2, rels);
                        }
                        else{
                            addWord(baseSem, mr, rels);
                        }

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
    //}

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


        if(!getMots(&baseSem, &mr, &mr2)) {

            // si mr 2 dans base alors ajout sinon continue

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
                            casSynAndLocution(&baseSem, mr, mr2, voisin, &fichier);
                        }
                        else if ((voisin.rel == "r_isa" && voisin.sens == 1)|| (voisin.rel == "r_hypo" && voisin.sens == 0) || (voisin.rel == "r_instance" && voisin.sens == 0))
                        {
                            casIsaANdHypo(&baseSem, mr, mr2, voisins , voisin, &fichier);
                        }
                    }
                }
            }
            cout << "---------------------------------------" << endl;
        }
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


void bakousemplay(ofstream* retour, BakuSemantic *baseSem, string m1, string mr, string m2){
    string sr = jdmExiste(mr);
    cout << mr << " to -> " << sr << endl;
    vector<RelSem> rels = baseSem->getRel(sr);

    cout << "Pour : " << m1 << " / " << sr << " / " << m2 << ", il existe : " << rels.size() << " relations"<< endl;

    if(!rels.empty()){
        // méthode de choix, pour l'instant le dernier plus grand poids
        RelSem rel = rels[0];
        cout << "OK" << endl;
        if(rels.size() != 1){
            for(int i=1; i<rels.size(); i++){
                if(rels[i].weight > rel.weight)
                    rel = rels[i];
            }
        }
        cout << m1 << " -- " << rel.name << " --> " << m2 << " | " << endl;
        // on écrit dans le fichier
        *retour << m1 << " -- " << rel.name << " --> " << m2 << " | " << endl;
    }
}


void bakuSemanticPlay() {

    ifstream fichier("./ressources/relationsMots.txt", ios::in);  // on ouvre le fichier en lecture
    ofstream retour("./ressources/relationTrouveSem.txt");
    BakuSemantic baseSem ;
    map<string, vector<RelSem>> t = baseSem.getBakuSemanticBase();

    cout << "début avec base de " << t.size() << " éléments" << endl;
    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        string word;
        int i=0;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            i=0;
            word = "";
            if(lireMot(&i, &ligne, &word, "|")){
                string m1 = word; word="";
                lireMot(&i, &ligne, &word, "|");
                string mr = word; word="";
                lireMot(&i, &ligne, &word, "|");
                string m2 = word;

                bakousemplay(&retour, &baseSem, m1, mr, m2);
            }
        }

        fichier.close();  // on referme le fichier
    }
    else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
}



void bakuSemanticPlayWD() {

    ifstream fichier("./ressources/relationsMotsWD.txt", ios::in);  // on ouvre le fichier en lecture
    ofstream retour("./ressources/relationTrouveSemWD.txt");
    BakuSemantic baseSem ;
    map<string, vector<RelSem>> t = baseSem.getBakuSemanticBase();

    cout << "début avec base de " << t.size() << " éléments" << endl;
    if(fichier)  // si l'ouverture a réussi
    {
        string ligne;
        string m1 ="", m2="", mr="";
        int i=0;
        while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
        {
            i=0;
            if(lireMot(&ligne,":") && !lireMot(&ligne, "(")) {
                m1 = "";
                lireMot(&i, &ligne, &m1, " :");
                cout << "Mot 1 :" << m1 << endl;
            }
            else {
                lireMot(&i, &ligne, &mr, " - ");
                lireMot(&i, &ligne, "( ");
                lireMot(&i, &ligne, &m2, " )");

                cout << "MotR : " << mr << ", Mot2 : " << m2 << endl;
            }


            if(m1 !="" && m2 != "" && mr != ""){
                bakousemplay(&retour, &baseSem, m1, mr, m2);
                m2 = ""; mr ="";
            }
        }

        fichier.close();  // on referme le fichier
    }
    else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
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
            if(lireMot(&ligne, "-")) {
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

