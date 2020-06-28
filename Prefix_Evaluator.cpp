#include "Prefix_Evaluator.h"
#include <sstream>
#include <cctype>
using std::stack;
using std::string;
using std::stringstream;
using std::istringstream;
using std::isdigit;
using std::reverse;

const std::string Prefix_Evaluator::OPERATORS = "+-*/";

std::string Prefix_Evaluator::reverse(const std::string& expression) {
	string rev = "", element;
	
	stringstream ss(expression);
	while (ss >> element) {
		rev.insert(0, element);
		rev.insert(0, " ");
	}

	// Removes space at beginning of string
	rev.erase(rev.begin());

	return rev;
}

// Evaluates a postfix expression
int Prefix_Evaluator::eval(const std::string& expression) {
	// Be sure the stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();

	// Reverse expression
	std::string rev_expression = reverse(expression);

	// Process each token
	istringstream tokens(rev_expression);
	char next_char;
	while (tokens >> next_char) {
		if (isdigit(next_char)) {
			tokens.putback(next_char);
			int value;
			tokens >> value;
			operand_stack.push(value);
		}
		else if (is_operator(next_char)) {
			int result = eval_op(next_char);
			operand_stack.push(result);
		}
		else {
			throw Syntax_Error("Invalid character encountered");
		}
	}
	if (!operand_stack.empty()) {
		int answer = operand_stack.top();
		operand_stack.pop();
		if (operand_stack.empty())
			return answer;
		else
			throw Syntax_Error("Stack should be empty");
	}
	else
		throw Syntax_Error("Stack is empty");
}


// Evaluates the current operator
// This function pops the two operands off the operand stack and applied the operator
int Prefix_Evaluator::eval_op(char op) {
	if (operand_stack.empty())
		throw Syntax_Error("Stack is empty");
	int lhs = operand_stack.top();
	operand_stack.pop();
	if (operand_stack.empty())
		throw Syntax_Error("Stack is empty");
	int rhs = operand_stack.top();
	operand_stack.pop();

	int result = 0;
	switch (op) {
	case '+': result = lhs + rhs;
		break;
	case '-': result = lhs - rhs;
		break;
	case '*': result = lhs * rhs;
		break;
	case '/': result = lhs / rhs;
		break;
	}

	return result;
}