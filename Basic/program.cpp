/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.hpp"
#include "Utils/error.hpp"

Program::Program() = default;

Program::~Program() {
    clear();
}

void Program::clear() {
    for (auto &p : parsed) delete p.second;
    parsed.clear();
    sources.clear();
    jumpFlag = false;
    jumpTarget = -1;
}

void Program::addSourceLine(int lineNumber, const std::string &line) {
    removeSourceLine(lineNumber);
    sources[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber) {
    auto it = parsed.find(lineNumber);
    if (it != parsed.end()) { delete it->second; parsed.erase(it); }
    sources.erase(lineNumber);
}

std::string Program::getSourceLine(int lineNumber) {
    auto it = sources.find(lineNumber);
    if (it == sources.end()) return "";
    return it->second;
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    if (sources.find(lineNumber) == sources.end()) error("LINE NUMBER ERROR");
    auto it = parsed.find(lineNumber);
    if (it != parsed.end()) { delete it->second; }
    parsed[lineNumber] = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = parsed.find(lineNumber);
    if (it == parsed.end()) return nullptr;
    return it->second;
}

int Program::getFirstLineNumber() {
    if (sources.empty()) return -1;
    return sources.begin()->first;
}

int Program::getNextLineNumber(int lineNumber) {
    auto it = sources.upper_bound(lineNumber);
    if (it == sources.end()) return -1;
    return it->first;
}

void Program::jumpTo(int lineNumber) {
    jumpFlag = true;
    jumpTarget = lineNumber;
}

bool Program::hasJump() const { return jumpFlag; }

int Program::takeJump() { jumpFlag = false; return jumpTarget; }
