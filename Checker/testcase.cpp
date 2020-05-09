#include <string>
#include <iostream>
#include "Checker.h"

using namespace std;

int main() {
    //fail
    string str2="unsigned int m, const PlaintextModulus ptModulus";
    //fail
    string str3="shared_ptr<typename Element::Params> params, const PlaintextModulus &plaintextModulus, float distributionParameter, float assuranceMeasure, float securityLevel, usint relinWindow, float distributionParmStst, int depth = 1";
	//pass
    string str4="shared_ptr<typename Element::Params> params, const PlaintextModulus plaintextmodulus, usint relinWindow, float stDev, float stDevStSt, int depth = 1, int assuranceMeasure = 9, float securityLevel = 1.006";
	//json path names for reference
    string path1="stst.json";
    string path2="Validate.json";
    if(check(str4)) {
        cout<<"Nice!"<<endl;
    }
    else {
        cout<<"Oh No!"<<endl;
    }
}
