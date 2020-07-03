#include "Infix_Evaluator.h"
#include <iostream>
#include <string>
#include <map>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

const char POSSIBLE_DOUBLE_OP[] = { '!', '+', '-', '>', '<', '=', '&', '|'};

const std::vector<std::string> Infix_Evaluator::OPERATORS = {
	"!", 
	"++", 
	"--", 
	"-",
	"^", 
	"*", "/", "%",
	"+", "-", 
	">", ">=", "<", "<=", 
	"==", "!=", 
	"&&", 
	"||",
	"&", "|", "="
};

// Negative / minus is a special case, not in map
std::map<std::string, int> precedence_map = {
	{"!", 8},
	{"++", 8},
	{"--", 8},
	{"^", 7},
	{"*", 6},
	{"/", 6},
	{"%", 6},
	{"+", 5},
	{"-", 5},
	{">", 4},
	{">=", 4},
	{"<", 4},
	{"<=", 4},
	{"==", 3},
	{"!=", 3},
	{"&&", 2},
	{"||", 1},
	{"(", -1},
	{")", -1},
	{"[", -1},
	{"]", -1},
	{"{", -1},
	{"}", -1}
};

/*
int eval(string expression) { <- wrapper function
	error checking happens here
	return expression_evaluator(string expression, int stack operand_stack, string stack operator_stack)
}

int eval_op(string op, int rhs) {
	Gets lhs from the operand stack, does the math and returns the int
}
*/

// Evaluates an infix expression
int Infix_Evaluator::eval(const std::string& expression) {

	// Check for possible errors before you even evaluate
	try
	{
		error_checker.error_checking(expression);
	}
	catch (const std::exception & e)
	{
		// Output error message and return from function
		// Does NOT evaluate expression
		std::cout << e.what() << std::endl;
		return -1;
	}

	std::stack<int> operand_stack;
	std::stack<std::string> operator_stack;
	istringstream tokens(expression);

	// Remove all spaces
	std::string no_space_expression = expression;
	no_space_expression.erase(std::remove(no_space_expression.begin(), no_space_expression.end(), ' ' ), 
		no_space_expression.end());

	return expression_evaluator(tokens, no_space_expression, operand_stack, operator_stack);
}

/*
int expression_evaluator(string expression, int stack operand_stack, string stack operator_stack) {
	int last_precendence, to keep track of precedence of last operator pushed
	function that loops through the string character by character
		- Adds operands and operators to their respective stack
	operator stack:
		- if both stacks are NOT empty
			- if precedence is 8
				- operand_stack.push(eval_op(string op, int rhs))
			- else
				- if operand stand has two
		- When adding to operator stack, if precedence is 8 OR greater than last precedence
			- operand_stack.push(expression_evaluator(the rest of the expression that's left));
		- else, precedence is lower, resolve previous operator
			- operand_stack.push(eval_op(string op, int rhs))
}
*/
int Infix_Evaluator::expression_evaluator(istringstream& tokens, const std::string& expression,
	std::stack<int> operand_stack, std::stack<std::string> operator_stack,
	int last_precedence, int expression_position) {

	// Process each token
	bool double_op = false;
	char next_char;
	std::string curr_op;
	int curr_precedence;

	while (tokens >> next_char) {
		expression_position++;
		// Digit found, throw on operand stack
		if (isdigit(next_char)) {
			tokens.putback(next_char);
			int value;
			tokens >> value;
			// Add correct number of digits
			// e.g. 10 needs to add 1 more to position, to keep track of where it is in the string
			expression_position += int(log10(value));
			operand_stack.push(value);
			continue;
		}

		// Check if possible double operator
		if (std::find(std::begin(POSSIBLE_DOUBLE_OP), std::end(POSSIBLE_DOUBLE_OP), next_char) != std::end(POSSIBLE_DOUBLE_OP)) {
			// Peak at next character in sequence
			char next_next_char = expression[expression_position];
			// If next_next_char is a digit, not a double operator, go back to top of loop
			if (isdigit(next_next_char)) {
				curr_op = std::string() + next_char;
				last_precedence = precedence_map[curr_op];
				operator_stack.push(curr_op);
				continue;
			}
			else { // Double operand, set flag
				double_op = true;
				curr_op = std::string() + next_char + next_next_char;
				// Skip reading the second character of the double operand
				tokens >> next_next_char;
				expression_position++;
			}
		}

		if (!double_op) {
			curr_op = std::string() + next_char;
		}

		curr_precedence = precedence_map[curr_op];

		// Ignore parentheses
		if (curr_precedence == -1)
			continue;
		// Case when precedence is 8 and an operand available, MUST solve right now
		if (curr_precedence == 8 && !operand_stack.empty()) {
			last_precedence = 8;
			operand_stack.push(eval_op(curr_op, 1, operand_stack));
		}
		// First operator
		else if (last_precedence == -2) {
			last_precedence = precedence_map[curr_op];
			operator_stack.push(curr_op);
		}
		// Recursive call
		else if (curr_precedence > last_precedence) {
			std::stack<int> recursive_operand_stack;
			recursive_operand_stack.push(operand_stack.top());
			operand_stack.pop();

			std::stack<std::string> recursive_operator_stack;
			recursive_operator_stack.push(curr_op);

			operand_stack.push(expression_evaluator(tokens, expression,
				recursive_operand_stack, recursive_operator_stack,
				precedence_map[curr_op], expression_position));
		}
		// When curr_precedence < last_precedence, we need to solve an operator
		else {
			if (operator_stack.empty()) {
				int rhs;
				tokens >> next_char;
				tokens.putback(next_char);
				tokens >> rhs;
				operand_stack.push(eval_op(curr_op, rhs, operand_stack));
			}
			else {
				int rhs = operand_stack.top();
				operand_stack.pop();
				operand_stack.push(eval_op(operator_stack.top(), rhs, operand_stack));
				operator_stack.push(curr_op);
			}
		}

		double_op = false;
	}

	int temp_top = operand_stack.top();
	operand_stack.pop();

	return eval_op(operator_stack.top(), temp_top, operand_stack);
}

// Evaluates the current operator
int Infix_Evaluator::eval_op(std::string op, int rhs, std::stack<int>& operand_stack) {
	int lhs;
	// Set to 0 for unary operators, all unary operator code below only affects rhs
	if (operand_stack.empty())
		lhs = 0;
	else {
		lhs = operand_stack.top();
		operand_stack.pop();
	}

	int result = 0;
	// Gets index of operator in OPERATORS vector, this matches the number assigned in enum
	// which is why this switch case works
	switch (operator_position(op)) {
	case Logical_Not: result = !rhs;
		break;
	case Prefix_Increment: result = ++rhs;
		break;
	case Prefix_Decrement: result = --rhs;
		break;
	case Plus: result = lhs + rhs;
		break;
	case Minus: case Negative: result = lhs - rhs;
		break;
	case Multiply: result = lhs * rhs;
		break;
	case Divide: result = lhs / rhs;
		break;
	case Modulus: result = lhs % rhs;
		break;
	case Power: result = pow(lhs, rhs);
		break;
	case Greater_Than: result = lhs > rhs;
		break;
	case Greater_Than_Equal: result = lhs >= rhs;
		break;
	case Less_Than: result = lhs < rhs;
		break;
	case Less_Than_Equal: result = lhs <= rhs;
		break;
	case Equal: result = lhs == rhs;
		break;
	case Not_Equal: result = lhs != rhs;
		break;
	case Logical_And: result = (lhs && rhs);
		break;
	case Logical_Or: result = (lhs || rhs);
		break;
	default:
		throw Syntax_Error("Unexpected Operand found :" + op);
	}

	return result;
}

