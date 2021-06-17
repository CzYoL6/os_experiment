#include<iostream>
#include<vector>
#include<cstring>
#include<cmath>
#include<string>
#include<algorithm>
using namespace std;

const int zhumian = 8, cidao = 2, jilu = 4;
#define total (zhumian*cidao*jilu)


string intToBiStr(int x, int len){
    string ret = "";
    while(x){
        ret += x%2+'0';
        x/=2;
    }
    while(ret.length() < len) ret+="0";
    reverse(ret.begin(), ret.end());
    return ret;
}

namespace BITMAP{
    int bitmap[65];

    int biToInt(const char* x){
        int len = strlen(x);
        int ret = 0;
        for(int i = 0; i < len; i++){
            if(x[i] == '1') ret += pow(2, len - i - 1);
        }
        return ret;
    }

    int getBit(int x){
        return bitmap[x];
    }

    int getBit(const char* x){
        return bitmap[biToInt(x)];
    }

    void setBit(int x, bool f){
        bitmap[x] = f;
    }

    void setBit(char *s, bool f){
        bitmap[biToInt(s)] = f;
    }

    void displayBitMap(){
        cout<<"---------------------------------------------------------"<<endl;
        
        for(int i = 0; i < zhumian; i++){
            cout<<i<<": ";
            for(int j = 0; j < cidao*jilu; j++)
                cout<< getBit(i*cidao*jilu+j);
            cout<<endl;
        }
        cout<<"----------------------------------------------------------"<<endl;
    }
};

namespace DISK{
    struct add{
        int zm,cd,jl;
    };
    using ret = pair<bool,add>;
    ret askForSpace(){
        int now = -1;
        for(int i = 0; i < total; i++){
            if(!BITMAP::getBit(i)){
                now = i;
                break;
            }
        }
        if(now >= 0){
            BITMAP::setBit(now, 1);
            
            return make_pair(true, add{now/(cidao*jilu), (now%(cidao*jilu))/4, (now%(cidao*jilu))%4});
        }
        else {
            
            return make_pair(false, add{0,0,0});
        }
    }

    using ret2 = pair<bool, int>;
    ret2 releaseSpace(add a){
        int biAdd = a.zm*(cidao*jilu) + a.cd*jilu + a.jl;
        if(!BITMAP::getBit(biAdd)){
            return make_pair(false, 0);
        }
        else {
            BITMAP::setBit(biAdd, 0);
            return make_pair(true, biAdd);
        }
    }
}


int n;
int main(){
    //freopen("h.in", "r", stdin);
    //cout<<intToBiStr(4, 4)<<endl;
    BITMAP::displayBitMap();
   // displayAvailTable();
    //return 0;
    cout << "how many opt?." << endl;
    cin >> n;
    for(int i = 1; i <= n; i++){
        cout <<endl<< "opt"<<i<<":"<<endl;
        char opt;
        cin >> opt;
        switch(opt){
            case 'A':{
                DISK::ret ret = DISK::askForSpace();
                if(ret.first){
                    cout << "succeeded, zhumian:"<<ret.second.zm<<" cidao:"<<ret.second.cd<<" jilu:"<<ret.second.jl<<endl;
                }
                else cout << "failed, full." << endl;
                break;
            }
            case 'R':{
                int zm, cd, jl;
                cin>>zm>>cd>>jl;
                DISK::ret2 ret = DISK::releaseSpace(DISK::add{zm,cd,jl});
                if(ret.first){
                    cout << "succeeded, bit address:"<<intToBiStr(ret.second, 6)<<endl;
                }
                else cout<< "failed, it's available."<<endl;
                break;
            }
        }
        BITMAP::displayBitMap();
    }
    return 0;
}