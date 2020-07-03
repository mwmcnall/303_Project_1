#include "Infix_Evaluator.h"
#include <iostream>
#include <cctype>
#include <string>
#include <cmath>
#include <map>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

const std::vector<char> CLOSING_PARENS = { '}',')',']' };
const std::vector<std::string> BINARY_OPS = { "<", "<=", ">", ">=", "==", "!=", "&&", "||"};
const char POSSIBLE_DOUBLE_OP[] = { '!', '+', '-', '>', '<', '=', '&', '|'};

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

// Constructor, instantiates all regex statements so they are only run once
Infix_Evaluator::Infix_Evaluator() {
	this->multi_op = std::regex(multi_operator_regex_builder());
	this->multi_operands = std::regex("(\\d+)(\\s+)(\\d+)");
	this->unary_binary = std::regex(unary_binary_regex_builder());
	this->div_zero = std::regex("(\\/)(\\s*)(0)");
}

// Creates the or syntax for a regex statement to look for operators within a statement appropriately
void Infix_Evaluator::regex_or_buildler(std::string& regex_statement, const std::vector<std::string>& vec,
	std::string end_line) {

	for (std::string s : vec) {
		// Separate each double_op with an or signifier
		regex_statement += s + "{1}|";
	}
	// Remove last |
	regex_statement.pop_back();

	regex_statement += end_line;

	return;
}

// Returns a regex statement that looks for multiple operators in a row
// Works with and without spaces by utilizing or statements
std::string Infix_Evaluator::multi_operator_regex_builder() {
	// Checking for multiple operators in a row is a regex statement that follows a pattern
	// Meaning we can build for a checker using operators
	std::string regex_statement = "(";

	// Operators where it's bad to see them repeated twice in a row
	const std::vector<std::string> repeat_twice = { "&&", "\\|\\|", "\\+\\+", "\\-\\-",
		"!", "\\^", "\\*", "\\/", "%", 
		"<", "<=", ">", ">=", "==", "!="};
	regex_or_buildler(regex_statement, repeat_twice, "){1}(\\s*)(");
	// Last thing to add to statement, no | or new [
	regex_or_buildler(regex_statement, repeat_twice, "){1}");

	return regex_statement;
}

// Create a regex statement that checks for a binary operator following a unary operator
std::string Infix_Evaluator::unary_binary_regex_builder() {
	std::string regex_statement = "(";

	const std::vector<std::string> unary_ops = { "\\+", "\\+\\+", "\\-", "\\-\\-", "!", "=" };
	const std::vector<std::string> binary_ops = { "\\*", "%", "\\/", 
		"<", "<=", ">", ">=", "==", "!=", "&&", "\\|\\|" };

	regex_or_buildler(regex_statement, unary_ops, "){1}(\\s*)(");
	regex_or_buildler(regex_statement, binary_ops, "){1}");

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

	std::smatch match;
	// A unary operator followed by a binary operator
	if (std::regex_search(expression, match, unary_binary)) {
		int pos = expression.find(match.str()) + match[1].length() + match[2].length();
		throw Syntax_Error("A unary operator can't be followed by a binary operator @ char: " + std::to_string(pos));
	}

	// Error Checking Multiple of the same operands in a row
	if (std::regex_search(expression, match, multi_op)) {
		// match[0] is the entire expression. We add half of it to pos because we're dealing with two
		// sets of matched expressions
		int pos = expression.find(match.str()) + std::ceil(match[0].length()/2.0);
		throw Syntax_Error("Two binary operators in a row @ char: " + std::to_string(pos));
	}

	// Error Checking Multiple Operands in a row
	// () represent three different capture groups in regex
	if (std::regex_search(expression, match, multi_operands)) {
		// Adds both match[1] and match[2] length to find the position of the third capture group
		// Done this way to avoid possibility of third capture group matching earlier in the expression
		int pos = expression.find(match.str()) + match[1].length() + match[2].length();
		throw Syntax_Error("Two operands in a row @ char: " + std::to_string(pos));
	}

	//(\/)(\s*)(0)
	// Division by Zero
	if (std::regex_search(expression, match, div_zero)) {
		// Adds both match[1] and match[2] length to find the position of the third capture group
		// Done this way to avoid possibility of third capture group matching earlier in the expression
		int pos = expression.find(match.str()) + match[1].length() + match[2].length();
		throw Syntax_Error("Division by zero @ char: " + std::to_string(pos));
	}

	return;
}

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
		error_checking(expression);
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
			// Yes, peak at next
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
		// When curr_precedence < last_precedence, we want to solve the current operator
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
	if (operand_stack.empty() && op == "-")
		return -rhs;

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
		throw Syntax_Error("Unexpected Operand found");
	}

	return result;
}

