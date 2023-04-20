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

    //DecimalOperations dec = DecimalOperations(op1, op2, &result);

    //Creamos objeto ieee
    IEEEOperations ieee = IEEEOperations(op1, op2);
    //Llamada al metodo suma
    ieee.add();
    //Convertimos op1 y op2 a binario y lo recogemos como string
    string bin1 = ieee.translateDecToIEEE(op1);
    string bin2 = ieee.translateDecToIEEE(op2);
    string binResult =ieee.getIEEEResult();
    ui->ResultDecimal->setText(QString::number(ieee.getResult()));

    //Mostramos los numeros en binario
    ui->OP1IEEE->setText(QString::fromStdString(bin1));
    ui->OP2IEEE->setText(QString::fromStdString(bin2));
    ui->ResultIEEE->setText(QString::fromStdString(binResult));
    //Mostramos los numeros en hexadecimal
    ui->OP1Hex->setText(QString::fromStdString(ieee.translateBinaryToHex(bin1)));
    ui->OP2Hex->setText(QString::fromStdString(ieee.translateBinaryToHex(bin2)));
    ui->ResultHex->setText(QString::fromStdString(ieee.translateBinaryToHex(binResult)));
    //Devolvemos la salida
}


void MainWindow::on_MultiplyButton_clicked()
{
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,255,0)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,0,255)");

    float op1 = ui->OP1Decimal->text().toFloat();
    float op2 = ui->OP2Decimal->text().toFloat();
    union Code result;

    //DecimalOperations dec = DecimalOperations(op1, op2, &result);

    //Creamos objeto ieee
    IEEEOperations ieee = IEEEOperations(op1, op2);
    //Llamada al metodo suma
    ieee.multiply();
    //Convertimos op1 y op2 a binario y lo recogemos como string
    string bin1 = ieee.translateDecToIEEE(op1);
    string bin2 = ieee.translateDecToIEEE(op2);
    string binResult = ieee.getIEEEResult();
    //ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));

    //Mostramos los numeros en binario
    ui->OP1IEEE->setText(QString::fromStdString(bin1));
    ui->OP2IEEE->setText(QString::fromStdString(bin2));
    ui->ResultIEEE->setText(QString::fromStdString(binResult));
    //Mostramos los numeros en hexadecimal
    ui->OP1Hex->setText(QString::fromStdString(ieee.translateBinaryToHex(bin1)));
    ui->OP2Hex->setText(QString::fromStdString(ieee.translateBinaryToHex(bin2)));
    ui->ResultHex->setText(QString::fromStdString(ieee.translateBinaryToHex(binResult)));
    //Devolvemos la salida
    ui->ResultDecimal->setText(QString::fromStdString(ieee.getIEEEResult()));
}


void MainWindow::on_DivisionButton_clicked()
{
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,0,255)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,255,0)");

    float op1 = ui->OP1Decimal->text().toFloat();
    float op2 = ui->OP2Decimal->text().toFloat();
    float result;

    //DecimalOperations dec = DecimalOperations(op1, op2, &result);

    //Creamos objeto ieee
    IEEEOperations ieee = IEEEOperations(op1, op2);
    //Llamada al metodo suma
    ieee.divide();
    //Convertimos op1 y op2 a binario y lo recogemos como string
    string bin1 = ieee.translateDecToIEEE(op1);
    string bin2 = ieee.translateDecToIEEE(op2);
    string binResult = ieee.getIEEEResult();
    //ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));

    //Mostramos los numeros en binario
    ui->OP1IEEE->setText(QString::fromStdString(bin1));
    ui->OP2IEEE->setText(QString::fromStdString(bin2));
    ui->ResultIEEE->setText(QString::fromStdString(binResult));
    //Mostramos los numeros en hexadecimal
    ui->OP1Hex->setText(QString::fromStdString(ieee.translateBinaryToHex(bin1)));
    ui->OP2Hex->setText(QString::fromStdString(ieee.translateBinaryToHex(bin2)));
    ui->ResultHex->setText(QString::fromStdString(ieee.translateBinaryToHex(binResult)));
    //Devolvemos la salida
    ui->ResultDecimal->setText(QString::fromStdString(to_string(result)));

}

