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
    //Para que el botón quede bonito
    ui->AddButton->setStyleSheet("color: rgb(0,255,0)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,0,255)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,0,255)");

    //Creamos op1 y op2
    union Code op1;
    op1.numero = ui->OP1Decimal->text().toFloat();

    union Code op2;
    op2.numero = ui->OP2Decimal->text().toFloat();



    //Creamos objeto ieee
    IEEEOperations ieee = IEEEOperations(op1, op2);

    //Llamada al metodo suma
    ieee.operations("suma");

    //Recogemos el valor de resultado
    union Code result = ieee.getResult();
    //Lo insertamos en la interfaz grafica
    ui->ResultDecimal->setText(QString::number(result.numero));

    //Mostramos los numeros en binario
    ui->OP1IEEE->setText(QString::fromStdString(ieee.translateIEEE(op1)));
    ui->OP2IEEE->setText(QString::fromStdString(ieee.translateIEEE(op2)));
    ui->ResultIEEE->setText(QString::fromStdString(ieee.translateIEEE(result)));

    //Mostramos los numeros en hexadecimal
    ui->OP1Hex->setText(QString::fromStdString(ieee.translateHex(op1)));
    ui->OP2Hex->setText(QString::fromStdString(ieee.translateHex(op2)));
    ui->ResultHex->setText(QString::fromStdString(ieee.translateHex(result)));
}


void MainWindow::on_MultiplyButton_clicked()
{

    //Para que el botón quede bonito
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,255,0)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,0,255)");

    //Creamos op1 y op2
    union Code op1;
    op1.numero = ui->OP1Decimal->text().toFloat();

    union Code op2;
    op2.numero = ui->OP2Decimal->text().toFloat();



    //Creamos objeto ieee
    IEEEOperations ieee = IEEEOperations(op1, op2);

    //Llamada al metodo multiplicacion
    ieee.operations("multiplicacion");

    //Booleano de control de denormales
    bool denormal = ieee.getMulti();
    cout<<denormal<<endl;


    //Recogemos el valor de resultado
    union Code result = ieee.getResult();
    //Lo insertamos en la interfaz grafica
    if (denormal) {
        ui->ResultDecimal->setText("NaN");
    } else {
        ui->ResultDecimal->setText(QString::number(result.numero));
    }

    //Mostramos los numeros en binario
    ui->OP1IEEE->setText(QString::fromStdString(ieee.translateIEEE(op1)));
    ui->OP2IEEE->setText(QString::fromStdString(ieee.translateIEEE(op2)));
    ui->ResultIEEE->setText(QString::fromStdString(ieee.translateIEEE(result)));

    //Mostramos los numeros en hexadecimal
    ui->OP1Hex->setText(QString::fromStdString(ieee.translateHex(op1)));
    ui->OP2Hex->setText(QString::fromStdString(ieee.translateHex(op2)));
    ui->ResultHex->setText(QString::fromStdString(ieee.translateHex(result)));
}


void MainWindow::on_DivisionButton_clicked()
{

    //Para que el botón quede bonito
    ui->AddButton->setStyleSheet("color: rgb(0,0,255)");
    ui->MultiplyButton->setStyleSheet("color: rgb(0,0,255)");
    ui->DivisionButton->setStyleSheet("color: rgb(0,255,0)");


    //Creamos op1 y op2
    union Code op1;
    op1.numero = ui->OP1Decimal->text().toFloat();

    union Code op2;
    op2.numero = ui->OP2Decimal->text().toFloat();



    //Creamos objeto ieee
    IEEEOperations ieee = IEEEOperations(op1, op2);

    //Llamada al metodo division
    ieee.operations("division");

    //Recogemos el valor de resultado
    union Code result = ieee.getResult();
    //Lo insertamos en la interfaz grafica
    ui->ResultDecimal->setText(QString::number(result.numero));

    //Mostramos los numeros en binario
    ui->OP1IEEE->setText(QString::fromStdString(ieee.translateIEEE(op1)));
    ui->OP2IEEE->setText(QString::fromStdString(ieee.translateIEEE(op2)));
    ui->ResultIEEE->setText(QString::fromStdString(ieee.translateIEEE(result)));

    //Mostramos los numeros en hexadecimal
    ui->OP1Hex->setText(QString::fromStdString(ieee.translateHex(op1)));
    ui->OP2Hex->setText(QString::fromStdString(ieee.translateHex(op2)));
    ui->ResultHex->setText(QString::fromStdString(ieee.translateHex(result)));


}

