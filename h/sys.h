/*
 * sys.h
 *
 *  Created on: Jun 27, 2020
 *      Author: OS1
 */

#ifndef SYS_H_
#define SYS_H_

//Use these without ";".
#define lock cs_forbidden = 1;
#define unlock cs_forbidden = 0;\
			   if(cs_requested) {\
				   dispatch();\
			   }

extern volatile int cs_forbidden;
extern volatile int cs_requested;
extern volatile int dispatch_requested;

void inic();

void restore();

void interrupt timer(...);

#endif /* SYS_H_ */
