#include "dialog.h"
#include "./ui_dialog.h"
#include<iostream>
#include <QMessageBox>
#include<stdio.h>
#include<windows.h>
#include <QThread>
#include <QDebug>
#include <unistd.h>
#include <QRegularExpressionValidator>
#include <QTimer>
#include <stdio.h>



HWND hwnd;//窗口句柄
HANDLE hProcess; //进程句柄
DWORD pid;//进程pid
QTimer *value;//数值定时器
DWORD sun_number;
DWORD gold_number;

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    // ui->sun->setEnabled(false);
    // ui->gold->setEnabled(false);
    //ui->kill->setEnabled(false);
    //ui->cooldown->setEnabled(0);

    value=new QTimer(this);//创建一个为数值的定时器
    //数值定时器信号槽
    connect(value,&QTimer::timeout,this,&Dialog::on_sun_clicked);
    connect(value,&QTimer::timeout,this,&Dialog::on_gold_clicked);
    connect(value,&QTimer::timeout,this,&Dialog::time);
    time();


}



void WriteMemory(void* value, DWORD valueSize, ...)
{
    if (value == NULL || valueSize == 0 || hProcess == NULL) return;

    DWORD tempValue = 0;

    va_list addresses;
    va_start(addresses, valueSize);
    DWORD offset = 0;
    DWORD lastAddress = 0;
    while ((offset = va_arg(addresses, DWORD)) != -1)
    {
        lastAddress = tempValue + offset;
        ::ReadProcessMemory(hProcess, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);

    }
    va_end(addresses);

    ::WriteProcessMemory(hProcess, (LPVOID)lastAddress, value, valueSize, NULL);

}


void WriteMemory(void* value, DWORD valueSize, DWORD address)
{
    WriteMemory(value, valueSize, address, -1);
}



Dialog::~Dialog()
{
    delete ui;
}

void Dialog:: time()
{
    if(1)
    {
        value->start(200);
    }

    HWND hwnd  = FindWindow(NULL, TEXT("Plants vs. Zombies"));

    //进程不存在则不启用
    if(hwnd==NULL)
    {
        ui->sun->setEnabled(false);
        ui->gold->setEnabled(false);
        ui->kill->setEnabled(false);
        ui->cooldown->setEnabled(0);
        ui->sun->setChecked(false);
        ui->gold->setChecked(false);
        ui->kill->setChecked(false);
        ui->cooldown->setChecked(0);
        hProcess = NULL;
    }

    else
    {

        ui->sun->setEnabled(true);
        ui->gold->setEnabled(true);
        ui->kill->setEnabled(true);
        ui->cooldown->setEnabled(true);
        GetWindowThreadProcessId(hwnd, &pid);//第二个参数用来保存进程标识符pid
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

}



void Dialog::on_pushButton_2_clicked()
{


    /*   HWND hwnd  = FindWindow(NULL, TEXT("Plants vs. Zombies"));

            //进程不存在则不启用
             if(hwnd==NULL)
             {

                //进程不存在则关闭软件
                QMessageBox::warning(this,"提示","请打开游戏",QMessageBox::Ok);//信息框提示
                // QTimer::singleShot(0,qApp,SLOT(quit()));//关闭软件
                 hProcess = NULL;
             }

             else
             {

                      QMessageBox::information(this,
                      tr("提示"),
                      tr("开启成功\n勾选生效"),
                      QMessageBox::Ok | QMessageBox::Cancel,
                      QMessageBox::Ok);
                     ui->sun->setEnabled(true);
                     ui->gold->setEnabled(true);
                     ui->kill->setEnabled(true);
                     ui->cooldown->setEnabled(true);

                     GetWindowThreadProcessId(hwnd, &pid);//第二个参数用来保存进程标识符pid
                     hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

             }


        */



}
void Dialog::on_sun_clicked()
{





    // update();
    if( ui->sun->isChecked() )
    {
        // ui->sun_value->setText(QStringLiteral("9000"));

        value->start(100);  //数值定时器开始以0.1秒一次写入执行
        // HWND hwnd  = FindWindow(NULL, TEXT("Plants vs. Zombies"));
        //GetWindowThreadProcessId(hwnd, &pid);//第二个参数用来保存进程标识符pid
        //hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
        int Addr;
        ReadProcessMemory(hProcess, (LPCVOID)(0x00729670), &Addr, 4, 0);
        ReadProcessMemory(hProcess, (LPCVOID)(Addr + 0x868), &Addr, 4, 0);
        ReadProcessMemory(hProcess, (LPVOID)(Addr + 0x5578), &Addr,4,0);
        sun_number=Addr;
        ui->sun_value->setText(QString::number(sun_number));//根据你的代码改变



    }


    else if(!ui->sun->isChecked())//复选框不被选中
    {
        ui->sun_value->setText(QStringLiteral(""));
        value->stop();  //数值定时器停止
    }

}



void Dialog::on_gold_clicked()
{


    if( ui->gold->isChecked() )
    {


        value->start(100);  //数值定时器开始以0.1秒一次写入执行
        int Addr;
        ReadProcessMemory(hProcess, (LPCVOID)(0x00729670), &Addr, 4, 0);
        ReadProcessMemory(hProcess, (LPCVOID)(Addr + 0x94C), &Addr, 4, 0);
        ReadProcessMemory(hProcess, (LPVOID)(Addr + 0x50), &Addr,4,0);
        gold_number=Addr;
        ui->gold_value->setText(QString::number(gold_number));//根据你的代码改变


    }


    else if(!ui->gold->isChecked())//复选框不被选中
    {
        ui->gold_value->setText(QStringLiteral(""));
        value->stop();  //数值定时器停止
    }
}






void Dialog::on_kill_clicked()
{

    if (ui->kill->isChecked())//需要秒杀僵尸
    {

        BYTE data[] = {0x29,0xED,0x89,0x44,0x24,0x10,0xE9,0xD7,0x0C,0x53,0x00};
        WriteMemory(data, sizeof(data), 0x11000);
        BYTE data_a[] = {0xE9 ,0x21,0xF3,0xAC,0xFF,0x90,0x90,0x90};
        WriteMemory(data_a, sizeof(data_a), 0x00541CDA);

        //int Addr;
        //普通僵尸
        /* BYTE data[] = {0x29, 0xED, 0x90, 0x90};
           // WriteMemory(data, sizeof(data), 0x00541CDA);
            WriteProcessMemory(hProcess, (LPVOID)(0x00541CDA),data, 4, 0);
            */
        //防护服僵尸
        BYTE data_1[] = { 0x29, 0xC9};
        WriteMemory(data_1, sizeof(data_1), 0x005419F4);

    }
    else  //不需要秒杀僵尸
    {

        BYTE data_a[] = {0x2B, 0x6c, 0x24, 0x20 , 0x89 , 0x44 , 0x24 , 0x10};
        WriteMemory(data_a, sizeof(data_a), 0x00541CDA);
        BYTE data[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
        WriteMemory(data, sizeof(data), 0x11000);

        /*
            //普通僵尸
            //int Addr;
            BYTE data[] = {0x2B, 0x6c, 0x24, 0x20 };
           // WriteMemory(data, sizeof(data), 0x00541CDA);
            WriteProcessMemory(hProcess, (LPVOID)(0x00541CDA),data, 4, 0);
            */
        //防护服僵尸
        BYTE data_1[] = { 0x29, 0xC1, 0x89, 0x74, 0x24,0x0C };
        WriteMemory(data_1, sizeof(data_1), 0x005419F4);


    }


}

void Dialog::on_cooldown_clicked()
{

    //数值复选框被单击

    if(ui->cooldown->isChecked())  //复选框被选中
    {
        int value = 999329936;
        int Addr=0x00491E4F;
        // ReadProcessMemory(hProcess, (LPVOID)(0x00491E4F), &Addr, 4, 0);
        WriteProcessMemory(hProcess, (LPVOID)(Addr),&value, 4, 0);
    }
    else if(!ui->cooldown->isChecked())//复选框不被选中
    {
        int value = 992233355;
        int Addr=0x00491E4F;
            // ReadProcessMemory(hProcess, (LPVOID)(0x00491E4F), &Addr, 4, 0);
        WriteProcessMemory(hProcess, (LPVOID)(Addr),&value, 4, 0);
    }

}



/*void Dialog::on_sun_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
      if( ui->sun->isChecked() )
        {

          QMessageBox::information(this,
              tr("提示"),
              tr("勾选"),
              QMessageBox::Ok | QMessageBox::Cancel,
              QMessageBox::Ok);
        }
      else if(ui->sun->isChecked() == false)
        {
          QMessageBox::information(this,
              tr("提示"),
              tr("没有勾选"),
              QMessageBox::Ok | QMessageBox::Cancel,
              QMessageBox::Ok);
        }

}
*/
void Dialog::on_sun_value_textChanged(const QString &arg1)
{

    ui->sun_value->setValidator(new QIntValidator(ui->sun_value));
    // ui->sun_value->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

    if( ui->sun->isChecked() )
    {

        /* QMessageBox::information(this,
              tr("提示"),
              tr("勾选"),
              QMessageBox::Ok | QMessageBox::Cancel,
              QMessageBox::Ok);
           */

        value->start(100);  //数值定时器开始以0.1秒一次写入执行
        int sun= ui-> sun_value->text().toInt();//获取编辑框你输入的数值
        DWORD d=sun;    //然后把阳光int转化成DWORD，可以回头参考函数类型
        int Addr;       //临时保存的地址

        //然后就根据基址和偏移写内存就行了，最后两个参数默认4和0就行了
        ReadProcessMemory(hProcess, (LPCVOID)(0x00729670), &Addr, 4, 0);
        ReadProcessMemory(hProcess, (LPCVOID)(Addr + 0x868), &Addr, 4, 0);
        WriteProcessMemory(hProcess, (LPVOID)(Addr + 0x5578), &d,4,0);
        // BYTE data[] = { 0x2B, 0xC8 };
        //WriteMemory(data, sizeof(data), 0x00729670,0x868,0x5578);
        //BYTE data_1[] = { 0x2B, 0xC8 };
        //WriteMemory(data_1, sizeof(data_1), 0x00729670);


    }


    else if(ui->sun->isChecked() == false)
    {
        value->stop();  //数值定时器停止
    }
}



void Dialog::on_gold_value_textChanged(const QString &arg1)
{
    ui->gold_value->setValidator(new QIntValidator(ui->gold_value));
    if( ui->gold->isChecked() )
    {

        int gold= ui-> gold_value->text().toInt();//获取编辑框你输入的数值
        DWORD d=gold;    //然后把阳光int转化成DWORD，可以回头参考函数类型
        int Addr;       //临时保存的地址

        //然后就根据基址和偏移写内存就行了，最后两个参数默认4和0就行了
        ReadProcessMemory(hProcess, (LPCVOID)(0x00729670), &Addr, 4, 0);
        ReadProcessMemory(hProcess, (LPCVOID)(Addr + 0x94C), &Addr, 4, 0);
        WriteProcessMemory(hProcess, (LPVOID)(Addr + 0x50), &d,4,0);


    }

}

