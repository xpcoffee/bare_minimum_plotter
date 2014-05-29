/*	Author(s):	Emerick Bosch
	Build:		0.1
	Date:		April 2014

	expression.cpp
	-------------
	
	"""""""""""""""""""""""""""""""""	
	"			Description			"
	"""""""""""""""""""""""""""""""""	
	
	Class Functions

	Interprets mathematical expressions from strings.
	Parses the strings iteratively into arrays.
	Arrayes solved iteratively.
*/

/*
 * TODO: check for illegal characters
 * TODO: check for undefined variables
 * TODO: check for illegal variables (sin, cos, etc)
 * TODO: check for unique variables
 * 
 * TODO: bracket multiplication: 4(3+7)2
 * TODO: holding variables
*/

//	"""""""""""""""""""""""""""""""""	
//	"			Includes			"
//	"""""""""""""""""""""""""""""""""	

	#include<string>
	#include<iostream>
    #include<stdlib.h>
	#include<sstream>
	#include<cassert>
    #include<vector>
    #include<math.h>
	#include"../include/expression.h"

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
//	"			Namespaces			"
//	"""""""""""""""""""""""""""""""""	
	using namespace std;

//	"""""""""""""""""""""""""""""""""	
//	"		Private Functions		"
//	"""""""""""""""""""""""""""""""""	

//	Setters
//	-------

void Expression::setExpression(string sInput){
	vOriginalExpression = parseExpressionArray(sInput);
    resetExpression();
    vProblemElements_Expression = checkExpressionArray(vOriginalExpression);
}


void Expression::addVariable(Variable myVar){
    assert(variableNameIsValid(myVar));
	vVariables.push_back(myVar);
}

void Expression::clearVariables(){
	vVariables.clear();
}

void Expression::resetExpression(){
	vExpression = vOriginalExpression;
}



//	Parsing
//	-------
	
bool Expression::charIsDigit(char c){ return (('0' <= c) && (c <= '9')) || (c == '.'); }

bool Expression::charIsAlpha(char c){ return (('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <= 'z')) || (c == '_'); }

bool Expression::charIsParenthesis(char c){ return (c == '(') || (c == ')'); }

bool Expression::charIsOperator(char c){
	return 	(c == '+') || (c == '-') ||
            (c == '*') || (c == '/') ||
            (c == '^');
}

bool Expression::charIsWhitespace(char c){
	return 	(c == ' ')  || (c == '\t') || (c == '\n')  || (c == '\r') ||
			(c == '\v') || (c == '\b') || (c == '\f');
}

vector<string> Expression::parseExpressionArray (string sExpression){
//	int nTerm = 0;
	// FUTURE: check if there is better way to declare dynamic array
	string sTerm = "";
	vector<string> vTermArray;
	string::iterator it = sExpression.begin();
	// flags that help determine wether a digit is start of number
	// and whether a minus is negative
	// both set to true to cater for the first term
	bool flag_newNum = true;
	bool flag_prevOp = true;
	while (it != sExpression.end()){
		if (charIsDigit(*it) || charIsAlpha(*it)){
            // if digit is the start of a new number, put it in the next term
			if (flag_newNum){
				sTerm = "";
				sTerm += *it;
			}
			else {
				sTerm += *it;
			}
			// a digit following this one, will not be the start of a new number
			flag_newNum = false;
            // this was not an operator
			flag_prevOp = false;
		} 
		else if (charIsParenthesis(*it) || charIsOperator(*it)){
			// if there was a number before this character, push it
			if (!flag_newNum){
				vTermArray.push_back(sTerm);
			}
			sTerm = "";
			sTerm += *it;
			// a digit following this term will be the start of a new number
			flag_newNum = true;
			// unless this is a '-' that shows a negative number
			// done by checking for combination of 'operator/parenthesis' followed by minus OR
			// minus is the first term (taken care of already by the if)
			if (*it == '-' && flag_prevOp) { flag_newNum = false; } 
			else { vTermArray.push_back(sTerm); }
			// if this is an operator, set the prev operator flag
			if (charIsOperator(*it)) { flag_prevOp = true; } 
			else { flag_prevOp = false; }
        } else {
            cerr << "[ERROR] Expression | parseExpressionArray() | " << "Unknown character in expression" << endl;
            assert(true);
        }
		it++;	
	}
	// if the last term is a number, push it
	if (!flag_newNum){
		vTermArray.push_back(sTerm);
	}
	nTerms = vTermArray.size();
	return vTermArray;
}

//	Checking and Error Handling
//	---------------------------

bool Expression::checkDecimalPoint(string sTerm){
   int nDecimalPoints = 0;
   bool flag_IsVar = false;
   for(string::iterator it = sTerm.begin(); it != sTerm.end(); it++) {
       flag_IsVar = flag_IsVar || charIsAlpha(*it);
       if (*it == '.' || *it == ',') { nDecimalPoints++; }
       if (nDecimalPoints > 1) { return true; }
       else if (nDecimalPoints > 0 && flag_IsVar) { return true;} // decimal points not allowed in vars
   }
   return false; // if no problem
}

bool Expression::checkIllegalChars(string){
   return false; // if no problem
}

bool Expression::checkIllegalVar(string){
   return false; // if no problem
}

bool Expression::checkOperators(string sTerm, int nTerm, int nSize, bool & flag_prev){

    if (charIsOperator(sTerm[0]) && (sTerm.size() == 1)){
        // consecutive operators
        if (flag_prev){
            return true;
        }

        // operator found, set flag
        // placement important - after flag check and before the other checks
        flag_prev = true;

        // starting or trailing operator
        if ((nTerm == 0) || (nTerm == nSize-1)){
            return true;
        }

    } else {
        flag_prev = false;
    }

   return false; // if no problem
}

vector<int> Expression::checkExpressionArray(vector<string> & vExpression){
    vector<int> vErrorTerms, vErrorParenth;
    string sTerm;
    int nTerm = 0;
    bool flag_prevOperator = false;
    bool flag_problem_ExpressionArray;
    flag_invalid = false;

    for (vector<string>::iterator it = vExpression.begin(); it != vExpression.end(); it++){
       sTerm = *it;

       flag_problem_ExpressionArray = 	checkIllegalVar(sTerm)			||
                                        checkOperators(sTerm, nTerm, static_cast<int>(vExpression.size()), flag_prevOperator)	||
                                        checkIllegalChars(sTerm) 		||
                                        checkDecimalPoint(sTerm);

       if(flag_problem_ExpressionArray) {
           cerr <<"[ERROR] checkExpressionArray() | " << "incorrect input in term: " << nTerm << endl;
           vErrorTerms.push_back(nTerm);
           flag_invalid = true;
       }

       if (sTerm[0] == '('){
           vErrorParenth.push_back(nTerm);
       }
       else if (sTerm[0] == ')'){
           if (vErrorParenth.size() == 0){
                vErrorTerms.push_back(nTerm);
                cerr <<"[ERROR] checkExpressionArray() | " << " unopened parenthesis, term: " << nTerm << endl;
           } // no previous open bracket

           else { vErrorParenth.pop_back(); }
       }

       nTerm++;
    }

    for (vector<int>::iterator it = vErrorParenth.begin(); it != vErrorParenth.end(); it++){
        vErrorTerms.push_back(*it);
        cerr <<"[ERROR] checkExpressionArray() | " << " unclosed parenthesis, term: " << *it << endl;
    }

    return vErrorTerms;
}

bool Expression::variableNameIsUnique(Variable& myVar){
    for (vector<Variable>::iterator it = vVariables.begin(); it != vVariables.end(); it++){
            if ((*it).getName() == myVar.getName()){ return false; }
    }
    return true;
}

bool Expression::variableNameIsValid(Variable & myVar){
    if (!myVar.nameIsLegal())
        cout << "[DEBUG] Expression | variableNameIsValid () | " << "variable name illegal" << endl;
    if (!variableNameIsUnique(myVar))
        cout << "[DEBUG] Expression | variableNameIsValid () | " << "variable is not unique" << endl;
    return myVar.nameIsLegal() && variableNameIsUnique(myVar);
}

//	Evaluation
//	-----------

bool Expression::doPowers(vector<string> & vExpression) {
    nTerms = vExpression.size();
    for (int i = 0; i < nTerms; i++){
        string sTerm = vExpression[i];
        // if operation found and it is the correct operation
        if (charIsOperator(sTerm[0]) && sTerm[0] == '^'){
                //do the math
                string termBeforeOperator = vExpression[i-1];
                string termAfterOperator  = vExpression[i+1];
                double result =  pow(atof(termBeforeOperator.c_str()), atof(termAfterOperator.c_str()));
                // update expression - operator and second value filled with special character
                ostringstream buffer;
                buffer << result;
                vExpression[i-1] = buffer.str();
                vExpression[i]   = COMPRESSION_CHAR;
                vExpression[i+1] = COMPRESSION_CHAR;
                // shift rest of expression into hitespace
                while(compressExpression(vExpression)){
                    // loop will auto-terminate
                }
                // if an operation was done:
                return true;
        }
    }
    //if no operation found
    return false;
}

bool Expression::doMultiplication (vector<string>& vExpression) {
	nTerms = vExpression.size();
	for (int i = 0; i < nTerms; i++){
		string sTerm = vExpression[i];
		// if operation found and it is the correct operation
		if (charIsOperator(sTerm[0]) && sTerm[0] == '*'){
				//do the math
				string termBeforeOperator = vExpression[i-1];
				string termAfterOperator  = vExpression[i+1];
                double result = atof(termBeforeOperator.c_str()) * atof(termAfterOperator.c_str());
				// update expression - operator and second value filled with special character 
				ostringstream buffer;
				buffer << result;
				vExpression[i-1] = buffer.str();
				vExpression[i]   = COMPRESSION_CHAR;
				vExpression[i+1] = COMPRESSION_CHAR;
				// shift rest of expression into hitespace
				while(compressExpression(vExpression)){
					// loop will auto-terminate	
				}
				// if an operation was done:
				return true;	
		}
	}
	// if no operation found
	return false;
}

bool Expression::doDivision (vector<string>& vExpression) {
	nTerms = vExpression.size();
	for (int i = 0; i < nTerms; i++){
		string sTerm = vExpression[i];
		// if operation found and it is the correct operation
		if (charIsOperator(sTerm[0]) && sTerm[0] == '/'){
				//do the math
				string termBeforeOperator = vExpression[i-1];
				string termAfterOperator  = vExpression[i+1];
                if (atof(termAfterOperator.c_str()) == 0)
                    throw 101;
                double result = atof(termBeforeOperator.c_str()) / atof(termAfterOperator.c_str());
				// update expression - operator and second value filled with special character 
				ostringstream buffer;
				buffer << result;
				vExpression[i-1] = buffer.str();
				vExpression[i]   = COMPRESSION_CHAR;
				vExpression[i+1] = COMPRESSION_CHAR;
				// shift rest of expression into whitespace
				while(compressExpression(vExpression)){
					// loop will auto-terminate	
				}
                return true; // if an operation was done:
        }
	}
    return false; // if no operation found
}

bool Expression::doAddition (vector<string>& vExpression) {
	nTerms = vExpression.size();
	for (int i = 0; i < nTerms; i++){
		string sTerm = vExpression[i];
		// if operation found and it is the correct operation
		if (charIsOperator(sTerm[0]) && sTerm[0] == '+'){
				//do the math
				string termBeforeOperator = vExpression[i-1];
				string termAfterOperator  = vExpression[i+1];
                double result = atof(termBeforeOperator.c_str()) + atof(termAfterOperator.c_str());
				// update expression - operator and second value filled with special character 
				ostringstream buffer;
				buffer << result;
				vExpression[i-1] = buffer.str();
				vExpression[i]   = COMPRESSION_CHAR;
				vExpression[i+1] = COMPRESSION_CHAR;
				// shift rest of expression into whitespace
				while(compressExpression(vExpression)){
					// loop will auto-terminate	
				}
				// if an operation was done:
				return true;	
		}
	}
	// if no operation found
	return false;
}

bool Expression::doSubtraction (vector<string>& vExpression) {
	nTerms = vExpression.size();
	for (int i = 0; i < nTerms; i++){
		string sTerm = vExpression[i];
		// if operation found and it is the correct operation
		if (charIsOperator(sTerm[0]) && sTerm[0] == '-' && !charIsDigit(sTerm[1])){
				//do the math
				string termBeforeOperator = vExpression[i-1];
				string termAfterOperator  = vExpression[i+1];
                double result = atof(termBeforeOperator.c_str()) - atof(termAfterOperator.c_str());
				// update expression - operator and second value filled with special character 
				ostringstream buffer;
				buffer << result;
				vExpression[i-1] = buffer.str();
				vExpression[i]   = COMPRESSION_CHAR;
				vExpression[i+1] = COMPRESSION_CHAR;
				// shift rest of expression into whitespace
				while(compressExpression(vExpression)){
					// loop will auto-terminate	
				}
				// if an operation was done:
				return true;	
		}
	}
	// if no operation found
	return false;
}

void Expression::doBasic(vector<string>& vExpression) {
    while (doPowers(vExpression)) {}
//    cout << "[INFO] doBasic() | " << "doPowers(): " << getExpression() << endl;
    while (doDivision(vExpression)) {}
//    cout << "[INFO] doBasic() | " << "doDivision(): " << getExpression() << endl;
    while (doMultiplication(vExpression)) {}
//    cout << "[INFO] doBasic() | " << "doMultiplication(): " << getExpression() << endl;
    while (doSubtraction(vExpression)) {}
//    cout << "[INFO] doBasic() | " << "doSubtraction(): " << getExpression() << endl;
    while (doAddition(vExpression)) {}
//    cout << "[INFO] doBasic() | " << "doAddition(): " << getExpression() << endl;
}

bool Expression::compressExpression(vector<string>& vExpression) {
    nTerms = vExpression.size();
    for (int i = 0; i < nTerms; i++){
        string sTerm = vExpression[i];
        // if compression character is found
        if (COMPRESSION_CHAR == sTerm[0]){
            // shift everything after the compression character  up
            for (int j = i; j < nTerms-1; j++){
                vExpression[j] = vExpression[j+1];
            }
            // delete last term
            vExpression.pop_back();
            return true;
        }
    }
    // if no whitespace is found
    return false;
}

bool Expression::doParenthesis (vector<string>& vExpression) {
    nTerms = vExpression.size();

    int nOpen = 0;
    int nRange = 0;
    string sTerm;
    bool flag_EmptyParenth;
	for (int i = 0; i < nTerms; i++){
        sTerm = vExpression[i];
		// if operation found and it is the correct operation
		if (charIsParenthesis(sTerm[0]) && sTerm[0] == '('){
			nOpen = i;
		}
		// if a closing bracket is found, parenthesis range is known 
		// do maths in parenthesis
		else if (charIsParenthesis(sTerm[0]) && sTerm[0] == ')'){
            nRange = i - nOpen;
            // if there is something in between the parentheses
            if (nRange > 1){
                flag_EmptyParenth = false;
                vector<string> sParenthesisArray;
                for (int j = 0; j < nRange -1; j++){
                    sParenthesisArray.push_back(vExpression[nOpen+1+j]);
                }
                doBasic(sParenthesisArray);
                // insert answer into expression, fill the rest of the processed
                // terms with compression characters
                vExpression[nOpen] = sParenthesisArray[0];             }
            else {
                flag_EmptyParenth = true;
                vExpression[nOpen] = "0";
            }
			for (int j = nOpen+1; j < i+1; j++){
				vExpression[j] = COMPRESSION_CHAR;
			}
			// compress expression
			while(compressExpression(vExpression)){
				// loop will auto-terminate	
			}
            // do special operations
//            cout << "[INFO] doParenthesis() | Expression before special: " << getExpression() << endl;
            doSpecial(vExpression, nOpen, flag_EmptyParenth);
			return true;
		}
	}
	return false;
}

void Expression::doSpecial(vector<string> & vExpression, int nEvalPos, bool flag_EmptyParenth){
    // if the brackets were right at the beginning, nothing before them
    // therefore no trig to do
    if (nEvalPos < 1) { return; }
    string termBeforeParenthesis = vExpression[nEvalPos-1];
    string sEvalTerm = vExpression[nEvalPos];
    double result;
    //trig functions
    if (termBeforeParenthesis == "sin"){ result = sin(atof(sEvalTerm.c_str())); }
    else if(termBeforeParenthesis == "cos"){
        assert(!flag_EmptyParenth);
        result = cos(atof(sEvalTerm.c_str()));
    }
    else if(termBeforeParenthesis == "tan"){
        assert(!flag_EmptyParenth);
        result = tan(atof(sEvalTerm.c_str()));
    }
    else if(termBeforeParenthesis == "arcsin"){
        assert(!flag_EmptyParenth);
        result = asin(atof(sEvalTerm.c_str()));
    }
    else if(termBeforeParenthesis == "arccos"){
        assert(!flag_EmptyParenth);
        result = acos(atof(sEvalTerm.c_str()));
    }
    else if(termBeforeParenthesis == "arctan"){
        assert(!flag_EmptyParenth);
        result = atan(atof(sEvalTerm.c_str()));
    }
    // exponential
    else if(termBeforeParenthesis == "exp"){
        assert(!flag_EmptyParenth);
        result = exp(atof(sEvalTerm.c_str()));
    }
    // logarythmic functions
    else if(termBeforeParenthesis == "log"){
        assert(!flag_EmptyParenth);
        result = log10(atof(sEvalTerm.c_str()));
    }
    else if(termBeforeParenthesis == "ln"){
        assert(!flag_EmptyParenth);
        result = log(atof(sEvalTerm.c_str()));
    }
    // values
    else if(termBeforeParenthesis == "pi"){
        //assert(sEvalTerm.empty());
        assert(flag_EmptyParenth);
        result = PI;
    }
        //no matching function
    else { return; }
    // can only get here if 'break' out of switch
    // update expression - operator and second value filled with special character
    ostringstream buffer;
    buffer << result;
    vExpression[nEvalPos-1] = buffer.str();
    vExpression[nEvalPos] = COMPRESSION_CHAR;
    // shift rest of expression into hitespace
    while(compressExpression(vExpression)){
        // loop will auto-terminate
    }
}

double Expression::evaluateExpression(){
    if (flag_invalid){
        // [DOCUMENTATION] 200 series = input checking
        throw 201; // [DOCUMENTATION] 201 | problem with expression array, still trying to do maths
    }
    assert(!flag_invalid);

//    cout << "[INFO] evaluateExpression | " << "vExpression before doing maths - getExpression(): " << getExpression() << endl;
    while (doParenthesis(vExpression)) {}
    // evaluate reduced expression
    doBasic(vExpression);
//    cout << "[INFO] evaluateExpression | " << "vExpression[0] after maths: " << vExpression[0] << endl;
//    cout << "[INFO] evaluateExpression | " << "vExpression[0].c_str: " << vExpression[0].c_str() << endl;
    string tmpString = vExpression[0];
//    cout << "[INFO] evaluateExpression | " << "tmpString = vExpression[0]: " << tmpString.c_str() << endl;
//    cout << "[INFO] evaluateExpression | " << "tmpString.c_str(): " << tmpString.c_str() << endl;
    double result = atof(tmpString.c_str());
//    cout << "[INFO] evaluateExpression | " << "atof(tmpString.c_str()): " << result << endl;
    resetExpression();
    return result;
}

void Expression::recEval(){
    // for all values of the current variable (current variable is global)
	for (int i = 0; i < vVariables[nCurrentVariable].size(); i++){
			int j = nCurrentVariable;
            // if this is the final nested variable;
            // substitute current values and evaluate...
            if (nCurrentVariable == static_cast<int>(vVariables.size()) - 1){
                    // do math
					subVariableValues();
                    double dResult;
                    try{
                        dResult = evaluateExpression();
                    }
                    catch(int e){
                        handleException(e);
                        resetExpression();
                        vProblemElements_Result.push_back(vResult.size());
                        dResult = 0;
                    }
                    vResult.push_back(dResult);

//                    cout << "[INFO] recEval | ";
//                    for (unsigned int k = 0; k < vVariables.size(); k++){
//							cout << "\t" << vVariables[k].getName() << ": " << vVariables[k].getCurrentValue();
//					}
//                    cout << "\tResult: " << dResult << endl;
			}
            // ...otherwise move on to the next nested variable level
            else {
					nCurrentVariable++;
					recEval();
					nCurrentVariable--;
			}
			vVariables[j].nextPosition();
	}
}

//	Output
//	------

string Expression::getStringArray(vector<string> vExpression){
	nTerms = vExpression.size();
	string sExpression = "";
    for (unsigned int i = 0; i < vExpression.size(); i++){
		sExpression += vExpression[i];
	}
	return sExpression;
}

void Expression::handleException(int e){
    cout << "[EXCEPTION] " << e << " | ";

    switch(e){
    case 101:
        cout << "Divide by 0" << endl;
        break;
    default:
        cout << "Unhandled exception." << endl;
        break;
    }
}

//	"""""""""""""""""""""""""""""""""	
//	"		Public Functions		"
//	"""""""""""""""""""""""""""""""""	

//	Output
//	------

void Expression::printExpression(){
	cout << getStringArray(vExpression) << endl;
}

//	Getters
//	-------

string Expression::getExpression(){
    return getStringArray(vExpression);
}

string Expression::getTerm(int nTerm){
    // [DOCUMENTATION] getTerm() | 0th terms are a thing in DePlot
    assert(nTerm >= 0 && nTerm < static_cast<int>(vExpression.size()));
    return vExpression[nTerm];
}

int Expression::getNumTerms(){
    return static_cast<int> (vExpression.size());
}

bool Expression::isInvalid(){
    return flag_invalid;
}

vector<int> Expression::getProblemElements_Expression(){
    return vProblemElements_Expression;
}

vector<int> Expression::getProblemElements_Result(){
    return vProblemElements_Result;
}

//	Evaluation
//	-----------
// [BREAK] combined operator -+ breaking
void Expression::subVariableValues(){
	nTerms = vExpression.size();
    for (int j = 0; j < nTerms; j++){
        for (int i = 0; i < static_cast<int>(vVariables.size()); i++){
//            cout << "[DEBUG] subVariableValues | " << "variabe name: " << vVariables[i].getName() << endl;
//            cout << "[DEBUG] subVariableValues | " << "with - to appended vExpression term: " <<
//                    std::string("-") + vExpression[j] << endl;
            if (vVariables[i].getName() == vExpression[j]){
				ostringstream buffer;
				buffer << vVariables[i].getCurrentValue();
				vExpression[j] = buffer.str();
			}
            else if (("-" + vVariables[i].getName()) == vExpression[j]){
                ostringstream buffer;
                buffer << -1 * vVariables[i].getCurrentValue();
                vExpression[j] = buffer.str();
            }
		}
	}
}

vector<double> Expression::evaluateAll(){
	nCurrentVariable = 0;
	vResult.clear();
//    cout << "[INFO] evaluateAll() | " << "Evaluating expression: " << getExpression() << endl;

	recEval();
	return vResult;
}
