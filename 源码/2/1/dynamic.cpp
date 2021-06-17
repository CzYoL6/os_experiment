#include<iostream>
#include<vector>
#include<cstring>
#include<map>
using namespace std;

struct Item{
    int index;
    int szeInByte;
    int startAddr;
    bool operator < (int a) const{
        return startAddr < a;
    }
};
vector<Item> availTable;

map<int, Item> jobs;

const int MAXN = 128;
int memory[MAXN], memoryPrefixSum[MAXN];

void init(){
    availTable.clear();
    availTable.push_back(Item{1, MAXN, 0});
    memset(memory, 0, sizeof memory);
    memset(memoryPrefixSum, 0, sizeof memoryPrefixSum);
}


void updateAvailTable(){
    int curI = 1;
    bool suc = true;
    for(auto i = availTable.begin(); i != availTable.end(); i++){
        if(i->szeInByte == 0) availTable.erase(i);
        else{
            i->index = curI++;
            if(i!=availTable.begin()){
                if(i->startAddr == (i - 1)->startAddr+(i-1)->szeInByte){
                    int str = (i - 1)->startAddr, sze = (i-1)->szeInByte+i->szeInByte;
                    
                    *i = Item{0, sze, str};
                    availTable.erase(i - 1);
                   
                    suc = false;
                    break;
                }
            }
        }
    }
    if(!suc) updateAvailTable();
}

bool askForSpace(int index, int byte){
    for(auto i = availTable.begin(); i != availTable.end(); i++){
        if(i->szeInByte >= byte){
            i->szeInByte -= byte;
            
            jobs[index] = Item{index, byte, i->startAddr};

            memory[i->startAddr]++;
            memory[i->startAddr + byte]--;

            i->startAddr += byte;
            if(!i->szeInByte) updateAvailTable();
            
            return true;
        }
    }
    return false;
}


void displayAvailTable(){
    cout << "-----------------------------------------------"<<endl;
    cout << "index\t|\tsze\t|\tstart\t|\t"<<endl;
    for(auto i = availTable.begin(); i != availTable.end(); i++){
        cout << i->index << "\t|\t" << i->szeInByte<<"\t|\t"<<i->startAddr<<"\t|\t\n";
    }
    cout << "-----------------------------------------------"<<endl;
}

void displayMemory(){
    memoryPrefixSum[0] = memory[0];
    if(memoryPrefixSum[0]) cout << "#" ;
    else cout << "=";
    for(int i = 1; i < MAXN; i++){
        memoryPrefixSum[i] =  memory[i] + memoryPrefixSum[i - 1];
        if(memoryPrefixSum[i]) cout << "#" ;
        else cout << "=";
    }
    cout << endl;
}

void releaseMemory(int index){
    int start = jobs[index].startAddr;
    int sze = jobs[index].szeInByte;
    memory[start]--;
    memory[start + sze]++;
    auto pos = lower_bound(availTable.begin(), availTable.end(), start);
    availTable.insert(pos, Item{0, sze, start});
    updateAvailTable();
   // return true;
}

int n;
int main(){
    //freopen("h.in", "r", stdin);
    init();
    displayMemory();
    displayAvailTable();
    //return 0;
    cout << "how many opt?." << endl;
    cin >> n;
    for(int i = 1; i <= n; i++){
        cout <<endl<< "opt"<<i<<":"<<endl;
        char opt;
        cin >> opt;
        switch(opt){
            case 'A':{
                int sze;
                cin >> sze;
                if(askForSpace(i, sze)){
                    cout << "ok!";
                }
                else{
                    cout << "damn!";
                }
                break;
            }
            case 'R':{
                int index;
                cin >> index;
                releaseMemory(index);
                break;
            }
        }
        displayMemory();
        displayAvailTable();
    }
    return 0;
}