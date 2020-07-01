#include "Infix_Evaluator.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::stack;
using std::string;
using std::istringstream;
using std::isdigit;

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
	"&", "|", "=",
	"(", ")", "[", "]", "{", "}"
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
	0, 0, 0,
	-1, -1, -1, -1, -1, -1
};

// Evaluates an infix expression
int Infix_Evaluator::eval(const std::string& expression) {
	// Be sure the stack is empty
	while (!operand_stack.empty())
		operand_stack.pop();

	// Process each token
	istringstream tokens(expression);
	char next_char;
	bool was_last_digit = false; //false if previous character was an operator, true if it was a number
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


			else if (is_operator(std::string(1, next_char))) { // hasn't this condition already been tested by this point?
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
	case Open_Parenthesis:
		break;
	case Close_Parenthesis:
		break;
	case Open_Bracket:
		break;
	case Close_Bracket:
		break;
	case Open_Brace:
		break;
	case Close_Brace:
		break;
	// TODO: Should have default case to throw error as it should never be reached
	}

	return result;
}