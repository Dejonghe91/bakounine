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
