#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddButton_clicked();

    void on_MultiplyButton_clicked();

    void on_DivisionButton_clicked();

private:
    Ui::MainWindow *ui;
    int button; //0 if ADD, 1 if MULTIPLY, 2 if DIVISION
};
#endif // MAINWINDOW_H
