
// Generated from EGrammar.g4 by ANTLR 4.8

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

  EGrammarLexer(antlr4::CharStream *input);
  ~EGrammarLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

