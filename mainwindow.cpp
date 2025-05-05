#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bignum.h"
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->calcButton, &QPushButton::clicked,this, &MainWindow::handleCalculate);
    connect(ui->precisionSlider,&QSlider::valueChanged,this,&MainWindow::handleSlide);
    connect(ui->precisionSlider,&QSlider::sliderReleased,this,&MainWindow::handleSlideFinished);
    BigNum::SetPrecision(20);
}
void MainWindow::handleCalculate()
{
    try
    {
        std::string s = ui->input->text().toStdString();
        std::size_t opePos = s.find_first_of("+-*/%");
        BigNum num1(s.substr(0,opePos));
        BigNum num2;
        if(s[opePos+1]=='(')
        {
            size_t lastPos = s.find_last_of(")");
            num2=BigNum(s.substr(opePos+2,lastPos-opePos-2));
        }
        else num2=BigNum(s.substr(opePos+1,s.size()-opePos-1));
        BigNum result;
        if(s[opePos]=='+')
            result = num1+num2;
        else if(s[opePos]=='-')
            result = num1-num2;
        else if(s[opePos]=='*')
            result = num1*num2;
        else if(s[opePos]=='/')
            result = num1/num2;
        else if(s[opePos]=='%')
            result = num1%num2;
        ui->resultLabel->setText(QString::fromStdString("科学计数法："+static_cast<std::string>(result)));
        ui->formatLabel->setText(QString::fromStdString("浮点数记法："+result.ToFloatFormart()));
    }
    catch (const std::exception& e)
    {
        ui->resultLabel->setText("Error: " + QString(e.what()));
        ui->formatLabel->setText("Error: " + QString(e.what()));
    }
}
void MainWindow::handleSlide()
{
    std::stringstream ss;
    ss << ui->precisionSlider->value();
    ui->precisionLabel->setText("精度设置："+QString::fromStdString(ss.str()));
}
void MainWindow::handleSlideFinished()
{
    BigNum::SetPrecision(ui->precisionSlider->value());
}
MainWindow::~MainWindow()
{
    delete ui;
}
