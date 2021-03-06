#include "include/plotworker.h"


///	Public Functions
///	=================

PlotWorker::PlotWorker(QObject *parent) :
    QObject(parent),
    m_prevCombination(CombinationNone),
    m_lastMatch(0)
{
    flag_Cancel = new bool(false);
}

void PlotWorker::doWork()
{
    for (int i = 0; i < m_inequalityCount + 1; i++){
        bool skip;

        for (unsigned int j = 0; j < m_inequalityInputs.size(); j++){
            if (*flag_Cancel)
                break;

            InequalityInput *input = m_inequalityInputs[j];

            if ( input->getNumber() == i ){
                if ( input->getSkip() ) {
                    skip = true;
                    break;
                }

                input->setCancelPointer(flag_Cancel);

                plotNew(j);
            }
        }

        for (int j = 0; j < static_cast<int>(m_inequalityLoaders.size()); j++){
            if (*flag_Cancel)
                break;

            InequalityLoader *loader = m_inequalityLoaders[j];

            if ( loader->getNumber() == i ){
                if ( loader->getSkip() ) {
                    skip = true;
                    break;
                }
                plotOld(j);
            }
        }

        if (*flag_Cancel) break;
        if (skip) continue;
    }

    if (*flag_Cancel) emit progressUpdate(100, "Cancelled.");

    emit PlotWorker::memberChanges(m_variableInputs, m_inequalityInputs, m_inequalityLoaders);
    emit PlotWorker::workFinished();
}

void PlotWorker::setCancelPointer(bool *ptr){ flag_Cancel = ptr; }



///	Public Slots
///	=============

void PlotWorker::createWorker() { emit dataRequest(); }

void PlotWorker::workerInit(VarInputArray var_inputs,
                            IneqInputArray ineq_inputs,
                            IneqLoaderArray ineq_loaders,
                            Variable x_variable,
                            Variable y_variable,
                            double comparison_precision)
{
    m_variableInputs = var_inputs;
    m_inequalityInputs = ineq_inputs;
    m_inequalityLoaders = ineq_loaders;

    m_inequalityCount = m_inequalityInputs.size() + m_inequalityLoaders.size();

    m_xVariable = x_variable;
    m_yVariable = y_variable;

    m_compPrec = comparison_precision;

    doWork();
}


///	Private Functions
///	==================

void PlotWorker::plotNew(int gui_number)
{
    InequalityInput *input = m_inequalityInputs[gui_number];
    QString gui_number_str = QString::number(gui_number);

    emit progressUpdate(0, "Evaluating inequality " + gui_number_str + "...");
    input->setXYVariables(m_xVariable, m_yVariable);
    if(!input->evaluate()){
        printError();
        return;
    }

    if (*flag_Cancel)
        return;

    emit progressUpdate(40, "Combining results, inequality " + gui_number_str + "...");
    combineResults(input);

    if (*flag_Cancel)
        return;

    m_prevCombination = input->getCombination();
    if (m_prevCombination != CombinationNone){	//	do not plot if combination requested
        emit progressUpdate(100, "Done.");
        return;
    }

    if (*flag_Cancel)
        return;

    input->setX(splitPlottingVectorX(m_results));
    input->setY(splitPlottingVectorY(m_results));

    emit progressUpdate(80, "Plotting results, inequality " + gui_number_str + "..." );
    emit newGraph(m_results, input->getShape(), input->getColor(), input->getName());

    if (!m_resultsProblem.isEmpty())
        emit newErrorGraph(m_resultsProblem);

    printWarning();
    emit progressUpdate(100, "Done.");
//    flag_empty = false;
}


void PlotWorker::plotOld(int gui_number)
{
    InequalityLoader* loader = m_inequalityLoaders[gui_number];
    QString gui_number_str = QString::number(gui_number);

    loader->setPlot();

    emit progressUpdate(40, "Combining results, inequality " + gui_number_str + "...");
    combineResults(loader);

    // log combination mode
    m_prevCombination = loader->getCombination();

    // determine if need to plot
    if (m_prevCombination != CombinationNone){
        emit progressUpdate(100, "Done.");
        return;
    }

    loader->setX(splitPlottingVectorX(m_results));
    loader->setY(splitPlottingVectorY(m_results));

    emit progressUpdate(80, "Plotting results, inequality " + gui_number_str + "..." );
    emit newGraph(m_results, loader->getShape(), loader->getColor(), loader->getName());

    if (!m_resultsProblem.isEmpty())
        emit newErrorGraph(m_resultsProblem);

    printWarning();
    emit progressUpdate(100, "Done.");
}

template<typename T>
void PlotWorker::combineResults(T *inequality)
{
    switch(m_prevCombination){
    case CombinationNone:
        combinationNone(inequality);
        break;
    case CombinationIntersect:
        combinationIntersection(inequality);
        break;
    case CombinationUnion:
        combinationUnion(inequality);
        break;
    case CombinationSubtract:
        combinationSubtraction(inequality);
        break;
    default:
        break;
    }
}

template<typename T>
void PlotWorker::combinationNone(T *inequality)
{
        m_results = createPlottingVector(inequality->getX(), inequality->getY());
        m_resultsProblem = createPlottingVector(inequality->getXProblem(), inequality->getYProblem());
}

template<typename T>
void PlotWorker::combinationIntersection(T *inequality)
{
    PlottingVector old_results = m_results;
    PlottingVector new_results = createPlottingVector(inequality->getX(), inequality->getY());
    m_results.clear();

    PlottingVector *smallest = old_results.count() < new_results.count() ? &old_results : &new_results;
    PlottingVector *biggest = old_results.count() > new_results.count() ? &old_results : &new_results;

    for (int i = 0; i < smallest->count(); i++){
        if ( biggest->contains(smallest->at(i)) )
            m_results << smallest->at(i);
    }
}

template<typename T>
void PlotWorker::combinationUnion(T *inequality)
{
    PlottingVector old_results = m_results;
    PlottingVector new_results = createPlottingVector(inequality->getX(), inequality->getY());
    m_results.clear();

    PlottingVector *smallest = old_results.count() < new_results.count() ? &old_results : &new_results;
    PlottingVector *biggest = old_results.count() > new_results.count() ? &old_results : &new_results;

    for (int i = 0; i < smallest->count(); i++){
        if ( biggest->contains(smallest->at(i)) )
            continue;
        m_results << smallest->at(i);
    }
    for (int i = 0; i < biggest->count(); i++){
        m_results << biggest->at(i);
    }
}

template<typename T>
void PlotWorker::combinationSubtraction(T *inequality)
{
    PlottingVector old_results = m_results;
    PlottingVector new_results = createPlottingVector(inequality->getX(), inequality->getY());
    m_results.clear();

    for (int i = 0; i < old_results.count(); i++){
        if (new_results.contains(old_results.at(i)))
            continue;
        m_results << old_results.at(i);
    }

}

PlottingVector PlotWorker::createPlottingVector(QVector<double> x_values, QVector<double> y_values)
{
   PlottingVector points;

   for (int i = 0; i < x_values.count(); i++){
       points << QPointF(x_values[i], y_values[i]);
   }

   return points;
}

QVector<double> PlotWorker::splitPlottingVectorX(PlottingVector points){
    QVector<double> x_values;

    for (int i = 0; i < points.count(); i++){
        x_values << points[i].x();
    }

    return x_values;
}

QVector<double> PlotWorker::splitPlottingVectorY(PlottingVector points){
    QVector<double> y_values;

    for (int i = 0; i < points.count(); i++){
        y_values << points[i].y();
    }

    return y_values;
}

void PlotWorker::printError()
{
    // get all error messages
    // display messages
    for (int i = 0; i < static_cast<int>(m_inequalityInputs.size()); i++){
        m_errorMessage += m_inequalityInputs[i]->getErrors();
    }
    for (int i = 0; i < static_cast<int>(m_inequalityLoaders.size()); i++){
        m_errorMessage += m_inequalityLoaders[i]->getErrors();
    }

    emit logMessage(m_errorMessage);

    // progress bar
    emit progressUpdate(100, "Error.");
}

void PlotWorker::printWarning()
{
    // get all error messages
    // display messages
    for (int i = 0; i < static_cast<int>(m_inequalityInputs.size()); i++){
        m_errorMessage += m_inequalityInputs[i]->getErrors();
    }
    for (int i = 0; i < static_cast<int>(m_inequalityLoaders.size()); i++){
        m_errorMessage += m_inequalityLoaders[i]->getErrors();
    }

    emit logMessage(m_errorMessage);

    // progress bar
    emit progressUpdate(100, "Printing Warnings.");
}
