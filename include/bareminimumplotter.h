/*!	Author(s):	Emerick Bosch
    Build:		0.3
    Date:		July 2014

    expression.h
    -------------

    Description
    ============
    Provides the GUI for:
    - organising input widgets for inequalities and variables
    - combining, plotting, saving and opening evaluation results
    - editing evaluation settings

    Saving and opening is done in JSON format.

    Subwidgets:
    - InequalityInput
    - InequalityLoader
    - VariableInput

    First Party Code
    =================
    A custom parser was coded for use in BareMinimumPlotter and is included:
    BlueJSON	-	Emerick Bosch	-	July 2014

    Third Party Code
    =================
    Plotting functionality is provided through QWT:
    http://qwt.sourceforge.net/
    link date: 29 July 2014

*/

#ifndef BAREMINIMUMPLOTTER_H
#define BAREMINIMUMPLOTTER_H

///	Includes
///	=========

#include <QMainWindow>
#include <QtWidgets>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qvalidator.h>
#include <qlocale.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "variable.h"
#include "inequality.h"
#include "expression.h"
#include "variableinput.h"
#include "inequalityinput.h"
#include "inequalityloader.h"
#include "bluejson.h"
#include "headerscrollarea.h"


///	Enumerated Types
///	=================

enum UIMode {
    Busy = 0,
    Available
};


///	Namespaces
///	===========

using namespace std;

namespace Ui {
class BareMinimumPlotter;
}


///	Class
///	======

class BareMinimumPlotter : public QMainWindow
{
    Q_OBJECT

public:
    explicit BareMinimumPlotter(QWidget *parent = 0);
    ~BareMinimumPlotter();

    //	overrides
    void showEvent(QShowEvent *event);

    // 	plotting and evaluation
    void plot();
    void configurePlot();
    void configureAxes();
    void evaluate();
    void combine();
    void plotNew(int gui_number);
    void plotOld(int gui_number);

    template <typename input_type>
    void combineResults(input_type *inequality);

    template <typename input_type>
    void vectorCombineNone(input_type *inequality);

    template <typename input_type>
    void vectorCombineIntersection(input_type *inequality);

    template <typename input_type>
    void vectorCombineUnion(input_type *inequality);

    template <typename input_type>
    void vectorCombineSubtraction(input_type *inequality);

    bool addVariables(InequalityInput *input);
    void createQwtSamples();
    void addGraph(QwtSymbol::Style shape, QColor color);
    void addErrorGraph();


    // 	validation
    void print(QString message);
    void printclr();
    void printError();
    bool checkVariables();
    bool checkExpressions();

    // 	gui
    void setupUiCont();
    void setupInputValidation();
    void setupQwtPlot();
    void setupScrollAreas();
    void setupButtons();
    void setupDynamicUi();
    void loadCSS();
    void loadSettings();

    void clearGUI();
    void clearFormatting();
    void resetQwtPlotWindow();
    void addVariableInput();
    void addInequalityInput();
    void addInequalityLoader(QString filename = "");
    void determineInequalityOrder();
    void determineButtonStates();
    void determineTabOrder();
    void setUIMode(UIMode mode);

    QWidget *getFocusInWidget(QWidget *widget);
    QWidget *getFocusOutWidget(QWidget *widget);

    //	parsing and file i/o
    void saveCase_JSON(QString filename);
    void openCase(QString filename);
    void openVariables(string json);

signals:
    void variableSplitterMoved(QList<int> sizes);
    void inequalitySplitterMoved(QList<int> sizes);
    void plotThreadCancel();

public slots:
    void checkAxisMode(int gui_number);
    void removeVariableInput(int gui_number);
    void removeInequalityInput(int gui_number);
    void moveInequalityInputUp(int gui_number);
    void moveInequalityInputDown(int gui_number);
    void setProgress(int value, QString message);

private slots:
    void menu_about();
    void menu_open();
    void menu_saveAs();
    void menu_new();
    void menu_quit();
    void on_toolButton_AddVariable_clicked();
    void on_toolButton_AddInequality_clicked();
    void on_toolButton_Plot_clicked();
    void on_toolButton_AddInequalityLoader_clicked();
    void on_lineEdit_SettingsTolerance_editingFinished();
    void on_splitter_Variable_splitterMoved(int pos, int index);
    void on_splitter_Inequality_splitterMoved(int pos, int index);
    void on_lineEdit_PlotTitle_returnPressed();
    void on_lineEdit_PlotTitle_textChanged(const QString &arg1);
    void on_lineEdit_PlotTitle_editingFinished();

private:
    Ui::BareMinimumPlotter *ui;

    //	plotter elements
    QwtPlot *plotter;
    vector<VariableInput*> m_variableInputs;
    vector<InequalityInput*> m_inequalityInputs;
    vector<InequalityLoader*> m_inequalityLoaders;

    //	plotting
    QString m_title;
    int m_graphCount;
    int m_prevCombination;
    bool flag_combination;
    Variable m_xVariable, m_yVariable;
    QVector<double> m_xResults, m_yResults;
    QVector<double> m_xResults_problem, m_yResults_problem;
    QVector<double> m_xResults_combination, m_yResults_combination;
    QVector<QPointF> m_samples, m_samples_problem;

    //	gui management
    int m_variableCount;
    int m_inequalityCount;
    bool flag_saved;

    //	error handling
    bool flag_problem;
    bool flag_empty;
    QString m_errorMessage;

    // 	settings
    double m_compPrec;
    QString m_defaultDir;

    // experimental

};


///	Template Definitions
///	=====================

#endif // BAREMINIMUMPLOTTER_H
