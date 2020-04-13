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
    vector<const char*> vecP;
    vector<const char*> shParameters;
    vecP.push_back(schemepath.c_str());
    shParameters.push_back("To be decided 1");
    shParameters.push_back("To be decided 2");
    replaceScheme(filepath.c_str(),scheme.c_str(),vecP,shParameters);
}
int main() {

    string token,line,schemename;
    bool foundq=false;
    //TODO: make sure when testing to use your own allscheme.h path
    fstream filepath("/home/allen/palisade-release/src/pke/include/scheme/allscheme.h",fstream::in);
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

    /*
    addScheme(filepath.c_str(),schemename.c_str(),vecP,shParameters);
    deleteScheme(filepath.c_str(),schemename.c_str());
    */
    return 0;

}


