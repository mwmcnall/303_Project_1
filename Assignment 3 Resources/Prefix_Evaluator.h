#pragma once
#ifndef PREFIX_EVALUATOR_H_
#define PREFIX_EVALUATOR_H_

#include <stack>
#include <string>
#include "Syntax_Error.h"
class Prefix_Evaluator
{
public:
	// Evaluates a postfix expression
	int eval(const std::string& expression);
private:
	// Evaluates the current operator
	// Pops the two operands off the operand stack and applies the operator
	int eval_op(char op);

	// Determines whether a character is an operator
	bool is_operator(char ch) const {
		return OPERATORS.find(ch) != std::string::npos;
	}

	std::string reverse(const std::string& s);

	static const std::string OPERATORS;
	std::stack<int> operand_stack;
};

#endif