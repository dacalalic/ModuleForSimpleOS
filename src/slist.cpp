#include "slist.h"

SemList::Elem::Elem(KernelSem* sem, Elem* next) {
	this->sem = sem;
	this->next = next;
}

SemList::SemList() {
	head = tail = current = 0;
}

SemList::~SemList() {
	while (deleteFirst() != 0);
}

void SemList::insert(KernelSem* sem) {
	if (head == 0) {
		head = tail = new Elem(sem);
	}
	else {
		tail = tail->next = new Elem(sem);
	}
}

KernelSem* SemList::deleteFirst() {
	if (head == 0) {
		return 0;
	}

	KernelSem* toBeReturned = head->sem;

	Elem* toBeDeleted = head;
	head = head->next;

	delete toBeDeleted;

	if (head == 0) {
		tail = head;
	}

	return toBeReturned;
}

void SemList::deleteElemBySem(KernelSem* sem) {
	Elem* temp = head;
	Elem* prev = 0;

	while (temp != 0 && temp->sem != sem) {
		prev = temp;
		temp = temp->next;
	}

	if (temp == 0) {
		return;
	}

	if (prev == 0) {
		head = head->next;
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

void SemList::resetPointer() {
	current = head;
}

KernelSem* SemList::getNext() {
	KernelSem* temp = 0;
	if (current != 0) {
		temp = current->sem;
		current = current->next;
	}

	return temp;
}