/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include "statement.hpp"

Statement::Statement() = default;
Statement::~Statement() = default;

RemStatement::RemStatement(TokenScanner &scanner) {
    comment = "";
    while (scanner.hasMoreTokens()) {
        std::string t = scanner.nextToken();
        if (!comment.empty()) comment += " ";
        comment += t;
    }
}
void RemStatement::execute(EvalState &, Program &) {}

LetStatement::LetStatement(TokenScanner &scanner) { exp = parseExp(scanner); }
LetStatement::~LetStatement() { delete exp; }
void LetStatement::execute(EvalState &state, Program &) { (void) exp->eval(state); }

PrintStatement::PrintStatement(TokenScanner &scanner) { exp = parseExp(scanner); }
PrintStatement::~PrintStatement() { delete exp; }
void PrintStatement::execute(EvalState &state, Program &) {
    int v = exp->eval(state);
    std::cout << v << std::endl;
}

InputStatement::InputStatement(TokenScanner &scanner) {
    std::string name = scanner.nextToken();
    TokenType tt = scanner.getTokenType(name);
    if (!(tt == WORD || tt == NUMBER)) error("SYNTAX ERROR");
    var = name;
}
void InputStatement::execute(EvalState &state, Program &) {
    std::cout << "? ";
    std::string line;
    if (!std::getline(std::cin, line)) error("INPUT ERROR");
    TokenScanner sc;
    sc.ignoreWhitespace();
    sc.scanNumbers();
    sc.setInput(line);
    std::string tok = sc.nextToken();
    if (sc.getTokenType(tok) != NUMBER || sc.hasMoreTokens()) error("SYNTAX ERROR");
    int val = stringToInteger(tok);
    state.setValue(var, val);
}

EndStatement::EndStatement() {}
void EndStatement::execute(EvalState &, Program &program) { program.jumpTo(-1); }

GotoStatement::GotoStatement(TokenScanner &scanner) {
    std::string tok = scanner.nextToken();
    if (scanner.getTokenType(tok) != NUMBER) error("SYNTAX ERROR");
    target = stringToInteger(tok);
}
void GotoStatement::execute(EvalState &, Program &program) { program.jumpTo(target); }

IfStatement::IfStatement(TokenScanner &scanner) {
    lhs = readE(scanner, 1); // stop before '='
    op = scanner.nextToken();
    if (!(op == "=" || op == "<" || op == ">")) error("SYNTAX ERROR");
    rhs = readE(scanner);
    scanner.verifyToken("THEN");
    std::string t = scanner.nextToken();
    if (scanner.getTokenType(t) != NUMBER) error("SYNTAX ERROR");
    target = stringToInteger(t);
}
IfStatement::~IfStatement() { delete lhs; delete rhs; }
void IfStatement::execute(EvalState &state, Program &program) {
    int lv = lhs->eval(state);
    int rv = rhs->eval(state);
    bool cond = false;
    if (op == "=") cond = (lv == rv);
    else if (op == "<") cond = (lv < rv);
    else if (op == ">") cond = (lv > rv);
    if (cond) program.jumpTo(target);
}
