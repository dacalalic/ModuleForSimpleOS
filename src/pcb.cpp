#include "pcb.h"
#include "list.h"
#include "thread.h"
#include "schedule.h"
#include "sys.h"
#include <dos.h>

volatile PCB* PCB::running = 0;
int PCB::idGen = 0;
List* PCB::allPCBs = new List();

PCB::PCB(Time timeSlice){
	//Dummy initialization because otherwise eclipse reports warnings.
	ss = 0;
	sp = 0;
	bp = 0;
	waitRet = 0;
	waitTime = 0;
	myThread = 0;
	stack = 0;

	id = idGen++;
	quant = timeSlice;
	state = READY;
	blockedPCBs = new List();
	if(timeSlice == 0) {
		isSliceless = 1;
	} else {
		isSliceless = 0;
	}

	allPCBs->insert(this);
}

PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice) {
	//Dummy initialization because otherwise eclipse reports warnings.
	ss = 0;
	sp = 0;
	bp = 0;
	waitRet = 0;
	waitTime = 0;

	id = idGen++;
	quant = timeSlice;
	state = NEW;
	blockedPCBs = new List();

	if(timeSlice == 0) {
		isSliceless = 1;
	} else {
		isSliceless = 0;
	}

	this->myThread = myThread;

	//Check if stack size is OK.
	if (stackSize > UPPER_STACK_BOUND || stackSize < LOWER_STACK_BOUND) {

	}
	//Stack size is given in bytes and on 8086 unsigned is 2 bytes.
	stackSize = stackSize / 2;
	stack = new unsigned int[stackSize];

	stack[stackSize - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize - 2] = FP_SEG(wrapper);
	stack[stackSize - 3] = FP_OFF(wrapper);

	ss = FP_SEG(stack + stackSize - 12);
	sp = FP_OFF(stack + stackSize - 12);
	bp = FP_OFF(stack + stackSize - 12);
#endif

	allPCBs->insert(this);
}

PCB::~PCB() {
	allPCBs->deleteElemById(this->id);
	delete blockedPCBs;
	delete[] stack;
}

ID PCB::getId() {
	return id;
}

ID PCB::getRunningId() {
	return ((PCB*)PCB::running)->getId();
}

Thread* PCB::getThreadById(ID id) {

	PCB* pcb = allPCBs->getPCBById(id);
	if (pcb != 0) {
		return pcb->myThread;
	}

	return 0; 
}

void PCB::start() {
	if(state != NEW) return;

	lock
	state = READY;
	Scheduler::put(this);
	unlock
}

void PCB::waitToComplete() {
	lock

	if(PCB::running == this || state == NEW || state == FINISHED){
		unlock
		return;
	}

	PCB::running->state = BLOCKED;
	blockedPCBs->insert((PCB*)PCB::running);

	unlock
	dispatch();
}

void PCB::wrapper(){
	PCB::running->myThread->run();

	lock

	//Release all threads that are waiting for this thread.
	PCB* temp = PCB::running->blockedPCBs->deleteFirst();
	while(temp) {
		temp->state = READY;
		Scheduler::put(temp);

		temp = PCB::running->blockedPCBs->deleteFirst();
	}

	PCB::running->state = FINISHED;

	unlock
	dispatch();
}

PCB* PCB::getPCBWithId(ID id){
	return allPCBs->getPCBById(id);
}
