#include <iostream>
#include "Infix_Evaluator.h"
#include "Test_Evaluator.h"

using namespace std;

int main() {
	// The constructor calls all test cases
	// Go into Test_Evaluator.cpp to comment in / out test cases you wish to test
	Test_Evaluator test_evals;

	Infix_Evaluator eval;
	// Or you can input custom test cases here!
	int result = eval.eval("3&&&& 5");

	cout << result;

	// To prevent program from closing so you can read test output
	cin.get();
}