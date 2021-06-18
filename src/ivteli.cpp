#include "ivteli.h"
#include "ivte.h"

IVTEList::IVTEList() {
	head = tail = 0;
}

IVTEList::~IVTEList() {
	while (deleteFirst() != 0);
}

IVTEList::Elem::Elem(IVTEntry* c_entry, Elem* c_next) {
	entry = c_entry;
	next = c_next;
}

void IVTEList::insert(IVTEntry* entry) {
	if (head == 0) {
		head = tail = new Elem(entry);
	}
	else {
		tail = tail->next = new Elem(entry);
	}
}

IVTEntry* IVTEList::deleteFirst() {
	if (head == 0) {
		return 0;
	}

	IVTEntry* toBeReturned = head->entry;

	Elem* toBeDeleted = head;
	head = head->next;

	delete toBeDeleted;

	if (head == 0) {
		tail = head;
	}

	return toBeReturned;
}

IVTEntry* IVTEList::getEntryByNo(unsigned char num) {
	for (Elem* temp = head; temp != 0; temp = temp->next) {
		if (temp->entry->getIVTNo() == num) {
			return temp->entry;
		}
	}

	return 0;
}

void IVTEList::deleteElemByIVTNo(unsigned char num) {
	Elem* temp, * prev = 0;

	//Find elem with this num
	for (temp = head; temp != 0; temp = temp->next) {
		if (temp->entry->getIVTNo() == num) {
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
