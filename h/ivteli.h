#ifndef _ivteli_h_
#define _ivteli_h_

class IVTEntry;

class IVTEList {
private:

	class Elem {
	public:
		IVTEntry* entry;
		Elem* next;
		Elem(IVTEntry* c_entry, Elem* c_next = 0);
	};

	Elem* head;
	Elem* tail;

public:

	IVTEList();
	~IVTEList();

	//Pushes an element to the end of the list
	void insert(IVTEntry* entry);

	//Returns a pointer to the first element of the list and deletes it from the list
	//Returns 0 if list is empty
	IVTEntry* deleteFirst();

	IVTEntry* getEntryByNo(unsigned char num);

	void deleteElemByIVTNo(unsigned char num);
};

#endif
