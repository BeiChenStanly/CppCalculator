#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bignum.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->calcButton, &QPushButton::clicked,
            this, &MainWindow::handleCalculate);
}
void MainWindow::handleCalculate() {
    try {
        std::string s = ui->input->text().toStdString();
        std::size_t plusPos = s.find('+');
        BigNum num1(s.substr(0,plusPos));
        BigNum num2(s.substr(plusPos+1,s.size()-plusPos-1));
        BigNum result = num1+num2;
        ui->resultLabel->setText(QString::fromStdString("科学计数法："+static_cast<std::string>(result)));
        ui->formatLabel->setText(QString::fromStdString("浮点数记法："+result.ToFloatFormart()));
    } catch (const std::exception& e) {
        ui->resultLabel->setText("Error: " + QString(e.what()));
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
