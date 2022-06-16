
// Generated from EGrammar.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  EGrammarLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, LOG = 25, LOG10 = 26, 
    COS = 27, SIN = 28, EXP = 29, C_FLOAT = 30, C_INT = 31, C_DOUBLE = 32, 
    C_INT64 = 33, SPEC_SYMBOL = 34, MUL = 35, DIV = 36, ADD = 37, SUB = 38, 
    EQ = 39, NEQ = 40, LESS_ = 41, MORE_ = 42, LSH = 43, RSH = 44, LESSEQ = 45, 
    MOREEQ = 46, BITWISE_AND = 47, BITWISE_OR = 48, BITWISE_XOR = 49, ID = 50, 
    INT64 = 51, INT = 52, DOUBLE = 53, FLOAT = 54, DoublePostfix = 55, FloatPostfix = 56, 
    STRINGLITERAL = 57, COMMENT = 58, NEWLINE = 59, WS = 60
  };

  explicit EGrammarLexer(antlr4::CharStream *input);

  ~EGrammarLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

