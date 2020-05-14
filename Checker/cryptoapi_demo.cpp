#include <string>
#include <iostream>
#include <vector>
#include "../include/Checker.h"
#include <unistd.h>


using namespace std;


string defaults(vector<string> params);
vector<string> sep_params(string temp_str);

int main(int argc, char** argv) {
 	if(argc<3){
 		cout<<"\nFirst argument as scheme, second argument as plaintext or encode\n"<<"Ex. $(functioncall) stst plaintext\n";
 		exit(1);
 	}
 	string scheme_ch=argv[1];
 	string plain_encode=argv[2];
 	
    string str4="shared_ptr<typename Element::Params> params, const PlaintextModulus plaintextmodulus, usint relinWindow, float stDev, float stDevStSt,int depth=1, int assuranceMeasure = 9, float securityLevel = 1.006";
    /*
	//json path names for reference
    //string path1="stst.json";
    //string path2="Validate.json";
    */
    
    cout<<"\n\nSubstituting Values if needed...\n";
    vector<string> param_vect=sep_params(str4);
	string def_str=defaults(param_vect);
    
    
    if (scheme_ch=="stst" && check(def_str)){
    
    	if(plain_encode=="plaintext"){  			
            cout<<"\nplaintext stst statement running\n";
           

        }
		else if(plain_encode=="encode"){  			

			cout<<"\nencoding stst statement running\n";
			
		}
		
		else
			cout<<"\nChoose either encode or plaintext\n";
    }
    else
		cout<<"\n Choose valid scheme to build\n";
    
    
    
    
    return 0;
}

vector<string> sep_params(string temp_str) {
	vector<string> vect;
	stringstream s_stream(temp_str);

	while (s_stream.good()) {
		string substr;
		getline(s_stream, substr, ',');
		vect.push_back(substr);
	}
	return vect;
}

string defaults(vector<string> params) {
	vector<string> default_vals{ "", "", "", "", "", "int depth = 1", "int assuranceMeasure = 9", "float securityLevel = 1.006" };
	int i;
	string string_params = "";

	for (i = 0; i < params.size(); i++) {
		if (params[i] == "") {
			params[i] = default_vals[i];
		}
		string_params.append(params[i]);
		if( i < params.size()-1){
			string_params.append(",");
		}
	}
	

	return string_params;
}
