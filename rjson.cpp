#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include <string>
#include <vector>
#include <cstdio>
#include <iostream>

using namespace rapidjson;
using namespace std;

void Merge(Value& doc,Value::AllocatorType& all) {
    if(doc.IsObject()) {
        for(Value::MemberIterator key=doc.MemberBegin();key!=doc.MemberEnd();key++) {
            if(!key->value.IsArray())
                key->value=Value(kArrayType).Move().PushBack(key->value,all);
        }   
        
        Value::MemberIterator key=doc.MemberBegin();
        while(key!=doc.MemberEnd()) {
            Value::MemberIterator tempkey=doc.FindMember(key->name);
            if(key!=tempkey) {
                tempkey->value.PushBack(key->value[0],all);
                key=doc.EraseMember(key);
            }
            else
                key++;
        }
        for(key=doc.MemberBegin();key!=doc.MemberEnd();key++) {
            if(key->value.Size()==1)
                key->value=key->value[0];
        }
    }
}
int main(int argc, char *argv[]) {
    if(argc<2) {
        cout<<"json file is required"<<endl;
        return 0;
    }
    if(argc>2) {
        cout<<"Too many arguments"<<endl;
        return 0;
    }
    //for manual testing
    //const char json[] = "{\"gcd\":\"unsigned long long a, unsigned long long b\",\"MysteryCrypto\":\"unsigned long long a, unsigned long long b, unsigned long long g, unsigned long long p\",\"MysteryCrypto\":\"unsigned long long p, unsigned long long q, unsigned long long m\"}";
    char buffer[1024];
    FILE *file=fopen(argv[1],"r");
    if(!file) {
        cout<<"file "<<argv[1]<<" is not found"<<endl;
        return 0;
    }
    FileReadStream frs(file,buffer,sizeof(buffer)); //find way to get file size
    Document document;
    document.ParseStream<0, UTF8<>, FileReadStream>(frs);

    Merge(document,document.GetAllocator());
    string line;
    string word;
    string name;
    vector<string> para;
    int step=0;
    getline(cin,line);
    for(char& c:line) {
        if(c=='('&&step==0) {
            name=word;
            word="";
            step+=1;
            continue;
        }
        if(c==','&&step==1) {
            para.push_back(word);
            word="";
            continue;
        }
        if(c==')'&&step==1) {
            para.push_back(word);
            step+=1;
        }   
        word+=c;
    }
    if(step!=2) {
        cout<<"Invalid input"<<endl;
        return 0;
    }
    for(Value::MemberIterator key=document.MemberBegin();key!=document.MemberEnd();key++) {
        if(key->name.GetString()==name) {
            if(key->value.IsArray()) {
                for(auto& val:key->value.GetArray()) {
                    string arg;
                    int index=0;
                    string temp=val.GetString();
                    for(int i=0;i<temp.size();i++) {
                        if(temp[i]==','&&arg==para[index]) {
                            arg="";
                            index+=1;
                            continue;
                        }
                        if(temp[i]==' ') {
                            arg="";
                            continue;
                        }
                        arg+=temp[i];
                    }
                    if(arg==para[index]&&(index+1)==para.size())
                        cout<<key->name.GetString()<<" : "<<val.GetString()<<endl;
                }
            }
            else {
                cout<<key->name.GetString()<<" : "<<key->value.GetString()<<endl;
            }
        }
    }
    //list out contents in documents
    /*for(Value::MemberIterator key=document.MemberBegin();key!=document.MemberEnd();key++) {
        if(key->value.IsArray()) {
            for(auto& val:key->value.GetArray())
                cout<<key->name.GetString()<<" : "<<val.GetString()<<endl;
            cout<<endl;
        }
        else
            cout<<key->name.GetString()<<" : "<<key->value.GetString()<<endl;
        
    }*/
    return 0;
}
