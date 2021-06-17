#include<iostream>
#include<map>
using namespace std;
void a(){
    cout<<"11"<<endl;
}
int main(){
    typedef void (*b)();
    map<int, b> m;
    m[0] = a;
    (*m[0])();
}