# 303_Project_1

# Things you can do (Last Updated 6/29)
* Update to work with parentheses, including error checking for matching parentheses (Infix_To_Postfix shows how to do this)
* Add more operators
  * Exponential should be the simplest to add
  * Prefix operators will have their own logic
  * Comparison operators will ahve their own logic
* Implement precedence
* Add error checking on switch case default
* Implement method that puts spaces into a string that doesn't have spaces (claimed by Zach)

## Project Goals
* Using stacks, write an infix expression parser
* Expressions
  * !
  * ++
  * --
  * negative
  * ^
  * *, /, %
  * +, minus
  * <, <=, >, >=
  * ==, !=
  * &&
  * ||
* Parse an expression given in a string format
  * Your program should be able to work with or without spaces in the string

# Larger Goals
* Create base class using elements from Assignment 3 as reference
  * Finish by 6/30
* Split a string with no spaces into arithmetic in numbers
  * Finish by 6/30
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
