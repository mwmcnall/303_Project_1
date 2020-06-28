#pragma once
#ifndef POSTFIX_EVALUATOR_H_
#define POSTFIX_EVALUATOR_H_

#include <stack>
#include <string>
#include "Syntax_Error.h"

class Postfix_Evaluator
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

	// Outputs stack to console
	void print_stack();

	// Outputs action to console
	void print_action(std::string action_type, int item);

	void print_eval_op(int lhs, int rhs, char op, int result);

	void print_expression(std::string curr_expression);

	static const std::string OPERATORS;
	std::stack<int> operand_stack;
	int expression_follower = 0;
};

#endif