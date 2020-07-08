#ifndef INFIX_EVALUATOR_H_
#define INFIX_EVALUATOR_H_
#pragma once

#include <stack>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <regex> // For error checking
#include "Syntax_Error.h"
#include "Error_Checking.h"

using std::istringstream;

class Infix_Evaluator
{
public:
	// Evaluates a postfix expression, the wrapper function
	int eval(const std::string& expression);
private: // Variables

	Error_Checker error_checker;

	static const int INITIAL_PRECEDENCE = -3;
	static const int PRECEDENCE_RECURSION = 9;

	static const std::vector<std::string> OPERATORS;

	enum OP_OPTIONS {
		Logical_Not,
		Prefix_Increment,
		Prefix_Decrement,
		Negative,
		Power,
		Multiply,
		Divide,
		Modulus,
		Plus,
		Minus,
		Greater_Than,
		Greater_Than_Equal,
		Less_Than,
		Less_Than_Equal,
		Equal,
		Not_Equal,
		Logical_And,
		Logical_Or
	};
private: // Methods

	// Recursive expression solver
	int eval_exp(istringstream& tokens, const std::string& expression,
		std::stack<int> operand_stack, std::stack<std::string> operator_stack,
		int& expression_position, int last_precedence = INITIAL_PRECEDENCE);

	// Evaluates the current operator
	// Pops the two operands off the operand stack and applies the operator
	int eval_op(std::string op, int rhs, std::stack<int>& operand_stack);

	// Returns the position of the operator within the array
	int operator_position(std::string op) {
		auto it = std::find(OPERATORS.begin(), OPERATORS.end(), op);
		return it - OPERATORS.begin();
	}
};

#endif // !INFIX_EVALUATOR_H_
