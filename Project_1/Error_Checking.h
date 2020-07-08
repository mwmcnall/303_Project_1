#ifndef ERROR_CHECKING_H_
#define ERROR_CHECKING_H_

#include "Syntax_Error.h"
#include <regex> // For error checking

#pragma once
class Error_Checker
{
public:
	Error_Checker();
	void check_for_errors(const std::string& expression);
private:
	std::regex multi_op, multi_operands, unary_binary, div_zero;
	std::string multi_operator_regex_builder();
	std::string unary_binary_regex_builder();
	void regex_or_buildler(std::string& regex_statement, const std::vector<std::string>& vec,
		std::string end_line);
};

#endif