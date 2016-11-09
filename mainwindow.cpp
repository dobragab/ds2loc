#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open LOC"), "", tr("Localization files (*.loc)"));

    std::ifstream file(fileName.toUtf8(), std::ios::binary);
    filecontent = FileContent(file);

    for(int i = 0; i < filecontent.sections.size(); ++i)
    {
        ui->comboBox->addItem(QString::number(filecontent.sections[i].id), QVariant(i));
    }
}

void MainWindow::on_actionSave_triggered()
{

}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    ui->listWidget->clear();

    for(int i = 0; i < filecontent.languages[index].strings.size(); ++i)
    {
        auto item = new StringItem(filecontent.languages[index].strings[i], ui->listWidget, i);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->listWidget->addItem(item);
    }
}

LanguageContent& MainWindow::lang()
{
    return filecontent.languages[ui->comboBox->currentIndex()];
}

void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem*)
{
    auto item = static_cast<StringItem*>(current);

    ui->label_index->setText(QString::number(item->index));
    auto l = lang();
    ui->label_16->setText(QString::number(l.str_sizes[item->index + 1] - l.str_sizes[item->index] - 1));
    ui->label_40->setText(QString::number(l.meta[item->index + 1].size - l.meta[item->index].size - 1));
    ui->label_strlen->setText(QString::number(l.strings[item->index].size()));
}
