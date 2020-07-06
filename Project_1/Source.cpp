#include <iostream>
#include "Infix_Evaluator.h"
#include "Test_Evaluator.h"
#include <string>

using namespace std;

int main() {
	// The constructor calls all test cases
	// Go into Test_Evaluator.cpp to comment in / out test cases you wish to test
	// Test_Evaluator test_evals;

	Infix_Evaluator eval;
	// Or you can input custom test cases here!
	int result;

	try {
		result = eval.eval("6+7+8");
	}

	catch (string myError) {
		cout << myError << endl;
		result = -1;
	}

	cout << result << endl << endl;

	// To prevent program from closing so you can read test output
	cin.get();
}