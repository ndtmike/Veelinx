#ifndef UPLOADMSG_H
#define UPLOADMSG_H

#include <QDialog>

namespace Ui {
class UploadMsg;
}

class UploadMsg : public QDialog
{
    Q_OBJECT

public:
    explicit UploadMsg(QWidget *parent = 0);
    ~UploadMsg();

private:
    Ui::UploadMsg *ui;
};

#endif // UPLOADMSG_H
