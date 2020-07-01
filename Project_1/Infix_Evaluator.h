#ifndef INFIX_EVALUATOR_H_
#define INFIX_EVALUATOR_H_
#pragma once

#include <stack>
#include <algorithm>
#include <string>
#include <vector>
#include "Syntax_Error.h"

class Infix_Evaluator
{
public:
	// Evaluates a postfix expression
	int eval(const std::string& expression);
private:
	// Evaluates the current operator
	// Pops the two operands off the operand stack and applies the operator
	int eval_op(std::string op, int rhs);

	/** Function to process operators.
		@param op The operator
		@throws Syntax_Error
	*/
	void process_operator(std::string op);

	// Returns true if string is only digits
	bool is_digits(const std::string& str) {
		return str.find_first_not_of("0123456789") == std::string::npos;
	}

	// Returns the position of the operator within the array
	int operator_position(std::string op) {
		auto it = std::find(OPERATORS.begin(), OPERATORS.end(), op);
		return it - OPERATORS.begin();
	}

	// Determines whether a character is an operator
	bool is_operator(std::string op) const {
		if (std::find(OPERATORS.begin(), OPERATORS.end(), op) != OPERATORS.end())
			return true;
		else
			return false;
	}

	/** Determines the precedence of an operator.
		@param op The operator
		@return The precedence
	*/
	int precedence(std::string op) const {
		auto it = std::find(OPERATORS.begin(), OPERATORS.end(), op);
		if (it != OPERATORS.end())
			return PRECEDENCE.at(it - OPERATORS.begin());
		else // TODO: throw error
			return -1;
	}

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
		Logical_Or,
		Open_Parenthesis,
		Close_Parenthesis,
		Open_Bracket,
		Close_Bracket,
		Open_Brace,
		Close_Brace
	};

	static const std::vector<std::string> OPERATORS;
	static const std::vector<int> PRECEDENCE;
	std::stack<int> operand_stack;
	std::stack<string> operator_stack;
};

#endif // !INFIX_EVALUATOR_H_
