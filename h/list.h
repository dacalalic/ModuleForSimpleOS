#ifndef H_LIST
#define H_LIST

class PCB;

class List {
private:
	
	class Elem {
	public:
		PCB* pcb;
		Elem* next;
		Elem(PCB* c_pcb, Elem* c_next = 0);
	};

	Elem* head;
	Elem* tail;
	Elem* current;

public:

	List();
	~List();

	//Pushes an element to the end of the list
	void insert(PCB* pcb);

	//Returns a pointer to the first element of the list and deletes it from the list
	//Returns 0 if list is empty
	PCB* deleteFirst();

	//Deletes an element with this id
	//If list doesn't have this element then it doesn't do anything
	void deleteElemById(int id);

	//Returns pcb if exists, otherwise 0
	PCB* getPCBById(int id);

	//Sets current to head.
	void resetPointer();

	//Returns pcb pointed by current and sets current to next element.
	PCB* getNext();
};

#endif
