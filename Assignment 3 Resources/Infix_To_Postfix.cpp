/** Implementation of Infix_To_Postfix that processes parentheses.*/

#include "Infix_To_Postfix.h"
#include <iostream>
#include <sstream>
#include <cctype>
using std::string;
using std::istringstream;

const string Infix_To_Postfix::OPERATORS = "+-*/%()[]{}^";
const int Infix_To_Postfix::PRECEDENCE[] = { 1, 1, 2, 2, 2, -1, -1, -1, -1, -1, -1, 3};

// Outputs stack to console
void Infix_To_Postfix::print_stack() {
    std::stack<char> temp_stack = operator_stack;
    std::cout << "--Stack" << std::endl;
    while (!temp_stack.empty()) {
        std::cout << "\t" << temp_stack.top() << std::endl;
        temp_stack.pop();
    }

    return;
}

// Outputs expression and place to console
void Infix_To_Postfix::print_expression(std::string curr_expression) {

    std::cout << "--Expression" << std::endl;
    std::cout << curr_expression << std::endl <<
        std::string(expression_follower, ' ') << "^" << std::endl;

    return;
}

void Infix_To_Postfix::print_postfix(const std::string& postfix) {
    std::cout << "--Current Postfix" << std::endl;
    std::cout << "\t" << postfix << std::endl;
}

/** Extract and process each token in infix and return the
    equivalent postfix string.
    @param expression The infix expression
    @return The equivalent postfix expression
    @throws Syntax_Error
*/
string Infix_To_Postfix::convert(const string& expression) {
    postfix = "";
    while (!operator_stack.empty())
        operator_stack.pop();
    istringstream infix_tokens(expression);
    string next_token;
    while (infix_tokens >> next_token) {
        print_expression(infix_tokens.str());
        if (isalnum(next_token[0])) {
            postfix += next_token;
            postfix += " ";
            expression_follower += next_token.size() - 1;
        }
        else if (is_operator(next_token[0])) {
            process_operator(next_token[0]);
            print_stack();
        }
        else {
            throw Syntax_Error("Unexpected Character Encountered");
        }
        print_postfix(postfix);
        expression_follower += 2;
        std::cout << std::endl;
    } // End while

    print_stack();
    // Pop any remaining operators and append them to postfix
    while (!operator_stack.empty()) {
        char op = operator_stack.top();
        operator_stack.pop();
        if (op == '(' || op == '[' || op == '}') {
            throw Syntax_Error("Unmatched open parenthesis");
        }
        postfix += op;
        postfix += " ";
    }

    std::cout << "-- Result" << std::endl << "\t" << postfix << std::endl;

    return postfix;
}

/** Function to process operators.
    @param op The operator
    @throws Syntax_Error\
*/
void Infix_To_Postfix::process_operator(char op) {
    if (operator_stack.empty() || (op == '(') || (op == '[') || (op == '{')) {
        if (op == ')' || op == ']' || op == '}')
            throw Syntax_Error("Unmatched close parenthesis");
        operator_stack.push(op);
    }
    else {
        if (precedence(op) > precedence(operator_stack.top())) {
            operator_stack.push(op);
        }
        else {
            // Pop all stacked operators with equal
            // or higher precedence than op.
            while (!operator_stack.empty()
                && (operator_stack.top() != '(')
                && (operator_stack.top() != '[')
                && (operator_stack.top() != '{')
                && (precedence(op) <= precedence(operator_stack.top()))) {
                postfix += operator_stack.top();
                postfix += " ";
                operator_stack.pop();
            }
            // assert: Operator stack is empty or 
            //         top of stack is '(' or current
            //         operator precedence > top of stack operator
            //         precedence;
            if (op == ')') {
                if (!operator_stack.empty()
                    && (operator_stack.top() == '(')) {
                    operator_stack.pop();
                }
                else {
                    throw Syntax_Error("Unmatched close parentheses");
                }
            }
            else if (op == ']') {
                if (!operator_stack.empty()
                    && (operator_stack.top() == '[')) {
                    operator_stack.pop();
                }
                else {
                    throw Syntax_Error("Unmatched close parentheses");
                }
            }
            else if (op == '}') {
                if (!operator_stack.empty()
                    && (operator_stack.top() == '{')) {
                    operator_stack.pop();
                }
                else {
                    throw Syntax_Error("Unmatched close parentheses");
                }
            }
            else {
                operator_stack.push(op);
            }
        }
    }
}
