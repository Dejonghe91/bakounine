#include "BakuSemantic.h"

BakuSemantic::BakuSemantic()
{
    //ctor
}

BakuSemantic::~BakuSemantic()
{
    //dtor
}


map<string, vector<RelSem> > getBakuSemanticBase() {

    if(this->base.empty()) {

        ifstream fichier(base_file, ios::in);  // on ouvre le fichier en lecture

        if(fichier)  // si l'ouverture a réussi
        {
            vector<string> VecStr;
            string ligne;
            while(getline(fichier, ligne))  // tant que l'on peut mettre la ligne dans "contenu"
            {
                int nb = Split(VecStr, ligne, '|');
                if(nb == 3){
                    RelSem r;
                    r.id = atoi(VecStr[1].c_str());
                    r.name = VecStr[2];
                    if(!base[VecStr[0]].empty()){
                        vector<RelSem> vect = base[VecStr[0]];
                        bool f = false;

                        for (long index=0; index<(long)vect.size(); ++index) {
                            if(vect.at(index).id == r.id){
                                f = true;
                            }
                        }

                        if(!f){
                            base[VecStr[0]].push_back(r);
                        }
                    }else{
                        vector<RelSem> vect(1,r);
                        base[VecStr[0]] = vect;
                    }
                    cout << "terme :"+  VecStr[0] +", "<< "id : " + VecStr[1] + " -> " << r.name << endl;
                }
            }

            return base;
        }
        else {
            cerr << "Impossible d'ouvrir le fichier !" << endl;
            return base;
        }
    }
    else{
        return base;
    }
}


void writeBakuSemanticBase() {
    ofstream fichier(this->base_file, ios::out | ios::trunc);

    if(fichier && !base.empty())  {
        map<string, vector<RelSem> >::iterator p;
        for(p = base.begin(); p != base.end(); p++) {
            vector<RelSem> val = p->second;
            for (long index=0; index<(long)val.size(); ++index) {
                fichier << p->first << "|" << val.at(index).id << "|" << val.at(index).name << endl;
            }
        }

        fichier.close();  // on referme le fichier
    }

    else {
         cerr << "Erreur à l'ouverture !" << endl;
    }
}


vector<RelSem> getRel(string terme){
    return base[terme];
}

bool isRelExist(string terme){
    return true;
}

bool addRel(string terme, int id, string relname){
    return true;
}

int Split(vector<string>& vecteur, string chaine, char separateur)
{
	vecteur.clear();

	string::size_type stTemp = chaine.find(separateur);

	while(stTemp != string::npos)
	{
		vecteur.push_back(chaine.substr(0, stTemp));
		chaine = chaine.substr(stTemp + 1);
		stTemp = chaine.find(separateur);
	}

	vecteur.push_back(chaine);

	return vecteur.size();
}

