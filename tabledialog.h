#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QWidget>
#include <QMainWindow>
#include<deque>
//#include<string>
using std::deque;
using std::string;
class Job;
class CpuManagement;
class WarningDialog;

namespace Ui {
class TableDialog;
}

class TableDialog : public QWidget
{
	Q_OBJECT

	bool timeMode;
public:
	explicit TableDialog(QWidget *parent = 0, QMainWindow* _main = 0);
    ~TableDialog();
	void setItemContent(int row, int column, QString content);
	QString getItemContent(int row, int column);
	void setTableContent(deque<Job*>& val);
	deque<Job*> getTableContent();
	bool getTimeMode(int row);
	void setNoEditable(int mode);
	bool isDigitStr(QString str);
public slots:
    void dialogDisplay(int,QString);
    void setEditable(bool);
	void calcalculateTable(bool);
	void showWarningDialog(const char*);
signals:
	void showWarning(QString);
	void keyPressCal();
private:
    Ui::TableDialog *ui;
    QMainWindow* main;
	WarningDialog* warning;
	CpuManagement* cpu;
};

#endif // TABLEDIALOG_H
