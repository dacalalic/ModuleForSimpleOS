#include "kevent.h"
#include "ivte.h"
#include "ksem.h"
#include "sys.h"
#include "pcb.h"
#include "ivteli.h"

KernelEv::KernelEv(IVTNo ivtNo) {
	sem = new KernelSem(0);
	IVTEntry* num = IVTEntry::allEntries->getEntryByNo(ivtNo);
	num->connect(this);
	owner = (PCB*)PCB::running;
}

KernelEv::~KernelEv() {
	delete sem;
}

void KernelEv::wait() {
	if(PCB::running != owner) {
		return;
	}
#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif
	sem->wait(0);
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}

void KernelEv::signal() {
	if(sem->val() != 1) {
		sem->signal(0, 1);
	}
	dispatch_requested = 1;
	timer();
}
