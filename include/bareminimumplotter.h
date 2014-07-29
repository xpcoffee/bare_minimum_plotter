#ifndef BAREMINIMUMPLOTTER_H
#define BAREMINIMUMPLOTTER_H

#include <QMainWindow>
#include <QtWidgets>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qvalidator.h>
#include <qlocale.h>
#include <vector>
//#include <stdio.h>
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

    //	overrides
    void showEvent(QShowEvent* event);

    // 	core
    void plot();
    void plotNew(int gui_number, string x_units, string y_units, int progress);
    void plotOld(int gui_number, string x_units, string y_units, int progress);
    void vectorCombineNone(int gui_number);
    void vectorCombineIntersection(int gui_number);
    void vectorCombineUnion(int gui_number);
    void vectorCombineSubtraction(int gui_number);
    void createQPoints();
    void addGraph(int shape, int color);
    void addErrorGraph();

    // 	validation
    void print(string message);
    void printclr();
    void printError();

    // 	gui
    void clearGUI();
    void clearFormatting();
    void resetPlotWindow();
    void addVariableInput();
    void addInequalityInput();
    void addInequalityLoader(string filename = "");
    void determineInequalityOrder();
    void determineButtonStates();
    void determineTabOrder();
    QWidget *getFocusInWidget(QWidget* widget);
    QWidget *getFocusOutWidget(QWidget* widget);

    //	parsing and file i/o
    void save_JSON(QString filename);
    void open_variables(string json);

signals:
    void variableSplitterMoved(QList<int> sizes);
    void inequalitySplitterMoved(QList<int> sizes);

public slots:
    void checkAxisMode(int gui_number);
    void removeVariableInput(int gui_number);
    void removeInequalityInput(int gui_number);
    void moveInequalityInputUp(int gui_number);
    void moveInequalityInputDown(int gui_number);

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
    vector<VariableInput*> m_VariableInputs;
    vector<InequalityInput*> m_InequalityInputs;
    vector<InequalityLoader*> m_InequalityLoaders;

    //	plotting
    QString m_Title;
    int m_Graph_Count;
    int m_Prev_Combination;
    bool flag_Combination;
    Variable m_XVariable, m_YVariable;
    QVector<double> m_XResults, m_YResults, m_XResults_Problem, m_YResults_Problem;
    QVector<QPointF> m_Samples, m_Samples_Problem;

    //	gui management
    int m_variable_count;
    int m_inequality_count;
    bool flag_Saved;

    //	error handling
    bool flag_Problem;
    bool flag_Empty;
    string m_error_message;

    // 	settings
    double m_tolerance;
    string m_default_directory;

    // experimental

};

#endif // BAREMINIMUMPLOTTER_H
