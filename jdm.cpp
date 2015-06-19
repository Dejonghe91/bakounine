#include "jdm.h"


const int seuil = 49;


string latin1(string UTF){
    string copieS;
    copieS+=UTF;
    char * sLatin = (char*) copieS.c_str();
    sLatin = Utf8ToLatin1String(sLatin);
    return sLatin;
}


//http://www.jeuxdemots.org/autocompletion/autocompletion.php?completionarg=proposition&proposition=saloperie
string jdmExiste(string s){ //false si on appelle la fonction la première fois, true si on l'a appelé en minuscule et qu'on la rappele en majuscule.
    cout<<" existe ?"<<s<<endl;
    string url="http://www.jeuxdemots.org/autocompletion/autocompletion.php?completionarg=proposition&proposition=";
    url+=s;
    cout<<url<<endl;
    url = transformer(&url," ","%20");
    cout<<"url : "<<url<<endl;
    //pause("url");
    string result=ouvrirPage(url);
    /*
    if(lireMot(&result,s) || lireMot(&result,latin1(s))){
        cout<<"le mot existe dans JDM!!!"<<endl;
        //pause("existe!");
        return s;
        //cout<<"le mot n'existe pas dans JDM!!!"<<endl;
    } else {
    */
        cout<<result<<endl;
        //cout<<"le mot n'existe pas dans JDM!!!"<<endl;
        int i=0;
        string candidat;
        lireMot(&i, &result, "[\"");
        lireChar(&i, &result, '"', &candidat);
        if(LevenshteinDistance(s, candidat)<4 || LevenshteinDistance(latin1(s), candidat)<4){
            return boost::locale::conv::to_utf<char>(candidat,"Latin1");;
        } else {
            return "";
        }
    //}
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
    cout<<"adresse : "<<adresse<<endl;
    //pause("adresse");
    string page = ouvrirPage(adresse);
    string relTexte = decouperPage(&page,"div","relations");
    int it=0;
    string rel;
    while(lireMot(&it, &relTexte, "<br>")){

        it++;
        lireMot(&it, &relTexte, " -- "); //mot1;
        it++;
        lireMot(&it, &relTexte, "_");
        lireMot(&it, &relTexte, &rel , " ");
        retour.push_back(rel);
        cout<<rel<<endl;
        rel.clear();
        //pause("relation");
    }
    return retour;
    //string = decouperPage(&c,"div","relations",&it);
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
                    if (rel == "r_hypo" || rel == "r_syn" || rel == "r_syn_strict"){
                        relfind r;
                        r.w = w;
                        r.rel = rel;
                        r.cible = voisin;
                        r.sens = !entrant;
                        retour.push_back(r);
                    }
                }
                else if (sortant && w > seuil) {
                    if (rel == "r_isa" || rel == "r_syn" || rel == "r_syn_strict"){
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
