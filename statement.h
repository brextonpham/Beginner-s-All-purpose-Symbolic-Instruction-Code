/*
 * File: statement.h
 * -----------------
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "strlib.h"
#include "string.h"
#include "tokenscanner.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

   Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

   virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

   virtual void execute(EvalState & state) = 0;

};

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

/*
 * Subclass: RemStmt
 * ----------------------------
 * This subclass represents a comment statement. Any text on
 * the line after the keyword "REM" is ignored.
 */

class RemStmt : public Statement {

public:

/*
 * Constructor: RemStmt
 * -------------------
 * Creates a new comment statement.
 */

    RemStmt(TokenScanner & scanner);

/* Prototypes for the virtual methods overridden by this class */

    virtual ~RemStmt();
    virtual void execute(EvalState & state);

    };

/*
 * Subclass: LetStmt
 * ----------------------------
 * This subclass represents Basic's assignment statment. The "LET"
 * keyword is followed by a variable name, an eqaul sign, and an
 * expression. As in C++, the effect of this statement is to assign
 * the value of the expression to the variable, replacing any previous
 * value. In Basic, assignment is not an operator and may not be nested
 * inside other expressions.
 */

class LetStmt : public Statement {

public:

/*
 * Constructor: LetStmt
 * -------------------
 * Creates a new assignment statement.
 */

    LetStmt(TokenScanner & scanner);

/* Prototypes for the virtual methods overridden by this class */

    virtual ~LetStmt();
    virtual void execute(EvalState & state);

private:

    std::string name;
    Expression *exp;

    };

/*
 * Subclass: PrintStmt
 * ----------------------------
 * This subclass represents a statement that prints the value of
 * an expression on the console and then print a new line character
 * so that the output from the next PRINT statement begins on a new
 * line.
 */

class PrintStmt : public Statement {

public:

/*
 * Constructor: PrintStmt
 * -------------------
 * Creates a new print statement.
 */

    PrintStmt(TokenScanner & scanner);

/* Prototypes for the virtual methods overridden by this class */

    virtual ~PrintStmt();
    virtual void execute(EvalState & state);

private:

    Expression *exp;

    };

/*
 * Subclass: InputStmt
 * ----------------------------
 * This subclass represents a statement where a variable is read in
 * from the user. The effect of this statement is to print a prompt
 * consisting of the string " ? " and then to read in a value ot be
 * stored in the variable.
 */

class InputStmt : public Statement {

public:

/*
 * Constructor: InputStmt
 * -------------------
 * Creates a new input statement.
 */

    InputStmt(TokenScanner & scanner);

/* Prototypes for the virtual methods overridden by this class */

    virtual ~InputStmt();
    virtual void execute(EvalState & state);

private:

    std::string name;
    int inputPrompt;

    };

/*
 * Subclass: GoToStmt
 * ----------------------------
 * This subclass represents a statement that forces an unconditional
 * change in the control flow of the program. When the program hits this
 * statement, the program continues from line n instead of continuing
 * with the next statement. Your program should report an error if line n
 * does not exist.
*/

class GoToStmt : public Statement {

public:

/*
 * Constructor: GoToStmt
 * -------------------
 * Creates a new GoTo statement.
 */

    GoToStmt(TokenScanner & scanner);

/* Prototypes for the virtual methods overridden by this class */

    virtual ~GoToStmt();
    virtual void execute(EvalState & state);

private:

    int goingToLineNumber;

    };

/*
 * Subclass: IfStmt
 * ----------------------------
 * This subclass represents a statement that provides conditional
 * control. If the condition holds, the program should continue
 * from line n just as in the GoTo statement. If not, the program
 * continues on to the next line.
*/

class IfStmt : public Statement {

public:

/*
 * Constructor: IfStmt
 * -------------------
 * Creates a new if statement.
 */

    IfStmt(TokenScanner & scanner);

/* Prototypes for the virtual methods overridden by this class */

    virtual ~IfStmt();
    virtual void execute(EvalState & state);

private:

    Expression *lhs;
    Expression *rhs;
    std::string comparison;
    int goingToLineNumber;

    };

/*
 * Subclass: EndStmt
 * ----------------------------
 * This subclass marks the end of the program. Execution halts when
 * this line is reached. This statement is usually optional in BASIC
 * programs because execution also stops if the program continues
 * past the last numbered line.
*/

class EndStmt : public Statement {

public:

/*
 * Constructor: EndStmt
 * -------------------
 * Creates a new end statement.
 */

    EndStmt(TokenScanner & scanner);

/* Prototypes for the virtual methods overridden by this class */

    virtual ~EndStmt();
    virtual void execute(EvalState & state);

private:

    };

#endif
