
// Generated from EGrammar.g4 by ANTLR 4.10.1


#include "EGrammarLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct EGrammarLexerStaticData final {
  EGrammarLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  EGrammarLexerStaticData(const EGrammarLexerStaticData&) = delete;
  EGrammarLexerStaticData(EGrammarLexerStaticData&&) = delete;
  EGrammarLexerStaticData& operator=(const EGrammarLexerStaticData&) = delete;
  EGrammarLexerStaticData& operator=(EGrammarLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag egrammarlexerLexerOnceFlag;
EGrammarLexerStaticData *egrammarlexerLexerStaticData = nullptr;

void egrammarlexerLexerInitialize() {
  assert(egrammarlexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<EGrammarLexerStaticData>(
    std::vector<std::string>{
      "T__0", "T__1", "T__2", "T__3", "T__4", "T__5", "T__6", "T__7", "T__8", 
      "T__9", "T__10", "T__11", "T__12", "T__13", "T__14", "T__15", "T__16", 
      "T__17", "T__18", "T__19", "T__20", "T__21", "T__22", "T__23", "LOG", 
      "LOG10", "COS", "SIN", "EXP", "C_FLOAT", "C_INT", "C_DOUBLE", "C_INT64", 
      "SPEC_SYMBOL", "MUL", "DIV", "ADD", "SUB", "EQ", "NEQ", "LESS_", "MORE_", 
      "LSH", "RSH", "LESSEQ", "MOREEQ", "BITWISE_AND", "BITWISE_OR", "BITWISE_XOR", 
      "ID", "INT64", "INT", "DOUBLE", "FLOAT", "DoublePostfix", "FloatPostfix", 
      "STRINGLITERAL", "COMMENT", "NEWLINE", "WS"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
    },
    std::vector<std::string>{
      "", "','", "'='", "'param'", "'('", "')'", "'out'", "'func'", "'function'", 
      "':'", "';'", "'end'", "'return '", "'integrate'", "'convolve'", "'recconvolve'", 
      "'range'", "'decimation'", "'shift'", "'['", "']'", "'**'", "'^'", 
      "'>='", "'\\u003F'", "", "", "", "", "", "'float'", "'int'", "'double'", 
      "'int64'", "", "'*'", "'/'", "'+'", "'-'", "'=='", "'!='", "'<'", 
      "'>'", "'<<'", "'>>'", "'<='", "'=>'", "'&'", "'|'", "'bxor'", "", 
      "", "", "", "", "'d'", "'f'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
      "", "", "", "", "", "", "", "", "LOG", "LOG10", "COS", "SIN", "EXP", 
      "C_FLOAT", "C_INT", "C_DOUBLE", "C_INT64", "SPEC_SYMBOL", "MUL", "DIV", 
      "ADD", "SUB", "EQ", "NEQ", "LESS_", "MORE_", "LSH", "RSH", "LESSEQ", 
      "MOREEQ", "BITWISE_AND", "BITWISE_OR", "BITWISE_XOR", "ID", "INT64", 
      "INT", "DOUBLE", "FLOAT", "DoublePostfix", "FloatPostfix", "STRINGLITERAL", 
      "COMMENT", "NEWLINE", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,0,60,479,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,2,23,7,23,2,24,7,24,2,25,7,25,2,26,7,26,2,27,7,27,2,28,
  	7,28,2,29,7,29,2,30,7,30,2,31,7,31,2,32,7,32,2,33,7,33,2,34,7,34,2,35,
  	7,35,2,36,7,36,2,37,7,37,2,38,7,38,2,39,7,39,2,40,7,40,2,41,7,41,2,42,
  	7,42,2,43,7,43,2,44,7,44,2,45,7,45,2,46,7,46,2,47,7,47,2,48,7,48,2,49,
  	7,49,2,50,7,50,2,51,7,51,2,52,7,52,2,53,7,53,2,54,7,54,2,55,7,55,2,56,
  	7,56,2,57,7,57,2,58,7,58,2,59,7,59,1,0,1,0,1,1,1,1,1,2,1,2,1,2,1,2,1,
  	2,1,2,1,3,1,3,1,4,1,4,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,7,1,7,1,7,
  	1,7,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,9,1,9,1,10,1,10,1,10,1,10,1,11,1,11,
  	1,11,1,11,1,11,1,11,1,11,1,11,1,12,1,12,1,12,1,12,1,12,1,12,1,12,1,12,
  	1,12,1,12,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,13,1,14,1,14,1,14,
  	1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,14,1,15,1,15,1,15,1,15,1,15,
  	1,15,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,16,1,17,1,17,
  	1,17,1,17,1,17,1,17,1,18,1,18,1,19,1,19,1,20,1,20,1,20,1,21,1,21,1,22,
  	1,22,1,22,1,23,1,23,1,24,1,24,1,24,1,24,1,24,1,24,3,24,244,8,24,1,25,
  	1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,1,25,3,25,256,8,25,1,26,1,26,
  	1,26,1,26,1,26,1,26,3,26,264,8,26,1,27,1,27,1,27,1,27,1,27,1,27,3,27,
  	272,8,27,1,28,1,28,1,28,1,28,1,28,1,28,3,28,280,8,28,1,29,1,29,1,29,1,
  	29,1,29,1,29,1,30,1,30,1,30,1,30,1,31,1,31,1,31,1,31,1,31,1,31,1,31,1,
  	32,1,32,1,32,1,32,1,32,1,32,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,
  	33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,
  	33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,
  	33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,1,33,3,33,350,8,33,1,34,1,34,1,
  	35,1,35,1,36,1,36,1,37,1,37,1,38,1,38,1,38,1,39,1,39,1,39,1,40,1,40,1,
  	41,1,41,1,42,1,42,1,42,1,43,1,43,1,43,1,44,1,44,1,44,1,45,1,45,1,45,1,
  	46,1,46,1,47,1,47,1,48,1,48,1,48,1,48,1,48,1,49,1,49,5,49,393,8,49,10,
  	49,12,49,396,9,49,1,50,4,50,399,8,50,11,50,12,50,400,1,50,1,50,1,50,1,
  	50,1,51,4,51,408,8,51,11,51,12,51,409,1,51,1,51,1,51,3,51,415,8,51,1,
  	52,4,52,418,8,52,11,52,12,52,419,1,52,1,52,5,52,424,8,52,10,52,12,52,
  	427,9,52,1,52,3,52,430,8,52,1,53,4,53,433,8,53,11,53,12,53,434,1,53,1,
  	53,5,53,439,8,53,10,53,12,53,442,9,53,1,53,1,53,1,54,1,54,1,55,1,55,1,
  	56,1,56,5,56,452,8,56,10,56,12,56,455,9,56,1,56,1,56,1,57,1,57,5,57,461,
  	8,57,10,57,12,57,464,9,57,1,57,1,57,1,58,3,58,469,8,58,1,58,1,58,1,59,
  	4,59,474,8,59,11,59,12,59,475,1,59,1,59,0,0,60,1,1,3,2,5,3,7,4,9,5,11,
  	6,13,7,15,8,17,9,19,10,21,11,23,12,25,13,27,14,29,15,31,16,33,17,35,18,
  	37,19,39,20,41,21,43,22,45,23,47,24,49,25,51,26,53,27,55,28,57,29,59,
  	30,61,31,63,32,65,33,67,34,69,35,71,36,73,37,75,38,77,39,79,40,81,41,
  	83,42,85,43,87,44,89,45,91,46,93,47,95,48,97,49,99,50,101,51,103,52,105,
  	53,107,54,109,55,111,56,113,57,115,58,117,59,119,60,1,0,6,2,0,65,90,97,
  	122,4,0,48,57,65,90,95,95,97,122,1,0,48,57,3,0,10,10,13,13,34,34,2,0,
  	10,10,13,13,2,0,9,9,32,32,503,0,1,1,0,0,0,0,3,1,0,0,0,0,5,1,0,0,0,0,7,
  	1,0,0,0,0,9,1,0,0,0,0,11,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,0,0,17,1,0,0,
  	0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,27,1,0,0,0,0,
  	29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,0,0,0,0,39,1,
  	0,0,0,0,41,1,0,0,0,0,43,1,0,0,0,0,45,1,0,0,0,0,47,1,0,0,0,0,49,1,0,0,
  	0,0,51,1,0,0,0,0,53,1,0,0,0,0,55,1,0,0,0,0,57,1,0,0,0,0,59,1,0,0,0,0,
  	61,1,0,0,0,0,63,1,0,0,0,0,65,1,0,0,0,0,67,1,0,0,0,0,69,1,0,0,0,0,71,1,
  	0,0,0,0,73,1,0,0,0,0,75,1,0,0,0,0,77,1,0,0,0,0,79,1,0,0,0,0,81,1,0,0,
  	0,0,83,1,0,0,0,0,85,1,0,0,0,0,87,1,0,0,0,0,89,1,0,0,0,0,91,1,0,0,0,0,
  	93,1,0,0,0,0,95,1,0,0,0,0,97,1,0,0,0,0,99,1,0,0,0,0,101,1,0,0,0,0,103,
  	1,0,0,0,0,105,1,0,0,0,0,107,1,0,0,0,0,109,1,0,0,0,0,111,1,0,0,0,0,113,
  	1,0,0,0,0,115,1,0,0,0,0,117,1,0,0,0,0,119,1,0,0,0,1,121,1,0,0,0,3,123,
  	1,0,0,0,5,125,1,0,0,0,7,131,1,0,0,0,9,133,1,0,0,0,11,135,1,0,0,0,13,139,
  	1,0,0,0,15,144,1,0,0,0,17,153,1,0,0,0,19,155,1,0,0,0,21,157,1,0,0,0,23,
  	161,1,0,0,0,25,169,1,0,0,0,27,179,1,0,0,0,29,188,1,0,0,0,31,200,1,0,0,
  	0,33,206,1,0,0,0,35,217,1,0,0,0,37,223,1,0,0,0,39,225,1,0,0,0,41,227,
  	1,0,0,0,43,230,1,0,0,0,45,232,1,0,0,0,47,235,1,0,0,0,49,243,1,0,0,0,51,
  	255,1,0,0,0,53,263,1,0,0,0,55,271,1,0,0,0,57,279,1,0,0,0,59,281,1,0,0,
  	0,61,287,1,0,0,0,63,291,1,0,0,0,65,298,1,0,0,0,67,349,1,0,0,0,69,351,
  	1,0,0,0,71,353,1,0,0,0,73,355,1,0,0,0,75,357,1,0,0,0,77,359,1,0,0,0,79,
  	362,1,0,0,0,81,365,1,0,0,0,83,367,1,0,0,0,85,369,1,0,0,0,87,372,1,0,0,
  	0,89,375,1,0,0,0,91,378,1,0,0,0,93,381,1,0,0,0,95,383,1,0,0,0,97,385,
  	1,0,0,0,99,390,1,0,0,0,101,398,1,0,0,0,103,407,1,0,0,0,105,417,1,0,0,
  	0,107,432,1,0,0,0,109,445,1,0,0,0,111,447,1,0,0,0,113,449,1,0,0,0,115,
  	458,1,0,0,0,117,468,1,0,0,0,119,473,1,0,0,0,121,122,5,44,0,0,122,2,1,
  	0,0,0,123,124,5,61,0,0,124,4,1,0,0,0,125,126,5,112,0,0,126,127,5,97,0,
  	0,127,128,5,114,0,0,128,129,5,97,0,0,129,130,5,109,0,0,130,6,1,0,0,0,
  	131,132,5,40,0,0,132,8,1,0,0,0,133,134,5,41,0,0,134,10,1,0,0,0,135,136,
  	5,111,0,0,136,137,5,117,0,0,137,138,5,116,0,0,138,12,1,0,0,0,139,140,
  	5,102,0,0,140,141,5,117,0,0,141,142,5,110,0,0,142,143,5,99,0,0,143,14,
  	1,0,0,0,144,145,5,102,0,0,145,146,5,117,0,0,146,147,5,110,0,0,147,148,
  	5,99,0,0,148,149,5,116,0,0,149,150,5,105,0,0,150,151,5,111,0,0,151,152,
  	5,110,0,0,152,16,1,0,0,0,153,154,5,58,0,0,154,18,1,0,0,0,155,156,5,59,
  	0,0,156,20,1,0,0,0,157,158,5,101,0,0,158,159,5,110,0,0,159,160,5,100,
  	0,0,160,22,1,0,0,0,161,162,5,114,0,0,162,163,5,101,0,0,163,164,5,116,
  	0,0,164,165,5,117,0,0,165,166,5,114,0,0,166,167,5,110,0,0,167,168,5,32,
  	0,0,168,24,1,0,0,0,169,170,5,105,0,0,170,171,5,110,0,0,171,172,5,116,
  	0,0,172,173,5,101,0,0,173,174,5,103,0,0,174,175,5,114,0,0,175,176,5,97,
  	0,0,176,177,5,116,0,0,177,178,5,101,0,0,178,26,1,0,0,0,179,180,5,99,0,
  	0,180,181,5,111,0,0,181,182,5,110,0,0,182,183,5,118,0,0,183,184,5,111,
  	0,0,184,185,5,108,0,0,185,186,5,118,0,0,186,187,5,101,0,0,187,28,1,0,
  	0,0,188,189,5,114,0,0,189,190,5,101,0,0,190,191,5,99,0,0,191,192,5,99,
  	0,0,192,193,5,111,0,0,193,194,5,110,0,0,194,195,5,118,0,0,195,196,5,111,
  	0,0,196,197,5,108,0,0,197,198,5,118,0,0,198,199,5,101,0,0,199,30,1,0,
  	0,0,200,201,5,114,0,0,201,202,5,97,0,0,202,203,5,110,0,0,203,204,5,103,
  	0,0,204,205,5,101,0,0,205,32,1,0,0,0,206,207,5,100,0,0,207,208,5,101,
  	0,0,208,209,5,99,0,0,209,210,5,105,0,0,210,211,5,109,0,0,211,212,5,97,
  	0,0,212,213,5,116,0,0,213,214,5,105,0,0,214,215,5,111,0,0,215,216,5,110,
  	0,0,216,34,1,0,0,0,217,218,5,115,0,0,218,219,5,104,0,0,219,220,5,105,
  	0,0,220,221,5,102,0,0,221,222,5,116,0,0,222,36,1,0,0,0,223,224,5,91,0,
  	0,224,38,1,0,0,0,225,226,5,93,0,0,226,40,1,0,0,0,227,228,5,42,0,0,228,
  	229,5,42,0,0,229,42,1,0,0,0,230,231,5,94,0,0,231,44,1,0,0,0,232,233,5,
  	62,0,0,233,234,5,61,0,0,234,46,1,0,0,0,235,236,5,63,0,0,236,48,1,0,0,
  	0,237,238,5,108,0,0,238,239,5,111,0,0,239,244,5,103,0,0,240,241,5,76,
  	0,0,241,242,5,111,0,0,242,244,5,103,0,0,243,237,1,0,0,0,243,240,1,0,0,
  	0,244,50,1,0,0,0,245,246,5,108,0,0,246,247,5,111,0,0,247,248,5,103,0,
  	0,248,249,5,49,0,0,249,256,5,48,0,0,250,251,5,76,0,0,251,252,5,111,0,
  	0,252,253,5,103,0,0,253,254,5,49,0,0,254,256,5,48,0,0,255,245,1,0,0,0,
  	255,250,1,0,0,0,256,52,1,0,0,0,257,258,5,99,0,0,258,259,5,111,0,0,259,
  	264,5,115,0,0,260,261,5,67,0,0,261,262,5,111,0,0,262,264,5,115,0,0,263,
  	257,1,0,0,0,263,260,1,0,0,0,264,54,1,0,0,0,265,266,5,115,0,0,266,267,
  	5,105,0,0,267,272,5,110,0,0,268,269,5,83,0,0,269,270,5,105,0,0,270,272,
  	5,110,0,0,271,265,1,0,0,0,271,268,1,0,0,0,272,56,1,0,0,0,273,274,5,101,
  	0,0,274,275,5,120,0,0,275,280,5,112,0,0,276,277,5,69,0,0,277,278,5,120,
  	0,0,278,280,5,112,0,0,279,273,1,0,0,0,279,276,1,0,0,0,280,58,1,0,0,0,
  	281,282,5,102,0,0,282,283,5,108,0,0,283,284,5,111,0,0,284,285,5,97,0,
  	0,285,286,5,116,0,0,286,60,1,0,0,0,287,288,5,105,0,0,288,289,5,110,0,
  	0,289,290,5,116,0,0,290,62,1,0,0,0,291,292,5,100,0,0,292,293,5,111,0,
  	0,293,294,5,117,0,0,294,295,5,98,0,0,295,296,5,108,0,0,296,297,5,101,
  	0,0,297,64,1,0,0,0,298,299,5,105,0,0,299,300,5,110,0,0,300,301,5,116,
  	0,0,301,302,5,54,0,0,302,303,5,52,0,0,303,66,1,0,0,0,304,305,5,102,0,
  	0,305,306,5,117,0,0,306,307,5,110,0,0,307,350,5,99,0,0,308,309,5,114,
  	0,0,309,310,5,101,0,0,310,311,5,116,0,0,311,312,5,117,0,0,312,313,5,114,
  	0,0,313,314,5,110,0,0,314,350,5,32,0,0,315,316,5,99,0,0,316,317,5,111,
  	0,0,317,318,5,110,0,0,318,319,5,118,0,0,319,320,5,111,0,0,320,321,5,108,
  	0,0,321,322,5,118,0,0,322,350,5,101,0,0,323,324,5,105,0,0,324,325,5,110,
  	0,0,325,326,5,101,0,0,326,327,5,103,0,0,327,328,5,114,0,0,328,329,5,97,
  	0,0,329,330,5,116,0,0,330,350,5,101,0,0,331,332,5,114,0,0,332,333,5,97,
  	0,0,333,334,5,110,0,0,334,335,5,103,0,0,335,350,5,101,0,0,336,337,5,115,
  	0,0,337,338,5,104,0,0,338,339,5,105,0,0,339,340,5,102,0,0,340,350,5,116,
  	0,0,341,342,5,112,0,0,342,343,5,97,0,0,343,344,5,114,0,0,344,345,5,97,
  	0,0,345,350,5,109,0,0,346,347,5,111,0,0,347,348,5,117,0,0,348,350,5,116,
  	0,0,349,304,1,0,0,0,349,308,1,0,0,0,349,315,1,0,0,0,349,323,1,0,0,0,349,
  	331,1,0,0,0,349,336,1,0,0,0,349,341,1,0,0,0,349,346,1,0,0,0,350,68,1,
  	0,0,0,351,352,5,42,0,0,352,70,1,0,0,0,353,354,5,47,0,0,354,72,1,0,0,0,
  	355,356,5,43,0,0,356,74,1,0,0,0,357,358,5,45,0,0,358,76,1,0,0,0,359,360,
  	5,61,0,0,360,361,5,61,0,0,361,78,1,0,0,0,362,363,5,33,0,0,363,364,5,61,
  	0,0,364,80,1,0,0,0,365,366,5,60,0,0,366,82,1,0,0,0,367,368,5,62,0,0,368,
  	84,1,0,0,0,369,370,5,60,0,0,370,371,5,60,0,0,371,86,1,0,0,0,372,373,5,
  	62,0,0,373,374,5,62,0,0,374,88,1,0,0,0,375,376,5,60,0,0,376,377,5,61,
  	0,0,377,90,1,0,0,0,378,379,5,61,0,0,379,380,5,62,0,0,380,92,1,0,0,0,381,
  	382,5,38,0,0,382,94,1,0,0,0,383,384,5,124,0,0,384,96,1,0,0,0,385,386,
  	5,98,0,0,386,387,5,120,0,0,387,388,5,111,0,0,388,389,5,114,0,0,389,98,
  	1,0,0,0,390,394,7,0,0,0,391,393,7,1,0,0,392,391,1,0,0,0,393,396,1,0,0,
  	0,394,392,1,0,0,0,394,395,1,0,0,0,395,100,1,0,0,0,396,394,1,0,0,0,397,
  	399,7,2,0,0,398,397,1,0,0,0,399,400,1,0,0,0,400,398,1,0,0,0,400,401,1,
  	0,0,0,401,402,1,0,0,0,402,403,5,105,0,0,403,404,5,54,0,0,404,405,5,52,
  	0,0,405,102,1,0,0,0,406,408,7,2,0,0,407,406,1,0,0,0,408,409,1,0,0,0,409,
  	407,1,0,0,0,409,410,1,0,0,0,410,414,1,0,0,0,411,412,5,105,0,0,412,413,
  	5,51,0,0,413,415,5,50,0,0,414,411,1,0,0,0,414,415,1,0,0,0,415,104,1,0,
  	0,0,416,418,7,2,0,0,417,416,1,0,0,0,418,419,1,0,0,0,419,417,1,0,0,0,419,
  	420,1,0,0,0,420,421,1,0,0,0,421,425,5,46,0,0,422,424,7,2,0,0,423,422,
  	1,0,0,0,424,427,1,0,0,0,425,423,1,0,0,0,425,426,1,0,0,0,426,429,1,0,0,
  	0,427,425,1,0,0,0,428,430,3,109,54,0,429,428,1,0,0,0,429,430,1,0,0,0,
  	430,106,1,0,0,0,431,433,7,2,0,0,432,431,1,0,0,0,433,434,1,0,0,0,434,432,
  	1,0,0,0,434,435,1,0,0,0,435,436,1,0,0,0,436,440,5,46,0,0,437,439,7,2,
  	0,0,438,437,1,0,0,0,439,442,1,0,0,0,440,438,1,0,0,0,440,441,1,0,0,0,441,
  	443,1,0,0,0,442,440,1,0,0,0,443,444,3,111,55,0,444,108,1,0,0,0,445,446,
  	5,100,0,0,446,110,1,0,0,0,447,448,5,102,0,0,448,112,1,0,0,0,449,453,5,
  	34,0,0,450,452,8,3,0,0,451,450,1,0,0,0,452,455,1,0,0,0,453,451,1,0,0,
  	0,453,454,1,0,0,0,454,456,1,0,0,0,455,453,1,0,0,0,456,457,5,34,0,0,457,
  	114,1,0,0,0,458,462,5,35,0,0,459,461,8,4,0,0,460,459,1,0,0,0,461,464,
  	1,0,0,0,462,460,1,0,0,0,462,463,1,0,0,0,463,465,1,0,0,0,464,462,1,0,0,
  	0,465,466,6,57,0,0,466,116,1,0,0,0,467,469,5,13,0,0,468,467,1,0,0,0,468,
  	469,1,0,0,0,469,470,1,0,0,0,470,471,5,10,0,0,471,118,1,0,0,0,472,474,
  	7,5,0,0,473,472,1,0,0,0,474,475,1,0,0,0,475,473,1,0,0,0,475,476,1,0,0,
  	0,476,477,1,0,0,0,477,478,6,59,0,0,478,120,1,0,0,0,20,0,243,255,263,271,
  	279,349,394,400,409,414,419,425,429,434,440,453,462,468,475,1,6,0,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  egrammarlexerLexerStaticData = staticData.release();
}

}

EGrammarLexer::EGrammarLexer(CharStream *input) : Lexer(input) {
  EGrammarLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *egrammarlexerLexerStaticData->atn, egrammarlexerLexerStaticData->decisionToDFA, egrammarlexerLexerStaticData->sharedContextCache);
}

EGrammarLexer::~EGrammarLexer() {
  delete _interpreter;
}

std::string EGrammarLexer::getGrammarFileName() const {
  return "EGrammar.g4";
}

const std::vector<std::string>& EGrammarLexer::getRuleNames() const {
  return egrammarlexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& EGrammarLexer::getChannelNames() const {
  return egrammarlexerLexerStaticData->channelNames;
}

const std::vector<std::string>& EGrammarLexer::getModeNames() const {
  return egrammarlexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& EGrammarLexer::getVocabulary() const {
  return egrammarlexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView EGrammarLexer::getSerializedATN() const {
  return egrammarlexerLexerStaticData->serializedATN;
}

const atn::ATN& EGrammarLexer::getATN() const {
  return *egrammarlexerLexerStaticData->atn;
}




void EGrammarLexer::initialize() {
  std::call_once(egrammarlexerLexerOnceFlag, egrammarlexerLexerInitialize);
}
