#ifndef BAREMINIMUMPLOTTER_H
#define BAREMINIMUMPLOTTER_H

#include <QMainWindow>
#include <QtWidgets>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <qvalidator.h>
#include <qlocale.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "deplot/include/variable.h"
#include "deplot/include/inequality.h"
#include "deplot/include/expression.h"
#include "variable_input/variableinput.h"
#include "inequality_input/inequalityinput.h"
#include "inequality_loader/inequalityloader.h"

#ifndef APPROX_EQUAL_PRECISION
#define APPROX_EQUAL_PRECISION	1E-5
#endif

using namespace std;

namespace Ui {
class BareMinimumPlotter;
}

class BareMinimumPlotter : public QMainWindow
{
    Q_OBJECT

public:
    explicit BareMinimumPlotter(QWidget *parent = 0);
    ~BareMinimumPlotter();

    // 	core
    void plot();
    void plotNew(int, string, string, int);
    void plotOld(int, string, string, int);
    void vectorCombineNone(int);
    void vectorCombineIntersection(int);
    void vectorCombineUnion(int);
    void vectorCombineSubtraction(int);
    void formatGraph(int, int);
    void formatErrorGraph();
    // 	validation
    void print(string);
    void printclr();
    void printError();
    // 	gui
    void addVariableInput();
    void addInequalityInput();
    void addInequalityLoader();
    void reOrderInequalityInputs();
    void setCombinationInputs();
    void clearFormatting();
    void clearGUI();
    //	parsing and file i/o
    void save_JSON(QString);

public slots:
    void checkAxisMode(int);
    void removeVariableInput(int);
    void removeInequalityInput(int);
    void moveInequalityInputUp(int);
    void moveInequalityInputDown(int);
    void on_button_Plot_clicked();

private slots:
    void menu_about();
    void menu_open();
    void menu_saveAs();
    void menu_new();
    void on_button_AddVariable_clicked();
    void on_button_AddInequality_clicked();
    void on_pushButton_AddInequalityLoader_clicked();

private:
    Ui::BareMinimumPlotter *ui;
    //	plotter elements
    vector<VariableInput*> vVariableInputs;
    vector<InequalityInput*> vInequalityInputs;
    vector<InequalityLoader*> vInequalityLoaders;
    //	plotting
    int nGraphIndex;
    int nPrevCombination;
    bool flag_Combination;
    Variable mVariableX, mVariableY;
    QVector<double> qvX, qvY, qvX_problem, qvY_problem;
    //	gui management
    int nLatestVariableInput;
    int nLatestInequalityInput;
    //	error handling
    bool flag_Problem;
    bool flag_Empty;
    string sErrorMessage;
    // 	application context
    string sDefaultDirectory;
    // experimental

};

#endif // BAREMINIMUMPLOTTER_H
