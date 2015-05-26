/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter. The programming language BASIC operates on a
 * sequence of numbered statements. The line numbers at the beginning
 * of the line establish the sequence of op\erations in a program. In the absence
 * of any control statements ot the contrary, the statements in a program
 * are executed in ascending numerical order starting at the lowest number.
 * Line numbers are also used to provide a simple editing mechanism. Statements
 * need not be entered in order, because the line numbers indicate their relative
 * position. Morever, as long as the user has left gaps in the number sequence,
 * new statements can be added in between other statements.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "console.h"
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "tokenscanner.h"
#include "simpio.h"
#include "strlib.h"
using namespace std;

/* Constants */

const int END_PROGRAM_LINE_NUMBER = -1;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);
void runCommand(string line, Program & program, EvalState & state);
void listCommand(Program & program);
void variableCommand(TokenScanner & scanner, EvalState & state, string stringInitialToken);
void lineNumberCommand(string stringInitialToken, string line, TokenScanner & scanner, Program & program);
void helpCommand();

/* Main program */

int main() {
   EvalState state;
   Program program;
   cout << "Minimal BASIC -- Type HELP for help" << endl;
   while (true) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state) {
   TokenScanner scanner;
   scanner.ignoreWhitespace();
   scanner.scanNumbers();
   scanner.setInput(line);
   string stringInitialToken = scanner.nextToken();
   if (toUpperCase(line) == "RUN") runCommand(toUpperCase(line), program, state);
   else if (toUpperCase(line) == "HELP") helpCommand();
   else if (toUpperCase(line) == "QUIT") exit(0);
   else if (toUpperCase(line) == "LIST") listCommand(program);
   else if (toUpperCase(line) == "CLEAR") program.clear();
   else if (toUpperCase(stringInitialToken) == "LET" || toUpperCase(stringInitialToken) == "PRINT" || toUpperCase(stringInitialToken) == "INPUT") variableCommand(scanner, state, toUpperCase(stringInitialToken));
   else if (line.length() > stringInitialToken.length() && stringIsInteger(stringInitialToken)) lineNumberCommand(toUpperCase(stringInitialToken), line, scanner, program);
   else if (!scanner.hasMoreTokens()) { //Remove that line number from program
       int intLineNumber = stringToInteger(stringInitialToken);
       program.removeSourceLine(intLineNumber);
   }
   else cout << "Not a valid statement" << endl;
}

//Runs all commands in the program when user requests
void runCommand(string line, Program & program, EvalState & state) {
    int currentLineNumber = program.getFirstLineNumber();
    state.setCurrentLine(currentLineNumber); //Sets the current line number to the first one
    while (currentLineNumber != END_PROGRAM_LINE_NUMBER) {
        program.getParsedStatement(currentLineNumber)->execute(state);
        if (currentLineNumber != state.getCurrentLine()) {
            currentLineNumber = state.getCurrentLine();
        }
        else {
            currentLineNumber = program.getNextLineNumber(currentLineNumber);
            state.setCurrentLine(currentLineNumber);
        }
    }
}

//Outputs all the inputted lines by the user that are stored.
void listCommand(Program & program) {
    int currentLineNumber = program.getFirstLineNumber();
    while (currentLineNumber != END_PROGRAM_LINE_NUMBER) {
        cout << program.getSourceLine(currentLineNumber) << endl;
        currentLineNumber = program.getNextLineNumber(currentLineNumber);
    }
}

//Parses the statement and then executes the statement
void variableCommand(TokenScanner & scanner, EvalState & state, string stringInitialToken) {
    scanner.saveToken(stringInitialToken);
    Statement *stmt = parseStatement(scanner);
    stmt->execute(state);
    delete stmt;
}

//When line starts with a line number, store the line and set the parsed statement
void lineNumberCommand(string stringInitialToken, string line, TokenScanner & scanner, Program & program) {
    int intLineNumber = stringToInteger(stringInitialToken);
    program.addSourceLine(intLineNumber, line);
    Statement *stmt = parseStatement(scanner);
    program.setParsedStatement(intLineNumber, stmt);
}

void helpCommand() {
    cout << "Available commands:" << endl;
    cout << "   RUN - Runs the program" << endl;
    cout << "   LIST - Lists the program" << endl;
    cout << "   CLEAR - Clears the program" << endl;
    cout << "   HELP -- Prints this message" << endl;
    cout << "   QUIT - Exits from the BASIC interpreter" << endl;
}
