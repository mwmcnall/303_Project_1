# 303_Project_1

## Project Goals
* Using stacks, write an infix expression parser
* Expressions
  * !
  * ++
  * --
  * -
  * ^
  * *, /, %
  * +, -
  * >=, <, <=, >
  * ==, !=
  * &&
  * ||
* Parse an expression given in a string format
  * Your program should be able to work with or without spaces in the string
  

# Assignable Objectives
* Create base class using elements from Assignment 3 as reference
  * Finish by 6/30
* Split a string with no spaces into arithmetic in numbers
  * Finish by 6/30
* Create an Evaluator class where we can run test cases and see if the desired output matches the examples
  * Evaluator eval. eval.eval()
* Error checking on invalid objectives
  * Create Syntax_Error class based off the one uploaded from Assignment 3
  * Be able to keep track of character that caused the error
  * Can't start with closing parenthesis
  * Can't start with opening parenthesis
  * Two binary operators cannot be in a row
  * Two operands in a row
  * A unary operand can't be followed by a binary operator
  * Division by 0
  * Check Project Assignment for examples of above
  
# Assumptions
* You may assume all operands are integers
