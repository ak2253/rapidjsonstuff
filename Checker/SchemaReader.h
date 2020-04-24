#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "otherFunctions.h" 
#include <string> 

void makeSchemajson(const char* path) {
    FILE* filepath=fopen(path,"r");
    if(!filepath) {
        fclose(filepath);
    }
}