#include "tabledialog.h"
#include "ui_tabledialog.h"
#include"CpuManagement.h"
#include<qtablewidget.h>
#include<QKeyEvent>
#include"WarningDialog.h"


TableDialog::TableDialog(QWidget *parent,QMainWindow* _main) :
    QWidget(parent),main(_main),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(5,QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(6,QHeaderView::Stretch);
	QObject::connect(ui->calculateButton, SIGNAL(clicked(bool)), ui->editCheck, SLOT(show()));
	QObject::connect(ui->calculateButton, SIGNAL(clicked(bool)), ui->saverage1, SLOT(show()));
	QObject::connect(ui->calculateButton, SIGNAL(clicked(bool)), ui->average1, SLOT(show()));
	QObject::connect(ui->calculateButton, SIGNAL(clicked(bool)), ui->saverage2, SLOT(show()));
	QObject::connect(ui->calculateButton, SIGNAL(clicked(bool)), ui->average2, SLOT(show()));
	QObject::connect(ui->calculateButton, SIGNAL(clicked(bool)), this, SLOT(calcalculateTable(bool)));
	QObject::connect(this, SIGNAL(keyPressCal()), this, SLOT(calcalculateTable(bool)));
	ui->calculateButton->setShortcut(QKeySequence::InsertParagraphSeparator);
	ui->calculateButton->setShortcut(Qt::Key_Enter);
	ui->calculateButton->setShortcut(Qt::Key_Return);
	ui->ReturnButton->setShortcut(Qt::Key_Escape);
    QObject::connect(ui->ReturnButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    QObject::connect(ui->ReturnButton,SIGNAL(clicked(bool)),main,SLOT(show()));
    QObject::connect(main,SIGNAL(display(int,QString)),this,SLOT(dialogDisplay(int,QString)));
    QObject::connect(ui->editCheck,SIGNAL(clicked(bool)),this,SLOT(setEditable(bool)));
	ui->editCheck->setShortcut(Qt::Key_Space);
	ui->editCheck->hide();
	ui->saverage1->hide();
	ui->average1->hide();
	ui->saverage2->hide();
	ui->average2->hide();
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
}

TableDialog::~TableDialog()
{
    delete ui;
	delete cpu;
}

void TableDialog::dialogDisplay(int x,QString str)
{
    ui->workNum->setText(QString::number(x));
    ui->algorithm->setText(str);
    ui->tableWidget->setRowCount(x);
	if (str == "FCFS")
		cpu = new CpuManagement(0);
	else if (str == "SJF")
		cpu = new CpuManagement(1);
	else if (str == "SRTF")
		cpu = new CpuManagement(2);
	else if (str == "HRRF")
		cpu = new CpuManagement(3);
	else if (str == "RR")
		cpu = new CpuManagement(4);
	else
		throw "mistake!!";
	setNoEditable(0);
}

void TableDialog::showWarningDialog(const char* cstr)
{
	warning = new WarningDialog(0, this);
	warning->show();
	emit showWarning(QString(QString::fromLocal8Bit(cstr)));
}

void TableDialog::setNoEditable(int mode)
{
	if (mode == 0)
		mode = 3;
	else
	{
		mode = 0;
		ui->editCheck->setChecked(false);
	}
	int n = ui->tableWidget->rowCount();
	for (int i = 0;i < n;++i)
		for (int j = 0;j < 7;++j)
		{
			QTableWidgetItem* val = ui->tableWidget->item(i, j);
			if (val == nullptr)
			{
				val = new QTableWidgetItem;
				ui->tableWidget->setItem(i, j, val);
			}
			if (j >= mode)
				ui->tableWidget->item(i, j)->setFlags(Qt::ItemFlags(0));
		}
}

void TableDialog::setEditable(bool)
{
    if(ui->editCheck->isChecked()==true)//ok
    {
		for (int i = 0;i < ui->tableWidget->rowCount();++i)
			for (int j = 0;j < 3;++j)
			{
				QTableWidgetItem* val = ui->tableWidget->item(i, j);
				if (val == nullptr)
				{
					val = new QTableWidgetItem;
					ui->tableWidget->setItem(i, j, val);
				}
				ui->tableWidget->item(i, j)->setFlags(Qt::ItemFlags(63));
			}
    }
    else//ban
    {
		for (int i = 0;i < ui->tableWidget->rowCount();++i)
			for (int j = 0;j < 3;++j)
			{
				QTableWidgetItem* val = ui->tableWidget->item(i, j);
				if (val==nullptr)
				{
					val = new QTableWidgetItem;
					ui->tableWidget->setItem(i, j, val);
				}
				ui->tableWidget->item(i, j)->setFlags(Qt::ItemFlags(0));
			}
    }
}

void TableDialog::calcalculateTable(bool)
{
	try
	{
		cpu->clear();
		cpu->setWaitJob(getTableContent());
		cpu->execution();
		cpu->calculate();
		setTableContent(cpu->getFinishJob());
		ui->average1->setText(QString::number(cpu->getAVGTurn()));
		ui->average2->setText(QString::number(cpu->getAVGWeight()));
		setNoEditable(1);
	}
	catch (const char* cstr)
	{
		showWarningDialog(cstr);
	}
}

void TableDialog::setItemContent(int row, int column, QString content)
{
	if (ui->tableWidget->item(row, column) == nullptr)
	{
		QTableWidgetItem *item = new QTableWidgetItem(content);
		ui->tableWidget->setItem(row, column, item);
	}
	else
		ui->tableWidget->item(row, column)->setText(content);
}

QString TableDialog::getItemContent(int row, int column)
{
	if (ui->tableWidget->item(row, column) == nullptr)
	{
		setItemContent(row, column, QString("0"));
		return QString("0");
	}
	else
		return ui->tableWidget->item(row, column)->text();
}

void TableDialog::setTableContent(deque<Job*>& val)
{
	int row = ui->tableWidget->rowCount();
	for (int i = 0; i < row; ++i)
	{
		setItemContent(i, 0, QString(QString::fromLocal8Bit(val[i]->name.c_str())));
		if (timeMode)
		{
			setItemContent(i, 1, QString(QString::fromLocal8Bit(Job::timeConversion(val[i]->inTime).c_str())));
			setItemContent(i, 3, QString(QString::fromLocal8Bit(Job::timeConversion(val[i]->startTime).c_str())));
			setItemContent(i, 4, QString(QString::fromLocal8Bit(Job::timeConversion(val[i]->finishTime).c_str())));
		}
		else
		{
			setItemContent(i, 1, QString::number(val[i]->inTime));
			setItemContent(i, 3, QString::number(val[i]->startTime));
			setItemContent(i, 4, QString::number(val[i]->finishTime));
		}
		setItemContent(i, 2, QString::number(val[i]->needTime));
		setItemContent(i, 5, QString::number(val[i]->turnaroundTime));
		setItemContent(i, 6, QString::number(val[i]->weight));
	}
}

deque<Job*> TableDialog::getTableContent()
{
	deque<Job*> val;
	timeMode = getTimeMode(0);
	int row = ui->tableWidget->rowCount();

	for (int i = 0; i < row; ++i)
	{
	Job* job = new Job();
	job->name = string((const char*)getItemContent(i, 0).simplified().toLocal8Bit());
	if (job->name == "")
		throw "名称为空！";
	QString str = getItemContent(i, 1).simplified();
	if (str == "")
		throw "时间为空！";
	if (timeMode == getTimeMode(i))
	{
		if (timeMode == true)
		{
			str.replace("：", ":");
			QStringList list = str.split(":");
			if (list[0].toInt() < 0 || list[0].toInt() > 23 || list[1].toInt() < 0 || list[1].toInt()>59)
				throw "时间数值出错！";
			job->inTime = list[0].toInt() * 60 + list[1].toInt();
		}
		else
		{
			job->inTime = str.toInt();
			if (job->inTime < 0)
				throw "时间数值出错！";
		}
	}
	else
		throw "时间格式不一致！";
	
	job->startTime = -1;
	job->finishTime = -1;
	str = getItemContent(i, 2).simplified();
	if (str == "")
		throw "执行时间为空！";
	if (isDigitStr(str))
		job->needTime = str.toInt();
	else
		throw "存在非数字字符！";
	job->RemainTime = job->needTime;
	val.push_back(job);
	}
	return val;
}

bool TableDialog::getTimeMode(int row)
{
	bool timeMode = true;
	QString str = getItemContent(row, 1).simplified();
	try {
		if (str == "")
			throw "时间为空!";
	}
	catch (const char* cstr)
	{
		showWarningDialog(cstr);
	}
	int index = str.indexOf("：");
	if (index == -1)
		{
			index = str.indexOf(":");
			if (index == -1)
				if (isDigitStr(str))
					timeMode = false;
				else
					showWarningDialog("存在非数字字符！");
			else
			{
				str = str.remove(":");
				if (isDigitStr(str))
					timeMode = true;
				else
					showWarningDialog("存在非数字字符！");
			}
		}
	else
	{
		str = str.replace(index, 1, ":");
		index = str.indexOf(":");
		if (index == -1)
			if (isDigitStr(str))
				timeMode = false;
			else
				showWarningDialog("存在非数字字符！");
		else
		{
			str = str.remove(":");
			if (isDigitStr(str))
				timeMode = true;
			else
				showWarningDialog("存在非数字字符！");
		}
	}
	return timeMode;
}

bool TableDialog::isDigitStr(QString str)
{
	QByteArray ba = str.toLatin1();
	const char *s = ba.data();
	while (*s && *s >= '0' && *s <= '9') s++;
	if (*s)
		return false;
	else
		return true;
}
