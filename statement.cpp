/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself. 
 */

#include <string>
#include "simpio.h"
#include "statement.h"
#include "parser.h"
using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

/*
 * Implementation notes: RemStmt
 * -----------------------------
 * This subclass represents a comment statement. Any text on
 * the line after the keyword "REM" is ignored.
 */

RemStmt::RemStmt(TokenScanner & scanner) { //Nothing is done in this subclass since it's a comment
}

RemStmt::~RemStmt() {
}

void RemStmt::execute(EvalState &state) {
}

/*
 * Implementation notes: LetStmt
 * -----------------------------
 * This subclass represents an assignment statement. The
 * implementation of execute assigns an expression to a
 * variable.
 */

LetStmt::LetStmt(TokenScanner & scanner) {
    name = scanner.nextToken();
    if (scanner.nextToken() != "=") error("Not an equal sign for assignment");
    exp = parseExp(scanner);

}

LetStmt::~LetStmt() {
    delete exp;
}

void LetStmt::execute(EvalState &state) {
    int expEval = exp->eval(state);
    state.setValue(name, expEval);
}

/*
 * Implementation notes: PrintStmt
 * -----------------------------
 * This subclass represents a printed expression. The implementation
 * of execute prints the value of the expression onto the console and
 * then prints a newline character so that the output from the next
 * PRINT statement begins on a new line.
 */

PrintStmt::PrintStmt(TokenScanner & scanner) {
    exp = parseExp(scanner);
    if (scanner.hasMoreTokens()) {
        error ("Too many tokens");
    }
}

PrintStmt::~PrintStmt() {
    delete exp;
}

void PrintStmt::execute(EvalState &state) {
    cout << exp->eval(state) << endl;
}

/*
 * Implementation notes: InputStmt
 * -----------------------------
 * This subclass represents statements read in from the user. The
 * implementation of execute prompts the user and then reads in a
 * value to be read in the variable.
 */


InputStmt::InputStmt(TokenScanner & scanner) {
    name = scanner.nextToken();
}

InputStmt::~InputStmt() {
}

void InputStmt::execute(EvalState &state) {
    inputPrompt = getInteger(" ? ");
    state.setValue(name, inputPrompt);
}

/*
 * Implementation notes: GoToStmt
 * -----------------------------
 * This subclass represents an unconditional change in the control
 * flow of the program. The implementation of execute continues the
 * program on line n and reports an error if line n does not exist.
 */


GoToStmt::GoToStmt(TokenScanner & scanner) {
    goingToLineNumber = stringToInteger(scanner.nextToken());
}

GoToStmt::~GoToStmt() {
}

void GoToStmt::execute(EvalState &state) {
    state.setCurrentLine(goingToLineNumber);
}

/*
 * Implementation notes: IfStmt
 * -----------------------------
 * This subclass represents a conditional change in the control flow.
 * The implementation of execute evaluates the condition. If the
 * condition holds, the program should continue from line n just
 * as in the GoTo statement. If not, the program continues on to the
 * next line.
 */

IfStmt::IfStmt(TokenScanner & scanner) {
    lhs = readE(scanner);
    comparison = scanner.nextToken();
    rhs = readE(scanner);
    if (scanner.nextToken() != "THEN") {
        error("Wrong statement: no 'then' included");
    }
    else {
        goingToLineNumber = stringToInteger(scanner.nextToken());
    }
}

IfStmt::~IfStmt() {
    delete lhs;
    delete rhs;
}

void IfStmt::execute(EvalState &state) {
    int lhsEval = lhs->eval(state);
    int rhsEval = rhs->eval(state);
    if ((comparison == "=" && lhsEval == rhsEval) || (comparison == ">" && lhsEval > rhsEval) || (comparison == "<" && lhsEval < rhsEval)) {
        state.setCurrentLine(goingToLineNumber);
    }
}

/*
 * Implementation notes: EndStmt
 * -----------------------------
 * This subclass represents the end of the program. The implementation
 * of execute halts the execution once the marked line is reached.
 */

EndStmt::EndStmt(TokenScanner & scanner) {
}

EndStmt::~EndStmt() {
}

void EndStmt::execute(EvalState &state) {
    state.setCurrentLine(-1);
}


