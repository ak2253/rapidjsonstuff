#include <string>
#include <cmath>

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
