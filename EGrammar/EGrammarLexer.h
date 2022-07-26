
// Generated from EGrammar.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  EGrammarLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, LOG = 26, 
    LOG10 = 27, COS = 28, SIN = 29, EXP = 30, C_FLOAT = 31, C_INT = 32, 
    C_DOUBLE = 33, C_INT64 = 34, SPEC_SYMBOL = 35, MUL = 36, DIV = 37, ADD = 38, 
    SUB = 39, EQ = 40, NEQ = 41, LESS_ = 42, MORE_ = 43, LESSEQ = 44, MOREEQ = 45, 
    LSH = 46, RSH = 47, BITWISE_AND = 48, BITWISE_OR = 49, BITWISE_XOR = 50, 
    ID = 51, INT64 = 52, INT = 53, DOUBLE = 54, FLOAT = 55, DoublePostfix = 56, 
    FloatPostfix = 57, STRINGLITERAL = 58, COMMENT = 59, NEWLINE = 60, WS = 61
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

