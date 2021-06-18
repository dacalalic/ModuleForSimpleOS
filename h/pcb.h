#ifndef H_PCB
#define H_PCB

#include "thread.h"

class List;

#define UPPER_STACK_BOUND 65536 //Max 64KB
#define LOWER_STACK_BOUND 20 //Actually 12 is enough but let's make it 20.
class PCB {
private:

	static ID idGen;
	ID id;
	static List* allPCBs;

	List* blockedPCBs;

	Thread* myThread;
	unsigned int* stack;

	static void wrapper();

public:
	enum State { NEW, READY, BLOCKED, FINISHED };

	unsigned int ss;
	unsigned int bp;
	unsigned int sp;

	Time quant;
	State state;
	int isSliceless;

	int waitRet;
	Time waitTime;

	static volatile PCB* running;

	PCB(Time timeSlice);
	PCB(Thread* myThread, StackSize stackSize, Time timeSlice);
	~PCB();

	void start();
	void waitToComplete();

	ID getId();
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	static PCB* getPCBWithId(ID id);
};

#endif
