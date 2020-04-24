#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>

#include <cstdio>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include <sstream>
using namespace rapidjson;
using namespace std;
//TODO: make sure before editing the json file that the file contains at least {}
vector<string>* extractParams(string paramstr) {
	string token="";
	string datatype="";
    int index, whindex;
    string delimiter1="\n";
	string delimiter2=" ";
	vector<string>* param=new vector<string>;
	if(paramstr.compare("")==0) {
		return NULL;
	}
	if(paramstr.find(delimiter1)==std::string::npos) { //TODO: fix later
		token=nltrim(paramstr);
		while((whindex=token.find(delimiter2))!=std::string::npos) {
			if(token[whindex+delimiter2.length()]=='=')
				break;
			datatype+=paramstr.substr(0,whindex)+" ";
			token.erase(0,whindex+delimiter2.length());
			if(token[0]=='&') {
				token.erase(0,1);
				datatype+='&';
			}
			else if(token[0]=='*') {
				token.erase(0,1);
				datatype+='*';
			}
            paramstr.erase(0,whindex+delimiter1.length());
		}
		param->push_back(datatype.substr(0,(datatype.size()-1)));
		param->push_back(token);
	}
	else {
		while((index=paramstr.find(delimiter1))!=std::string::npos) {
			token=trim(paramstr.substr(0,index));
			while((whindex=token.find(delimiter2))!=std::string::npos) {
				if(token[whindex+delimiter2.length()]=='=')
					break;
				datatype+=token.substr(0,whindex);
				if(token[0]=='&') {
					token.erase(0,1);
					datatype+='&';
				}
				else if(token[0]=='*') {
					token.erase(0,1);
					datatype+='*';
				}
                token.erase(0,whindex+delimiter2.length());
			}
            param->push_back(datatype);
			param->push_back(token);
            datatype="";
            paramstr.erase(0,index+delimiter1.length());
		}
        token=nltrim(paramstr);
		while((whindex=token.find(delimiter2))!=std::string::npos) {
			if(token[whindex+delimiter2.length()]=='=')
				break;
			datatype+=paramstr.substr(0,whindex)+" ";
			token.erase(0,whindex+delimiter2.length());
			if(token[0]=='&') {
				token.erase(0,1);
				datatype+='&';
			}
			else if(token[0]=='*') {
				token.erase(0,1);
				datatype+='*';
			}
            paramstr.erase(0,whindex+delimiter1.length());
		}
		param->push_back(datatype.substr(0,(datatype.size()-1)));
		param->push_back(token);
	}
    cout<<paramstr<<endl;
	return param;
}
void addScheme(const char* fpath,const char* scheme,vector<const char*> vecP,vector<string>* parameters) {
    FILE* file=fopen(fpath, "r");
    if(!file) {
        std::cout<<"Error file "<<fpath<<" not found"<<endl;
        fclose(file);
    }
    else {
        char rBuffer[1024];
        FileReadStream is(file,rBuffer,sizeof(rBuffer));
        Document doc;
        doc.ParseStream(is);
        fclose(file);
        if(doc["properties"].HasMember(scheme)) {
            std::cout<<"Scheme "<<scheme<<" already exist."<<endl;
            return;
        }
        assert(doc.IsObject());
        Value schemename(scheme,doc.GetAllocator());
        Value schemepath(vecP[0],doc.GetAllocator());
        Value para(kArrayType);
        Value schema(kArrayType); //does nothing yet
        for(int i=0;i<parameters->size();i++) {
            //cout<<(*parameters)[i]<<endl;
            Value num(to_string(i).c_str(),doc.GetAllocator());
			vector<string>* parsedParams=extractParams(parameters->at(i));
			if(parsedParams==NULL) {
				Value parameter(kObjectType);
				parameter.AddMember("","No parameters",doc.GetAllocator());
                para.PushBack(parameter,doc.GetAllocator());
			}
            else if(parsedParams->at(1)=="os")
                continue;
			else {
                Value parameter(kObjectType);
				for(int j=0;j<parsedParams->size();j+=2) {
					Value datatype(parsedParams->at(j).c_str(),doc.GetAllocator());
					Value variable(parsedParams->at(j+1).c_str(),doc.GetAllocator());
                    parameter.AddMember(variable,datatype,doc.GetAllocator());
                }
                para.PushBack(parameter,doc.GetAllocator());
            }
        }
        Value property(kObjectType);
        property.AddMember("schemepath",schemepath,doc.GetAllocator()); //TODO: edit later when we know where to put library
        property.AddMember("requiredParameter",para,doc.GetAllocator());
        property.AddMember("schema",schema,doc.GetAllocator());
        property.AddMember("validation",false,doc.GetAllocator());
        doc["properties"].AddMember(schemename,property,doc.GetAllocator());
        FILE* file=fopen(fpath,"w");
        char wBuffer[1024];
        FileWriteStream os(file,wBuffer,sizeof(wBuffer));
        PrettyWriter<FileWriteStream> writer(os);
        doc.Accept(writer);
        fclose(file);
    }
}
void deleteScheme(const char* fpath,const char* scheme) {
    FILE* file=fopen(fpath, "r");
    if(!file) {
        std::cout<<"Error file "<<fpath<<" not found"<<endl;
        fclose(file);
    }
    else {
        char rBuffer[1024];
        FileReadStream is(file,rBuffer,sizeof(rBuffer));
        Document doc;
        doc.ParseStream(is);
        fclose(file);
        assert(doc.IsObject());
        Value schemename(scheme,doc.GetAllocator());
        //edit
        doc["properties"].RemoveMember(schemename);
        FILE* file=fopen(fpath,"w");
        char wBuffer[1024];
        FileWriteStream os(file,wBuffer,sizeof(wBuffer));
        PrettyWriter<FileWriteStream> writer(os);
        doc.Accept(writer);
        fclose(file);
    }
}
void replaceScheme(const char* fpath,const char* scheme,vector<const char*> vecP,vector<string>* parameters) {
    //laziness at its finest
    deleteScheme(fpath,scheme);
    addScheme(fpath,scheme,vecP,parameters);
}

vector<string>* getSchemeParameter(string path) {
    string token, line;
    char ch;
	fstream filepath(path,fstream::in);
	if(!filepath) {
		cout<<"Error file "<<path<<" not found"<<endl;
		return NULL;
        filepath.close();
	}
	vector<string>* vecParam=new vector<string>;
    while(getline(filepath,line)) {
        if(line.rfind('\t',0)==0) {
            line=tabtrim(line);
            if(line.substr(0,18).compare("LPCryptoParameters")==0) {
                int parthsisCount=0; 
                line=line.substr(line.find("(")+1,line.size());
                bool foundp=false;
                for(int i=0;i<line.size();i++) {
                    if(line[i]=='(')
                        parthsisCount+=1;
                    else if(line[i]==')') {
                        if(parthsisCount==0) {
                            vecParam->push_back(token);
                            foundp=true;
                            token="";
                            break;
                        }
                        parthsisCount-=1;
                    }
                    else if(line[i]==',') {
                        token+='\n';
                        continue;
                    }
                    token+=line[i];
                }
                if(foundp==false) {
                    while(filepath>>ch>>std::noskipws) {
                        if(ch=='(')
                            parthsisCount+=1;
                        else if(ch==')') {
                            if(parthsisCount==0) {
                                token=tabtrim(token);
                                vecParam->push_back(token);
                                token="";
                                break;
                            }
                            parthsisCount-=1;
                        }
                        else if(ch==',')
                            continue;
                        if(token.compare("/*")==0) {
                            token="";
                            continue;
                        }
                        token+=ch;
                    }
                }
		    }
        }
    }
    return vecParam;
}
