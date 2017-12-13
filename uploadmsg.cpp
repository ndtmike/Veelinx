#include "uploadmsg.h"
#include "ui_uploadmsg.h"

UploadMsg::UploadMsg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UploadMsg)
{
    ui->setupUi(this);
}

UploadMsg::~UploadMsg()
{
    delete ui;
}
