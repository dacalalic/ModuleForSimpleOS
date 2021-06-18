/*
 * sys.cpp
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */

#include "sys.h"
#include <iostream.h>
#include <dos.h>
#include "pcb.h"
#include "list.h"
#include "thread.h"
#include "SCHEDULE.h"
#include "ksem.h"

typedef void interrupt (*iRoutine)(...);
volatile int dispatch_requested = 0;
volatile int counter = 20;
volatile int cs_forbidden = 0;
volatile int cs_requested = 0;

Thread* idle;

void interrupt timer(...) {
	static unsigned tsp;
	static unsigned tss;
	static unsigned tbp;

	if (dispatch_requested || (!PCB::running->isSliceless && --counter == 0)) {
		if (!cs_forbidden) {
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			//If thread is ready to run and if it's not "idle" thread put it in scheduler.
			if (PCB::running->state == PCB::READY && ((PCB*)PCB::running)->getId() != 0) {
				Scheduler::put((PCB*) PCB::running);
			}
			PCB::running = (volatile PCB*) Scheduler::get();

			//If there are no ready threads set "idle" thread.
			if (!PCB::running) {
				PCB::running = (volatile PCB*)PCB::getPCBWithId(0);
			}

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			counter = PCB::running->quant;
#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
#endif
			cs_requested = 0;

		} else {
			//If context switch is forbidden remember that context switch was requested.
			cs_requested = 1;
		}
	}

	if (dispatch_requested) {
		dispatch_requested = 0;
	} else {
		KernelSem::semTick();
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
#endif
	}
}

class Idle: public Thread{
public:
	Idle(): Thread(defaultStackSize, 1) {}
	void run();
};

void Idle::run(){
	while(1){
		//lock
		//cout << "Idle" <<endl;
		//unlock

		//for(int i = 0; i < 20000; i++){
		//	for(int j = 0; j < 30000; j++);
		//}
	}
}

void inic() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;

	iRoutine iOld = getvect(0x08);
	setvect(0x60, iOld);
	setvect(0x08, timer);

	idle = new Idle();
	//Main pcb. It doesn't have thread.
	PCB::running = new PCB(20);
	asm sti;
#endif
}

void restore() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;

	iRoutine iOld = getvect(0x60);
	setvect(0x08, iOld);

	PCB* tempMain = PCB::getPCBWithId(1);

	delete tempMain;
	delete idle;

	asm sti;
#endif
}
