// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

// *************************** dustin landry: start ***************************

// *************************** PROJECT 1 TASK 1 START ***************************

// global variable
const int INPUT_SIZE = 30;

/**
 * Function: promptCheck()
 * Description: Function to prompt the user for some input.
 */
char* promptCheck() {
	char input1[INPUT_SIZE];
	char *input = input1;

	// prompt user for input
	printf("Enter arbitrary input:");
	printf("\n");
	scanf("%s",input);
	
	return strdup(input);
}

/**
 * Function: display(char*,char)
 * Description: Function to display the input type given an input and the quantified type.
 */
void display(char *input, char type) {
	if (type == 'i') {
		printf("%s is an integer.",input);
	} else if (type == 'd') {
		printf("%s is a decimal.",input);
	} else if (type == 'n') {
		printf("%s is a negative.",input);
	} else if (type == 'c') {
		printf("%s is a character string.", input);
	} else if (type == 'b') {
		printf("%s is a negative decimal.", input);
	} else {
		printf("Invalid input.");
	}
	printf("\n\n");
}

/**
 * Function: identification(char*)
 * Description: function that accepts a char pointer as input and returns the input type.
 */
char identification(char* input) {
	// start identifying input
	char type = ' ';
	int count = 0;	
	int nCount = 0;	
	int dCount = 0;

	for (int i = 0; i < strlen(input); i++) { 
		if (*(input + i) == '0' || *(input + i) == '1' || *(input + i) == '2' || *(input + i) == '3' || *(input + i) == '4' || *(input + i) == '5' || *(input + i) == '6' || *(input + i) == '7' || *(input + i) == '8' || *(input + i) == '9' || *(input + i) == '-' || *(input + i) == '.') { 
			count++;

			if (*(input + i) == '.') {
				//count++;
				dCount++;
			}

			if (*(input + 0) == '-') {
				//count++;
				nCount++;
			}

			if (*(input + 0) == '.') {
				count = count + 10000;
			}
		} 
	}

	if (count == strlen(input)) {
		type = 'i';

		if (nCount > 0) 
			type = 'n';
		if (dCount > 0)
			type = 'd'; 
		if (*(input + strlen(input)-1) == '.') 
			type = 'c';
		if (nCount > 0 && dCount > 0)
			type = 'b';
		if (*(input + 1) == '.') 
			type = 'c';

	} else if (count > strlen(input)) {
		if (*(input + 0) == '-') {
			type = 'n';
		} else if (*(input + 0) == '.') {
			type = 'c';
		} else {
			type = 'd';
		}

		if (nCount > 0 && dCount > 0)
			type = 'b';
	} else {
		type = 'c';
	}

	return type;
}

/**
 * Function: tests()
 * Description: Function to test several different types of input. 
 */
void tests() {
	char *input = "1123214";
	char type = ' ';
	
	type = identification(input);
	display(input,type);

	input = "-124414";
	type = identification(input);
	display(input,type);

	input = "yo";
	type = identification(input);
	display(input,type);

	input = "yo124";
	type = identification(input);
	display(input,type);

	input = "12yo";
	type = identification(input);
	display(input,type);

	input = "-1232.123";
	type = identification(input);
	display(input,type);

	input = "1232.123";
	type = identification(input);
	display(input,type);

	input = "1232.123a";
	type = identification(input);
	display(input,type);

	input = "-1232.123a";
	type = identification(input);
	display(input,type);	

	input = "a1232.123";
	type = identification(input);
	display(input,type);

	input = "-a1232.123";
	type = identification(input);
	display(input,type);

	input = ".324";
	type = identification(input);
	display(input,type);

	input = "42334.";		
	type = identification(input);
	display(input,type);

	input = " 34fef";
	type = identification(input);
	display(input,type);

	input = "-.34324";		
	type = identification(input);
	display(input,type);

	input = " -1232.123";
	type = identification(input);
	display(input,type);

	printf("\n");	
}

/**
 * Function: InputThread(int)
 * Description: Called by ThreadTest. Prompts the user for input, identifies the input,
		and displays the input.
 */
void InputThread(int which) {
	currentThread->Yield();

	char *input = 0;
	char type = ' ';	
	
	input = promptCheck();
	type = identification(input);
	display(input,type);
	
	//tests();
}

// *************************** PROJECT 1 TASK 2 START ***************************

// global variables
char c[INPUT_SIZE];
char *numShouts = c;
const int SHOUT_COUNT = 10;

/**
 * Function: promptShout(int&,int&)
 * Description: Prompts the user to enter two integer values: one for the amount of
 *		threads, and the second for the amount of shouts. 
 */
void promptShout(char *&tNum, char *&sNum) {
	// prompt user for input
	printf("Enter total number of threads:");
	printf("\n");
	scanf("%s",tNum);

	printf("Enter total number of shouts:");
	printf("\n");
	scanf("%s",sNum);
}

/**
 * Function: validation(char*,char*)
 * Description: validates that the two values inputted by the user are both positive integer 			values.
 */
void validation(char *tNum, char *sNum, char inType) {
	char type1 = ' ';
	char type2 = ' ';

	type1 = identification(tNum); 
	type2 = identification(sNum); 
	
	if (type1 != inType || type2 != inType) {
		printf("Input doesnt match. Rerun program with positive integer values.\n\n");
		currentThread->Finish();
	} 
}

/**
 * Function: Shouts(char*[])
 * Description: Assigns 10 shouts to a char array of pointers.
 */
void Shouts(char *shout[]) {
	shout[0] = "Dustin";
	shout[1] = "Greg";
	shout[2] = "That";
	shout[3] = "Life";
	shout[4] = "Man";
	shout[5] = "Little";
	shout[6] = "Guys";
	shout[7] = "Best";
	shout[8] = "the";
	shout[9] = "Rest";
}

/**
 * Function: ShoutThread(int)
 * Description: Function that shouts random strings via which thread is called.
 */
void ShoutThread(int which) {
	char *shout[10];
	int randWait = 0;
	int randShout = 0;

	Shouts(shout);
	
	for (int i = 0; i < atoi(numShouts); i++) { 
		RandomInit(which); 	
		randShout = (Random() % SHOUT_COUNT);

		printf("Thread: %u, Shouting: %s, %i times.",which,shout[randShout],i+1); 
		printf("\n");

		randWait = ((Random() % 5) + 2);

		for (int j = 0; j < randWait; j++) {
			currentThread->Yield();
		}
	}
}

/**
 * Function: createThreads(int,void(*)(int))
 * Description: Creates the amount of threads equal to the tNum value entered by the user.
 */
void createThreads(int tNum, void (*funct)(int)) {			
	Thread *t[tNum];
	
	for (int i = 0; i < tNum; i++) {
		t[i] = new Thread((char*)i); 
	}	

	for (int i = 0; i < tNum; i++)
		t[i]->Fork((*funct),i); 

	currentThread->Finish();
}

/**
 * Function: ShoutStart()
 * Description: Called by ThreadTest(). Main function for Task 2. Prompts user for input,
 *		creates threads, and shouts the threads.
 */
void ShoutStart(int which) {
	char input1[INPUT_SIZE];
	char input2[INPUT_SIZE];
	char *threadNum = input1;
	char *shoutNum = input2;

	promptShout(threadNum,shoutNum);
	validation(threadNum,shoutNum,'i');
	numShouts = shoutNum;
	createThreads(atoi(threadNum),ShoutThread);
}

// *************************** PROJECT 2 TASK 1 START ***************************
int pNum = 0;

/**
 * Function: promptDP1(char*&,char*&)
 * Description: Prompts the user to enter two integer values: one for the amount of
 *		threads, and the second for the amount of shouts. 
 */
void promptDP1(char *&philNum, char *&mealNum) {
	// prompt user for input
	printf("Enter total number of philosophers:");
	printf("\n");
	scanf("%s",philNum);

	printf("Enter total number of meals to be eaten:");
	printf("\n");
	scanf("%s",mealNum);
}

/**
 * Function: sit(int)
 * Description: Implements the DP algorithm.
 */
void sit(int pNum) {
	printf("Philosopher %i is now sitting.",pNum);
	printf("\n");
}

/**
 * Function: pickupChopstick(int,char*)
 * Description: 
 */
void pickupChopstick(int pNum, char* which) {
	printf("Philosopher %i has picked up the %s chopstick.",pNum,which);
	printf("\n");
}

/**
 * Function: eat(int)
 * Description: 
 */
void eat(int pNum) {
	printf("Philosopher %i is now eating.",pNum);
	printf("\n");
}

/**
 * Function: prevention(int,char*)
 * Description: 
 */
void synchronization(int pNum, char* which) {
	int randWait = 0;
	
	RandomInit(pNum); 	
	randWait = ((Random() % 5) + 2);
	
	for (int j = 0; j < randWait; j++) {
		printf("Philiosopher %i is %s.",pNum,which);
		printf("\n");
	}
}

/**
 * Function: putdownChopstick(int,char*)
 * Description: 
 */
void putdownChopstick(int pNum, char* which) {
	printf("Philosopher %i has put down the %s chopstick.",pNum,which);
	printf("\n");
}

/**
 * Function: think(int)
 * Description: 
 */
void think(int pNum) {
	printf("Philosoper %i has started thinking.");
	printf("\n");
}

/**
 * Function: leave()
 * Description: Implements the DP algorithm.
 */
void leave() {

}

/**
 * Function: DiningPhilosophers(int)
 * Description: Implements the DP algorithm.
 */
void DiningPhilosophers(int which) {
	bool chopNum[pNum];
	if (true) {
		sit(which);
		pickupChopstick(which,"left");
		pickupChopstick(which,"right");
		eat(which);
		synchronization(which,"eating");
		putdownChopstick(which,"left");
		putdownChopstick(which,"right");
		think(which);
		synchronization(which,"thinking");
		currentThread->Yield();
	} else {
		leave();
	}
}

/**
 * Function: DiningStart(int)
 * Description:Called by ThreadTest(). Main function for Task 1. Prompts user for input,
 *		creates threads, and implements the DP algorithm.
 */
void DiningStart(int which) {
	char p[INPUT_SIZE];
	char m[INPUT_SIZE];
	char *philNum = p;
	char *mealNum = m;

	promptDP1(philNum,mealNum);
	validation(philNum,mealNum,'i');
	pNum = atoi(philNum);
	createThreads(atoi(philNum),DiningPhilosophers);
}

// *************************** PROJECT 2 TASK 2 START ***************************

// *************************** PROJECT 2 TASK 3 START ***************************

// *************************** PROJECT 2 TASK 4 START ***************************

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

/**
 * Function: ThreadTests()
 * Description: Main function of the threadtest.cc file. When called will create 
		and fork a new Thread.
 */
void ThreadTest()
{
    DEBUG('t', "Entering ThreadTest");
    Thread *t = new Thread("forked thread");

//changes by Gregory Ledet
    if (assignmentCommand == TRUE) { 
	if (*assignmentChoice == '1'){
		t->Fork(InputThread,0);
	} else if (*assignmentChoice == '2'){
		t->Fork(ShoutStart,0);
	} else if (*assignmentChoice == '3') {
		t->Fork(DiningStart,0);
    	} else {
		printf("Invalid Input!\n");
	}

    } 
	currentThread->Finish();
//end changes
}

// ************************ dustin landry: end **************************
