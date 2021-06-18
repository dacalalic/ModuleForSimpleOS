#include "list.h"
#include "pcb.h"

List::List() {
	head = tail = current = 0;
}

List::~List() {
	while (deleteFirst() != 0);
}

List::Elem::Elem(PCB* c_pcb, Elem* c_next) {
	pcb = c_pcb;
	next = c_next;
}

void List::insert(PCB* pcb) {
	if (head == 0) {
		head = tail = new Elem(pcb);
	}
	else {
		tail = tail->next = new Elem(pcb);
	}
}

PCB* List::deleteFirst(){
	if (head == 0) {
		return 0;
	}

	PCB* toBeReturned = head->pcb;

	Elem* toBeDeleted = head;
	head = head->next;

	delete toBeDeleted;

	if (head == 0) {
		tail = head;
	}

	return toBeReturned;
}

void List::deleteElemById(int id) {
	Elem* temp, *prev = 0;

	//Find pcb with this id
	for (temp = head; temp != 0; temp = temp->next) {
		if (temp->pcb->getId() == id) {
			break;
		}
		prev = temp;
	}

	//Return if there is no such element
	if (temp == 0) {
		return;
	}

	if (prev == 0) {
		head = temp->next;

		if (head == 0) {
			tail = 0;
		}
	}
	else {
		prev->next = temp->next;

		if (tail == temp) {
			tail = prev;
		}
	}

	delete temp;
}

PCB* List::getPCBById(int id) {
	for (Elem* temp = head; temp != 0; temp = temp->next) {
		if (temp->pcb->getId() == id) {
			return temp->pcb;
		}
	}
	return 0;
}

void List::resetPointer() {
	current = head;
}

PCB* List::getNext() {
	PCB* temp = 0;
	if (current != 0) {
		temp = current->pcb;
		current = current->next;
	}

	return temp;
}
