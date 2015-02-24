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
#include "stdio.h"

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

// Begin code changes by Conner Chaney

// Create structs for option 1 and option 2 for passing the addresses to the
// functions
struct threadStruct {
	int threadID;
	int shoutNum;
};
struct threadStruct2 {
	int size;
	char *arr;
};

// Create the strings the threads will shout
const char shout1[] = "War. War never changes.\n";
const char shout2[] = "I used to be an adventurer like you, then I took an arrow in the knee.\n";
const char shout3[] = "Do a barrel roll\n";
const char shout4[] = "Would you kindly...\n";
const char shout5[] = "Job's done.\n";
const char shout6[] = "Hey! Listen!\n";
const char shout7[] = "All your base are belong to us.\n";
const char shout8[] = "Fatality\n";
const char shout9[] = "Get Over Here!!!\n";

// Create external boolean variables that were set in the file system.cc
extern bool option1Flag, option2Flag, errorFlag;

// Boolean variable that makes one thread wait if both options are selected
// For example, if the command was ./nachos -A 1 -rs 234 -A 2, then one option's
// thread will yield until finishedFlag is set to true.
bool finishedFlag = false;

// Function that threads are forked to which makes them shout. The parameter
// numAndTimes is meant to hold the address of a threadStruct pointer.
void shoutOut(int numAndTimes) {
	// Loop until the thread has shouted the number of times stored in the
	// threadStruct's integer data member shouNum.
	for (int i = 0; i < ((threadStruct*) numAndTimes)->shoutNum; i++) {
		// Output the thread id stored in the threadStruct's member
		// variable threadID.
		printf("\nThread %d says ", ((threadStruct*) numAndTimes)->threadID);
		
		// Randomly select one of the 9 shouts by generating a random 
		// integer between 1 and 9 using a switch structure.
		int select = Random() % 9 + 1;
		switch (select) {
			case 1:
				printf("%s", shout1);
				break;
			case 2:
				printf("%s", shout2);
				break;
			case 3:
				printf("%s", shout3);
				break;
			case 4:
				printf("%s", shout4);
				break;
			case 5:
				printf("%s", shout5);
				break;
			case 6:
				printf("%s", shout6);
				break;
			case 7:
				printf("%s", shout7);
				break;
			case 8:
				printf("%s", shout8);
				break;
			case 9:
				printf("%s", shout9);
				break;
			default:
				printf("%s", "ERROR");
				break;
		}
		// Randomly generate an integer between 2 and 5.
		int k = Random() % 4 + 2;
		// Loop 2-5 times depending on the randomly generated number and
		// yield the thread each cycle.
		int l = 0;
		while (l < k) {
			currentThread->Yield();
			l++;
		}
	}
	// Delete the threadStruct pointer by casting the integer address.
	delete ((threadStruct*) numAndTimes);
	// Finish the current thread.
	currentThread->Finish();
}

// Function that accepts the number of threads to generate and the number of
// times each thread should shout from user input.  Then it forks the
// appropriate number of threads to the ShoutOut function. The parameter is a
// dummy variable that is not used, but only exists in order to fork a thread to
// this function.
void CreateShoutingThreads(int x) {
	while (!finishedFlag) {
		currentThread->Yield();
	}
	int threadNum;
	int shoutNum;
	threadStruct *ts;
	char ch;
	printf("\nIf a number is entered that is greater than 2147483647 then");
	printf(" a default amount will be created of 2147483647.\n");
	printf("\nEnter the number of Threads: ");
	while (scanf("%d", &threadNum) != 1 || threadNum < 0) {
		while ((ch = getchar()) != '\n') {
		}
		printf("\nPlease enter a non-negative integer: ");
	}
	if (threadNum == 2147483647)
		printf("\n\nCreating maximum representable amount of threads\n");
	printf("\nEnter the number of times each Thread should shout: ");
	while (scanf("%d", &shoutNum) !=1 || shoutNum < 0) {
		while ((ch = getchar()) != '\n') {
		}
		printf("\nPlease enter a non-negative integer: ");
	}
	if (shoutNum == 2147483647) {
		printf("\n\nCreating maximum amount of shouts possible\n");
	}
	printf("\nForking %d threads that will shout ", threadNum);
	printf("%d times each\n", shoutNum);
	printf("-------------------------------------------------------------");
	printf("-----------------------");
	for (int i = 0; i < threadNum; i++) {
		ts = new threadStruct;
		ts->threadID = (i + 1);
		ts->shoutNum = shoutNum;
		Thread *t = new Thread("shouting thread");
		t->Fork(shoutOut, (int) ts);
	}
	currentThread->Finish();
	
}

// Function used in input validation to detect if the user enters the empty
// string and output the appropriate message. The finishedFlag variable is set
// to true just in case the user selected to enter both options.
void EmptyStringDetect() {
	printf("\nEmpty character array detected.\n\n");
	finishedFlag = true;
}

// Function used in input validation to detect if the user enters a character
// array and output the appropriate message. The finishedFlag variable is set
// to true just in case the user selected to enter both options.
void CharacterDetected() {
	printf("\nThe input was a character string\n\n");
	finishedFlag = true;
}

// Funcion that is called if the first character entered by the user is '-'. It
// will output wheteher or not the input is a negative integer, a negative
// decimal, or a character string. The parameter is the address of a pointer to
// a threadStruct2 variable. threadStruct2 contains the size of the array input
// by the user and a deep copy of the chracter array itself.
void DetectNegative(int ts) {
	bool decCheck = false, charCheck = false, intCheck = true;
	int i;
	if (((threadStruct2*) ts)->arr[1] == '\0') {
		CharacterDetected();
		delete ((threadStruct2*) ts)->arr;
		((threadStruct2*) ts)->arr = NULL;
		delete (threadStruct2*) ts;
		currentThread->Finish();
	}
	
	for (i = 1; i < ((threadStruct2*) ts)->size; i++) {
		if ((((threadStruct2*) ts)->arr[i] >= '0' &&
		    ((threadStruct2*) ts)->arr[i] <= '9')
		    || ((threadStruct2*) ts)->arr[i] == '.') {
			if (((threadStruct2*) ts)->arr[i] == '.'){
				decCheck = true;
				intCheck = false;
				i++;
				break;
			}
			else
				continue;
		}
		else {
			charCheck = true;
			intCheck = false;
			CharacterDetected();
			break;
		}
	}
	if (decCheck == true && (((threadStruct2*)ts)->arr[i] >= '0'
	    && ((threadStruct2*)ts)->arr[i] <= '9')) {
		for (;i < ((threadStruct2*) ts)->size; i++) {
			if (((threadStruct2*) ts)->arr[i] >= '0' 
			&& ((threadStruct2*) ts)->arr[i] <= '9')
				continue;
			else {
				charCheck = true;
				CharacterDetected();
				break;
			}
		}
	}
	else if (intCheck == false && charCheck != true) {
		charCheck = true;
		CharacterDetected();
	}
	if (decCheck == false && charCheck != true) {
		printf("\nNegative integer detected.\n\n");
		finishedFlag = true;
	}
	else if (decCheck == true && charCheck != true) {
		printf("\nNegative decimal detected.\n\n");
		finishedFlag = true;
	}
	delete ((threadStruct2*) ts)->arr;
	((threadStruct2*) ts)->arr = NULL;
	delete (threadStruct2*) ts;
	currentThread->Finish();
}

// Funcion that is called if the first character entered by the user is a digit.
// It will output wheteher or not the input is an integer, a decimal, or a
// character string. The parameter is the address of a pointer to a
// threadStruct2 variable. threadStruct2 contains the size of the array input by
// the user and a deep copy of the chracter array itself.
void DetectIntegerOrDecimal(int ts) {
	bool decCheck = false, charCheck = false, intCheck = true;
	int i;

	for (i = 1; i < ((threadStruct2*) ts)->size; i++) {
		if ((((threadStruct2*) ts)->arr[i] >= '0' &&
		    ((threadStruct2*) ts)->arr[i] <= '9') ||
		    ((threadStruct2*) ts)->arr[i] == '.') {
			if (((threadStruct2*) ts)->arr[i] == '.'){
				decCheck = true;
				intCheck = false;
				i++;
				break;
			}
			else
				continue;
		}
		else {
			charCheck = true;
			intCheck = false;
			CharacterDetected();
			break;
		}
	}
	if (decCheck == true && (((threadStruct2*)ts)->arr[i] >= '0' &&
	    ((threadStruct2*)ts)->arr[i] <= '9')) {
		for (;i < ((threadStruct2*) ts)->size; i++) {
			if (((threadStruct2*) ts)->arr[i] >= '0' &&
			    ((threadStruct2*) ts)->arr[i] <= '9')
				continue;
			else {
				charCheck = true;
				CharacterDetected();
				break;
			}
		}
	}
	else if (intCheck == false && charCheck != true) {
		charCheck = true;
		CharacterDetected();
	}
	if (decCheck == false && charCheck != true) {
		printf("\nInteger detected.\n\n");
		finishedFlag = true;
	}
	else if (decCheck == true && charCheck != true) {
		printf("\nDecimal detected.\n\n");
		finishedFlag = true;
	}
	delete ((threadStruct2*) ts)->arr;
	((threadStruct2*) ts)->arr = NULL;
	delete (threadStruct2*) ts;
	currentThread->Finish();
}

// Function that accepts a string input by the user and stores it in a character
// array.  If the first character input by the user is '-' then it forks a
// thread to the DetectNegative function.  If the first character input by the
// user is a digit then it forks a thread to the DetectIntegerOrDecimal
// function.  If the user enters a character other than a digit or '-' then it
// simply calls the CharacterDetected function in order to produce the correct
// output. If the user does not enter any input then it call the
// EmptyStringDetected function in order to generate the proper output. The
// parameter is a dummy variable that is not used, but only exists in order to
// fork a thread to this function.
void CreateInputRecognition(int x) {
	printf("\nIn CreateInputRecognition\n");
	threadStruct2 *ts;
	int maxLength = 4;
	int curSize = 0;
	
	char *str = new char[maxLength];
	char *temp;
	curSize = maxLength;
	int currSize;
	int ch = EOF;
	int i = 0;
	
	printf("Enter a string: ");
	
	if (str != NULL) {
		while ((ch = getchar()) != '\n' && ch != EOF) {
			//if ((char) ch != ' ') { // Add this to ignore leading whitespace
				str[i++] = (char) ch;
				if (i == curSize) {
					currSize = curSize;
					curSize = i + maxLength;
					temp = new char[currSize];
					for (int j = 0; j < currSize; j++) {
						temp[j] = str[j];
					}
					delete str;
					str = NULL;
					str = new char[curSize];
					for (int j = 0; j < currSize; j++) {
						str[j] = temp[j];
					}
					delete temp;
					temp = NULL;
				
				}
			//} // Add this to ignore leading whitespace
		}
		
		str[i] = '\0';
		if (str[0] == '-') {
			ts = new threadStruct2;
			ts->size = i;
			ts->arr = new char[curSize];
			for (int k = 0; k < curSize; k++) {
				ts->arr[k] = str[k];
			}
			
			Thread *t = new Thread("shouting thread");
			t->Fork(DetectNegative, (int) ts);
		}
		else if (str[0] >= '0' && str[0] <= '9') {
			ts = new threadStruct2;
			ts->size = i;
			ts->arr = new char[curSize];
			for (int k = 0; k < curSize; k++) {
				ts->arr[k] = str[k];
			}
			
			Thread *t = new Thread("shouting thread");
			t->Fork(DetectIntegerOrDecimal, (int) ts);
		}
		else {
			if (str[0] == '\0')
				EmptyStringDetect();
			else
				CharacterDetected();
		}
	}
	delete str;
	str = NULL;
	currentThread->Finish();
}
// End code changes made by Conner Chaney


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    DEBUG('t', "Entering ThreadTest");
// Begin code changes by Conner Chaney
    // Execute the following code only if the errorFlag has not been set. If the
    // errorFlag variable has been set to true then output an error message and
    // exit gracefully.
    if (!errorFlag) {
        // If the command was entered twice with both options selected then fork
        // a thread to CreateShoutingThreads function and fork another thread to
        // CreateInputRecognition. In this case the CreateShoutingThreads thread
        // will yield until the CreateInputRecognition thread is finished and
        // then it will execute.
    	if (option1Flag == true && option2Flag == true) {
    		Thread *t = new Thread("forked thread1");

    		t->Fork(CreateShoutingThreads, 0);
    
    		Thread *t2 = new Thread("forked thread2");

    		t2->Fork(CreateInputRecognition, 0);
    	}
    	// If option 1 was selected fork a thread to the CreateInputRecognition
    	// function.
    	else if (option1Flag == true && option2Flag == false) {
    		Thread *t2 = new Thread("forked thread2");

    		t2->Fork(CreateInputRecognition, 0);
    	}
    	// If option 2 was selected fork a thread to the CreateShoutingThreads
    	// function.
    	else if (option1Flag == false && option2Flag == true) {
    		finishedFlag = true;
    		Thread *t = new Thread("forked thread1");

    		t->Fork(CreateShoutingThreads, 0);
    	}
    	// If neither option was selected do nothing.  This case would be a
    	// command like ./nachos or ./nachos -rs 1234
    	else {
    	
    	}
    }
    // Otherwise output an error message.
    else
    	printf("\nErroneous Command Line Argument!\n\n"); 
    
    // Finish the current thread.
    currentThread->Finish();
// End code changes by Conner Chaney

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

