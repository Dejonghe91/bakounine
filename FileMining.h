#ifndef FILEMINING_H
#define FILEMINING_H

#include <iostream>
#include <map>
#include <vector>
#include "BakuSemantic.h"

using namespace std;

struct PNode {
    string word;
    vector<PNode> next;
    int depth;
};

/**
 *  Permet de récupérer les phrases contenues dans un texte.
 */
vector<string> getPhrases(string text);

vector<string> getBagOfTreGrammAndBiGramm(string phrase);

/**
 *  Permet de la phrase sous forme de graphe
 */
PNode getGraphPhrase(string phrase);

vector<string> getPhrase();

/// TESTS ///
void testGetPhrase();

#endif
