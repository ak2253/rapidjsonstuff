#include "otherFunctions.h"
#include <iostream>
#include <cmath>
using namespace std;

unsigned long long DHK(unsigned long long a, unsigned long long b, unsigned long long g, unsigned long long p) {
    cout<<"With g= "<<g<<" p= "<<p<<":"<<endl;
    unsigned long long alice=a;
    unsigned long long bob=b;
    cout<<"Alice has "<<alice<<endl;
    cout<<"Bob has "<<bob<<endl;
    if(alice>=g||bob>=g) {
        cout<<"Cannot proceed further, Alice or Bob has number greater than "<<g<<endl;
        return 0;
    }
    a=(fmodl(pow(p,alice),g));
    b=(fmodl(pow(p,bob),g));
    cout<<"Alice now sends "<<a<<" to Bob";
    cout<<" and Bob sends "<<b<<" to Alice"<<endl;
    
    unsigned long long key1=(fmodl(pow(b,alice),g));
    unsigned long long key2=(fmodl(pow(a,bob),g));
    if(key1==key2)
        cout<<"Success! Alice and Bob both have key: "<<key1<<" and "<<key2<<endl;
    else
        cout<<"Oh No! there seems to be a problem"<<endl;
    
    return key1;
}

unsigned long long RSA(unsigned long long p,unsigned long long q,unsigned long long m) {
    unsigned long long n=p*q;
    unsigned long long lowerkey=2;
    unsigned long long phi=(p-1)*(q-1);
    
    for(lowerkey;lowerkey<phi;lowerkey++) {
        if(gcd(lowerkey,phi)==1)
            break;
    }
    unsigned long long k=2;
    unsigned long long d=(1+(k*phi))/lowerkey;
    
    cout<<"Message data is: "<<m<<endl;
    unsigned long long puben=fmodl(pow(m,lowerkey),n);
    cout<<"Data has been encrpted into: "<<puben<<endl;
    unsigned long long prien=fmodl(pow(puben,d),n);
    cout<<"Decrpted data is now "<<prien<<endl;
    return prien;
}
