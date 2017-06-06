/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: dataplot.h
**
** Header file for dataplot.cpp - aggralinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#ifndef DATAPLOT_H
#define DATAPLOT_H


#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <QStringList>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include <qwt_plot_textlabel.h>
#include <QPointF>

#include <QtCore>
#include <QDialog>
#include <QMessageBox>
#include <QTranslator>


class DataPlot : public QwtPlot
{
    Q_OBJECT
public:
    explicit DataPlot(QWidget *parent = NULL );
    ~DataPlot();
    void SetData( const QPolygonF data);

private:

    void createClasses();

    qreal MaxX(const QVector<QPointF>& in);
    qreal MaxY(const QVector<QPointF>& in);
    qreal MinY(const QVector<QPointF>& in);

    void SetCurveParameters();
    void SetGridParameters();
    void SetPlotParameters();

    QwtPlotCurve* Curve;
    QwtPlotCurve* rCurve;
    QwtPlotGrid* Grid;
    QwtPlot* Plot;
    QPolygonF PlotDataPoints;
    QwtSymbol* rSymbol;
    QwtSymbol* Symbol;

    static int endLineConst(void){ return(-1); };
    static int readingPosConst(void){ return(15); };
    static int readingWidthConst(void){ return(5); };
    static int moisturePosConst(void){ return(28); };
    static int lineSizeConst(void){ return(27); };
    static qreal maxAD(void){return(409.6);};
    static qreal minAxisScale(void){return(0.0);};
    static qreal maxYAxisScale(void){return(20.0);};
    static qreal maxXAxisScale(void){return(409.5);};

};

#endif // DATAPLOT_H
