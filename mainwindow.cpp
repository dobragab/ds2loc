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

    for(const auto& item : filecontent.languages[index].strings)
        ui->listWidget->addItem(item);
}
