/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.hpp"
#include "parser.hpp"
#include "program.hpp"
#include "Utils/error.hpp"
#include "Utils/tokenScanner.hpp"
#include "Utils/strlib.hpp"


/* Function prototypes */

void processLine(std::string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    //cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            std::string input;
            if (!std::getline(std::cin, input)) break;
            if (input.empty()) continue;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            if (ex.getMessage() == "__QUIT__") break;
            std::cout << ex.getMessage() << std::endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(std::string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    std::string first = scanner.nextToken();
    if (first == "QUIT") { error("__QUIT__"); }
    if (scanner.getTokenType(first) == NUMBER) {
        size_t spacePos = line.find_first_of(" \t");
        int ln = stringToInteger(first);
        std::string stmtText = (spacePos==std::string::npos)?"":trim(line.substr(spacePos+1));
        if (stmtText.empty()) { program.removeSourceLine(ln); return; }
        program.addSourceLine(ln, line);
        TokenScanner sc2;
        sc2.ignoreWhitespace();
        sc2.scanNumbers();
        sc2.setInput(stmtText);
        std::string kw = sc2.nextToken();
        Statement *stmt = nullptr;
        if (kw == "REM") stmt = new RemStatement(sc2);
        else if (kw == "LET") stmt = new LetStatement(sc2);
        else if (kw == "PRINT") stmt = new PrintStatement(sc2);
        else if (kw == "INPUT") stmt = new InputStatement(sc2);
        else if (kw == "END") stmt = new EndStatement();
        else if (kw == "GOTO") stmt = new GotoStatement(sc2);
        else if (kw == "IF") stmt = new IfStatement(sc2);
        else error("SYNTAX ERROR");
        if (sc2.hasMoreTokens()) error("SYNTAX ERROR");
        program.setParsedStatement(ln, stmt);
        return;
    }

    if (first == "LIST") {
        int ln = program.getFirstLineNumber();
        while (ln != -1) {
            std::cout << program.getSourceLine(ln) << std::endl;
            ln = program.getNextLineNumber(ln);
        }
        return;
    }
    if (first == "CLEAR") { program.clear(); state.Clear(); return; }
    if (first == "RUN") {
        int ln = program.getFirstLineNumber();
        while (ln != -1) {
            Statement *stmt = program.getParsedStatement(ln);
            if (!stmt) error("SYNTAX ERROR");
            stmt->execute(state, program);
            if (program.hasJump()) {
                int target = program.takeJump();
                if (target == -1) break;
                if (program.getSourceLine(target).empty()) error("LINE NUMBER ERROR");
                ln = target;
            } else {
                ln = program.getNextLineNumber(ln);
            }
        }
        return;
    }
    if (first == "PRINT") {
        Expression *e = parseExp(scanner);
        int v = e->eval(state);
        std::cout << v << std::endl;
        delete e;
        return;
    }
    if (first == "LET") {
        Expression *e = parseExp(scanner);
        (void) e->eval(state);
        delete e;
        return;
    }
    if (first == "INPUT") {
        InputStatement is(scanner);
        is.execute(state, program);
        return;
    }
    if (first == "REM") return;
    if (first == "END" || first == "GOTO" || first == "IF") error("SYNTAX ERROR");
    error("SYNTAX ERROR");
}

