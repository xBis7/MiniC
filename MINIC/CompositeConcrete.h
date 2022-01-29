#pragma once
#include "Composite.h"

class CStatement;
class CFunctionDef;
class CExpression;
class CIDENTIFIER;


class CCompileUnit : public STNode {
public:
	CCompileUnit(STNode* arg1);
	CCompileUnit(STNode* arg1, STNode* arg2);
	virtual ~CCompileUnit();
	void Visit_SyntaxTreePrinter(ofstream* dotFile, STNode* parent) override;
};

class CFunctionDef : public STNode {
public:
	CFunctionDef(STNode* id, STNode* fargs, STNode* stat);
	CFunctionDef(STNode* id, STNode* stat);
	virtual ~CFunctionDef();
};

class CStatementList : public STNode {
public:
	CStatementList(STNode* arg1);
	CStatementList(STNode* arg1, STNode* arg2);
	virtual ~CStatementList();
};

class CFargs : public STNode {
public:
	CFargs(STNode* id);
	CFargs(STNode* fargs, STNode* id);
	virtual ~CFargs();
};

class CArgs : public STNode {
public:
	CArgs(STNode* exp);
	CArgs(STNode* args, STNode* exp);
	virtual ~CArgs();
};

class CStatement : public STNode {
public:
	CStatement(NodeType type);
	CStatement(NodeType type, int num);
	CStatement(NodeType type, int num, STNode* arg);
	CStatement(NodeType type, int num, STNode* arg1, STNode* arg2);
	CStatement(NodeType type, int num, STNode* arg1, STNode* arg2, STNode* arg3);

	virtual ~CStatement();
};

class CExpStatement : public CStatement {
public:
	CExpStatement(STNode* arg);
	virtual ~CExpStatement();

	double Visit_Eval(STNode* parent) override;
};

class CEmptyStatement : public CStatement {
public:
	CEmptyStatement();
	virtual ~CEmptyStatement();
};

class CCompoundStatement : public CStatement {
public:
	CCompoundStatement();
	CCompoundStatement(STNode* stat);
	virtual ~CCompoundStatement();
};

class CIfStatement : public CStatement {
public:
	CIfStatement(STNode* exp, STNode* stat);
	CIfStatement(STNode* exp, STNode* stat, STNode* stat2);
	virtual ~CIfStatement();

	double Visit_Eval(STNode* parent) override;
};

class CWhileStatement : public CStatement {
public:
	CWhileStatement(STNode* exp, STNode* compStat);
	virtual ~CWhileStatement();

	double Visit_Eval(STNode* parent) override;
};

class CBreakStatement : public CStatement {
public:
	CBreakStatement();
	virtual ~CBreakStatement();

	double Visit_Eval(STNode* parent) override;
};

class CReturnStatement : public CStatement {
public:
	CReturnStatement(STNode* exp);
	virtual ~CReturnStatement();

	double Visit_Eval(STNode* parent) override;
};

class CExpression : public STNode {
public:
	CExpression(NodeType type);
	CExpression(NodeType type, int num, STNode* arg);
	CExpression(NodeType type, int num, STNode* arg1, STNode* arg2);
	virtual ~CExpression();
};

class CFunctionCall : public CExpression {
public:
	//declare list of formal arguments and initialize to null
	list<CIDENTIFIER*>* m_fargs = nullptr;

	//declare list of actual arguments and initialize to null
	list<CExpression*>* m_args = nullptr;

	CFunctionCall(STNode* id);
	CFunctionCall(STNode* id, STNode* args);
	virtual ~CFunctionCall();

	double Visit_Eval(STNode* parent) override;

	void actualToFormalArgs();
	double GetActualArg(int index);
};

class CAddition : public CExpression {
public:
	CAddition(STNode* arg1, STNode* arg2);
	virtual ~CAddition();

	double Visit_Eval(STNode* parent) override;
};

class CSubtraction : public CExpression {
public:
	CSubtraction(STNode* arg1, STNode* arg2);
	virtual ~CSubtraction();

	double Visit_Eval(STNode* parent) override;
};

class CMultiplication : public CExpression {
public:
	CMultiplication(STNode* arg1, STNode* arg2);
	virtual ~CMultiplication();

	double Visit_Eval(STNode* parent) override;
};

class CDivision : public CExpression {
public:
	CDivision(STNode* arg1, STNode* arg2);
	virtual ~CDivision();

	double Visit_Eval(STNode* parent) override;
};

class CNUMBER : public CExpression {
public:
	string m_name;
	double m_number;

	CNUMBER(char* text, double num);
	virtual ~CNUMBER();

	double Visit_Eval(STNode* parent) override;
};

class CExpIdentifier : public CExpression {
public:
	string m_name;
	CExpIdentifier(STNode* id);
	virtual ~CExpIdentifier();

	double Visit_Eval(STNode* parent) override;
};

class CIDENTIFIER : public CExpression {
public:
	int m_number;
	string m_name;

	CIDENTIFIER(const char* text);
	virtual ~CIDENTIFIER();

	double Visit_Eval(STNode* parent) override;
};

class CAssignment : public CExpression {
public:
	CAssignment(STNode* id, STNode* arg);
	virtual ~CAssignment();

	double Visit_Eval(STNode* parent) override;
};

class CUnaryPlus : public CExpression {
public:
	CUnaryPlus(STNode* arg);
	virtual ~CUnaryPlus();

	double Visit_Eval(STNode* parent) override;
};

class CUnaryMinus : public CExpression {
public:
	CUnaryMinus(STNode* arg);
	virtual ~CUnaryMinus();

	double Visit_Eval(STNode* parent) override;
};

class CExpAnd : public CExpression {
public:
	CExpAnd(STNode* arg1, STNode* arg2);
	virtual ~CExpAnd();

	double Visit_Eval(STNode* parent) override;
};

class CExpOr : public CExpression {
public:
	CExpOr(STNode* arg1, STNode* arg2);
	virtual ~CExpOr();

	double Visit_Eval(STNode* parent) override;
};

class CExpNot : public CExpression {
public:
	CExpNot(STNode* arg);
	virtual ~CExpNot();

	double Visit_Eval(STNode* parent) override;
};

class CExpEqual : public CExpression {
public:
	CExpEqual(STNode* arg1, STNode* arg2);
	virtual ~CExpEqual();

	double Visit_Eval(STNode* parent) override;
};

class CExpNEqual : public CExpression {
public:
	CExpNEqual(STNode* arg1, STNode* arg2);
	virtual ~CExpNEqual();

	double Visit_Eval(STNode* parent) override;
};

class CExpGT : public CExpression {
public:
	CExpGT(STNode* arg1, STNode* arg2);
	virtual ~CExpGT();

	double Visit_Eval(STNode* parent) override;
};

class CExpLT : public CExpression {
public:
	CExpLT(STNode* arg1, STNode* arg2);
	virtual ~CExpLT();

	double Visit_Eval(STNode* parent) override;
};

class CExpGTE : public CExpression {
public:
	CExpGTE(STNode* arg1, STNode* arg2);
	virtual ~CExpGTE();

	double Visit_Eval(STNode* parent) override;
};

class CExpLTE : public CExpression {
public:
	CExpLTE(STNode* arg1, STNode* arg2);
	virtual ~CExpLTE();

	double Visit_Eval(STNode* parent) override;
};

