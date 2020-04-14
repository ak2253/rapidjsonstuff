#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace rapidjson;
//TODO: make sure before editing the json file that the file contains at least {}
void addScheme(const char* fpath,const char* scheme,vector<const char*> vecP,vector<const char*> parameters) {
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
        if(doc.HasMember(scheme)) {
            std::cout<<"Scheme "<<scheme<<" already exist."<<endl;
            return;
        }
        assert(doc.IsObject());
        Value schemename(scheme,doc.GetAllocator());
        Value schemepath(vecP[0],doc.GetAllocator());
        Value para(kArrayType);
        for(int i=0;i<parameters.size();i++) {
            Value tempstr(parameters[i],doc.GetAllocator());
            para.PushBack(tempstr,doc.GetAllocator());
        }
        Value property(kObjectType);
        property.AddMember("schemepath",schemepath,doc.GetAllocator()); //TODO: edit later
        property.AddMember("parameter",para,doc.GetAllocator());
        property.AddMember("validation",false,doc.GetAllocator());
        doc.AddMember(schemename,property,doc.GetAllocator());
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
        doc.RemoveMember(schemename);
        FILE* file=fopen(fpath,"w");
        char wBuffer[1024];
        FileWriteStream os(file,wBuffer,sizeof(wBuffer));
        PrettyWriter<FileWriteStream> writer(os);
        doc.Accept(writer);
        fclose(file);
    }
}
void replaceScheme(const char* fpath,const char* scheme,vector<const char*> vecP,vector<const char*> parameters) {
    //laziness at its finest
    deleteScheme(fpath,scheme);
    addScheme(fpath,scheme,vecP,parameters);
}
