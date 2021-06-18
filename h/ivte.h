#ifndef _ivte_h_
#define _ivte_h_

typedef void interrupt (*iRout)(...);

typedef unsigned char IVTNo;
class KernelEv;
class IVTEList;

class IVTEntry {
private:
	IVTNo num;
	KernelEv* kevent;
	iRout oldRout;

public:

	static IVTEList* allEntries;

	IVTEntry(IVTNo ivtNo, iRout oRout);
	~IVTEntry();

	void signal();
	void connect(KernelEv* ev);
	void callOldRoutine();

	IVTNo getIVTNo();

};

#define PREPAREENTRY(entryNo, callOld)\
	void interrupt inter##entryNo(...);\
	IVTEntry newEntry##entryNo(entryNo, inter##entryNo);\
	void interrupt inter##entryNo(...){\
		newEntry##entryNo.signal();\
		if(callOld == 1)\
			newEntry##entryNo.callOldRoutine();\
	}

#endif
