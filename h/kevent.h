#ifndef _kevent_h_
#define _kevent_h_

#include "event.h"

class PCB;
class KernelSem;

class KernelEv {
private:
	PCB* owner;
	KernelSem* sem;

public:

	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();
};

#endif
