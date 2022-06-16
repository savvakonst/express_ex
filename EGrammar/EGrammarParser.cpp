
// Generated from EGrammar.g4 by ANTLR 4.10.1


#include "EGrammarListener.h"

#include "EGrammarParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct EGrammarParserStaticData final {
  EGrammarParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  EGrammarParserStaticData(const EGrammarParserStaticData&) = delete;
  EGrammarParserStaticData(EGrammarParserStaticData&&) = delete;
  EGrammarParserStaticData& operator=(const EGrammarParserStaticData&) = delete;
  EGrammarParserStaticData& operator=(EGrammarParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag egrammarParserOnceFlag;
EGrammarParserStaticData *egrammarParserStaticData = nullptr;

void egrammarParserInitialize() {
  assert(egrammarParserStaticData == nullptr);
  auto staticData = std::make_unique<EGrammarParserStaticData>(
    std::vector<std::string>{
      "start", "stat", "func", "line", "args", "unaryBInFunc", "tConvBInFunc", 
      "left_expr", "right_expr", "expr"
    },
    std::vector<std::string>{
      "", "','", "'='", "'param'", "'('", "')'", "'out'", "'func'", "'function'", 
      "':'", "';'", "'end'", "'return '", "'integrate'", "'convolve'", "'recconvolve'", 
      "'range'", "'decimation'", "'shift'", "'**'", "'^'", "'>='", "'\\u003F'", 
      "'['", "']'", "", "", "", "", "", "'float'", "'int'", "'double'", 
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
  	4,1,60,268,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,
  	7,7,7,2,8,7,8,2,9,7,9,1,0,5,0,22,8,0,10,0,12,0,25,9,0,1,0,3,0,28,8,0,
  	1,0,5,0,31,8,0,10,0,12,0,34,9,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,5,1,43,8,
  	1,10,1,12,1,46,9,1,1,1,1,1,1,1,1,1,1,1,1,1,5,1,54,8,1,10,1,12,1,57,9,
  	1,3,1,59,8,1,1,1,1,1,1,1,1,1,1,1,5,1,66,8,1,10,1,12,1,69,9,1,3,1,71,8,
  	1,1,2,1,2,1,2,1,2,1,2,1,2,3,2,79,8,2,1,2,5,2,82,8,2,10,2,12,2,85,9,2,
  	1,2,1,2,5,2,89,8,2,10,2,12,2,92,9,2,4,2,94,8,2,11,2,12,2,95,1,2,1,2,1,
  	3,1,3,1,3,1,3,1,3,1,3,1,3,5,3,107,8,3,10,3,12,3,110,9,3,3,3,112,8,3,1,
  	4,1,4,1,4,5,4,117,8,4,10,4,12,4,120,9,4,1,5,1,5,1,6,1,6,1,7,1,7,1,8,1,
  	8,1,9,1,9,1,9,1,9,1,9,1,9,5,9,136,8,9,10,9,12,9,139,9,9,1,9,1,9,1,9,1,
  	9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,3,9,155,8,9,1,9,1,9,1,9,1,9,
  	1,9,1,9,1,9,1,9,1,9,3,9,166,8,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,5,9,175,8,
  	9,10,9,12,9,178,9,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
  	9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,
  	1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,5,9,218,8,9,10,9,12,9,221,9,9,1,9,1,9,
  	3,9,225,8,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,
  	9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,1,9,
  	1,9,1,9,1,9,1,9,5,9,263,8,9,10,9,12,9,266,9,9,1,9,0,1,18,10,0,2,4,6,8,
  	10,12,14,16,18,0,12,1,0,7,8,1,0,10,11,1,0,25,29,1,0,30,33,1,0,51,54,1,
  	0,19,20,1,0,35,36,1,0,37,38,1,0,43,44,1,0,41,42,2,0,21,21,45,45,1,0,39,
  	40,303,0,23,1,0,0,0,2,70,1,0,0,0,4,72,1,0,0,0,6,111,1,0,0,0,8,113,1,0,
  	0,0,10,121,1,0,0,0,12,123,1,0,0,0,14,125,1,0,0,0,16,127,1,0,0,0,18,224,
  	1,0,0,0,20,22,5,59,0,0,21,20,1,0,0,0,22,25,1,0,0,0,23,21,1,0,0,0,23,24,
  	1,0,0,0,24,32,1,0,0,0,25,23,1,0,0,0,26,28,3,2,1,0,27,26,1,0,0,0,27,28,
  	1,0,0,0,28,29,1,0,0,0,29,31,5,59,0,0,30,27,1,0,0,0,31,34,1,0,0,0,32,30,
  	1,0,0,0,32,33,1,0,0,0,33,35,1,0,0,0,34,32,1,0,0,0,35,36,5,0,0,1,36,1,
  	1,0,0,0,37,71,3,4,2,0,38,71,3,6,3,0,39,44,5,50,0,0,40,41,5,1,0,0,41,43,
  	5,50,0,0,42,40,1,0,0,0,43,46,1,0,0,0,44,42,1,0,0,0,44,45,1,0,0,0,45,47,
  	1,0,0,0,46,44,1,0,0,0,47,48,5,2,0,0,48,49,5,3,0,0,49,58,5,4,0,0,50,55,
  	5,57,0,0,51,52,5,1,0,0,52,54,5,57,0,0,53,51,1,0,0,0,54,57,1,0,0,0,55,
  	53,1,0,0,0,55,56,1,0,0,0,56,59,1,0,0,0,57,55,1,0,0,0,58,50,1,0,0,0,58,
  	59,1,0,0,0,59,60,1,0,0,0,60,71,5,5,0,0,61,62,5,6,0,0,62,67,5,50,0,0,63,
  	64,5,1,0,0,64,66,5,50,0,0,65,63,1,0,0,0,66,69,1,0,0,0,67,65,1,0,0,0,67,
  	68,1,0,0,0,68,71,1,0,0,0,69,67,1,0,0,0,70,37,1,0,0,0,70,38,1,0,0,0,70,
  	39,1,0,0,0,70,61,1,0,0,0,71,3,1,0,0,0,72,73,7,0,0,0,73,74,5,50,0,0,74,
  	75,5,4,0,0,75,76,3,8,4,0,76,78,5,5,0,0,77,79,5,9,0,0,78,77,1,0,0,0,78,
  	79,1,0,0,0,79,83,1,0,0,0,80,82,5,59,0,0,81,80,1,0,0,0,82,85,1,0,0,0,83,
  	81,1,0,0,0,83,84,1,0,0,0,84,93,1,0,0,0,85,83,1,0,0,0,86,90,3,2,1,0,87,
  	89,5,59,0,0,88,87,1,0,0,0,89,92,1,0,0,0,90,88,1,0,0,0,90,91,1,0,0,0,91,
  	94,1,0,0,0,92,90,1,0,0,0,93,86,1,0,0,0,94,95,1,0,0,0,95,93,1,0,0,0,95,
  	96,1,0,0,0,96,97,1,0,0,0,97,98,7,1,0,0,98,5,1,0,0,0,99,100,5,50,0,0,100,
  	101,5,2,0,0,101,112,3,18,9,0,102,103,5,12,0,0,103,108,3,18,9,0,104,105,
  	5,1,0,0,105,107,3,18,9,0,106,104,1,0,0,0,107,110,1,0,0,0,108,106,1,0,
  	0,0,108,109,1,0,0,0,109,112,1,0,0,0,110,108,1,0,0,0,111,99,1,0,0,0,111,
  	102,1,0,0,0,112,7,1,0,0,0,113,118,5,50,0,0,114,115,5,1,0,0,115,117,5,
  	50,0,0,116,114,1,0,0,0,117,120,1,0,0,0,118,116,1,0,0,0,118,119,1,0,0,
  	0,119,9,1,0,0,0,120,118,1,0,0,0,121,122,7,2,0,0,122,11,1,0,0,0,123,124,
  	7,3,0,0,124,13,1,0,0,0,125,126,3,18,9,0,126,15,1,0,0,0,127,128,3,18,9,
  	0,128,17,1,0,0,0,129,130,6,9,-1,0,130,131,5,50,0,0,131,132,5,4,0,0,132,
  	137,3,18,9,0,133,134,5,1,0,0,134,136,3,18,9,0,135,133,1,0,0,0,136,139,
  	1,0,0,0,137,135,1,0,0,0,137,138,1,0,0,0,138,140,1,0,0,0,139,137,1,0,0,
  	0,140,141,5,5,0,0,141,225,1,0,0,0,142,143,5,13,0,0,143,144,5,4,0,0,144,
  	145,3,18,9,0,145,146,5,5,0,0,146,225,1,0,0,0,147,148,5,14,0,0,148,149,
  	5,4,0,0,149,150,3,18,9,0,150,151,5,1,0,0,151,154,3,18,9,0,152,153,5,1,
  	0,0,153,155,5,57,0,0,154,152,1,0,0,0,154,155,1,0,0,0,155,156,1,0,0,0,
  	156,157,5,5,0,0,157,225,1,0,0,0,158,159,5,15,0,0,159,160,5,4,0,0,160,
  	161,3,18,9,0,161,162,5,1,0,0,162,165,3,18,9,0,163,164,5,1,0,0,164,166,
  	5,57,0,0,165,163,1,0,0,0,165,166,1,0,0,0,166,167,1,0,0,0,167,168,5,5,
  	0,0,168,225,1,0,0,0,169,170,5,16,0,0,170,171,5,4,0,0,171,176,3,18,9,0,
  	172,173,5,1,0,0,173,175,3,18,9,0,174,172,1,0,0,0,175,178,1,0,0,0,176,
  	174,1,0,0,0,176,177,1,0,0,0,177,179,1,0,0,0,178,176,1,0,0,0,179,180,5,
  	5,0,0,180,225,1,0,0,0,181,182,5,17,0,0,182,183,5,4,0,0,183,184,3,18,9,
  	0,184,185,5,1,0,0,185,186,3,18,9,0,186,187,5,5,0,0,187,225,1,0,0,0,188,
  	189,5,18,0,0,189,190,5,4,0,0,190,191,3,18,9,0,191,192,5,1,0,0,192,193,
  	3,18,9,0,193,194,5,5,0,0,194,225,1,0,0,0,195,196,3,10,5,0,196,197,5,4,
  	0,0,197,198,3,18,9,0,198,199,5,5,0,0,199,225,1,0,0,0,200,201,3,12,6,0,
  	201,202,5,4,0,0,202,203,3,18,9,0,203,204,5,5,0,0,204,225,1,0,0,0,205,
  	206,5,38,0,0,206,225,3,18,9,5,207,225,7,4,0,0,208,225,5,50,0,0,209,210,
  	5,4,0,0,210,211,3,18,9,0,211,212,5,5,0,0,212,225,1,0,0,0,213,214,5,23,
  	0,0,214,219,3,18,9,0,215,216,5,1,0,0,216,218,3,18,9,0,217,215,1,0,0,0,
  	218,221,1,0,0,0,219,217,1,0,0,0,219,220,1,0,0,0,220,222,1,0,0,0,221,219,
  	1,0,0,0,222,223,5,24,0,0,223,225,1,0,0,0,224,129,1,0,0,0,224,142,1,0,
  	0,0,224,147,1,0,0,0,224,158,1,0,0,0,224,169,1,0,0,0,224,181,1,0,0,0,224,
  	188,1,0,0,0,224,195,1,0,0,0,224,200,1,0,0,0,224,205,1,0,0,0,224,207,1,
  	0,0,0,224,208,1,0,0,0,224,209,1,0,0,0,224,213,1,0,0,0,225,264,1,0,0,0,
  	226,227,10,16,0,0,227,228,7,5,0,0,228,263,3,18,9,17,229,230,10,15,0,0,
  	230,231,7,6,0,0,231,263,3,18,9,16,232,233,10,14,0,0,233,234,7,7,0,0,234,
  	263,3,18,9,15,235,236,10,13,0,0,236,237,7,8,0,0,237,263,3,18,9,14,238,
  	239,10,12,0,0,239,240,7,9,0,0,240,263,3,18,9,13,241,242,10,11,0,0,242,
  	243,7,10,0,0,243,263,3,18,9,12,244,245,10,10,0,0,245,246,7,11,0,0,246,
  	263,3,18,9,11,247,248,10,9,0,0,248,249,5,47,0,0,249,263,3,18,9,10,250,
  	251,10,8,0,0,251,252,5,48,0,0,252,263,3,18,9,9,253,254,10,7,0,0,254,255,
  	5,49,0,0,255,263,3,18,9,8,256,257,10,6,0,0,257,258,5,22,0,0,258,259,3,
  	14,7,0,259,260,5,9,0,0,260,261,3,16,8,0,261,263,1,0,0,0,262,226,1,0,0,
  	0,262,229,1,0,0,0,262,232,1,0,0,0,262,235,1,0,0,0,262,238,1,0,0,0,262,
  	241,1,0,0,0,262,244,1,0,0,0,262,247,1,0,0,0,262,250,1,0,0,0,262,253,1,
  	0,0,0,262,256,1,0,0,0,263,266,1,0,0,0,264,262,1,0,0,0,264,265,1,0,0,0,
  	265,19,1,0,0,0,266,264,1,0,0,0,23,23,27,32,44,55,58,67,70,78,83,90,95,
  	108,111,118,137,154,165,176,219,224,262,264
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  egrammarParserStaticData = staticData.release();
}

}

EGrammarParser::EGrammarParser(TokenStream *input) : EGrammarParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

EGrammarParser::EGrammarParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  EGrammarParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *egrammarParserStaticData->atn, egrammarParserStaticData->decisionToDFA, egrammarParserStaticData->sharedContextCache, options);
}

EGrammarParser::~EGrammarParser() {
  delete _interpreter;
}

const atn::ATN& EGrammarParser::getATN() const {
  return *egrammarParserStaticData->atn;
}

std::string EGrammarParser::getGrammarFileName() const {
  return "EGrammar.g4";
}

const std::vector<std::string>& EGrammarParser::getRuleNames() const {
  return egrammarParserStaticData->ruleNames;
}

const dfa::Vocabulary& EGrammarParser::getVocabulary() const {
  return egrammarParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView EGrammarParser::getSerializedATN() const {
  return egrammarParserStaticData->serializedATN;
}


//----------------- StartContext ------------------------------------------------------------------

EGrammarParser::StartContext::StartContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* EGrammarParser::StartContext::EOF() {
  return getToken(EGrammarParser::EOF, 0);
}

std::vector<tree::TerminalNode *> EGrammarParser::StartContext::NEWLINE() {
  return getTokens(EGrammarParser::NEWLINE);
}

tree::TerminalNode* EGrammarParser::StartContext::NEWLINE(size_t i) {
  return getToken(EGrammarParser::NEWLINE, i);
}

std::vector<EGrammarParser::StatContext *> EGrammarParser::StartContext::stat() {
  return getRuleContexts<EGrammarParser::StatContext>();
}

EGrammarParser::StatContext* EGrammarParser::StartContext::stat(size_t i) {
  return getRuleContext<EGrammarParser::StatContext>(i);
}


size_t EGrammarParser::StartContext::getRuleIndex() const {
  return EGrammarParser::RuleStart;
}

void EGrammarParser::StartContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStart(this);
}

void EGrammarParser::StartContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStart(this);
}

EGrammarParser::StartContext* EGrammarParser::start() {
  StartContext *_localctx = _tracker.createInstance<StartContext>(_ctx, getState());
  enterRule(_localctx, 0, EGrammarParser::RuleStart);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(23);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        setState(20);
        match(EGrammarParser::NEWLINE); 
      }
      setState(25);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
    }
    setState(32);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << EGrammarParser::T__5)
      | (1ULL << EGrammarParser::T__6)
      | (1ULL << EGrammarParser::T__7)
      | (1ULL << EGrammarParser::T__11)
      | (1ULL << EGrammarParser::ID)
      | (1ULL << EGrammarParser::NEWLINE))) != 0)) {
      setState(27);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << EGrammarParser::T__5)
        | (1ULL << EGrammarParser::T__6)
        | (1ULL << EGrammarParser::T__7)
        | (1ULL << EGrammarParser::T__11)
        | (1ULL << EGrammarParser::ID))) != 0)) {
        setState(26);
        stat();
      }
      setState(29);
      match(EGrammarParser::NEWLINE);
      setState(34);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(35);
    match(EGrammarParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StatContext ------------------------------------------------------------------

EGrammarParser::StatContext::StatContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t EGrammarParser::StatContext::getRuleIndex() const {
  return EGrammarParser::RuleStat;
}

void EGrammarParser::StatContext::copyFrom(StatContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AssignParamContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> EGrammarParser::AssignParamContext::ID() {
  return getTokens(EGrammarParser::ID);
}

tree::TerminalNode* EGrammarParser::AssignParamContext::ID(size_t i) {
  return getToken(EGrammarParser::ID, i);
}

std::vector<tree::TerminalNode *> EGrammarParser::AssignParamContext::STRINGLITERAL() {
  return getTokens(EGrammarParser::STRINGLITERAL);
}

tree::TerminalNode* EGrammarParser::AssignParamContext::STRINGLITERAL(size_t i) {
  return getToken(EGrammarParser::STRINGLITERAL, i);
}

EGrammarParser::AssignParamContext::AssignParamContext(StatContext *ctx) { copyFrom(ctx); }

void EGrammarParser::AssignParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignParam(this);
}
void EGrammarParser::AssignParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignParam(this);
}
//----------------- NoneContext ------------------------------------------------------------------

EGrammarParser::LineContext* EGrammarParser::NoneContext::line() {
  return getRuleContext<EGrammarParser::LineContext>(0);
}

EGrammarParser::NoneContext::NoneContext(StatContext *ctx) { copyFrom(ctx); }

void EGrammarParser::NoneContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNone(this);
}
void EGrammarParser::NoneContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNone(this);
}
//----------------- OutParamContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> EGrammarParser::OutParamContext::ID() {
  return getTokens(EGrammarParser::ID);
}

tree::TerminalNode* EGrammarParser::OutParamContext::ID(size_t i) {
  return getToken(EGrammarParser::ID, i);
}

EGrammarParser::OutParamContext::OutParamContext(StatContext *ctx) { copyFrom(ctx); }

void EGrammarParser::OutParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOutParam(this);
}
void EGrammarParser::OutParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOutParam(this);
}
//----------------- DefFuncContext ------------------------------------------------------------------

EGrammarParser::FuncContext* EGrammarParser::DefFuncContext::func() {
  return getRuleContext<EGrammarParser::FuncContext>(0);
}

EGrammarParser::DefFuncContext::DefFuncContext(StatContext *ctx) { copyFrom(ctx); }

void EGrammarParser::DefFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDefFunc(this);
}
void EGrammarParser::DefFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDefFunc(this);
}
EGrammarParser::StatContext* EGrammarParser::stat() {
  StatContext *_localctx = _tracker.createInstance<StatContext>(_ctx, getState());
  enterRule(_localctx, 2, EGrammarParser::RuleStat);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(70);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<EGrammarParser::DefFuncContext>(_localctx);
      enterOuterAlt(_localctx, 1);
      setState(37);
      func();
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<EGrammarParser::NoneContext>(_localctx);
      enterOuterAlt(_localctx, 2);
      setState(38);
      line();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<EGrammarParser::AssignParamContext>(_localctx);
      enterOuterAlt(_localctx, 3);
      setState(39);
      match(EGrammarParser::ID);
      setState(44);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == EGrammarParser::T__0) {
        setState(40);
        match(EGrammarParser::T__0);
        setState(41);
        match(EGrammarParser::ID);
        setState(46);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(47);
      match(EGrammarParser::T__1);
      setState(48);
      antlrcpp::downCast<AssignParamContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__2);
      setState(49);
      match(EGrammarParser::T__3);
      setState(58);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == EGrammarParser::STRINGLITERAL) {
        setState(50);
        match(EGrammarParser::STRINGLITERAL);
        setState(55);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == EGrammarParser::T__0) {
          setState(51);
          match(EGrammarParser::T__0);
          setState(52);
          match(EGrammarParser::STRINGLITERAL);
          setState(57);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
      }
      setState(60);
      match(EGrammarParser::T__4);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<EGrammarParser::OutParamContext>(_localctx);
      enterOuterAlt(_localctx, 4);
      setState(61);
      antlrcpp::downCast<OutParamContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__5);

      setState(62);
      match(EGrammarParser::ID);
      setState(67);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == EGrammarParser::T__0) {
        setState(63);
        match(EGrammarParser::T__0);
        setState(64);
        match(EGrammarParser::ID);
        setState(69);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncContext ------------------------------------------------------------------

EGrammarParser::FuncContext::FuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* EGrammarParser::FuncContext::ID() {
  return getToken(EGrammarParser::ID, 0);
}

EGrammarParser::ArgsContext* EGrammarParser::FuncContext::args() {
  return getRuleContext<EGrammarParser::ArgsContext>(0);
}

std::vector<tree::TerminalNode *> EGrammarParser::FuncContext::NEWLINE() {
  return getTokens(EGrammarParser::NEWLINE);
}

tree::TerminalNode* EGrammarParser::FuncContext::NEWLINE(size_t i) {
  return getToken(EGrammarParser::NEWLINE, i);
}

std::vector<EGrammarParser::StatContext *> EGrammarParser::FuncContext::stat() {
  return getRuleContexts<EGrammarParser::StatContext>();
}

EGrammarParser::StatContext* EGrammarParser::FuncContext::stat(size_t i) {
  return getRuleContext<EGrammarParser::StatContext>(i);
}


size_t EGrammarParser::FuncContext::getRuleIndex() const {
  return EGrammarParser::RuleFunc;
}

void EGrammarParser::FuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunc(this);
}

void EGrammarParser::FuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunc(this);
}

EGrammarParser::FuncContext* EGrammarParser::func() {
  FuncContext *_localctx = _tracker.createInstance<FuncContext>(_ctx, getState());
  enterRule(_localctx, 4, EGrammarParser::RuleFunc);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    _la = _input->LA(1);
    if (!(_la == EGrammarParser::T__6

    || _la == EGrammarParser::T__7)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
    setState(73);
    match(EGrammarParser::ID);
    setState(74);
    match(EGrammarParser::T__3);
    setState(75);
    args();
    setState(76);
    match(EGrammarParser::T__4);
    setState(78);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == EGrammarParser::T__8) {
      setState(77);
      match(EGrammarParser::T__8);
    }
    setState(83);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == EGrammarParser::NEWLINE) {
      setState(80);
      match(EGrammarParser::NEWLINE);
      setState(85);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(93); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(86);
      stat();
      setState(90);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == EGrammarParser::NEWLINE) {
        setState(87);
        match(EGrammarParser::NEWLINE);
        setState(92);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(95); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << EGrammarParser::T__5)
      | (1ULL << EGrammarParser::T__6)
      | (1ULL << EGrammarParser::T__7)
      | (1ULL << EGrammarParser::T__11)
      | (1ULL << EGrammarParser::ID))) != 0));
    setState(97);
    antlrcpp::downCast<FuncContext *>(_localctx)->SPEC_SYM = _input->LT(1);
    _la = _input->LA(1);
    if (!(_la == EGrammarParser::T__9

    || _la == EGrammarParser::T__10)) {
      antlrcpp::downCast<FuncContext *>(_localctx)->SPEC_SYM = _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LineContext ------------------------------------------------------------------

EGrammarParser::LineContext::LineContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t EGrammarParser::LineContext::getRuleIndex() const {
  return EGrammarParser::RuleLine;
}

void EGrammarParser::LineContext::copyFrom(LineContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AssignRetParamContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::AssignRetParamContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::AssignRetParamContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

EGrammarParser::AssignRetParamContext::AssignRetParamContext(LineContext *ctx) { copyFrom(ctx); }

void EGrammarParser::AssignRetParamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignRetParam(this);
}
void EGrammarParser::AssignRetParamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignRetParam(this);
}
//----------------- AssignContext ------------------------------------------------------------------

tree::TerminalNode* EGrammarParser::AssignContext::ID() {
  return getToken(EGrammarParser::ID, 0);
}

EGrammarParser::ExprContext* EGrammarParser::AssignContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}

EGrammarParser::AssignContext::AssignContext(LineContext *ctx) { copyFrom(ctx); }

void EGrammarParser::AssignContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssign(this);
}
void EGrammarParser::AssignContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssign(this);
}
EGrammarParser::LineContext* EGrammarParser::line() {
  LineContext *_localctx = _tracker.createInstance<LineContext>(_ctx, getState());
  enterRule(_localctx, 6, EGrammarParser::RuleLine);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(111);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case EGrammarParser::ID: {
        _localctx = _tracker.createInstance<EGrammarParser::AssignContext>(_localctx);
        enterOuterAlt(_localctx, 1);
        setState(99);
        match(EGrammarParser::ID);
        setState(100);
        match(EGrammarParser::T__1);
        setState(101);
        expr(0);
        break;
      }

      case EGrammarParser::T__11: {
        _localctx = _tracker.createInstance<EGrammarParser::AssignRetParamContext>(_localctx);
        enterOuterAlt(_localctx, 2);
        setState(102);
        antlrcpp::downCast<AssignRetParamContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__11);

        setState(103);
        expr(0);
        setState(108);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == EGrammarParser::T__0) {
          setState(104);
          match(EGrammarParser::T__0);
          setState(105);
          expr(0);
          setState(110);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArgsContext ------------------------------------------------------------------

EGrammarParser::ArgsContext::ArgsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> EGrammarParser::ArgsContext::ID() {
  return getTokens(EGrammarParser::ID);
}

tree::TerminalNode* EGrammarParser::ArgsContext::ID(size_t i) {
  return getToken(EGrammarParser::ID, i);
}


size_t EGrammarParser::ArgsContext::getRuleIndex() const {
  return EGrammarParser::RuleArgs;
}

void EGrammarParser::ArgsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArgs(this);
}

void EGrammarParser::ArgsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArgs(this);
}

EGrammarParser::ArgsContext* EGrammarParser::args() {
  ArgsContext *_localctx = _tracker.createInstance<ArgsContext>(_ctx, getState());
  enterRule(_localctx, 8, EGrammarParser::RuleArgs);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(113);
    match(EGrammarParser::ID);
    setState(118);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == EGrammarParser::T__0) {
      setState(114);
      match(EGrammarParser::T__0);
      setState(115);
      match(EGrammarParser::ID);
      setState(120);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnaryBInFuncContext ------------------------------------------------------------------

EGrammarParser::UnaryBInFuncContext::UnaryBInFuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* EGrammarParser::UnaryBInFuncContext::LOG() {
  return getToken(EGrammarParser::LOG, 0);
}

tree::TerminalNode* EGrammarParser::UnaryBInFuncContext::LOG10() {
  return getToken(EGrammarParser::LOG10, 0);
}

tree::TerminalNode* EGrammarParser::UnaryBInFuncContext::COS() {
  return getToken(EGrammarParser::COS, 0);
}

tree::TerminalNode* EGrammarParser::UnaryBInFuncContext::SIN() {
  return getToken(EGrammarParser::SIN, 0);
}

tree::TerminalNode* EGrammarParser::UnaryBInFuncContext::EXP() {
  return getToken(EGrammarParser::EXP, 0);
}


size_t EGrammarParser::UnaryBInFuncContext::getRuleIndex() const {
  return EGrammarParser::RuleUnaryBInFunc;
}

void EGrammarParser::UnaryBInFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnaryBInFunc(this);
}

void EGrammarParser::UnaryBInFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnaryBInFunc(this);
}

EGrammarParser::UnaryBInFuncContext* EGrammarParser::unaryBInFunc() {
  UnaryBInFuncContext *_localctx = _tracker.createInstance<UnaryBInFuncContext>(_ctx, getState());
  enterRule(_localctx, 10, EGrammarParser::RuleUnaryBInFunc);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(121);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << EGrammarParser::LOG)
      | (1ULL << EGrammarParser::LOG10)
      | (1ULL << EGrammarParser::COS)
      | (1ULL << EGrammarParser::SIN)
      | (1ULL << EGrammarParser::EXP))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TConvBInFuncContext ------------------------------------------------------------------

EGrammarParser::TConvBInFuncContext::TConvBInFuncContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* EGrammarParser::TConvBInFuncContext::C_FLOAT() {
  return getToken(EGrammarParser::C_FLOAT, 0);
}

tree::TerminalNode* EGrammarParser::TConvBInFuncContext::C_INT() {
  return getToken(EGrammarParser::C_INT, 0);
}

tree::TerminalNode* EGrammarParser::TConvBInFuncContext::C_DOUBLE() {
  return getToken(EGrammarParser::C_DOUBLE, 0);
}

tree::TerminalNode* EGrammarParser::TConvBInFuncContext::C_INT64() {
  return getToken(EGrammarParser::C_INT64, 0);
}


size_t EGrammarParser::TConvBInFuncContext::getRuleIndex() const {
  return EGrammarParser::RuleTConvBInFunc;
}

void EGrammarParser::TConvBInFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTConvBInFunc(this);
}

void EGrammarParser::TConvBInFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTConvBInFunc(this);
}

EGrammarParser::TConvBInFuncContext* EGrammarParser::tConvBInFunc() {
  TConvBInFuncContext *_localctx = _tracker.createInstance<TConvBInFuncContext>(_ctx, getState());
  enterRule(_localctx, 12, EGrammarParser::RuleTConvBInFunc);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(123);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << EGrammarParser::C_FLOAT)
      | (1ULL << EGrammarParser::C_INT)
      | (1ULL << EGrammarParser::C_DOUBLE)
      | (1ULL << EGrammarParser::C_INT64))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Left_exprContext ------------------------------------------------------------------

EGrammarParser::Left_exprContext::Left_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

EGrammarParser::ExprContext* EGrammarParser::Left_exprContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}


size_t EGrammarParser::Left_exprContext::getRuleIndex() const {
  return EGrammarParser::RuleLeft_expr;
}

void EGrammarParser::Left_exprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLeft_expr(this);
}

void EGrammarParser::Left_exprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLeft_expr(this);
}

EGrammarParser::Left_exprContext* EGrammarParser::left_expr() {
  Left_exprContext *_localctx = _tracker.createInstance<Left_exprContext>(_ctx, getState());
  enterRule(_localctx, 14, EGrammarParser::RuleLeft_expr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(125);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Right_exprContext ------------------------------------------------------------------

EGrammarParser::Right_exprContext::Right_exprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

EGrammarParser::ExprContext* EGrammarParser::Right_exprContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}


size_t EGrammarParser::Right_exprContext::getRuleIndex() const {
  return EGrammarParser::RuleRight_expr;
}

void EGrammarParser::Right_exprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRight_expr(this);
}

void EGrammarParser::Right_exprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRight_expr(this);
}

EGrammarParser::Right_exprContext* EGrammarParser::right_expr() {
  Right_exprContext *_localctx = _tracker.createInstance<Right_exprContext>(_ctx, getState());
  enterRule(_localctx, 16, EGrammarParser::RuleRight_expr);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(127);
    expr(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

EGrammarParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t EGrammarParser::ExprContext::getRuleIndex() const {
  return EGrammarParser::RuleExpr;
}

void EGrammarParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ParensContext ------------------------------------------------------------------

EGrammarParser::ExprContext* EGrammarParser::ParensContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}

EGrammarParser::ParensContext::ParensContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::ParensContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterParens(this);
}
void EGrammarParser::ParensContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitParens(this);
}
//----------------- ConstContext ------------------------------------------------------------------

tree::TerminalNode* EGrammarParser::ConstContext::DOUBLE() {
  return getToken(EGrammarParser::DOUBLE, 0);
}

tree::TerminalNode* EGrammarParser::ConstContext::FLOAT() {
  return getToken(EGrammarParser::FLOAT, 0);
}

tree::TerminalNode* EGrammarParser::ConstContext::INT64() {
  return getToken(EGrammarParser::INT64, 0);
}

tree::TerminalNode* EGrammarParser::ConstContext::INT() {
  return getToken(EGrammarParser::INT, 0);
}

EGrammarParser::ConstContext::ConstContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::ConstContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConst(this);
}
void EGrammarParser::ConstContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConst(this);
}
//----------------- ShiftContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::ShiftContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::ShiftContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

EGrammarParser::ShiftContext::ShiftContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::ShiftContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterShift(this);
}
void EGrammarParser::ShiftContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitShift(this);
}
//----------------- BitwiseXorContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::BitwiseXorContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::BitwiseXorContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::BitwiseXorContext::BITWISE_XOR() {
  return getToken(EGrammarParser::BITWISE_XOR, 0);
}

EGrammarParser::BitwiseXorContext::BitwiseXorContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::BitwiseXorContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBitwiseXor(this);
}
void EGrammarParser::BitwiseXorContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBitwiseXor(this);
}
//----------------- CondExprContext ------------------------------------------------------------------

EGrammarParser::ExprContext* EGrammarParser::CondExprContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}

EGrammarParser::Left_exprContext* EGrammarParser::CondExprContext::left_expr() {
  return getRuleContext<EGrammarParser::Left_exprContext>(0);
}

EGrammarParser::Right_exprContext* EGrammarParser::CondExprContext::right_expr() {
  return getRuleContext<EGrammarParser::Right_exprContext>(0);
}

EGrammarParser::CondExprContext::CondExprContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::CondExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCondExpr(this);
}
void EGrammarParser::CondExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCondExpr(this);
}
//----------------- RangeContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::RangeContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::RangeContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

EGrammarParser::RangeContext::RangeContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::RangeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRange(this);
}
void EGrammarParser::RangeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRange(this);
}
//----------------- CallRecConvolveContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::CallRecConvolveContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::CallRecConvolveContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::CallRecConvolveContext::STRINGLITERAL() {
  return getToken(EGrammarParser::STRINGLITERAL, 0);
}

EGrammarParser::CallRecConvolveContext::CallRecConvolveContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::CallRecConvolveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallRecConvolve(this);
}
void EGrammarParser::CallRecConvolveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallRecConvolve(this);
}
//----------------- SmallArrayDefinitionContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::SmallArrayDefinitionContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::SmallArrayDefinitionContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

EGrammarParser::SmallArrayDefinitionContext::SmallArrayDefinitionContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::SmallArrayDefinitionContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSmallArrayDefinition(this);
}
void EGrammarParser::SmallArrayDefinitionContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSmallArrayDefinition(this);
}
//----------------- MoreeqLesseqContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::MoreeqLesseqContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::MoreeqLesseqContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::MoreeqLesseqContext::LESSEQ() {
  return getToken(EGrammarParser::LESSEQ, 0);
}

EGrammarParser::MoreeqLesseqContext::MoreeqLesseqContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::MoreeqLesseqContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMoreeqLesseq(this);
}
void EGrammarParser::MoreeqLesseqContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMoreeqLesseq(this);
}
//----------------- BitwiseAndContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::BitwiseAndContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::BitwiseAndContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::BitwiseAndContext::BITWISE_AND() {
  return getToken(EGrammarParser::BITWISE_AND, 0);
}

EGrammarParser::BitwiseAndContext::BitwiseAndContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::BitwiseAndContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBitwiseAnd(this);
}
void EGrammarParser::BitwiseAndContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBitwiseAnd(this);
}
//----------------- MulDivContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::MulDivContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::MulDivContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::MulDivContext::MUL() {
  return getToken(EGrammarParser::MUL, 0);
}

tree::TerminalNode* EGrammarParser::MulDivContext::DIV() {
  return getToken(EGrammarParser::DIV, 0);
}

EGrammarParser::MulDivContext::MulDivContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::MulDivContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMulDiv(this);
}
void EGrammarParser::MulDivContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMulDiv(this);
}
//----------------- DecimationContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::DecimationContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::DecimationContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

EGrammarParser::DecimationContext::DecimationContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::DecimationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDecimation(this);
}
void EGrammarParser::DecimationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDecimation(this);
}
//----------------- CallTConvBInFuncContext ------------------------------------------------------------------

EGrammarParser::TConvBInFuncContext* EGrammarParser::CallTConvBInFuncContext::tConvBInFunc() {
  return getRuleContext<EGrammarParser::TConvBInFuncContext>(0);
}

EGrammarParser::ExprContext* EGrammarParser::CallTConvBInFuncContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}

EGrammarParser::CallTConvBInFuncContext::CallTConvBInFuncContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::CallTConvBInFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallTConvBInFunc(this);
}
void EGrammarParser::CallTConvBInFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallTConvBInFunc(this);
}
//----------------- BitwiseShiftContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::BitwiseShiftContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::BitwiseShiftContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::BitwiseShiftContext::LSH() {
  return getToken(EGrammarParser::LSH, 0);
}

tree::TerminalNode* EGrammarParser::BitwiseShiftContext::RSH() {
  return getToken(EGrammarParser::RSH, 0);
}

EGrammarParser::BitwiseShiftContext::BitwiseShiftContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::BitwiseShiftContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBitwiseShift(this);
}
void EGrammarParser::BitwiseShiftContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBitwiseShift(this);
}
//----------------- PowContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::PowContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::PowContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

EGrammarParser::PowContext::PowContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::PowContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPow(this);
}
void EGrammarParser::PowContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPow(this);
}
//----------------- BitwiseOrContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::BitwiseOrContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::BitwiseOrContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::BitwiseOrContext::BITWISE_OR() {
  return getToken(EGrammarParser::BITWISE_OR, 0);
}

EGrammarParser::BitwiseOrContext::BitwiseOrContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::BitwiseOrContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBitwiseOr(this);
}
void EGrammarParser::BitwiseOrContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBitwiseOr(this);
}
//----------------- IdContext ------------------------------------------------------------------

tree::TerminalNode* EGrammarParser::IdContext::ID() {
  return getToken(EGrammarParser::ID, 0);
}

EGrammarParser::IdContext::IdContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::IdContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterId(this);
}
void EGrammarParser::IdContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitId(this);
}
//----------------- EqualityContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::EqualityContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::EqualityContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::EqualityContext::EQ() {
  return getToken(EGrammarParser::EQ, 0);
}

tree::TerminalNode* EGrammarParser::EqualityContext::NEQ() {
  return getToken(EGrammarParser::NEQ, 0);
}

EGrammarParser::EqualityContext::EqualityContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::EqualityContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEquality(this);
}
void EGrammarParser::EqualityContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEquality(this);
}
//----------------- CallConvolveContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::CallConvolveContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::CallConvolveContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::CallConvolveContext::STRINGLITERAL() {
  return getToken(EGrammarParser::STRINGLITERAL, 0);
}

EGrammarParser::CallConvolveContext::CallConvolveContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::CallConvolveContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallConvolve(this);
}
void EGrammarParser::CallConvolveContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallConvolve(this);
}
//----------------- MoreLessContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::MoreLessContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::MoreLessContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::MoreLessContext::MORE_() {
  return getToken(EGrammarParser::MORE_, 0);
}

tree::TerminalNode* EGrammarParser::MoreLessContext::LESS_() {
  return getToken(EGrammarParser::LESS_, 0);
}

EGrammarParser::MoreLessContext::MoreLessContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::MoreLessContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMoreLess(this);
}
void EGrammarParser::MoreLessContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMoreLess(this);
}
//----------------- CallUnaryBInFuncContext ------------------------------------------------------------------

EGrammarParser::UnaryBInFuncContext* EGrammarParser::CallUnaryBInFuncContext::unaryBInFunc() {
  return getRuleContext<EGrammarParser::UnaryBInFuncContext>(0);
}

EGrammarParser::ExprContext* EGrammarParser::CallUnaryBInFuncContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}

EGrammarParser::CallUnaryBInFuncContext::CallUnaryBInFuncContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::CallUnaryBInFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallUnaryBInFunc(this);
}
void EGrammarParser::CallUnaryBInFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallUnaryBInFunc(this);
}
//----------------- AddSubContext ------------------------------------------------------------------

std::vector<EGrammarParser::ExprContext *> EGrammarParser::AddSubContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::AddSubContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

tree::TerminalNode* EGrammarParser::AddSubContext::ADD() {
  return getToken(EGrammarParser::ADD, 0);
}

tree::TerminalNode* EGrammarParser::AddSubContext::SUB() {
  return getToken(EGrammarParser::SUB, 0);
}

EGrammarParser::AddSubContext::AddSubContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::AddSubContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAddSub(this);
}
void EGrammarParser::AddSubContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAddSub(this);
}
//----------------- CallFuncContext ------------------------------------------------------------------

tree::TerminalNode* EGrammarParser::CallFuncContext::ID() {
  return getToken(EGrammarParser::ID, 0);
}

std::vector<EGrammarParser::ExprContext *> EGrammarParser::CallFuncContext::expr() {
  return getRuleContexts<EGrammarParser::ExprContext>();
}

EGrammarParser::ExprContext* EGrammarParser::CallFuncContext::expr(size_t i) {
  return getRuleContext<EGrammarParser::ExprContext>(i);
}

EGrammarParser::CallFuncContext::CallFuncContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::CallFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallFunc(this);
}
void EGrammarParser::CallFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallFunc(this);
}
//----------------- InvContext ------------------------------------------------------------------

tree::TerminalNode* EGrammarParser::InvContext::SUB() {
  return getToken(EGrammarParser::SUB, 0);
}

EGrammarParser::ExprContext* EGrammarParser::InvContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}

EGrammarParser::InvContext::InvContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::InvContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInv(this);
}
void EGrammarParser::InvContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInv(this);
}
//----------------- CallIntegrateContext ------------------------------------------------------------------

EGrammarParser::ExprContext* EGrammarParser::CallIntegrateContext::expr() {
  return getRuleContext<EGrammarParser::ExprContext>(0);
}

EGrammarParser::CallIntegrateContext::CallIntegrateContext(ExprContext *ctx) { copyFrom(ctx); }

void EGrammarParser::CallIntegrateContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallIntegrate(this);
}
void EGrammarParser::CallIntegrateContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<EGrammarListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallIntegrate(this);
}

EGrammarParser::ExprContext* EGrammarParser::expr() {
   return expr(0);
}

EGrammarParser::ExprContext* EGrammarParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  EGrammarParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  EGrammarParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 18;
  enterRecursionRule(_localctx, 18, EGrammarParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(224);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<CallFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(130);
      match(EGrammarParser::ID);
      setState(131);
      match(EGrammarParser::T__3);

      setState(132);
      expr(0);
      setState(137);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == EGrammarParser::T__0) {
        setState(133);
        match(EGrammarParser::T__0);
        setState(134);
        expr(0);
        setState(139);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(140);
      match(EGrammarParser::T__4);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CallIntegrateContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(142);
      antlrcpp::downCast<CallIntegrateContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__12);
      setState(143);
      match(EGrammarParser::T__3);
      setState(144);
      expr(0);
      setState(145);
      match(EGrammarParser::T__4);
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<CallConvolveContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(147);
      antlrcpp::downCast<CallConvolveContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__13);
      setState(148);
      match(EGrammarParser::T__3);
      setState(149);
      expr(0);
      setState(150);
      match(EGrammarParser::T__0);
      setState(151);
      expr(0);
      setState(154);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == EGrammarParser::T__0) {
        setState(152);
        match(EGrammarParser::T__0);
        setState(153);
        match(EGrammarParser::STRINGLITERAL);
      }
      setState(156);
      match(EGrammarParser::T__4);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<CallRecConvolveContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(158);
      antlrcpp::downCast<CallRecConvolveContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__14);
      setState(159);
      match(EGrammarParser::T__3);
      setState(160);
      expr(0);
      setState(161);
      match(EGrammarParser::T__0);
      setState(162);
      expr(0);
      setState(165);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == EGrammarParser::T__0) {
        setState(163);
        match(EGrammarParser::T__0);
        setState(164);
        match(EGrammarParser::STRINGLITERAL);
      }
      setState(167);
      match(EGrammarParser::T__4);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<RangeContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(169);
      antlrcpp::downCast<RangeContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__15);
      setState(170);
      match(EGrammarParser::T__3);

      setState(171);
      expr(0);
      setState(176);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == EGrammarParser::T__0) {
        setState(172);
        match(EGrammarParser::T__0);
        setState(173);
        expr(0);
        setState(178);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(179);
      match(EGrammarParser::T__4);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<DecimationContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(181);
      antlrcpp::downCast<DecimationContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__16);
      setState(182);
      match(EGrammarParser::T__3);
      setState(183);
      expr(0);
      setState(184);
      match(EGrammarParser::T__0);
      setState(185);
      expr(0);
      setState(186);
      match(EGrammarParser::T__4);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<ShiftContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(188);
      antlrcpp::downCast<ShiftContext *>(_localctx)->SPEC_SYM = match(EGrammarParser::T__17);
      setState(189);
      match(EGrammarParser::T__3);
      setState(190);
      expr(0);
      setState(191);
      match(EGrammarParser::T__0);
      setState(192);
      expr(0);
      setState(193);
      match(EGrammarParser::T__4);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<CallUnaryBInFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(195);
      unaryBInFunc();
      setState(196);
      match(EGrammarParser::T__3);
      setState(197);
      expr(0);
      setState(198);
      match(EGrammarParser::T__4);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<CallTConvBInFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(200);
      tConvBInFunc();
      setState(201);
      match(EGrammarParser::T__3);
      setState(202);
      expr(0);
      setState(203);
      match(EGrammarParser::T__4);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<InvContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(205);
      match(EGrammarParser::SUB);
      setState(206);
      expr(5);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<ConstContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(207);
      _la = _input->LA(1);
      if (!((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << EGrammarParser::INT64)
        | (1ULL << EGrammarParser::INT)
        | (1ULL << EGrammarParser::DOUBLE)
        | (1ULL << EGrammarParser::FLOAT))) != 0))) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<IdContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(208);
      match(EGrammarParser::ID);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<ParensContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(209);
      match(EGrammarParser::T__3);
      setState(210);
      expr(0);
      setState(211);
      match(EGrammarParser::T__4);
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<SmallArrayDefinitionContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(213);
      match(EGrammarParser::T__22);

      setState(214);
      expr(0);
      setState(219);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == EGrammarParser::T__0) {
        setState(215);
        match(EGrammarParser::T__0);
        setState(216);
        expr(0);
        setState(221);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(222);
      match(EGrammarParser::T__23);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(264);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(262);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 21, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<PowContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(226);

          if (!(precpred(_ctx, 16))) throw FailedPredicateException(this, "precpred(_ctx, 16)");
          setState(227);
          _la = _input->LA(1);
          if (!(_la == EGrammarParser::T__18

          || _la == EGrammarParser::T__19)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(228);
          expr(17);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<MulDivContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(229);

          if (!(precpred(_ctx, 15))) throw FailedPredicateException(this, "precpred(_ctx, 15)");
          setState(230);
          antlrcpp::downCast<MulDivContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == EGrammarParser::MUL

          || _la == EGrammarParser::DIV)) {
            antlrcpp::downCast<MulDivContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(231);
          expr(16);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<AddSubContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(232);

          if (!(precpred(_ctx, 14))) throw FailedPredicateException(this, "precpred(_ctx, 14)");
          setState(233);
          antlrcpp::downCast<AddSubContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == EGrammarParser::ADD

          || _la == EGrammarParser::SUB)) {
            antlrcpp::downCast<AddSubContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(234);
          expr(15);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<BitwiseShiftContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(235);

          if (!(precpred(_ctx, 13))) throw FailedPredicateException(this, "precpred(_ctx, 13)");
          setState(236);
          antlrcpp::downCast<BitwiseShiftContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == EGrammarParser::LSH

          || _la == EGrammarParser::RSH)) {
            antlrcpp::downCast<BitwiseShiftContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(237);
          expr(14);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<MoreLessContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(238);

          if (!(precpred(_ctx, 12))) throw FailedPredicateException(this, "precpred(_ctx, 12)");
          setState(239);
          antlrcpp::downCast<MoreLessContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == EGrammarParser::LESS_

          || _la == EGrammarParser::MORE_)) {
            antlrcpp::downCast<MoreLessContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(240);
          expr(13);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<MoreeqLesseqContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(241);

          if (!(precpred(_ctx, 11))) throw FailedPredicateException(this, "precpred(_ctx, 11)");
          setState(242);
          antlrcpp::downCast<MoreeqLesseqContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == EGrammarParser::T__20

          || _la == EGrammarParser::LESSEQ)) {
            antlrcpp::downCast<MoreeqLesseqContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(243);
          expr(12);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<EqualityContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(244);

          if (!(precpred(_ctx, 10))) throw FailedPredicateException(this, "precpred(_ctx, 10)");
          setState(245);
          antlrcpp::downCast<EqualityContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == EGrammarParser::EQ

          || _la == EGrammarParser::NEQ)) {
            antlrcpp::downCast<EqualityContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(246);
          expr(11);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<BitwiseAndContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(247);

          if (!(precpred(_ctx, 9))) throw FailedPredicateException(this, "precpred(_ctx, 9)");
          setState(248);
          match(EGrammarParser::BITWISE_AND);
          setState(249);
          expr(10);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<BitwiseOrContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(250);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(251);
          match(EGrammarParser::BITWISE_OR);
          setState(252);
          expr(9);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<BitwiseXorContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(253);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(254);
          match(EGrammarParser::BITWISE_XOR);
          setState(255);
          expr(8);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<CondExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(256);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(257);
          match(EGrammarParser::T__21);
          setState(258);
          left_expr();
          setState(259);
          match(EGrammarParser::T__8);
          setState(260);
          right_expr();
          break;
        }

        default:
          break;
        } 
      }
      setState(266);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

bool EGrammarParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 9: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool EGrammarParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 16);
    case 1: return precpred(_ctx, 15);
    case 2: return precpred(_ctx, 14);
    case 3: return precpred(_ctx, 13);
    case 4: return precpred(_ctx, 12);
    case 5: return precpred(_ctx, 11);
    case 6: return precpred(_ctx, 10);
    case 7: return precpred(_ctx, 9);
    case 8: return precpred(_ctx, 8);
    case 9: return precpred(_ctx, 7);
    case 10: return precpred(_ctx, 6);

  default:
    break;
  }
  return true;
}

void EGrammarParser::initialize() {
  std::call_once(egrammarParserOnceFlag, egrammarParserInitialize);
}
