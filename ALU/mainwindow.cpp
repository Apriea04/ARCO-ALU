#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

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

    float op1 = ui->OP1Decimal->text().toFloat();
    float op2 = ui->OP2Decimal->text().toFloat();
    float result;

    DecimalOperations dec = DecimalOperations(op1, op2, &result);
    //Llamada al metodo ieeeOperations
    IEEEOperations ieee = IEEEOperations(op1, op2, &result);
    dec.add();
    //IeeeOperations devuelve un string con el valor del numero en binario ieee754
    string sop1 = ieee.translateDecToIEEE(op1);
    string sop2 = ieee.translateDecToIEEE(op2);
    //Muestra de prueba
    cout<<"Numero 1: "<<sop1<<endl<<"Numero 2: "<<sop2<<endl;
    //Llamada de la suma
    ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));
}


void MainWindow::on_MultiplyButton_clicked()
{
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,255,0)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,0,255)");

    float op1 = ui->OP1Decimal->text().toFloat();
    float op2 = ui->OP2Decimal->text().toFloat();
    float result;
    DecimalOperations dec = DecimalOperations(op1, op2, &result);
    dec.multiply();
    ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));
}


void MainWindow::on_DivisionButton_clicked()
{
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,0,255)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,255,0)");

    float op1 = ui->OP1Decimal->text().toFloat();
    float op2 = ui->OP2Decimal->text().toFloat();
    float result;
    DecimalOperations dec = DecimalOperations(op1, op2, &result);
    dec.divide();
    ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));

}

