/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: dataplot.cpp
**
** plots the data on a graph - aggralinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2016
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

/*
 * Window always stays at the back in Linux, check same in Windows?
*/

#include "dataplot.h"

/*
 * Blind Constructor with no datapoints
 * Mostly Sets Parameters
*/

DataPlot::DataPlot(QWidget *parent )
{
    createClasses();
    SetPlotParameters();
    SetGridParameters();
    SetCurveParameters();

    PlotDataPoints<< QPointF (1.0,1.0) <<QPointF (-1.0, -1.0);
}

/*
 *Destructor makes sure everything is deleted
*/

DataPlot::~DataPlot()
{
//delete symbol creates a problem.
    delete Curve;
    delete Grid;
}

/*
 * creates the new classes (allocates memory)
*/

void DataPlot::createClasses()
{
    Curve = new QwtPlotCurve;
    Grid = new QwtPlotGrid;
//    Symbol = new QwtSymbol( QwtSymbol::Ellipse,
//                QBrush( Qt::red ),
//                QPen( Qt::red, 2 ),
//                QSize( 4, 4 ) );
}

/*
 * loads datapoints from string
*/

void DataPlot::SetData(const QPolygonF data)
{
    PlotDataPoints = data;

    setAxisScale(QwtPlot::yLeft, MinY(PlotDataPoints), MaxY(PlotDataPoints));
    setAxisScale(QwtPlot::yRight, MinY(PlotDataPoints), MaxY(PlotDataPoints));
    enableAxis(QwtPlot::yRight, true);
    setAxisScale(QwtPlot::xBottom, minAxisScale(), MaxX(PlotDataPoints));
    Curve->setSamples( PlotDataPoints );
    Curve->attach(this);

    resize( 1200, 800 );
    replot();
}

/*
 * a constructor function to set general grid parameters
 */

void DataPlot::SetGridParameters()
{
    Grid->attach(this);
}

/*
 * a constructor function to set general chart parameters
 * */

void DataPlot::SetPlotParameters()
{
    setTitle(QObject::tr("V-Meter Signal"));
    setAxisTitle( QwtPlot::xBottom, QObject::tr("Time"));
    setAxisTitle( QwtPlot::yLeft, QObject::tr("Signal"));
    setAxisTitle(QwtPlot::yRight, "");
    setCanvasBackground( Qt::white );

 //   insertLegend( new QwtLegend() );
}

/*
 * a constructor function to set general curve parameters
 */
void DataPlot::SetCurveParameters()
{
    Curve->setTitle( "" );
    Curve->setStyle(QwtPlotCurve::Lines);
    Curve->setRenderHint( QwtPlotItem::RenderAntialiased, true );
    Curve->setSymbol( Symbol );
}

/*
 * to set scale of chart
 */

qreal DataPlot::MaxY(const QVector<QPointF> &in)
{
    qreal rvalue = 0.0;
    qreal test = 0.0;
    QVector<QPointF> buffer = in;

    for(int i = 0; i<in.size();++i){
        test = buffer[i].ry();
        rvalue = qMax( test, rvalue);
    }
    return(rvalue);
}
qreal DataPlot::MinY(const QVector<QPointF> &in)
{
    qreal rvalue = 0.0;
    qreal test = 0.0;
    QVector<QPointF> buffer = in;

    for(int i = 0; i<in.size();++i){
        test = buffer[i].ry();
        rvalue = qMin( test, rvalue);
    }
    return(rvalue);
}
/*
 * to set scale of chart
 */

qreal DataPlot::MaxX(const QVector<QPointF> &in)
{
    qreal rvalue = 0.0;
    qreal test = 0.0;
    QVector<QPointF> buffer = in;

    for(int i = 0; i<in.size();++i){
        test = buffer[i].rx();
        rvalue = qMax( test, rvalue);
    }
    return(rvalue);
}

