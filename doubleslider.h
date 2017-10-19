#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include <QSlider>

class DoubleSlider : public QSlider {
    Q_OBJECT

public:
    DoubleSlider(QWidget *parent = 0) : QSlider(parent) {
        connect(this, SIGNAL(valueChanged(int)),
            this, SLOT( notifyValueChanged(int) ));
    }

signals:
    void doubleValueChanged(double value);

public slots:
    void notifyValueChanged(int value) {
        double doubleValue = value / 10.0;
        emit doubleValueChanged(doubleValue);
    }

    void notifySetValue( double value ){
        int intValue = value * 10;
        setValue( intValue );
    }
};

#endif // DOUBLESLIDER_H
