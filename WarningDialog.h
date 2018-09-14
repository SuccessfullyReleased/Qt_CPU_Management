#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <QDialog>
#include <QMainWindow>
#include"ui_WarningDialog.h"

namespace Ui {
	class WarningDialog;
}

class WarningDialog : public QDialog
{
	Q_OBJECT

public:
	explicit WarningDialog(QDialog *parent = 0, QWidget* _table = 0);
	~WarningDialog();

public slots:
	void setLabelText(QString);
signals:

private:
	Ui::WarningDialog *ui;
	QWidget* table;
};

#endif
