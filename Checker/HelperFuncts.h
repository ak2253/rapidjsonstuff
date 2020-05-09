#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

unsigned long long gcd(unsigned long long a, unsigned long long b) {
    unsigned long long temp;
    while(true) {
        temp=fmodl(a,b);
        if(temp==0)
            return b;
        a=b;
        b=temp;
    }
}

string trim(const string& s) {
    size_t start=s.find_first_not_of(' ');
    size_t end=s.find_last_not_of(' ');
    return s.substr(start,(end-start+1));
}
string tabtrim(string s) {
    s.erase(std::remove(s.begin(),s.end(),'\t'),s.end());
    return s;
}
string nltrim(string s) {
	s.erase(std::remove(s.begin(),s.end(),'\n'),s.end());
	return s;
}

string getVariable(string& s) {
    string delimiter=" ";
    size_t index=0;
    string parse;
    while((index=s.find(delimiter))!=std::string::npos) {
        parse=s.substr(0,index);
        s.erase(0,index+delimiter.length());
    }
    return s;
}
