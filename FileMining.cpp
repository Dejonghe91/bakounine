#include "FileMining.h"


vector<string> getPhrases(string text){
    vector<string> retour;
	string::size_type stTemp = text.find('.');

	while(stTemp != string::npos)
	{
		retour.push_back(text.substr(0, stTemp));
		text = text.substr(stTemp + 1);
		stTemp = text.find('.');
	}

	retour.push_back(text);
	return retour;
}


vector<string> getBagOfTreGrammAndBiGramm(string phrase) {
    BakuSemantic base;
    base.getBakuSemanticBase();

    vector<string> vp;
	vector<string> res;
	string::size_type stTemp = phrase.find(' ');

	while(stTemp != string::npos)
	{
		vp.push_back(phrase.substr(0, stTemp));
		phrase = phrase.substr(stTemp + 1);
		stTemp = phrase.find(' ');
	}

	vp.push_back(phrase);

	// construction de l'unigramme et remplaçement mot baseSem
	for(int i=0; i<vp.size(); i++){
        if(base.isRelExist(vp[i]))
        {
            vector<RelSem> rels = base.getRel(vp[i]);
            RelSem rs;
            for(int j=0; j<rels.size(); j++){
                if(rels[j].weight > rs.weight)
                    rs = rels[j];
            }

            vp[i] = rs.name;
        }

        res.push_back(vp[i]);
        cout << vp[i] << endl;
    }


    // construction du bi-gramme
    for(int i=0; i<vp.size()-1; i++){
        string s = vp[i] + " " + vp[i+1];
        cout << s << endl;
        res.push_back(s);
    }


    return res;
}


PNode getGraphPhrase(string phrase){

}


void testGetPhrase(){

    string s = "Je vais à la plage. Il fait trés chaud, je boirai bien une bonne bière.";

    vector<string> res = getPhrases(s);

    for(int i=0; i< res.size(); i++){
        cout << res[i] << " |" << endl;
    }

    cout << "test sac de mot" << endl;

    s = "La maison fait partie du quartier.";
    vector<string> bag = getBagOfTreGrammAndBiGramm(s);
}



vector<string> getPhrase(){
    BakuSemantic base;
    map<string,vector<RelSem> > motSem = base.getBakuSemanticBase();
    vector<string> phrases;
    vector<string> result;
    string page, stub;
    string adresse = "fr.wikipedia.org/wiki/";
    ofstream trace("./traces/phrases.txt");

    for(int i=0; i<termesavisiter.size(); i++){
        stub = termesavisiter[i];
        page = ouvrirPageHttps(adresse+stub);

        if(page != ""){
            string texte = decouperPage(&page, "div", "mw-content-text");
            string phrase ="";

            string res = "", texteres ="";

            int i=0;
            // on supprimer les balises
            while(lireMot(&i, &texte, &res, "<")){
                texteres += res;
                lireMot(&i, &texte, ">");
                res.clear();
            }
            texte = texteres;
            texteres.clear();

            // on supprimer les éléments entre crochets
            i = 0;
            while(lireMot(&i, &texte, &res, "[")){
                texteres += res;
                lireMot(&i, &texte, "]");
                res.clear();
            }

            //on supprime les espaces insécables
            //texteres = transformer(&texteres, "&#160;","");
            trace << "------------------ TEXTE COMPLET ----------------" << endl;
            trace << texteres << endl;
            trace << "-------------------------------------------------" << endl;
            trace << "-------------- PHRASES EXTRAITES ----------------" << endl;

            // on récupère les phrases (attention)
            i=0;
            while(lireMot(&i, &texteres, &phrase, ".")){
                phrases.push_back(phrase);
                phrase.clear();
                i++;
            }

            map<string, vector<RelSem>>::iterator it;
            for(int j=0; j<phrases.size(); j++){
                for(it=motSem.begin(); it != motSem.end(); it++){
                    if(lireMot(&phrases[j], it->first)){
                        result.push_back(phrases[j]);
                        cout << " ------ " << endl;
                        cout << phrases[j] << endl;
                        cout << " ------ " << it->first << " --------- " << endl;
                        trace << " ------ " << endl;
                        trace << phrases[j] << endl;
                        trace << " ------ " << it->first << " --------- " << endl;
                    }
                }
            }
        }else{
            cout << "Page vide pour : " << stub << endl;
        }
        cout << "FIN FIN" << endl;
    }
    cout << "nombre de phrases récupérées : " << result.size() << " / " << phrases.size() << endl;
    return result;
}
