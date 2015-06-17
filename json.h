#ifndef JSON_H
#define JSON_H

#include "rapidjson/document.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace rapidjson;
using namespace std;


static const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };




void recurtest(int decalage, Value& sousArbre);
string espace(int n);
void afficheA(int decalage, Value::ConstMemberIterator itr, Value& sousArbre);
void recurtest(int decalage, Value& sousArbre);
string getNom(int id);
string getNom(Document d);
int test(string json);

#endif
