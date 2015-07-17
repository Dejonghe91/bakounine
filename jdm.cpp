#include "jdm.h"


const int seuil = 24;


string latin1(string UTF){
    string copieS;
    copieS+=UTF;
    char * sLatin = (char*) copieS.c_str();
    sLatin = Utf8ToLatin1String(sLatin);
    return sLatin;
}

bool existeRel(string mot1, string mot2, string rel){
    vector <string> lRel= jdmRel(mot1, mot2);
    for(int i=0; i<lRel.size(); i++){
        if(rel==lRel[i]){
            return true;
        }
    }
    return false;
}

int poidJDM(string s){
    string adresse = "http://www.jeuxdemots.org/rezo-xml.php?gotermsubmit=Chercher";
    string stub = "&gotermrel=";
    string finAdresse = "&output=onlyxml";
    string page = ouvrirPage(adresse+stub+s+finAdresse);
    int i =0;
    string spoids;
    lireMot(&i, &page, "<mot poids=\"");
    lireMot(&i, &page, &spoids, "\"");
    return atoi(spoids.c_str());

}

string jdmExiste2(string s){
    string url="http://www.jeuxdemots.org/autocompletion/autocompletion.php?completionarg=proposition&proposition=";
    url+=s;
    url = transformer(&url," ","%20");
    string result=ouvrirPage(url);
    int i=0;
    string candidat;
    lireMot(&i, &result, "[\"");
    string maxs="";
    int maxi=10;
    while(lireMot(&i, &result, &candidat, "\"")){
        if((candidat==s || candidat==latin1(s) )&& poidJDM(s)>50){
            return s;
        } else if( maxi<(poidJDM(candidat)/pow(LevenshteinDistance(s, candidat),3)) ){
            maxi=poidJDM(candidat)/pow(LevenshteinDistance(s, candidat),3);
            maxs=s;
        }
        candidat.clear();
        lireMot(&i, &result, "\"");
    }
    return maxs;
}

string jdmExiste(string s){
    string url="http://www.jeuxdemots.org/autocompletion/autocompletion.php?completionarg=proposition&proposition=";
    url+=s;
    url = transformer(&url," ","%20");
    string result=ouvrirPage(url);
    int i=0;
    string candidat;
    lireMot(&i, &result, "[\"");
    string maxs="";
    int maxi=10;
    while(lireMot(&i, &result, &candidat, "\"")){
        if( maxi<(poidJDM(candidat)/pow(LevenshteinDistance(s, candidat),3)) ){
            maxi=poidJDM(candidat)/pow(LevenshteinDistance(s, candidat),3);
            maxs=s;
        }
        candidat.clear();
        lireMot(&i, &result, "\"");
    }
    return maxs;
}


bool jdmEquivalent(string s){ //TRUE uniquement si le premier mot de l('autocomplétion est strictement identique.
    //cout<<" existe ?"<<s<<endl;
    int i=0;
    //CURL *curl;
    //curl = curl_easy_init();
    string adresse = "http://www.jeuxdemots.org/rezo-xml.php?gotermsubmit=Chercher";
    string stub = "&gotermrel=";
    string finAdresse = "&output=onlyxml";
    string result = ouvrirPage(adresse+stub+s+finAdresse);
    string r="n'existe pas";

    /*
    string url="http://www.jeuxdemots.org/autocompletion/autocompletion.php?completionarg=proposition&proposition=";
    url += curl_easy_escape(curl, s.c_str(), s.size());;
    //cout<<url<<endl;
    url = transformer(&url," ","%20");
    url = transformer(&url,"%20","+");
    url = transformer(&url,"%E2%80%99","%27");
//    cout<<"url : "<<url<<endl;
    //pause("url");
    string result=ouvrirPage(url);
//    cout<<result<<endl;
    //cout<<"le mot n'existe pas dans JDM!!!"<<endl;
    string r = "\"";
    r+=s;
    r+="\"";
    r=latin1(r);
    r=transformer(&r,"_", "\'");
    // cout<<"R: "<<r<<endl;
*/
    return !lireMot( &result, r);

}


vector <string> jdmRel(string mot1, string mot2){
    //mot1.resize(mot1.size()-1);
    mot1=transformer(&mot1," ","%20");
    mot2=transformer(&mot2," ","%20");

    //http://www.jeuxdemots.org/intern_lexical_signature.php?affichrel_submit=Afficher&term_1=Pulp%20fiction&term_2=film
    vector <string> retour;
    string adresse = "http://www.jeuxdemots.org/intern_lexical_signature.php?affichrel_submit=Afficher&term_1=";
    adresse+=latin1(mot1);
    adresse+="&term_2=";
    adresse+=latin1(mot2);
    //cout<<"adresse : "<<adresse<<endl;
    //pause("adresse");
    string page = ouvrirPage(adresse);
    string relTexte = decouperPage(&page,"div","relations");
    int it=0;
    string rel;
    string score;
    while(lireMot(&it, &relTexte, "<br>")){

        it++;
        lireMot(&it, &relTexte, " -- "); //mot1;
        it++;
        lireMot(&it, &relTexte, "_");
        lireMot(&it, &relTexte, &rel , " : ");
        lireMot(&it, &relTexte, &score , " -->");

        if(atoi(score.c_str())>20){
            retour.push_back(rel);
        }
        //cout<<score<<endl;
        //cout<<atoi(score.c_str())<<endl;
        //cout<<rel<<endl;
        //pause("score");
        score.clear();
        rel.clear();
        //pause("relation");
    }
    return retour;
    //string = decouperPage(&c,"div","relations",&it);
}

void afficheRelfind(relfind r){
    cout << "cible: " << r.cible << ", nom : " << r.rel << ", sens : " << r.sens << ", poids:" << r.w << endl;
}

vector<relfind> getNeightboors(string mot) {

    vector<relfind> retour;

    string adresse = "http://www.jeuxdemots.org/rezo-xml.php?gotermsubmit=Chercher";
    string stub = "&gotermrel=";
    string finAdresse = "&output=onlyxml";
    string word = jdmExiste(mot);   //mot orthographié jdm

    if(word != "") {
        string page = ouvrirPage(adresse+stub+latin1(word)+finAdresse);
        string ligne;
        string rel;
        string voisin;
        string poids;
        int i=0;
        int j=0;
        int w = 0;

        bool entrant = false;
        bool sortant = false;

        while(lireMot(&i, &page, &ligne, "<")) {
            j=0;
            if(lireMot(&ligne, "entrant>"))
                entrant = !entrant;

            if(lireMot(&ligne, "sortant>"))
                sortant = !sortant;


            if(lireMot(&j, &ligne, "type=\"")){

                lireMot(&j, &ligne, &rel, "\"");
                lireMot(&j, &ligne, "poids=\"");
                lireMot(&j, &ligne, &poids, "\"");
                w = atoi(poids.c_str());
                lireMot(&j, &ligne, ">");
                lireMot(&j, &ligne, &voisin, "<");

                // on ajoute à la structure
                if(entrant && w > seuil) {

                    if (rel == "r_hypo" || rel == "r_syn" || rel == "r_syn_strict" || rel == "r_locution"){
                        relfind r;
                        r.w = w;
                        r.rel = rel;
                        r.cible = voisin;
                        r.sens = !entrant;
                        if(rel == "r_instance"){
                            afficheRelfind(r);
                        }
                        retour.push_back(r);
                    }
                }
                else if (sortant && w > seuil) {
                    if (rel == "r_isa"  || rel == "r_instance" || rel == "r_syn" || rel == "r_syn_strict" || rel=="r_locution"){
                        relfind r;
                        r.w = w;
                        r.rel = rel;
                        r.sens = sortant;
                        r.cible = voisin;
                        retour.push_back(r);
                    }
                }

                // on rinit les variables
                rel = "";
                voisin = "";
                poids= "";
                w = 0;

            } // fin de la récupération d'une relation

            ligne = "";
        } // fin du parcours des lignes

    }

    return retour;
}


vector<relfind> getNeightboors(string mot, vector<string> relToFind){
    vector<relfind> retour;
    string adresse = "http://www.jeuxdemots.org/rezo-xml.php?gotermsubmit=Chercher";
    string stub = "&gotermrel=";
    string finAdresse = "&output=onlyxml";
    string word = jdmExiste(mot);   //mot orthographié jdm

    if(word != "") {
        string page = ouvrirPage(adresse+stub+latin1(word)+finAdresse);
        string ligne;
        string rel;
        string voisin;
        string poids;
        int i=0;
        int j=0;
        int w = 0;

        bool entrant = false;
        bool sortant = false;

        while(lireMot(&i, &page, &ligne, "<")) {
            j=0;
            if(lireMot(&ligne, "entrant>"))
                entrant = !entrant;

            if(lireMot(&ligne, "sortant>"))
                sortant = !sortant;


            if(lireMot(&j, &ligne, "type=\"")){

                lireMot(&j, &ligne, &rel, "\"");
                lireMot(&j, &ligne, "poids=\"");
                lireMot(&j, &ligne, &poids, "\"");
                w = atoi(poids.c_str());
                lireMot(&j, &ligne, ">");
                lireMot(&j, &ligne, &voisin, "<");


                vector<string>::iterator find_it;
                find_it = find (relToFind.begin(), relToFind.end(), rel);
                if (find_it != relToFind.end() && w > seuil)
                {
                    // on ajoute à la structure
                    relfind r;
                    r.w = w;
                    r.rel = rel;
                    r.cible = voisin;
                    r.sens = !entrant;
                    retour.push_back(r);
                }

                // on rinit les variables
                rel = "";
                voisin = "";
                poids= "";
                w = 0;

            } // fin de la récupération d'une relation

            ligne = "";
        } // fin du parcours des lignes
    }

    return retour;
}

map <string, string> relationJDM(){

    map <string, string> retour;
    retour["associated"]="0";
    retour["raff_sem"]="1";
    retour["raff_morpho"]="2";
    retour["domain"]="3";
    retour["pos"]="4";
    retour["syn"]="5";
    retour["isa"]="6";
    retour["anto"]="7";
    retour["hypo"]="8";
    retour["has_part"]="9";
    retour["holo"]="10";
    retour["locution"]="11";
    retour["flpot"]="12";
    retour["agent"]="13";
    retour["patient"]="14";
    retour["lieu"]="15";
    retour["instr"]="16";
    retour["carac"]="17";
    retour["data"]="18";
    retour["lemma"]="19";
    retour["magn"]="20";
    retour["antimagn"]="21";
    retour["familly"]="22";
    retour["carac-1"]="23";
    retour["agent-1"]="24";
    retour["instr-1"]="25";
    retour["patient-1"]="26";
    retour["domain-1"]="27";
    retour["lieu-1"]="28";
    retour["chunk_pred"]="29";
    retour["lieu_action"]="30";
    retour["action_lieu"]="31";
    retour["sentiment"]="32";
    retour["error"]="33";
    retour["maner"]="34";
    retour["meaning"]="35";
    retour["infopot"]="36";
    retour["telic_role"]="37";
    retour["agentif_role"]="38";
    retour["verbe-action"]="39";
    retour["action-verbe"]="40";
    retour["conseq"]="41";
    retour["causatif"]="42";
    retour["adj-verbe"]="43";
    retour["verbe-adj"]="44";
    retour["chunk_sujet"]="45";
    retour["chunk_objet"]="46";
    retour["chunk_loc"]="47";
    retour["chunk_instr"]="48";
    retour["time"]="49";
    retour["object>mater"]="50";
    retour["mater>object"]="51";
    retour["successeur-time"]="52";
    retour["make"]="53";
    retour["product_of"]="54";
    retour["against"]="55";
    retour["against-1"]="56";
    retour["implication"]="57";
    retour["quantificateur"]="58";
    retour["masc"]="59";
    retour["fem"]="60";
    retour["equiv"]="61";
    retour["maner-1"]="62";
    retour["agentive_implication"]="63";
    retour["instance"]="64";
    retour["verb_real"]="65";
    retour["chunk_head"]="66";
    retour["similar"]="67";
    retour["set>item"]="68";
    retour["item>set"]="69";
    retour["processus>agent"]="70";
    retour["variante"]="71";
    retour["syn_strict"]="72";
    retour["bigger_than"]="73";
    retour["smaller_than"]="74";
    retour["accomp"]="75";
    retour["der_morpho"]="99";
    retour["has_auteur"]="100";
    retour["has_personnage"]="101";
    retour["can_eat"]="102";
    retour["has_actors"]="103";
    retour["deplac_mode"]="104";
    retour["has_interpret"]="105";
    retour["color"]="106";
    retour["cible"]="107";
    retour["symptomes"]="108";
    retour["predecesseur-time"]="109";
    retour["diagnostique"]="110";
    retour["predecesseur-space"]="111";
    retour["successeur-space"]="112";
    retour["social_tie"]="113";
    retour["tributary"]="114";
    retour["sentiment-1"]="115";
    retour["beneficiaire"]="150";
    retour["descend_de"]="151";
    retour["aki"]="666";
    retour["wiki"]="777";
    retour["anotation_exception"]="997";
    retour["anotation"]="998";
    retour["inhib"]="999";
    retour["prev"]="1000";
    retour["succ"]="1001";
    retour["termgroup"]="1002";
    retour["learning_model"]="2001";

    return retour;
}



map <string, string> relationJDMTrue(){

    map <string, string> retour;
    retour["r_associated"]="0";
    retour["r_raff_sem"]="1";
    retour["r_raff_morpho"]="2";
    retour["r_domain"]="3";
    retour["r_pos"]="4";
    retour["r_syn"]="5";
    retour["r_isa"]="6";
    retour["r_anto"]="7";
    retour["r_hypo"]="8";
    retour["r_has_part"]="9";
    retour["r_holo"]="10";
    retour["r_locution"]="11";
    retour["r_flpot"]="12";
    retour["r_agent"]="13";
    retour["r_patient"]="14";
    retour["r_lieu"]="15";
    retour["r_instr"]="16";
    retour["r_carac"]="17";
    retour["r_data"]="18";
    retour["r_lemma"]="19";
    retour["r_magn"]="20";
    retour["r_antimagn"]="21";
    retour["r_familly"]="22";
    retour["r_carac-1"]="23";
    retour["r_agent-1"]="24";
    retour["r_instr-1"]="25";
    retour["r_patient-1"]="26";
    retour["r_domain-1"]="27";
    retour["r_lieu-1"]="28";
    retour["r_chunk_pred"]="29";
    retour["r_lieu_action"]="30";
    retour["r_action_lieu"]="31";
    retour["r_sentiment"]="32";
    retour["r_error"]="33";
    retour["r_maner"]="34";
    retour["r_meaning"]="35";
    retour["r_infopot"]="36";
    retour["r_telic_role"]="37";
    retour["r_agentif_role"]="38";
    retour["r_verbe-action"]="39";
    retour["r_action-verbe"]="40";
    retour["r_conseq"]="41";
    retour["r_causatif"]="42";
    retour["r_adj-verbe"]="43";
    retour["r_verbe-adj"]="44";
    retour["r_chunk_sujet"]="45";
    retour["r_chunk_objet"]="46";
    retour["r_chunk_loc"]="47";
    retour["r_chunk_instr"]="48";
    retour["r_time"]="49";
    retour["r_object>mater"]="50";
    retour["r_mater>object"]="51";
    retour["r_successeur-time"]="52";
    retour["r_make"]="53";
    retour["r_product_of"]="54";
    retour["r_against"]="55";
    retour["r_against-1"]="56";
    retour["r_implication"]="57";
    retour["r_quantificateur"]="58";
    retour["r_masc"]="59";
    retour["r_fem"]="60";
    retour["r_equiv"]="61";
    retour["r_maner-1"]="62";
    retour["r_agentive_implication"]="63";
    retour["r_instance"]="64";
    retour["r_verb_real"]="65";
    retour["r_chunk_head"]="66";
    retour["r_similar"]="67";
    retour["r_set>item"]="68";
    retour["r_item>set"]="69";
    retour["r_processus>agent"]="70";
    retour["r_variante"]="71";
    retour["r_syn_strict"]="72";
    retour["r_bigger_than"]="73";
    retour["r_smaller_than"]="74";
    retour["r_accomp"]="75";
    retour["r_der_morpho"]="99";
    retour["r_has_auteur"]="100";
    retour["r_has_personnage"]="101";
    retour["r_can_eat"]="102";
    retour["r_has_actors"]="103";
    retour["r_deplac_mode"]="104";
    retour["r_has_interpret"]="105";
    retour["r_color"]="106";
    retour["r_cible"]="107";
    retour["r_symptomes"]="108";
    retour["r_predecesseur-time"]="109";
    retour["r_diagnostique"]="110";
    retour["r_predecesseur-space"]="111";
    retour["r_successeur-space"]="112";
    retour["r_social_tie"]="113";
    retour["r_tributary"]="114";
    retour["r_sentiment-1"]="115";
    retour["r_beneficiaire"]="150";
    retour["r_descend_de"]="151";
    retour["r_aki"]="666";
    retour["r_wiki"]="777";
    retour["r_anotation_exception"]="997";
    retour["r_anotation"]="998";
    retour["r_inhib"]="999";
    retour["r_prev"]="1000";
    retour["r_succ"]="1001";
    retour["r_termgroup"]="1002";
    retour["r_learning_model"]="2001";

    return retour;
}




