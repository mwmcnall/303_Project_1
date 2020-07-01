#ifndef INFIX_EVALUATOR_H_
#define INFIX_EVALUATOR_H_
#pragma once

#include <stack>
#include <algorithm>
#include <string>
#include <vector>
#include <regex> // For error checking
#include "Syntax_Error.h"

class Infix_Evaluator
{
public:
	Infix_Evaluator();
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

	void error_checking(const std::string& expression);
	
	std::regex multi_op, multi_operands, unary_binary, div_zero;
	std::string multi_operator_regex_builder();
	std::string unary_binary_regex_builder();
	void regex_or_buildler(std::string &regex_statement, const std::vector<std::string> &vec,
		std::string end_line);

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

	static const std::vector<std::string> OPERATORS;
	static const std::vector<int> PRECEDENCE;
	std::stack<int> operand_stack;
};

#endif // !INFIX_EVALUATOR_H_
