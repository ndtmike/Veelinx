/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: parser.cpp
**
** parser.cpp - Veelinx software
** Divides the input stream up into data packets
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#include "parser.h"
//#include "datapacket.h"

Parser::Parser( QWidget*, const QByteArray &in ) //no parameter for QWidget Pointer it is not used
{
    QString strcopy( in );
    QStringList strlistcopy = strcopy.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    Data = new DataSet;

    DataSet::Test newtest = CreateTest(strlistcopy);

    Data->AddTest(newtest);
}

Parser::~Parser()
{
}

std::vector<DataSet::Test>::iterator Parser::GetBeginItr()
{
    return(Data->GetBeginItr());
}

std::vector<DataSet::Test>::iterator Parser::GetEndItr()
{
    return(Data->GetEndItr());
}


std::vector<long> Parser::CreateADC(QStringList sl)
{
    std::vector<long> return_vector;

    for(auto i = sl.begin(); i != sl.end(); ++i){
        bool ok = true;
        QString line = *i;
        return_vector.push_back( line.toLong( &ok ));
    }

    return( return_vector);
}

DataSet::Prop Parser::CreateProp(QStringList sl)
{
    DataSet::Prop return_prop;
    int n;

    n = sl.indexOf(QRegExp(".*Gain:.*")); // .* wildcard for QRegExp
    return_prop.PropAmpGain = QStringtoAmpGain( sl.at(n));

    n=sl.indexOf(QRegExp(".*Measured.*"));
    return_prop.PropCalc = QStringtoCalc( sl.at(n));

    n=sl.indexOf(QRegExp(".*Young's modulus:.*"));
    return_prop.PropEMethod = QStringtoEMethod(sl.at(n));

    n=sl.indexOf(QRegExp(".*Rate:.*"));
    return_prop.PropRate = QStringtoRate(sl.at(n));

    n=sl.indexOf(QRegExp(".*feet/second"));
    if(n != -1 ){
        return_prop.PropUnits = DataSet::Imperial;
    }else{
        return_prop.PropUnits = DataSet::Metric;
    }

    n=sl.indexOf(QRegExp("Measured P-VELOCITY:.*"));
    if(n != -1 ){
        return_prop.PropCalc = DataSet::Vel;
    }else{
        return_prop.PropCalc = DataSet::Dist;
    }

    n=sl.indexOf(QRegExp("WAVE TYPE:.*"));
    return_prop.PropWave = QStringtoWave(sl.at(n));

    n=sl.indexOf(QRegExp(".*Density:.*"));
    return_prop.PropDensity = QStringtoDensity( sl.at(n) );

    return(return_prop);
}


DataSet::Test Parser::CreateTest(QStringList sl)
{
    DataSet::Test return_test;
    int n;//index
    bool ok = true;

    QStringList header = sl.mid(0,8);
    QStringList adc_data = sl.mid(8,sl.size()-8);

    return_test.TestProp = CreateProp( header);

    n = header.indexOf(QRegExp(".*Date/Time:.*"));
    return_test.TestTime = QStringtoDateTime( sl.at(n) );

    n = header.indexOf(QRegExp("Transit Time:.*"));
    return_test.TransitTime = sl.at(n).mid(14,7).toDouble( &ok );

    return_test.ADC = CreateADC(adc_data);

    return(return_test);
}


QByteArray Parser::RemoveAscii(QByteArray &in){

    QByteArray out;

    for(qint16 i=0; i<in.size();++i){
        out[i]=in[i]-48;//removes ascii offset
    }
    return(out);
}

qint64 Parser::HexQByteArraytoInt(QByteArray &in)
{
    QByteArray data = in;
    bool ok;
    qint64 out;

    data = data.toHex(); //convert to hex string
    out = data.toInt( &ok, 16 );
    return(out);
}

DataSet::AmpGain Parser::QStringtoAmpGain(QString in)
{
    DataSet::AmpGain return_value;

    if( in.contains(" 1 ")){
        return_value = DataSet::Gain_1;
    }else if( in.contains(" 5 ")){
        return_value = DataSet::Gain_5;
    }else if( in.contains(" 10 ")){
        return_value = DataSet::Gain_10;
    }else if( in.contains(" 25 ")){
        return_value = DataSet::Gain_25;
    }else if( in.contains(" 50 ")){
        return_value = DataSet::Gain_50;
    }else if( in.contains(" 100 ")){
        return_value = DataSet::Gain_100;
    }else if( in.contains(" 250 ")){
        return_value = DataSet::Gain_250;
    }else if( in.contains(" 500 ")){
        return_value = DataSet::Gain_500;
    }

    return(return_value);
}

DataSet::Calc Parser::QStringtoCalc(QString in)
{
    DataSet::Calc return_value;

    if( in.contains("-Distance ")){
        return_value = DataSet::Dist;
    }else if( in.contains("-Velocity ")){
        return_value = DataSet::Vel;
    }
    return(return_value);
}


tm Parser::QStringtoDateTime(QString in)
{
    int working_int;
    tm return_tm;

    int current_pos = in.indexOf( "Time");
    bool ok = true;
    QString working_qstring = "";

    current_pos += 9;//position of month

    working_qstring = in.mid( current_pos, 2 );//month
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_mon = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//day
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_mday = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//year
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_year = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//hour
    working_int = working_qstring.toInt( &ok );
    if(in.indexOf("PM") != -1)
        working_int +=12;
    return_tm.tm_hour = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//min
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_min = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//sec
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_sec = working_int;

    return(return_tm);
}

unsigned Parser::QStringtoDensity(QString in)
{
    bool ok = false;
    int pos = in.indexOf(QString("Density:"));
    QString return_string = in.mid( pos + 8, 5 );
    unsigned return_unsigned = return_string.toUInt( &ok );

    return(return_unsigned);
}

DataSet::EMethod Parser::QStringtoEMethod(QString in)
{
    DataSet::EMethod return_value;

    if( in.contains("SIMPLE")){
        return_value = DataSet::SimpleE;
    }else if( in.contains("ARBITRARY")){
        return_value = DataSet::ArbMu;
    }else if( in.contains("DERIVED")){
        return_value = DataSet::DerivedMu;
    }

    return(return_value);
}


DataSet::Rate Parser::QStringtoRate(QString in)
{
    DataSet::Rate return_value;

    if( in.contains(tr(" 250 kHz "))){
        return_value = DataSet::RATE_250KHZ;
    }else if( in.contains(tr(" 500 kHz "))){
        return_value = DataSet::RATE_500KHZ;
    }else if( in.contains(tr(" 1.0 mHz "))){
        return_value = DataSet::RATE_1000KHZ;
    }else if( in.contains(tr(" 2.0 mHz "))){
        return_value = DataSet::RATE_2000KHZ;
    }

    return(return_value);
}

DataSet::Wave Parser::QStringtoWave(QString in)
{
    DataSet::Wave return_value;

    if( in.contains("'P'")){
        return_value = DataSet::PWave;
    }else if( in.contains("'S'")){
        return_value = DataSet::SWave;
    }

    return(return_value);
}

QString Parser::ToQStrAmpGain(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    switch (Data->GetTest( itr_test ).TestProp.PropAmpGain) {
    case DataSet::Gain_1:
         return_string = tr(" 1 ");
         break;
    case DataSet::Gain_5:
        return_string = tr(" 5 ");
        break;
    case DataSet::Gain_10:
         return_string = tr(" 10 ");
         break;
    case DataSet::Gain_25:
        return_string = tr(" 25 ");
        break;
    case DataSet::Gain_50:
         return_string = tr(" 50 ");
         break;
    case DataSet::Gain_100:
        return_string = tr(" 100 ");
        break;
    case DataSet::Gain_250:
         return_string = tr(" 250 ");
         break;
    case DataSet::Gain_500:
        return_string = tr(" 500 ");
        break;
    }
    return(return_string);
}
QPolygonF Parser::ToQPFADC(std::vector<DataSet::Test>::iterator itr_test)
{
    QPolygonF return_polygon;
    double time = 0.0;
    double interval = 0.1;

    switch(Data->GetTest(itr_test).TestProp.PropRate){
    case DataSet::RATE_250KHZ:
        interval = 1/250000.0;
        break;
    case DataSet::RATE_500KHZ:
        interval = 1/500000.0;
        break;
    case DataSet::RATE_1000KHZ:
        interval = 1/1000000.0;
        break;
    case DataSet::RATE_2000KHZ:
        interval = 1/2000000.0;
        break;
    }

    std::vector<long> lng_buffer = Data->GetTest(itr_test).ADC;

    for(std::vector<long>::iterator vi = lng_buffer.begin();
        vi != lng_buffer.end(); ++ vi){
        double x = *vi;
        return_polygon << QPointF(time, x);
        time = time + interval;
    }

    return(return_polygon);
}

QStringList Parser::ToQSLADC(std::vector<DataSet::Test>::iterator itr_test)
{
    QStringList return_list;
    double time = 0.0;
    double interval = 0.1;

    switch(Data->GetTest(itr_test).TestProp.PropRate){
    case DataSet::RATE_250KHZ:
        interval = 1/250000.0;
        break;
    case DataSet::RATE_500KHZ:
        interval = 1/500000.0;
        break;
    case DataSet::RATE_1000KHZ:
        interval = 1/1000000.0;
        break;
    case DataSet::RATE_2000KHZ:
        interval = 1/2000000.0;
        break;
    }

    std::vector<long> lng_buffer = Data->GetTest(itr_test).ADC;

    return_list << tr("Time (sec)") + '\t' + tr("Signal");

    for(std::vector<long>::iterator vi = lng_buffer.begin();
        vi != lng_buffer.end(); ++ vi){
        long x= *vi;
        QString stradc = "";
        QString strtime = "";
        return_list << strtime.setNum(time,'g',6) + '\t' + stradc.setNum( x,10 );
        time = time + interval;
    }

    return(return_list);
}

QString Parser::ToQStrCalc(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    switch (Data->GetTest( itr_test ).TestProp.PropCalc) {
    case DataSet::Dist:
         return_string = tr(" Distance ");
         break;
    case  DataSet::Vel:
        return_string = tr(" Velocity ");
        break;
    }

    return(return_string);
}

QDateTime Parser::ToQDateTime(std::vector<DataSet::Test>::iterator itr_test)
{
    tm test_time;
    QDateTime return_time;

    DataSet::Test current_test = Data->GetTest( itr_test );
    test_time = current_test.TestTime;
    return_time.setDate( QDate( test_time.tm_year + 1900, test_time.tm_mon , test_time.tm_mday ) );
    return_time.setTime( QTime( test_time.tm_hour, test_time.tm_min ) );

    return(return_time);
}

QString Parser::ToQStrDensity(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string = "";
    QTextStream stream( &return_string );

    stream<< Data->GetTest(itr_test).TestProp.PropDensity;

    return( return_string );
}

QString Parser::ToQStrEMethod(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    switch (Data->GetTest( itr_test ).TestProp.PropEMethod) {
    case DataSet::SimpleE:
         return_string = tr(" SIMPLE ");
         break;
    case  DataSet::ArbMu:
        return_string = tr(" ARBITRARY ");
        break;
    case  DataSet::DerivedMu:
        return_string = tr(" DERIVED ");
        break;
    }
    return(return_string);
}

QString Parser::ToQStrWave(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    switch (Data->GetTest( itr_test ).TestProp.PropWave) {
    case DataSet::PWave:
         return_string = tr(" 'P' ");
         break;
    case DataSet::SWave:
        return_string = tr(" 'S' ");
        break;
    }
    return(return_string);
}

QString Parser::ToQStrRate(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    switch (Data->GetTest( itr_test ).TestProp.PropRate) {
    case DataSet::RATE_250KHZ:
         return_string = tr(" 250 kHz ");
         break;
    case  DataSet::RATE_500KHZ:
        return_string = tr(" 500 kHz ");
        break;
    case  DataSet::RATE_1000KHZ:
        return_string = tr(" 1.0 MHz ");
        break;
    case DataSet::RATE_2000KHZ:
         return_string = tr(" 2.0 MHz ");
         break;
    }
    return(return_string);
}

QString Parser::ToQStrTransitTime(std::vector<DataSet::Test>::iterator itr_test)
{
    double return_double = Data->GetTest( itr_test ).TransitTime;
    QString return_string = QString( "%1" ).arg(return_double, 3, 'f', 1);

    return( return_string );
}


