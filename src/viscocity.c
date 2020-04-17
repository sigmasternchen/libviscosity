#include "viscocity.h"

#include <stdio.h>
#include <string.h>

int failedTests = 0;
bool testOkay = true;

const char* currentTestName;
const char* currentTestFunction;

int totalTests = 0;


void _assert(bool b, const char* condition, const char* name, const char* file, int line, const char* function) {
	if (!b) {
		printf("\033[31mAssertion failed");
		if (name != NULL) {
		
			printf(": %s", name);
		}
		printf("\033[0m\n");
		if (condition != NULL) {
			printf("%s\n", condition);
		}
		if (strcmp(function, "__fn__") == 0)
				function = "lambda";
		
		printf("in %s:%d (%s)\n", file, line, function);
		
		testOkay = false;
	}
}

#define MAX_CALL_ASSERTIONS (10)

struct callAssertion {
	const char* name;
	int target;
	int current;
	enum callAssertionBehaviour behaviour;
	const char* file;
	int line;
	const char* function;
} callAssertions[MAX_CALL_ASSERTIONS];
int numberOfCallAssertions;

void _assertCall(int number, const char* name, enum callAssertionBehaviour behaviour, const char* file, int line, const char* function) {
	int i;
	for (i = 0; i < numberOfCallAssertions; i++) {
		if (strcmp(callAssertions[i].name, name) == 0)
			break;
	}
	if (i == numberOfCallAssertions) {
		numberOfCallAssertions++;
		callAssertions[i] = (struct callAssertion) {
			.name = name,
			.target = number,
			.current = 0,
			.behaviour = behaviour
		};
	}
	
	callAssertions[i].current++;
	callAssertions[i].file = file;
	callAssertions[i].line = line;
	callAssertions[i].function = function;
	
}

#define CALL_ASSERTION_CONDITION_BUFFER_SIZE (255)

void checkCallAssertions() {
	for (int i = 0; i < numberOfCallAssertions; i++) {
		struct callAssertion assertion = callAssertions[i];
		
		char buffer[CALL_ASSERTION_CONDITION_BUFFER_SIZE];
		
		switch(assertion.behaviour) {
			case MIN:
				snprintf(buffer, CALL_ASSERTION_CONDITION_BUFFER_SIZE, ">%d calls expected; got %d", assertion.target, assertion.current);
				_assert(assertion.current >= assertion.target, buffer, assertion.name, assertion.file, assertion.line, assertion.function);
				break;
			case MAX:
				snprintf(buffer, CALL_ASSERTION_CONDITION_BUFFER_SIZE, "<%d calls expected; got %d", assertion.target, assertion.current);
				_assert(assertion.current <= assertion.target, buffer, assertion.name, assertion.file, assertion.line, assertion.function);
				break;
			case EXACT:
				snprintf(buffer, CALL_ASSERTION_CONDITION_BUFFER_SIZE, "%d calls expected; got %d", assertion.target, assertion.current);
				_assert(assertion.current == assertion.target, buffer, assertion.name, assertion.file, assertion.line, assertion.function);
				break;
		}
	}
}

void testBegin(const char* name, const char* file, int line, const char* function) {
	testOkay = true;
	
	printf("=====================\n");
	printf("\033[33mTest %d", ++totalTests);
	if (name != NULL) {
		printf(": %s", name);
	}
	printf("\033[0m\n");
	printf("in %s:%d (%s)\n\n", file, line, function);
	
	currentTestName = name;
	currentTestFunction = function;
	numberOfCallAssertions = 0;
}

void testEnd() {
	checkCallAssertions();

	if (!testOkay) {
		failedTests++;
		
		printf("\n");
		printf("\033[31mTest failed\033[0m ");
		/*if (currentTestName != NULL) {
			 printf("%s ", currentTestName);
		}*/
		printf("(%s)\n\n", currentTestFunction);
	} else {
		printf("\033[32mTest succeeded\033[0m\n\n");
	}
}

int testsFinal() {
	printf("\n");
	if (failedTests == 0) {
		printf("\033[32mAll tests were successful.\033[0m\n");
	} else {
		printf("\033[31mSome tests failed.\033[0m\n");
	}
	printf("%d of %d tests failed.\n", failedTests, totalTests);
	
	return failedTests == 0 ? 0 : 1;
}

#define MAX_NUMBER_OF_TESTS (1<<15)

struct test {
	void (*testfunc)();
	const char* name;
	const char* file;
	const char* function;
	int line;
} testRegister[MAX_NUMBER_OF_TESTS];
int numberOfTests = 0;

void addTest(void (*testfunc)(), const char* name, const char* file, const char* function, int line) {
	testRegister[numberOfTests++] = (struct test) {
		.testfunc = testfunc,
		.name = name,
		.file = file,
		.function = function,
		line = line
	};
}

int doTests() {
	for (int i = 0; i < numberOfTests; i++) {
		struct test test = testRegister[i];
	
		testBegin(test.name, test.file, test.line, test.function);
		test.testfunc();
		testEnd();
	}

	return testsFinal();
}

