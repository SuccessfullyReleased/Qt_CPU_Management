#include"WarningDialog.h"
#include"ui_WarningDialog.h"

WarningDialog::WarningDialog(QDialog *parent,QWidget* _table) :
	QDialog(parent),table(_table),
	ui(new Ui::WarningDialog)
{
	ui->setupUi(this);
	ui->OKButton->setShortcut(Qt::Key_Enter);
	ui->OKButton->setShortcut(Qt::Key_Return);
	QObject::connect(table, SIGNAL(showWarning(QString)), this, SLOT(setLabelText(QString)));
	setAttribute(Qt::WA_DeleteOnClose);
}

WarningDialog::~WarningDialog()
{
	delete ui;
}

void WarningDialog::setLabelText(QString str)
{
	ui->WarningLabel->setText(str);
}