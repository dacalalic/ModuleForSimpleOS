#include "ksem.h"
#include "list.h"
#include "slist.h"
#include "sys.h"
#include "pcb.h"
#include "schedule.h"

SemList* KernelSem::allSems = new SemList();

KernelSem::KernelSem(int init) {
	value = init;
	blockedPCBs = new List();
	allSems->insert(this);
}

KernelSem::~KernelSem() {
	delete blockedPCBs;
	allSems->deleteElemBySem(this);
}

int KernelSem::signal(int n, int ev) {
	if(n < 0) {
		return n;
	}
	int freeCount = 0;

	if(!ev){
		lock
	}
	if(n == 0) {
		freeCount--;
	}
	while(freeCount < n) {
		PCB* temp = blockedPCBs->deleteFirst();
		if(temp) {
			temp->state = PCB::READY;
			temp->waitRet = 1;
			Scheduler::put(temp);
			freeCount++;
		}
		else {
			break;
		}
	}

	value += (n == 0 ? 1 : n);
	if(!ev){
		unlock
	}

	return (freeCount < 0 ? 0 : freeCount);
}

int KernelSem::wait(Time time) {
	lock
	if(--value >= 0){
		unlock
		return 1;
	}
	PCB::running->state = PCB::BLOCKED;
	PCB::running->waitTime = time;
	blockedPCBs->insert((PCB*)PCB::running);
	dispatch();
	unlock

	return PCB::running->waitRet;
}

int KernelSem::val() const {
	return value;
}

void KernelSem::semTick() {
	allSems->resetPointer();

	//Iterate through semaphore list.
	KernelSem* semTemp = allSems->getNext();
	while(semTemp != 0) {
		semTemp->blockedPCBs->resetPointer();
		//Iterate through list of blocked pcbs.
		PCB* pcbTemp = semTemp->blockedPCBs->getNext();

		while(pcbTemp != 0) {
			if(pcbTemp->waitTime != 0) {
				if(--pcbTemp->waitTime == 0) {
					semTemp->blockedPCBs->deleteElemById(pcbTemp->getId());
					pcbTemp->state = PCB::READY;
					pcbTemp->waitRet = 0;
					Scheduler::put(pcbTemp);
				}
			}
			pcbTemp = semTemp->blockedPCBs->getNext();
		}
		semTemp = allSems->getNext();
	}
}
