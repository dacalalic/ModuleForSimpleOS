#include "ivte.h"
#include "kevent.h"
#include "ivteli.h"
#include <dos.h>

IVTEList* IVTEntry::allEntries = new IVTEList();

IVTEntry::IVTEntry(IVTNo ivtNo, iRout newRout) {
	oldRout = 0;
	num = ivtNo;
	kevent = 0;
	allEntries->insert(this);

#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldRout = getvect(num);
	setvect(num, newRout);
	asm sti;
#endif
}

IVTEntry::~IVTEntry() {

	allEntries->deleteElemByIVTNo(num);

#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(num, oldRout);
	asm sti;
#endif
}

void IVTEntry::signal() {
	if (kevent == 0) {
		return;
	}
	kevent->signal();
}

void IVTEntry::connect(KernelEv* ev) {
	kevent = ev;
}

IVTNo IVTEntry::getIVTNo() {
	return num;
}

void IVTEntry::callOldRoutine(){
	oldRout();
}
