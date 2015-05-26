/*
 * File: program.cpp
 * -----------------
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
}

Program::~Program() {
   clear();
}

/*
 * Method: clear
 * Usage: program.clear();
 * -----------------------
 * Removes all lines from the program.
 */

void Program::clear() {
   for (int key : storage) { //Goes through and deletes all entries in the map
       delete storage[key];
   }
   lineNumbers.clear();
   storage.clear();
}

/*
 * Method: addSourceLine
 * Usage: program.addSourceLine(lineNumber, line);
 * -----------------------------------------------
 * Adds a source line to the program with the specified line number.
 * If that line already exists, the text of the line replaces
 * the text of any existing line and the parsed representation
 * (if any) is deleted.  If the line is new, it is added to the
 * program in the correct sequence.
 */

void Program::addSourceLine(int lineNumber, string line) {
   int newLineNumberIndex; //The following lines add line numbers to the vector
   if (lineNumbers.isEmpty()) { //Adds first element to vector
       lineNumbers.add(lineNumber);
       newLineNumberIndex = 0;
   }
   else {
       for (int i = lineNumbers.size() - 1; i >= 0; i--) { //Inserts line number according to order in vector
           if (lineNumber > lineNumbers[i]) {
               lineNumbers.insert(i + 1, lineNumber);
               newLineNumberIndex = i;
               break;
           }
           if (i == 0) {
               lineNumbers.insert(0, lineNumber);
               newLineNumberIndex = i;

           }
       }
   }
   SourceLine *newSourceLine = new SourceLine; //Deals with adding information to the map "storage"
   newSourceLine->lineString = line;
   newSourceLine->lineNumbersIndex = newLineNumberIndex;
   newSourceLine->lineParsed = NULL;
   storage.put(lineNumber, newSourceLine);
   for (int i = 0; i < lineNumbers.size(); i++) { //Shift line numbers indices accordingly in the map
                                                  //when something is added
       storage[lineNumbers[i]]->lineNumbersIndex = i;
   }
}

/*
 * Method: removeSourceLine
 * Usage: program.removeSourceLine(lineNumber);
 * --------------------------------------------
 * Removes the line with the specified number from the program,
 * freeing the memory associated with any parsed representation.
 * If no such line exists, this method simply returns without
 * performing any action.
 */

void Program::removeSourceLine(int lineNumber) {
   int removeIndex = storage.get(lineNumber)->lineNumbersIndex; //Obtain index to remove from vector
   lineNumbers.remove(removeIndex);
   storage.remove(lineNumber); //Remove from map
}

/*
 * Method: getSourceLine
 * Usage: string line = program.getSourceLine(lineNumber);
 * -------------------------------------------------------
 * Returns the program line with the specified line number.
 * If no such line exists, this method returns the empty string.
 */

string Program::getSourceLine(int lineNumber) {
   return storage.get(lineNumber)->lineString;
}

/*
 * Method: setParsedStatement
 * Usage: program.setParsedStatement(lineNumber, stmt);
 * ----------------------------------------------------
 * Adds the parsed representation of the statement to the statement
 * at the specified line number.  If no such line exists, this
 * method raises an error.  If a previous parsed representation
 * exists, the memory for that statement is reclaimed.
 */

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   if (storage.containsKey(lineNumber)) {
       if (storage[lineNumber]->lineParsed != NULL) delete storage[lineNumber]->lineParsed;
       //If the line parsed field of the source line containes something, delete it and replace it with
       //given statement
       storage[lineNumber]->lineParsed = stmt;
   }
}

/*
 * Method: getParsedStatement
 * Usage: Statement *stmt = program.getParsedStatement(lineNumber);
 * ----------------------------------------------------------------
 * Retrieves the parsed representation of the statement at the
 * specified line number.  If no value has been set, this method
 * returns NULL.
 */

Statement *Program::getParsedStatement(int lineNumber) {
    if (storage.containsKey(lineNumber)) return storage.get(lineNumber)->lineParsed;
    else return NULL;
}

/*
 * Method: getFirstLineNumber
 * Usage: int lineNumber = program.getFirstLineNumber();
 * -----------------------------------------------------
 * Returns the line number of the first line in the program.
 * If the program has no lines, this method returns -1.
 */

int Program::getFirstLineNumber() {
   if (!lineNumbers.isEmpty()) {
       return lineNumbers[0];
   }
   return -1;
}

/*
 * Method: getNextLineNumber
 * Usage: int nextLine = program.getNextLineNumber(lineNumber);
 * ------------------------------------------------------------
 * Returns the line number of the first line in the program whose
 * number is larger than the specified one, which must already exist
 * in the program.  If no more lines remain, this method returns -1.
 */
int Program::getNextLineNumber(int lineNumber) {
   if (!lineNumbers.isEmpty()) {
       int nextLineNumberIndex = storage.get(lineNumber)->lineNumbersIndex + 1; //Obtains the index of the next line number
       if (nextLineNumberIndex < lineNumbers.size() && lineNumbers.size() != 0) { //If that index is in the vector
           return lineNumbers[nextLineNumberIndex]; //Return that next line number
       }
   }
   return -1;
}
