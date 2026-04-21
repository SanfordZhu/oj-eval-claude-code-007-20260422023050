/*
 * File: evalstate.h
 * -----------------
 * This interface exports a class called EvalState, which keeps track
 * of additional information required by the evaluator, most notably
 * the values of variables.
 */

#ifndef _evalstate_h
#define _evalstate_h

#include <string>
#include <map>

/*
 * Class: EvalState
 * ----------------
 * This class is passed by reference through the recursive levels
 * of the evaluator and contains information from the evaluation
 * environment that the evaluator may need to know.  In this
 * version, the only information maintained by the EvalState class
 * is a symbol table that maps variable names into their values.
 * In your implementation, you may include additional information
 * in the EvalState class.
 */

class EvalState {

public:
    EvalState();
    ~EvalState();

    void setValue(std::string var, int value);
    int getValue(std::string var);
    bool isDefined(std::string var);
    void Clear();

    void setQuit(bool q);
    bool shouldQuit() const;

private:
    std::map<std::string, int> symbolTable;
    bool quit = false;
};

#endif
