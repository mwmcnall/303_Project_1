#include "Infix_Evaluator.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <regex> // For error checking
#include <string>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

const std::vector<char> CLOSING_PARENS = { '}',')',']' };
const std::vector<std::string> BINARY_OPS = { "<", "<=", ">", ">=", "==", "!=", "&&", "||"};

const std::vector<std::string> Infix_Evaluator::OPERATORS = {
	"!", 
	"++", 
	"--", 
	"---", //denotes Negative. Needed to differentiate between minus and negative.
	"^", 
	"*", "/", "%",
	"+", "-", 
	">", ">=", "<", "<=", 
	"==", "!=", 
	"&&", 
	"||",
	"&", "|", "="
};
const std::vector<int> Infix_Evaluator::PRECEDENCE = {
	8, 
	8, 
	8, 
	8, 
	7, 
	6, 6, 6, 
	5, 5, 
	4, 4, 4, 4, 
	3, 3, 
	2, 
	1,
	0, 0, 0
};

void Infix_Evaluator::regex_or_buildler(std::string& regex_statement, const std::vector<std::string>& vec,
	std::string end_line, int repeat = 1) {

	for (std::string s : vec) {
		// Separate each double_op with an or signifier
		for (int i = 0; i < repeat; i++)
			regex_statement += s;
		regex_statement += "|";
	}
	// Remove last |
	regex_statement.pop_back();

	regex_statement += end_line;

	return;
}

// TODO: Make this idempotent
std::string Infix_Evaluator::multi_operator_regex_builder() {
	// Checking for multiple operators in a row is a regex statement that follows a pattern
	// Meaning we can build for a checker using operators
	std::string regex_statement = "[";

	// We need to escape certain characters for regex
	const std::vector<std::string> double_ops = {"&", "\\|", "+", "\\-", "="};
	// Four in row of double_ops
	regex_or_buildler(regex_statement, double_ops, "]{4}|[");
	// Two two in a row of double_ops with spaces
	// e.g. && &&
	regex_or_buildler(regex_statement, double_ops, "]{2}\\s+[", 2);
	regex_or_buildler(regex_statement, double_ops, "]{2}|[", 2);

	// Operators where it's bad to see them repeated twice in a row
	const std::vector<std::string> repeat_twice = { "!", "^", "*", "/", "%", "<", "<=", ">", ">=", "==", "!="};
	// repeat_twice with no spaces
	regex_or_buildler(regex_statement, repeat_twice, "]{2}|[");

	// Now with spaces
	regex_or_buildler(regex_statement, repeat_twice, "]{1}\\s[", 2);
	// Last thing to add to statement, no | or new [
	regex_or_buildler(regex_statement, repeat_twice, "]{1}", 2);

	return regex_statement;
}

void Infix_Evaluator::error_checking(const std::string& expression) {
	// Error Checking first element of expression
	char first = expression[0];
	if (std::count(CLOSING_PARENS.begin(), CLOSING_PARENS.end(), first))
		throw Syntax_Error("Expression can't start with a closing parenthesis @ char: 0");
	else if (first == '<' || first == '>')
		throw Syntax_Error("Expression can't start with a binary operator @ char: 0");
	std::string first_two = expression.substr(0, 2);
	if (std::count(BINARY_OPS.begin(), BINARY_OPS.end(), first_two))
		throw Syntax_Error("Expression can't start with a binary operator @ char: 0");

	// Error Checking Multiple of the same operands in a row
	std::regex multi_op(multi_operator_regex_builder());
	std::smatch match;
	if (std::regex_search(expression, match, multi_op)) {
		// match[0] is the entire expression. We add half of it to pos because we're dealing with two
		// sets of matched expressions
		int pos = expression.find(match.str()) + match[0].length()/2;
		throw Syntax_Error("Two binary operators in a row @ char: " + std::to_string(pos));
	}

	// Error Checking Multiple Operands in a row
	// () represent three different capture groups in regex
	std::regex multi_operands("(\\d+)(\\s+)(\\d+)");
	if (std::regex_search(expression, match, multi_operands)) {
		// Adds both match[1] and match[2] length to find the position of the third capture group
		// Done this way to avoid possibility of third capture group matching earlier in the expression
		int pos = expression.find(match.str()) + match[1].length() + match[2].length();
		throw Syntax_Error("Two operands in a row @ char: " + std::to_string(pos));
	}

	return;
}

// Evaluates an infix expression
int Infix_Evaluator::eval(const std::string& expression) {

	try
	{
		error_checking(expression);
	}
	catch (const std::exception& e)
	{
		// Output error message and return from function
		// Does NOT evaluate expression
		std::cout << e.what() << std::endl;
		return -1;
	}

	// Be sure the stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;
	//false if previous character was an operator, true if it was a number
	bool was_last_digit = false; 
	while (tokens >> next_char) {
		// Digit found
		
		if (isdigit(next_char)) {
			tokens.putback(next_char);
			int value;
			tokens >> value;
			operand_stack.push(value);
			was_last_digit = true;
		}

		else if (next_char == '!') {
			char op = next_char;
			tokens >> next_char;

			if (next_char == '=') {
				int rhs;
				tokens >> next_char;
				tokens.putback(next_char);
				tokens >> rhs;
				int result = eval_op("!=", rhs);
				operand_stack.push(result);
			}

			else {
				int rhs;
				tokens.putback(next_char);
				tokens >> rhs;
				operand_stack.push(0);
				int result = eval_op("!", rhs);
				operand_stack.push(result);
			}
			was_last_digit = false;
		}
		
		
		
		else if (next_char == '+') {
			char op = next_char;
			tokens >> next_char;
			if (isdigit(next_char)) {
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				int result = eval_op("+", rhs);
				operand_stack.push(result);

			}
		
			else if (next_char == '+') {
				tokens >> next_char;
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				operand_stack.push(1);
				int result = eval_op("++", rhs);
				operand_stack.push(result);

			}
			was_last_digit = false;
		}

/*		else if (next_char == '-') {
			char op = next_char;
			tokens >> next_char;
			if (isdigit(next_char) && (negate_or_subtract == false)) {
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				operand_stack.push(0);
				int result = eval_op("---", rhs);
				operand_stack.push(result);
			}

			else if (next_char == '-') {
				tokens >> next_char;
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				operand_stack.push(-1);
				int result = eval_op("--", rhs);
				operand_stack.push(result);

			}

			else {
				tokens >> next_char;

				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				int result = eval_op(std::string(1, op), rhs);
				operand_stack.push(result);

			}
			negate_or_subtract == true;
		}*/



		// is_operator only allows strings, this converts the character into a string
		// TODO: Will FAIL on any two-digit operator
		else if (is_operator(std::string(1, next_char))) {
			// Read in another element from the expression and store it as the rhs of the equation
			// TODO: eval_op will FAIL if it detects another operator in a row
			char op = next_char;
			tokens >> next_char;

		
		
			if (op == '-' && isdigit(next_char) && was_last_digit == false) {
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				operand_stack.push(0);
				int result = eval_op("---", rhs);
				operand_stack.push(result);
				was_last_digit == true;
			}

			else if (op == '-' && next_char == '-') {
				tokens >> next_char;
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				operand_stack.push(-1);
				int result = eval_op("--", rhs);
				operand_stack.push(result);
				was_last_digit == true;

			}


			else if (is_operator(std::string(1, next_char))) {
				string ops;
				ops = (std::string(1, op) + std::string(1, next_char));
				tokens >> next_char;
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				int result = eval_op(ops, rhs);
				operand_stack.push(result);
				was_last_digit == true;

			}



			else {
				tokens.putback(next_char);
				int rhs;
				tokens >> rhs;
				int result = eval_op(std::string(1, op), rhs);
				operand_stack.push(result);
				was_last_digit == true;

			}
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
	case Logical_Not: result = !rhs;
		break;
	case Plus: case Prefix_Increment: case Prefix_Decrement: result = lhs + rhs;
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
	case Greater_Than : result = lhs > rhs;
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
	// TODO: Should have default case to throw error as it should never be reached
	}

	return result;
}