#ifndef _ksem_h_
#define _ksem_h_

typedef unsigned int Time;

class Semaphore;
class List;
class SemList;

class KernelSem {

private:

	static SemList* allSems;

	int value;
	List* blockedPCBs;

public:

	KernelSem(int init);
	~KernelSem();

	static void semTick();

	int signal(int n, int ev = 0);
	int wait(Time time);

	int val()const;

};

#endif
