#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "otherFunctions.h" 
#include <string> 
#include <vector>     
#include <cstdio>
#include <iostream>
#include <fstream>
//#include "src/pke/include/scheme/allscheme.h"

#include "rapidjson/schema.h"
#include "rjHelper.h"

using namespace rapidjson;
void makeScheme(string scheme, string schemepath) { //TODO: change both parameters to vector to delete no ,longer implemented shcemes
    string filepath="allscheme.json";
    //TODO: make sure when testing to use your own paths
    string path="src/pke/include/";
    vector<const char*> vecP;
    vector<string>* params=getSchemeParameter(path+schemepath.c_str());
    vecP.push_back(schemepath.c_str());
    replaceScheme(filepath.c_str(),scheme.c_str(),vecP,params);
}
int main() {

    string token,line,schemename;
    bool foundq=false;
    //TODO: make sure when testing to use your own paths
    fstream filepath("src/pke/include/scheme/allscheme.h",fstream::in);
    if(!filepath) {
        cout<<"file allscheme.h not found"<<endl;
        return 0;
    }
    while (std::getline(filepath, line)) {
        if(line[0]=='#'&&line.substr(0,8).compare("#include")==0) {
            string nline=line.substr(9,line.size());
            for(char& ch : nline) {
                if(ch=='\"'&&foundq==false) {
                    foundq=true;
                    continue;
                }
                else if(ch=='\"'&&foundq==true) {
                    makeScheme(schemename.substr(0,schemename.size()-2),token);
                    token="";
                    schemename="";
                    foundq=false;
                    continue;
                }
                schemename+=ch;
                if(ch=='/') {
                    schemename="";
                }
                token+=ch;
            }
        }
    }
    string prefix="genCryptoContext";
	//TODO: find what we are comparing/checking
    return 0;
}


