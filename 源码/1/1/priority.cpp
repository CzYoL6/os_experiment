#include<iostream>
#include<queue>
# include <sstream>
using namespace std;

enum PROCESS_STATUS{
    READY, ZOMBIE
};

struct PCB{
    bool operator<(const PCB& a) const{
        return priority < a.priority; //大顶堆
    }
    string processName;
    int priority;
   // PCB* nxtPCB;
    int demandTime;
    PROCESS_STATUS status;
}pool[5];

priority_queue<PCB> readyQueue;

void printReadyQueue(){
    priority_queue<PCB> readyQueueClone = readyQueue;
    printf("\nreadyQueue:\n");
    printf("ProcessName\tPriority\tdemandTime\t\n");
    printf("----------------------------------------------------\n");
    while(!readyQueueClone.empty()){
        PCB x = readyQueueClone.top();
        readyQueueClone.pop();
        printf("%s\t\t%d\t\t%d\t\n", x.processName.c_str(), x.priority, x.demandTime);
    }
    printf("\n");
}

void start(){
    while(!readyQueue.empty()){
        printReadyQueue();
        PCB x = readyQueue.top();
        readyQueue.pop();
        cout << "currently running: " << x.processName << endl;
        cout << "remaining demand time: " << --x.demandTime << endl;
        cout << "priority after running: " << --x.priority << endl;
        if(x.demandTime <= 0) x.status = ZOMBIE;
        else readyQueue.push(x);
        system("pause");
    }
}



int main(){
    //10 5freopen("h.in", "r", stdin);
    cout << "please input the priority and demand time of each process." << endl;
    for(int i = 0; i <= 4; i++){
        int pri, dt;
        cin >> pri >> dt;
        pool[i].priority = pri;
        pool[i].demandTime = dt;
        std::ostringstream oss;
        oss << "P " << i;
        std::string str = oss.str();
        pool[i].processName = str;

        pool[i].status = READY;
        cout<<pool[i].processName<<endl;
        readyQueue.push(pool[i]);

    }
    
    // while(!readyQueue.empty()){
    //     cout << readyQueue.top().processName << endl;
    //     readyQueue.pop();
    // }return 0;
    start();
    return 0;
}