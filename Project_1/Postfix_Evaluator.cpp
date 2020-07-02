#include "Postfix_Evaluator.h"
#include <iostream>
#include <sstream>
#include <cctype>

using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

const std::string Postfix_Evaluator::OPERATORS = "+-*/";

// Outputs stack to console
void Postfix_Evaluator::print_stack() {
	std::stack<int> temp_stack = operand_stack;
	std::cout << "--Stack" << std::endl;
	while (!temp_stack.empty()) {
		std::cout << "\t" << temp_stack.top() << std::endl;
		temp_stack.pop();
	}

	return;
}

// Outputs action to console
void Postfix_Evaluator::print_action(std::string action_type, int item) {
	std::cout << action_type << " " << item << std::endl;

	return;
}

// Outputs expression and place to console
void Postfix_Evaluator::print_expression(std::string curr_expression) {

	std::cout << "--Expression" << std::endl;
	std::cout << curr_expression << std::endl <<
		std::string(expression_follower, ' ') << "^" << std::endl;

	return;
}

// Evaluates a postfix expression
int Postfix_Evaluator::eval(const std::string& expression) {
	// Be sure the stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;
	while (tokens >> next_char) {
		if (isdigit(next_char)) {
			tokens.putback(next_char);
			int value;
			tokens >> value;
			print_expression(tokens.str());
			expression_follower += std::to_string(value).size() - 1;
			operand_stack.push(value);
			print_action("Push", value);
		}
		else if (is_operator(next_char)) {
			//expression_follower + 2;
			print_expression(tokens.str());
			int result = eval_op(next_char);
			operand_stack.push(result);
			//expression_follower += 2;
		}
		else {
			throw Syntax_Error("Invalid character encountered");
		}
		print_stack();
		expression_follower += 2;
		std::cout << std::endl;
	}
	if (!operand_stack.empty()) {
		int answer = operand_stack.top();
		operand_stack.pop();
		if (operand_stack.empty()) {
			print_action("Result:", answer);
			return answer;
		}
		else
			throw Syntax_Error("Stack should be empty");
	}
	else
		throw Syntax_Error("Stack is empty");
}

void Postfix_Evaluator::print_eval_op(int lhs, int rhs, char op, int result) {
	std::cout << "--Eval op" << std::endl;
	std::cout << "\t" << lhs << " " << op << " " << rhs << " = " << result << std::endl;

	return;
}

// Evaluates the current operator
// This function pops the two operands off the operand stack and applied the operator
int Postfix_Evaluator::eval_op(char op) {
	if (operand_stack.empty())
		throw Syntax_Error("Stack is empty");
	int rhs = operand_stack.top();
	operand_stack.pop();
	if (operand_stack.empty())
		throw Syntax_Error("Stack is empty");
	int lhs = operand_stack.top();
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

	print_eval_op(lhs, rhs, op, result);

	return result;
}