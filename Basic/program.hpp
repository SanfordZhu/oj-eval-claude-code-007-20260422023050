/*
 * File: program.h
 * ---------------
 * This interface exports a Program class for storing a BASIC
 * program.
 */

#ifndef _program_h
#define _program_h

#include <string>
#include <map>
#include "statement.hpp"

class Statement;

class Program {
public:
    Program();
    ~Program();

    void clear();
    void addSourceLine(int lineNumber, const std::string& line);
    void removeSourceLine(int lineNumber);
    std::string getSourceLine(int lineNumber);
    void setParsedStatement(int lineNumber, Statement *stmt);
    Statement *getParsedStatement(int lineNumber);
    int getFirstLineNumber();
    int getNextLineNumber(int lineNumber);

    void jumpTo(int lineNumber);
    bool hasJump() const;
    int takeJump();

private:
    std::map<int, std::string> sources;
    std::map<int, Statement*> parsed;
    bool jumpFlag = false;
    int jumpTarget = -1;
};

#endif
