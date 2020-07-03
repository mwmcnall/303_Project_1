#include "Error_Checking.h"

const std::vector<char> CLOSING_PARENS = { '}',')',']' };
const std::vector<std::string> BINARY_OPS = { "<", "<=", ">", ">=", "==", "!=", "&&", "||" };

Error_Checker::Error_Checker() {
	// Regex builders commented out for efficiency sake, code is below for how regex statements were
	// generated programatically
	//this->multi_op = std::regex(multi_operator_regex_builder());
	this->multi_op = std::regex("(&&{1}|\\|\\|{1}|!{1}|\\^{1}|\\*{1}|\\/{1}|%{1}|<{1}|<={1}|>{1}|>={1}|=={1}|!={1}){1}(\\s*)(&&{1}|\\|\\|{1}|!{1}|\\^{1}|\\*{1}|\\/{1}|%{1}|<{1}|<={1}|>{1}|>={1}|=={1}|!={1}){1}");
	this->multi_operands = std::regex("(\\d+)(\\s+)(\\d+)");
	//this->unary_binary = std::regex(unary_binary_regex_builder());
	this->unary_binary = std::regex("(\\+{1}|\\+\\+{1}|\\-{1}|\\-\\-{1}|!{1}|={1}){1}(\\s*)(\\*{1}|%{1}|\\/{1}|<{1}|<={1}|>{1}|>={1}|=={1}|!={1}|&&{1}|\\|\\|{1}){1}");
	this->div_zero = std::regex("(\\/)(\\s*)(0)");
}

// Creates the or syntax for a regex statement to look for operators within a statement appropriately
void Error_Checker::regex_or_buildler(std::string& regex_statement, const std::vector<std::string>& vec,
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
std::string Error_Checker::multi_operator_regex_builder() {
	// Checking for multiple operators in a row is a regex statement that follows a pattern
	// Meaning we can build for a checker using operators
	std::string regex_statement = "(";

	// Operators where it's bad to see them repeated twice in a row
	const std::vector<std::string> repeat_twice = { "&&", "\\|\\|",// "\\+\\+", "\\-\\-",
		"!", "\\^", "\\*", "\\/", "%",
		"<", "<=", ">", ">=", "==", "!=" };
	regex_or_buildler(regex_statement, repeat_twice, "){1}(\\s*)(");
	// Last thing to add to statement, no | or new [
	regex_or_buildler(regex_statement, repeat_twice, "){1}");

	return regex_statement;
}

// Create a regex statement that checks for a binary operator following a unary operator
std::string Error_Checker::unary_binary_regex_builder() {
	std::string regex_statement = "(";

	const std::vector<std::string> unary_ops = { "\\+", "\\+\\+", "\\-", "\\-\\-", "!", "=" };
	const std::vector<std::string> binary_ops = { "\\*", "%", "\\/",
		"<", "<=", ">", ">=", "==", "!=", "&&", "\\|\\|" };

	regex_or_buildler(regex_statement, unary_ops, "){1}(\\s*)(");
	regex_or_buildler(regex_statement, binary_ops, "){1}");

	return regex_statement;
}

void Error_Checker::error_checking(const std::string& expression) {
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
		int pos = expression.find(match.str()) + std::ceil(match[0].length() / 2.0);
		throw Syntax_Error("Two binary operators in a row @ char: " + std::to_string(pos));
	}

	// Error Checking Multiple Operands in a row
	if (std::regex_search(expression, match, multi_operands)) {
		// Adds both match[1] and match[2] length to find the position of the third capture group
		// Done this way to avoid possibility of third capture group matching earlier in the expression
		int pos = expression.find(match.str()) + match[1].length() + match[2].length();
		throw Syntax_Error("Two operands in a row @ char: " + std::to_string(pos));
	}

	// Division by Zero
	if (std::regex_search(expression, match, div_zero)) {
		// Adds both match[1] and match[2] length to find the position of the third capture group
		// Done this way to avoid possibility of third capture group matching earlier in the expression
		int pos = expression.find(match.str()) + match[1].length() + match[2].length();
		throw Syntax_Error("Division by zero @ char: " + std::to_string(pos));
	}

	return;
}
