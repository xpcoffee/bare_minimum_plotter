/*!	Author(s):	Emerick Bosch
    Build:		0.3
    Date:		July 2014

    inequalityloader.h
    -------------------

    Description
    ============
    Provides the GUI for:
    - loading inequality
    - selecting specific plot from loaded inequality
    - plot settings

    Subwidget of BareMinimumPlotter.
*/


#ifndef INEQUALITYLOADER_H
#define INEQUALITYLOADER_H

///	Includes
///	=========

#include <QWidget>
#include <QtWidgets>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <limits>

#include "inequalityinput.h"	// CombinationMode, ComboBoxColor, ComboBoxShape
#include "bluejson.h"


///	Typedef
///	========

typedef std::numeric_limits<double> precDouble;


///	Namespaces
///	===========

using namespace std;

namespace Ui {
class InequalityLoader;
}

///	Structs
///	========

struct DetailItem
{
    int level;
    QString type;
    QString description;
    QString data1;
    QString data2;
};

///	Class
/// ======

class InequalityLoader : public QWidget
{
    Q_OBJECT

public:
    explicit InequalityLoader(QWidget *parent = 0);
    ~InequalityLoader();

    //	setters
    void setNumber(int number);
    void setCaseName(QString value);
    void loadCase(QString filename);
    void setX(QVector<double> vector);
    void setY(QVector<double> vector);
    void clearCombinationResults();
    void createDetailItem(int level, QString type, QString desc, QString data1, QString data2);

    //	getters: ui
    QString getName();
    int getNumber();
    int getShapeIndex();
    int getCombination();
    int getPlot();
    bool getSkip();
    QWidget *getFocusInWidget();
    QWidget *getFocusOutWidget();
    QColor getColor();
    QwtSymbol::Style getShape();

    //	getters: data
    QVector<double> getX();
    QVector<double> getY();
    QVector<double> getXProblem();
    QVector<double> getYProblem();
    QString getFile();
    QString getErrors();

    //	parsers
    void parseFile(QString json);
    void parsePlotData(QString json);
    void parseProblem(QString problem);

    void formatPlot();
    void formatVariables(QString json);
    void formatInequality(QString json);
    void formatCase(QString json);
    void formatExpressions(QString json);

    QString expressionToJSON();
    QString dataToJSON();

    inline void checkOK(QString message);

    //	evaluation
    void setPlot(int index = -1);

    //	gui
    void enablePositionButtons(bool flag_enable);
    void enableCombinations(bool flag_enable);
    void resetCombinations();
    void setComboBoxPlot();
    void createDetailDialog();

signals:
    void moveUp(int guiNumber);
    void moveDown (int guiNumber);
    void killThis (int guiNumber);

public slots:
    void splitterResize(QList<int> sizes);

private slots:
    void on_pushButton_Details_clicked();
    void on_pushButton_Delete_clicked();
    void on_pushButton_Up_clicked();
    void on_pushButton_Down_clicked();
    void on_checkBox_Skip_toggled(bool checked);
    void on_comboBox_Combination_currentIndexChanged(int index);
    void on_comboBox_Plot_currentIndexChanged(int index);

private:
    Ui::InequalityLoader *ui;

    //	data
    QList< QVector<double> > m_xResults, m_yResults;
    QList< QVector<double> > m_xResults_Problem, m_yResults_Problem;
    QVector<double> m_xResults_combination, m_yResults_combination;
    QStringList m_expressions;
    QList<DetailItem> m_details;

    //	meta
    int m_guiNumber;
    int m_detailLevel;
    bool flag_skip;
    bool flag_ok;
    QFile m_file;
    QString m_name;
    QString m_variables;
    QString m_errorMessage;
    QStringList m_dropDownList;

    //	evaluation
    unsigned int m_currentPlot;
};

#endif // INEQUALITYLOADER_H
