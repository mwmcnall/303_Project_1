#include <iostream>
#include "Infix_Evaluator.h"

using namespace std;

int main() {
	Infix_Evaluator eval;
	// Input test cases here!
	int result = eval.eval("10 % 5");

	cout << result;

	// To prevent program from closing so you can read test output
	cin.get();
}