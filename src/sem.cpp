#include "sem.h"
#include "ksem.h"

Semaphore::Semaphore(int init) {
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore() {
	delete myImpl;
}

int Semaphore::wait(Time time) {
	return myImpl->wait(time);
}

int Semaphore::signal(int n) {
	return myImpl->signal(n);
}

int Semaphore::val()const {
	return myImpl->val();
}
