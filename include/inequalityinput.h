/*!	Author(s):	Emerick Bosch
    Build:		0.3
    Date:		July 2014

    inequalityinput.h
    ------------------

    Description
    ============
    Provides the GUI for:
    - input into inequality
    - error feedback from inequality
    - plot settings

    Subwidget of BareMinimumPlotter.
*/

#ifndef INEQUALITYINPUT_H
#define INEQUALITYINPUT_H


///	Includes
///	=========

#include <QWidget>
#include <QtWidgets>
#include <sstream>
#include <qvalidator.h>
#include <qwt_symbol.h>

#include "inequality.h"
#include "expression.h"
#include "bluejson.h"

///	Typedef
///	========

typedef std::numeric_limits<double> precDouble;


///	Enumerated Types
///	=================

enum CombinationMode
{
    CombinationNone 		= 0,
    CombinationIntersect,
    CombinationUnion,
    CombinationSubtract,

};

enum ComboboxColor {
    Blue 		= 0,
    Green,
    Red,
    DarkBlue,
    DarkGreen,
    DarkRed,
    Grey,
    Black,
    White,
};

enum ComboboxShape {
    CrossX 		= 0,
    CrossPlus,
    Circle,
    UpTriangle,
    DownTriangle,
    Square,
    Diamond,
    Dots
};


///	Namespaces
/// ===========

namespace Ui {
class InequalityInput;
}


///	Static Variables
///	=================

static string s_combinations [] = { "none", "intersection", "union", "subtraction" };


///	Class
///	======

class InequalityInput : public QWidget
{
    Q_OBJECT

public:
    explicit InequalityInput(QWidget *parent = 0);
    ~InequalityInput();

    //	setters
    void setNumber(int);
    void setXYVariables(Variable, Variable);
    void setX(QVector<double>);
    void setY(QVector<double>);
    void enablePositionButtons(bool);
    void enableCombinations(bool);
    void resetCombinations();
    void setCancelPointer(bool *ptr);

    // 	parsers
    string expressionToJSON();
    string dataToJSON();
    string problemDataToJSON();
    void fromJSON(string);
    InequalitySymbol symbolFromString(string value);

    //	getters: ui
    QString getName();
    int getNumber();
    int getShapeIndex();
    int getCombination();
    double getPrecision();
    bool getSkip();
    QString getLeftExpression();
    QString getRightExpression();
    QString getErrors();
    QWidget *getFocusInWidget();
    QWidget *getFocusOutWidget();
    QColor getColor();
    InequalitySymbol getSymbol();
    QwtSymbol::Style getShape();
    void setLineEditTextFormat(QLineEdit* lineEdit, const QList<QTextLayout::FormatRange>& formats); /*! Vasaka*/
    void clearLineEditTextFormat(QLineEdit* lineEdit); /*! Vasaka*/

    //	getters: data
    QVector<double> getX();
    QVector<double> getY();
    QVector<double> getXProblem();
    QVector<double> getYProblem();

    //	validation
    bool highlightInvalidExpressionTerms();
    bool checkVariablesInit();

    // 	formatting
    void clearFormatting();
    void clearFields();

    //	core
    bool createInequality();
    bool addVariable(Variable variable);
    bool evaluate();

signals:
    void moveUp(int gui_number);
    void moveDown (int gui_number);
    void killThis (int gui_number);

public slots:
    void splitterResize(QList<int> sizes);

private slots:
    void on_pushButton_Up_clicked();
    void on_pushButton_Down_clicked();
    void on_pushButton_Delete_clicked();
    void on_lineEdit_Left_textChanged(const QString&);
    void on_lineEdit_Right_textChanged(const QString&);
    void on_comboBox_Interact_currentIndexChanged(int index);
    void on_checkBox_Skip_toggled(bool checked);
    void on_comboBox_Inequality_currentIndexChanged(int index);

private:
    Ui::InequalityInput *ui;

    // data
    Inequality m_inequality;
    Variable m_xVariable, m_yVariable;
    bool *flag_Cancel;

    // meta
    QString m_name;
    int m_guiNumber;
    int m_precisionIndex;
    bool flag_skip;

    // result vectors
    vector<bool> m_points;
    vector<int> m_points_problem;
    QVector<double> m_x, m_y, m_x_problem, m_y_problem;

    // error handling
    QString m_errorMessage;
    QString m_bugMail;
};

#endif // INEQUALITYINPUT_H
