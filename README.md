# 303_Project_1

# Things you can do (Last Updated 7/1)
* Update to work with parentheses, including error checking for matching parentheses (Infix_To_Postfix shows how to do this)
* Implement precedence (Zach)
* Add error checking on switch case default
* Implement Error Checking (Wes)
  * Along with how to find where the error occurred (Wes)

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
  * Finished by 6/30
* Split a string with no spaces into arithmetic in numbers
  * Uneeded
* Error checking on invalid objectives
  * Create Syntax_Error class based off the one uploaded from Assignment 3
    * Finished by 6/30
  * Be able to keep track of character that caused the error
    * Finished by 7/1, all progress on Error_Checking branch
  * Can't start with closing parenthesis
    * Finished by 7/1
  * Can't start with opening parenthesis
    * Finished by 7/1
  * Two binary operators cannot be in a row
    * Finished by 7/1
  * Two operands in a row
    * Finished by 7/1
  * A unary operand can't be followed by a binary operator
  * Division by 0
  * Check Project Assignment for examples of above
  
# Assumptions
* You may assume all operands are integers
