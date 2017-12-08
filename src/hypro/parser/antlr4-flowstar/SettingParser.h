
// Generated from Setting.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"


	#include <map>
	#include <string>




class  SettingParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, EXPONENTIAL = 13, 
    IN = 14, PAR = 15, JUMPS = 16, URGENT = 17, GUARD = 18, RESET = 19, 
    PARALLELOTOPE = 20, BOX = 21, JUMP = 22, DEFINE = 23, COMMENT = 24, 
    EQUALS = 25, BOOLRELATION = 26, PLUS = 27, MINUS = 28, TIMES = 29, SBOPEN = 30, 
    SBCLOSE = 31, CBOPEN = 32, CBCLOSE = 33, COMMA = 34, NUMBER = 35, CONSTANT = 36, 
    VARIABLE = 37, WS = 38
  };

  enum {
    RuleSetting = 0, RuleFixedsteps = 1, RuleTime = 2, RulePlotsetting = 3, 
    RuleFilename = 4, RuleMaxjumps = 5, RulePrint = 6, RuleRemainder = 7, 
    RuleIdentity = 8, RuleFixedorders = 9, RuleCutoff = 10, RulePrecision = 11, 
    RuleReplacedexpr = 12, RuleConstantexpr = 13, RuleConnector = 14, RuleTerm = 15, 
    RulePolynom = 16, RuleInterval = 17, RuleEquation = 18, RuleConstraint = 19, 
    RuleIntervalexpr = 20, RuleConstrset = 21
  };

  SettingParser(antlr4::TokenStream *input);
  ~SettingParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  	inline const std::map<std::string, std::string>& getConstants() const { return constants; }


  class SettingContext;
  class FixedstepsContext;
  class TimeContext;
  class PlotsettingContext;
  class FilenameContext;
  class MaxjumpsContext;
  class PrintContext;
  class RemainderContext;
  class IdentityContext;
  class FixedordersContext;
  class CutoffContext;
  class PrecisionContext;
  class ReplacedexprContext;
  class ConstantexprContext;
  class ConnectorContext;
  class TermContext;
  class PolynomContext;
  class IntervalContext;
  class EquationContext;
  class ConstraintContext;
  class IntervalexprContext;
  class ConstrsetContext; 

  class  SettingContext : public antlr4::ParserRuleContext {
  public:
    SettingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FixedstepsContext *> fixedsteps();
    FixedstepsContext* fixedsteps(size_t i);
    std::vector<TimeContext *> time();
    TimeContext* time(size_t i);
    std::vector<RemainderContext *> remainder();
    RemainderContext* remainder(size_t i);
    std::vector<IdentityContext *> identity();
    IdentityContext* identity(size_t i);
    std::vector<PlotsettingContext *> plotsetting();
    PlotsettingContext* plotsetting(size_t i);
    std::vector<FixedordersContext *> fixedorders();
    FixedordersContext* fixedorders(size_t i);
    std::vector<CutoffContext *> cutoff();
    CutoffContext* cutoff(size_t i);
    std::vector<PrecisionContext *> precision();
    PrecisionContext* precision(size_t i);
    std::vector<FilenameContext *> filename();
    FilenameContext* filename(size_t i);
    std::vector<MaxjumpsContext *> maxjumps();
    MaxjumpsContext* maxjumps(size_t i);
    std::vector<PrintContext *> print();
    PrintContext* print(size_t i);

   
  };

  SettingContext* setting();

  class  FixedstepsContext : public antlr4::ParserRuleContext {
  public:
    FixedstepsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

   
  };

  FixedstepsContext* fixedsteps();

  class  TimeContext : public antlr4::ParserRuleContext {
  public:
    TimeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

   
  };

  TimeContext* time();

  class  PlotsettingContext : public antlr4::ParserRuleContext {
  public:
    PlotsettingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> VARIABLE();
    antlr4::tree::TerminalNode* VARIABLE(size_t i);

   
  };

  PlotsettingContext* plotsetting();

  class  FilenameContext : public antlr4::ParserRuleContext {
  public:
    FilenameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();

   
  };

  FilenameContext* filename();

  class  MaxjumpsContext : public antlr4::ParserRuleContext {
  public:
    MaxjumpsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

   
  };

  MaxjumpsContext* maxjumps();

  class  PrintContext : public antlr4::ParserRuleContext {
  public:
    PrintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();

   
  };

  PrintContext* print();

  class  RemainderContext : public antlr4::ParserRuleContext {
  public:
    RemainderContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPONENTIAL();

   
  };

  RemainderContext* remainder();

  class  IdentityContext : public antlr4::ParserRuleContext {
  public:
    IdentityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

   
  };

  IdentityContext* identity();

  class  FixedordersContext : public antlr4::ParserRuleContext {
  public:
    FixedordersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

   
  };

  FixedordersContext* fixedorders();

  class  CutoffContext : public antlr4::ParserRuleContext {
  public:
    CutoffContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPONENTIAL();

   
  };

  CutoffContext* cutoff();

  class  PrecisionContext : public antlr4::ParserRuleContext {
  public:
    PrecisionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

   
  };

  PrecisionContext* precision();

  class  ReplacedexprContext : public antlr4::ParserRuleContext {
  public:
    ReplacedexprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
    antlr4::tree::TerminalNode *EQUALS();
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);

   
  };

  ReplacedexprContext* replacedexpr();

  class  ConstantexprContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *constantToken = nullptr;;
    antlr4::Token *minusToken = nullptr;;
    antlr4::Token *numberToken = nullptr;;
    ConstantexprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *CONSTANT();
    antlr4::tree::TerminalNode *EQUALS();
    antlr4::tree::TerminalNode *NUMBER();
    antlr4::tree::TerminalNode *MINUS();

   
  };

  ConstantexprContext* constantexpr();

  class  ConnectorContext : public antlr4::ParserRuleContext {
  public:
    ConnectorContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();

   
  };

  ConnectorContext* connector();

  class  TermContext : public antlr4::ParserRuleContext {
  public:
    TermContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> VARIABLE();
    antlr4::tree::TerminalNode* VARIABLE(size_t i);
    std::vector<antlr4::tree::TerminalNode *> TIMES();
    antlr4::tree::TerminalNode* TIMES(size_t i);
    std::vector<ConnectorContext *> connector();
    ConnectorContext* connector(size_t i);

   
  };

  TermContext* term();

  class  PolynomContext : public antlr4::ParserRuleContext {
  public:
    PolynomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TermContext *> term();
    TermContext* term(size_t i);
    std::vector<ConnectorContext *> connector();
    ConnectorContext* connector(size_t i);

   
  };

  PolynomContext* polynom();

  class  IntervalContext : public antlr4::ParserRuleContext {
  public:
    IntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);
    std::vector<antlr4::tree::TerminalNode *> MINUS();
    antlr4::tree::TerminalNode* MINUS(size_t i);

   
  };

  IntervalContext* interval();

  class  EquationContext : public antlr4::ParserRuleContext {
  public:
    EquationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *EQUALS();
    PolynomContext *polynom();
    ConnectorContext *connector();
    IntervalContext *interval();

   
  };

  EquationContext* equation();

  class  ConstraintContext : public antlr4::ParserRuleContext {
  public:
    ConstraintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<PolynomContext *> polynom();
    PolynomContext* polynom(size_t i);
    antlr4::tree::TerminalNode *BOOLRELATION();
    antlr4::tree::TerminalNode *EQUALS();

   
  };

  ConstraintContext* constraint();

  class  IntervalexprContext : public antlr4::ParserRuleContext {
  public:
    IntervalexprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *IN();
    IntervalContext *interval();

   
  };

  IntervalexprContext* intervalexpr();

  class  ConstrsetContext : public antlr4::ParserRuleContext {
  public:
    ConstrsetContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ConstraintContext *> constraint();
    ConstraintContext* constraint(size_t i);
    std::vector<IntervalexprContext *> intervalexpr();
    IntervalexprContext* intervalexpr(size_t i);

   
  };

  ConstrsetContext* constrset();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  	std::map<std::string, std::string> constants;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

