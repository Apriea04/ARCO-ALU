#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_AddButton_clicked()
{
    ui->AddButton->setStyleSheet("color: rgb(0,255,0)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,0,255)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,0,255)");

    long op1 = stol(ui->OP1Decimal->text().toStdString());
    long op2 = stol(ui->OP2Decimal->text().toStdString());
    long result;

    DecimalOperations dec = DecimalOperations(op1, op2, &result);
    IEEEOperations ieee = IEEEOperations(op1, op2, &result);
    dec.add();
    ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));
}


void MainWindow::on_MultiplyButton_clicked()
{
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,255,0)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,0,255)");

    long op1 = stol(ui->OP1Decimal->text().toStdString());
    long op2 = stol(ui->OP2Decimal->text().toStdString());
    long result;
    DecimalOperations dec = DecimalOperations(op1, op2, &result);
    dec.multiply();
    ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));
}


void MainWindow::on_DivisionButton_clicked()
{
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,0,255)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,255,0)");

    long op1 = stol(ui->OP1Decimal->text().toStdString());
    long op2 = stol(ui->OP2Decimal->text().toStdString());
    long result;
    DecimalOperations dec = DecimalOperations(op1, op2, &result);
    dec.divide();
    ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));

}

