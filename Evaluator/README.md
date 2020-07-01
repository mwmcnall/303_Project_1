# Evaluator

## Changes from original implementation
- Operators / Precedence are now stored in static const vectors
	- Done because vectors are safer, also because OPERATORS now contains multi-digit characters (strings)
	- Wes: Thinking more about this, if we want to implement evaluating character by character, we might be able to get away just sensing x number of the same operator in a row? And then we can use the old implementation? lmk if y'all like that better and we can go back to the static const char *
- switch / case done with an enum
	- switch / case cannot be done with strings. enums are very readable in code as well
	- is_operator and precedence methods re-written to deal with new vector storage method

## Debug notes
- Remove any operator that you want to test on a given expression
	- NOTE: This must be done in 4 places to catch all possible errors:
		- OPERATORS vector (top of .cpp)
		- PRECEDENCE (top of .cpp)
		- enum OP_OPTIONS (bottom of .h)
		- switch / case (inside eval_op method in .cpp)
- Feel free to leave // TODO: comments within code if you realize there is an error case not checked or future functionality you don't wish to implement atm

## Missing functionality (Updated 7/1)
- Precedence currently completely ignored
	- Meaning that it reads from left to right and only applies operators in order, it doesn't understand PEMDAS atm
- No parenthesis functionality. 
	- Refer to Infix_To_Postfix.cpp for how that was implemented (in Assignment 3 folder)
- Some final Error Checking cases
