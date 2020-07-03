#include <iostream>
#include "Infix_Evaluator.h"
#include "Test_Evaluator.h"

using namespace std;

int main() {
	// The constructor calls all test cases
	// Go into Test_Evaluator.cpp to comment in / out test cases you wish to test
	// Test_Evaluator test_evals;

	Infix_Evaluator eval;
	// Or you can input custom test cases here!
	int result;
	/*
	result = eval.eval("1+2*3");
	cout << result << endl << endl;

	result = eval.eval("2+2^2*3");
	cout << result << endl << endl;
	
	result = eval.eval("1==2");
	cout << result << endl << endl;
	
	result = eval.eval("1+3 > 2");
	cout << result << endl << endl;

	result = eval.eval("(4>=4) && 0");
	cout << result << endl << endl;
	*/

	result = eval.eval("++++2-5*(3^2)");
	cout << result << endl << endl;

	// To prevent program from closing so you can read test output
	cin.get();
}