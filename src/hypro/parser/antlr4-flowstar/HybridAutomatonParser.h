
// Generated from HybridAutomaton.g4 by ANTLR 4.7

#pragma once


#include "antlr4-runtime.h"




class  HybridAutomatonParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, PARALLELOTOPE = 31, 
    BOX = 32, JUMP = 33, DEFINE = 34, IN = 35, COMMENT = 36, EQUALS = 37, 
    BOOLRELATION = 38, PLUS = 39, TIMES = 40, NUMBER = 41, VARIABLE = 42, 
    WS = 43, EXPONENTIAL = 44
  };

  enum {
    RuleStart = 0, RuleVardeclaration = 1, RuleJumps = 2, RuleTransition = 3, 
    RuleFromto = 4, RuleUrgent = 5, RuleGuard = 6, RuleAllocation = 7, RuleResetfct = 8, 
    RuleAggregation = 9, RuleTerm = 10, RulePolynom = 11, RuleInterval = 12, 
    RuleEquation = 13, RuleConstraint = 14, RuleIntervalexpr = 15, RuleConstrset = 16, 
    RuleSetting = 17, RuleFixedsteps = 18, RuleTime = 19, RulePlotsetting = 20, 
    RuleFilename = 21, RuleMaxjumps = 22, RulePrint = 23, RuleRemainder = 24, 
    RuleIdentity = 25, RuleFixedorders = 26, RuleCutoff = 27, RulePrecision = 28, 
    RuleModes = 29, RuleLocation = 30, RuleActivities = 31, RuleInvariants = 32, 
    RuleInit = 33, RuleInitstate = 34
  };

  HybridAutomatonParser(antlr4::TokenStream *input);
  ~HybridAutomatonParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class StartContext;
  class VardeclarationContext;
  class JumpsContext;
  class TransitionContext;
  class FromtoContext;
  class UrgentContext;
  class GuardContext;
  class AllocationContext;
  class ResetfctContext;
  class AggregationContext;
  class TermContext;
  class PolynomContext;
  class IntervalContext;
  class EquationContext;
  class ConstraintContext;
  class IntervalexprContext;
  class ConstrsetContext;
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
  class ModesContext;
  class LocationContext;
  class ActivitiesContext;
  class InvariantsContext;
  class InitContext;
  class InitstateContext; 

  class  StartContext : public antlr4::ParserRuleContext {
  public:
    StartContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    VardeclarationContext *vardeclaration();
    SettingContext *setting();
    ModesContext *modes();
    JumpsContext *jumps();
    InitContext *init();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StartContext* start();

  class  VardeclarationContext : public antlr4::ParserRuleContext {
  public:
    VardeclarationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> VARIABLE();
    antlr4::tree::TerminalNode* VARIABLE(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VardeclarationContext* vardeclaration();

  class  JumpsContext : public antlr4::ParserRuleContext {
  public:
    JumpsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TransitionContext *> transition();
    TransitionContext* transition(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  JumpsContext* jumps();

  class  TransitionContext : public antlr4::ParserRuleContext {
  public:
    TransitionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    FromtoContext *fromto();
    std::vector<UrgentContext *> urgent();
    UrgentContext* urgent(size_t i);
    std::vector<GuardContext *> guard();
    GuardContext* guard(size_t i);
    std::vector<ResetfctContext *> resetfct();
    ResetfctContext* resetfct(size_t i);
    std::vector<AggregationContext *> aggregation();
    AggregationContext* aggregation(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TransitionContext* transition();

  class  FromtoContext : public antlr4::ParserRuleContext {
  public:
    FromtoContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> VARIABLE();
    antlr4::tree::TerminalNode* VARIABLE(size_t i);
    antlr4::tree::TerminalNode *JUMP();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FromtoContext* fromto();

  class  UrgentContext : public antlr4::ParserRuleContext {
  public:
    UrgentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UrgentContext* urgent();

  class  GuardContext : public antlr4::ParserRuleContext {
  public:
    GuardContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstrsetContext *constrset();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  GuardContext* guard();

  class  AllocationContext : public antlr4::ParserRuleContext {
  public:
    AllocationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *DEFINE();
    PolynomContext *polynom();
    IntervalContext *interval();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AllocationContext* allocation();

  class  ResetfctContext : public antlr4::ParserRuleContext {
  public:
    ResetfctContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<AllocationContext *> allocation();
    AllocationContext* allocation(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ResetfctContext* resetfct();

  class  AggregationContext : public antlr4::ParserRuleContext {
  public:
    AggregationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PARALLELOTOPE();
    antlr4::tree::TerminalNode *BOX();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AggregationContext* aggregation();

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

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TermContext* term();

  class  PolynomContext : public antlr4::ParserRuleContext {
  public:
    PolynomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<TermContext *> term();
    TermContext* term(size_t i);
    std::vector<antlr4::tree::TerminalNode *> PLUS();
    antlr4::tree::TerminalNode* PLUS(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PolynomContext* polynom();

  class  IntervalContext : public antlr4::ParserRuleContext {
  public:
    IntervalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> NUMBER();
    antlr4::tree::TerminalNode* NUMBER(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IntervalContext* interval();

  class  EquationContext : public antlr4::ParserRuleContext {
  public:
    EquationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *EQUALS();
    PolynomContext *polynom();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstraintContext* constraint();

  class  IntervalexprContext : public antlr4::ParserRuleContext {
  public:
    IntervalexprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    antlr4::tree::TerminalNode *IN();
    IntervalContext *interval();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
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

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ConstrsetContext* constrset();

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

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SettingContext* setting();

  class  FixedstepsContext : public antlr4::ParserRuleContext {
  public:
    FixedstepsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedstepsContext* fixedsteps();

  class  TimeContext : public antlr4::ParserRuleContext {
  public:
    TimeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeContext* time();

  class  PlotsettingContext : public antlr4::ParserRuleContext {
  public:
    PlotsettingContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> VARIABLE();
    antlr4::tree::TerminalNode* VARIABLE(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PlotsettingContext* plotsetting();

  class  FilenameContext : public antlr4::ParserRuleContext {
  public:
    FilenameContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FilenameContext* filename();

  class  MaxjumpsContext : public antlr4::ParserRuleContext {
  public:
    MaxjumpsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MaxjumpsContext* maxjumps();

  class  PrintContext : public antlr4::ParserRuleContext {
  public:
    PrintContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrintContext* print();

  class  RemainderContext : public antlr4::ParserRuleContext {
  public:
    RemainderContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPONENTIAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RemainderContext* remainder();

  class  IdentityContext : public antlr4::ParserRuleContext {
  public:
    IdentityContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  IdentityContext* identity();

  class  FixedordersContext : public antlr4::ParserRuleContext {
  public:
    FixedordersContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FixedordersContext* fixedorders();

  class  CutoffContext : public antlr4::ParserRuleContext {
  public:
    CutoffContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EXPONENTIAL();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CutoffContext* cutoff();

  class  PrecisionContext : public antlr4::ParserRuleContext {
  public:
    PrecisionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  PrecisionContext* precision();

  class  ModesContext : public antlr4::ParserRuleContext {
  public:
    ModesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<LocationContext *> location();
    LocationContext* location(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ModesContext* modes();

  class  LocationContext : public antlr4::ParserRuleContext {
  public:
    LocationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    ActivitiesContext *activities();
    InvariantsContext *invariants();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocationContext* location();

  class  ActivitiesContext : public antlr4::ParserRuleContext {
  public:
    ActivitiesContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<EquationContext *> equation();
    EquationContext* equation(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ActivitiesContext* activities();

  class  InvariantsContext : public antlr4::ParserRuleContext {
  public:
    InvariantsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ConstrsetContext *constrset();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InvariantsContext* invariants();

  class  InitContext : public antlr4::ParserRuleContext {
  public:
    InitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    InitstateContext *initstate();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InitContext* init();

  class  InitstateContext : public antlr4::ParserRuleContext {
  public:
    InitstateContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *VARIABLE();
    ConstrsetContext *constrset();

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  InitstateContext* initstate();


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


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};
