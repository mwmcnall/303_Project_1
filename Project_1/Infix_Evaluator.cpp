#include "Infix_Evaluator.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

const std::vector<std::string> Infix_Evaluator::OPERATORS = {
	//"!", 
	//"++", 
	//"--", 
	//"-", 
	//"^", 
	"*", "/", "%",
	"+", "-", 
	//">", ">=", "<", "<=", 
	//"==", "!=", 
	//"&&", 
	//"||" 
};
const std::vector<int> Infix_Evaluator::PRECEDENCE = {
	//8, 
	//8, 
	//8, 
	//8, 
	//7, 
	6, 6, 6, 
	5, 5, 
	//4, 4, 4, 4, 
	//3, 3, 
	//2, 
	//1
};

// Evaluates an infix expression
int Infix_Evaluator::eval(const std::string& expression) {
	// Be sure the stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;
	while (tokens >> next_char) {
		// Digit found
		if (isdigit(next_char)) {
			tokens.putback(next_char);
			int value;
			tokens >> value;
			operand_stack.push(value);
		}
		// is_operator only allows strings, this converts the character into a string
		// TODO: Will FAIL on any two-digit operator
		else if (is_operator(std::string(1, next_char))) {
			// Read in another element from the expression and store it as the rhs of the equation
			// TODO: eval_op will FAIL if it detects another operator in a row
			char op = next_char;
			tokens >> next_char;
			tokens.putback(next_char);
			int rhs;
			tokens >> rhs;
			int result = eval_op(std::string(1, op), rhs);
			operand_stack.push(result);
		}
		else {
			throw Syntax_Error("Invalid character encountered");
		}
	}
	if (!operand_stack.empty()) {
		int answer = operand_stack.top();
		operand_stack.pop();
		if (operand_stack.empty()) {
			return answer;
		}
		else
			throw Syntax_Error("Stack should be empty");
	}
	else
		throw Syntax_Error("Stack is empty");
}

// Evaluates the current operator
int Infix_Evaluator::eval_op(std::string op, int rhs) {
	// Assign lhs of equation
	if (operand_stack.empty())
		throw Syntax_Error("Stack is empty");
	int lhs = operand_stack.top();
	operand_stack.pop();

	int result = 0;
	// debug line: int temp = operator_position(op);
	// Gets index of operator in OPERATORS vector, this matches the number assigned in enum
	// which is why this switch case works
	switch (operator_position(op)) {
	case Plus: result = lhs + rhs;
		break;
	case Minus: result = lhs - rhs;
		break;
	case Multiply: result = lhs * rhs;
		break;
	case Divide: result = lhs / rhs;
		break;
	}

	return result;
}