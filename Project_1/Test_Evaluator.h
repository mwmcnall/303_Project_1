#ifndef TEST_EVALUATOR_H_
#define TEST_EVALUATOR_H_

#include <iostream>
#include <assert.h>
#include "Infix_Evaluator.h"

#pragma once
class Test_Evaluator
{
public:
	Test_Evaluator();
private: // Variables
	Infix_Evaluator evaluator;
private: // Methods
	void Test_Spaces();
	void Test_No_Spaces();
	void Test_Errors();

	void Test_Prefix_Operators_Spaces();
	void Test_Combo_Operators_Spaces();
	void Test_Boolean_Operators_Spaces();

	void Test_Prefix_Operators_No_Spaces();
	void Test_Combo_Operators_No_Spaces();
	void Test_Boolean_Operators_No_Spaces();
};

#endif // !TEST_EVALUATOR_H_