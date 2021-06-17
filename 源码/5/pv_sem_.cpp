#include<iostream>
#include<fcntl.h>
#include<string>
#include<cstdio>
#include<queue>
#include<vector>
#include<cstdlib>
#include<map>

enum PROCESS_STATUS{
    RUNNING, READY, WAITING, ZOMBIE
};

struct PCB{
    std::string name;
    PROCESS_STATUS status;
    std::string reasonForWaiting;
    int breakpoint;
    int pc;
};

int s1, s2;

std::vector<PCB*> readyQueue; 

char productPool[100];


PCB consumer, producer;
PCB *currentPCB;

namespace Sync{
    bool p(PCB &pcb, int &sem, std::string semName){
        sem--;
        if(sem < 0){
            pcb.status = WAITING;
            pcb.reasonForWaiting = semName;
            //pcb.reasonForWaiting.append(semName);
            printf("reason for waiting: %s\n", pcb.reasonForWaiting.c_str());
            pcb.breakpoint = pcb.pc;
            if(pcb.name == "consumer" && producer.status == ZOMBIE){
                currentPCB = nullptr;
                return false;
            }
            int randid = rand() % readyQueue.size();
            std::cout<<randid<<std::endl;
            currentPCB = readyQueue[randid];
            //std::cout<<randid<<" "<<currentPCB->name.c_str()<<" "<<currentPCB->pc<<" "<<std::endl;
            readyQueue.erase(readyQueue.begin() + randid);
            return false;

        }
        return true;
    }

    bool v(PCB &pcb, int &sem, std::string semName){
        sem++;
        if(sem <= 0){
            //release a process
            std::string reasonForWaiting = semName;
            //reasonForWaiting.append(semName);
           // std::cout<<"!!"<<reasonForWaiting<<std::endl;
           // std::cout<<consumer.reasonForWaiting<<std::endl;
           // std::cout<<producer.reasonForWaiting<<std::endl;
            if(consumer.reasonForWaiting == semName) {
                
                consumer.status = READY;
                consumer.reasonForWaiting = "";
                consumer.pc = consumer.breakpoint;
                readyQueue.push_back(&consumer);
                printf("consumer is waken up\n");
            }
            else if(producer.reasonForWaiting == semName){
                
                producer.status = READY;
                producer.reasonForWaiting = "";
                producer.pc = producer.breakpoint;
                readyQueue.push_back(&producer);
                printf("producer is waken up\n");
            }
            
            return false;
        }
        return true;
    }
}


namespace Producer{
   // PCB producer;
    int id = 0;
    int in = 0;
    char C;
    typedef void (*code)();
    std::map<int, code> PA;
    
    void ready(){
        producer.status = READY;
        
        readyQueue.push_back(&producer);
      //  std::cout<<"!!"<<producer.name<<std::endl;
      //  std::cout<<readyQueue.size()<<std::endl;
        // printf("\nreadyQueue!!!!!");
        // for(auto x : readyQueue){
        //     printf("%s ", x->name.c_str());
        // }
        int randid = rand() % readyQueue.size();
        std::cout<<randid<<std::endl;
        currentPCB = readyQueue[randid];
        //std::cout<<randid<<" "<<currentPCB->name.c_str()<<" "<<currentPCB->pc<<" "<<std::endl;
        readyQueue.erase(readyQueue.begin() + randid);
        printf("already put into ready queue\n");
    }

    void produce(){
        C = 'A' + (id++);
        printf("producing a product %c\n", C);
        ready();
    }
    void put(){
        productPool[in++] = C;
        in %= 10;
        printf("putting product %c into the pool\n", C);
        ready();
    }
    void ps1(){
        printf("producer is excuting p(s1) %d\n", s1 - 1);
        if(Sync::p(producer,s1, "s1")){
            ready();
        }else{
            printf("producer is waiting\n");
        }
    }
    void vs2(){
        printf("producer is excuting v(s2) %d\n", s2 + 1);
        if(Sync::v(producer,s2, "s2"))
            ready();
        else {
            //printf("producer is waken up\n");
        }
    }
    void goto0(){
        printf("producer is excuting goto(0)\n");
        producer.pc = 0;
        ready();
    }
}

namespace Consumer{
    int outId = 0;
    char C;
    typedef void (*code)();
    std::map<int, code> SA;

    void ready(){
        consumer.status = READY;
        readyQueue.push_back(&consumer);
        int randid = rand() % readyQueue.size();
        std::cout<<randid<<std::endl;
        currentPCB = readyQueue[randid];
        readyQueue.erase(readyQueue.begin() + randid);
        printf("already put into ready queue\n");
    }

    void get(){
        C = productPool[outId++];
        printf("consumer got a product %c from the pool\n", C);
        outId %= 10;
        ready();
    }
    void consume(){
        printf("consumer is consuming product %c\n", C);
        int ip = ((outId - 1) + 10) % 10;
        productPool[ip] = '0';
        ready();
    }
    void ps2(){
        printf("consumer is excuting p(s2) %d\n", s2 - 1);
        if(Sync::p(consumer,s2, "s2"))
            ready();
        else printf("consumer is waiting\n");
    }
    void vs1(){
        printf("consumer is excuting v(s1) %d\n", s1 + 1);
        if(Sync::v(consumer,s1, "s1"))
            ready();
        else{
            //printf("consumere is waken up\n");
        }
    }
    void goto0(){
        printf("consumer is excuting goto(0)\n");
        consumer.pc = 0;
        ready();
    }
}



void initConsumer(){
    consumer.name = "consumer";
    consumer.status = READY;
    consumer.reasonForWaiting = "";
    consumer.breakpoint = 0;
    consumer.pc = 0;
    Consumer::SA[0] = Consumer::ps2;
    Consumer::SA[1] = Consumer::get;
    Consumer::SA[2] = Consumer::vs1;
    Consumer::SA[3] = Consumer::consume;
    Consumer::SA[4] = Consumer::goto0;
}

void initProducer(){
    producer.name = "producer";
    producer.status = READY;
    producer.reasonForWaiting = "";
    producer.breakpoint = 0;
    producer.pc = 0;
    Producer::PA[0] = Producer::produce;
    Producer::PA[1] = Producer::ps1;
    Producer::PA[2] = Producer::put;
    Producer::PA[3] = Producer::vs2;
    Producer::PA[4] = Producer::goto0;
}

int main(){
    initConsumer();
    initProducer();

    

    s1 = 10;
    s2 = 0;

    srand(0);
    readyQueue.push_back(&consumer);
    currentPCB = &producer;
    std::cout<<"starting...\n"<<std::endl;
    while(currentPCB != nullptr){
        printf("-------------------------------------\n");
        printf("\n\npool: ");
        for(int i=0; i < Producer::in; i++){
            printf("%c ", productPool[i]);
        }
        printf("\nreadyQueue:");
        for(auto x : readyQueue){
            printf("%s ", x->name.c_str());
        }
        printf("\n");
        printf("currently running: %s, pc:%d\n", currentPCB->name.c_str(), currentPCB->pc);
        //char x; std::cin>>x;
        int curPC = (*currentPCB).pc++;
        //printf("currently running: %s, pc:%d\n", currentPCB->name.c_str(), currentPCB->pc);
        if(currentPCB->name == "producer"){
            //printf("currently running: %s, pc:%d\n", currentPCB->name.c_str(), currentPCB->pc);
 //           printf("currently running: %s, pc:%d\n", Producer::producer.name.c_str(), currentPCB->pc);
            (*Producer::PA[curPC])();
            //std::cout<<currentPCB->name<<std::endl;
            char opt;
            printf("stop the producer from running? y/n ");
            std::cin >> opt;
            if(opt == 'y' || opt == 'Y'){
                producer.status = ZOMBIE;
                currentPCB = &consumer;
                readyQueue.clear();
            }
            else if(opt == 'n' || opt == 'N'){
                continue;
            }
        }
        else if(currentPCB->name == "consumer"){
            (*Consumer::SA[curPC])();
            char opt;
            //printf("stop the producer from running? y/n ");
            std::cin >> opt;
        }
    }

}





