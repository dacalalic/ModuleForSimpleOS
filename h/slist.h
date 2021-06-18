#ifndef _slist_h_
#define _slist_h_

class KernelSem;

class SemList {
private:

	class Elem {
	public:
		KernelSem* sem;
		Elem* next;
		Elem(KernelSem* sem, Elem* next = 0);
	};

	Elem* head;
	Elem* tail;
	Elem* current;

public:

	SemList();
	~SemList();


	void insert(KernelSem* sem);
	KernelSem* deleteFirst();
	void deleteElemBySem(KernelSem* sem);

	void resetPointer();
	KernelSem* getNext();
};

#endif