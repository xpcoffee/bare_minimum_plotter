/*	Author(s):	Emerick Bosch
	Build:		0.1
	Date:		April 2014

	expression.
	-------------
	
	"""""""""""""""""""""""""""""""""	
	"			Description			"
	"""""""""""""""""""""""""""""""""	
	
	Class

	Interprets mathematical expressions from strings.
	Parses the strings iteratively into arrays.
	Arrayes solved iteratively.
*/

#ifndef EXPRESSION_H
#define EXPRESSION_H

//	"""""""""""""""""""""""""""""""""	
//	"			Includes			"
//	"""""""""""""""""""""""""""""""""	

#include<string>
#include<vector>
#include<iostream>
#include<locale.h>
#include<stdlib.h>
#include<iomanip>
#include"variable.h"

//	"""""""""""""""""""""""""""""""""
//	"	Preprocessor Definitions	"
//	"""""""""""""""""""""""""""""""""

    #ifndef COMPRESSION_CHAR
    #define COMPRESSION_CHAR '$'
    #endif

    #ifndef PI
    #define PI 3.141592653589793238462643383279502884
    #endif

//	"""""""""""""""""""""""""""""""""
//	"		Enumerated Types		"
//	"""""""""""""""""""""""""""""""""
    enum READABLILITY_CODES
    {
        UNINITIALIZED_COUNTER = -1,
    };

    enum INPUT_ERROR_CODES
    {
        INPUT_ERROR_UNINITIALIZED_VARIABLE = 300,
        INPUT_ERROR_INVALID_EXPRESSION = 301,
        INPUT_ERROR_PARENTH_EMPTY = 302,
        INPUT_ERROR_PARENTH_NOT_EMPTY = 303,
    };

    enum MATH_ERROR_CODES
    {
        MATH_DIVIDE_BY_ZERO = 200,
        MATH_NAN = 201,
        MATH_POLE = 202,
    };


//	"""""""""""""""""""""""""""""""""
//	"			Namespaces			"
//	"""""""""""""""""""""""""""""""""	
using namespace std;

//	"""""""""""""""""""""""""""""""""	
//	"		Class Definition: 		"
//	"			Expression			"
//	"""""""""""""""""""""""""""""""""	

class Expression
{
private:
	// member variables
    // - meta
    int nTerms;
    int nVariables;
    int nCurrentVariable;
    // - data
    vector<string> vOriginalExpression;
    vector<string> vExpression;
    vector<Variable> vVariables;
    vector<double> vResult;
    vector<int> vProblemElements_Result;
    // - error handling
    bool flag_isValid;
    string sErrorMessage;
    int nProblemTerm;
    vector<int> vProblemElements_Expression;
    // - evaluation events
    bool flag_Nan;
    bool flag_Pole;
    bool flag_DivByZero;

	// functions
    // - parsing
	bool charIsDigit(char);
	bool charIsParenthesis(char);
	bool charIsOperator(char);
    bool charIsAlpha(char);
	bool charIsWhitespace(char);
    // - validation
    bool check_DecimalPointOK(string);
    bool checkIllegalChars(string);
    bool check_CharsOK(string);
    bool check_NumbersOK(string);
    bool check_OperatorsOK(string, int, int, bool&);
    bool variableNameIsUnique(Variable&);
    bool termIsNumeric(string);
    bool termIsAlpha(string);
    bool termIsFunction(string);
    bool termIsStandardValue(string);
    // - recursive evaluation functions
    bool compressExpression(vector<string>&);
    bool doPowers(vector<string>&);
	bool doMultiplication(vector<string>&);
	bool doDivision(vector<string>&);
	bool doSubtraction(vector<string>&);
	bool doAddition(vector<string>&);
	bool doParenthesis(vector<string>&);
    // - evaluation functions
    void doSpecial(vector<string>&, int, bool);
	void doBasic(vector<string>&);
    void recEval();
    void resetExpression();
    double evaluateExpression();
    // - internal getters
    string getStringArray(vector<string>);
    // - exceptions and error handling
    void handleMathException(MATH_ERROR_CODES);
    void resetEvaluation();

public:
	// constructor
	Expression(string sExpressionString = "")
	{
        setlocale(LC_NUMERIC,"C"); // make '.' the decimal separator
        // initialize
		vOriginalExpression = parseExpressionArray(sExpressionString);
        resetExpression();
        resetEvaluation();
        // validate expression
        sErrorMessage = "";
        vProblemElements_Expression = checkExpressionArray(vExpression);
    }

	// functions
    // - parsing
	vector<string> parseExpressionArray(string);
    vector<int> checkExpressionArray(vector<string>&);
    // - evaluation
    void subVariableValues();
    vector<double> evaluateAll();
    // - setters
	void setExpression(string);
	void addVariable(Variable);
	void clearVariables();
    // - getters
	string getExpression();
    string getTerm(int);
    string getErrors();
    int getNumTerms();
    vector<int> getProblemElements_Expression();
    vector<int> getProblemElements_Result();
    // - validation
    bool isValid();
    bool charIsValid(char);
    bool variableNameIsValid(Variable&);

};



#endif
