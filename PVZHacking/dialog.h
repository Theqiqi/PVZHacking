#ifndef DIALOG_H
#define DIALOG_H


#include <QDialog>
#include<windows.h>
//#include<QTimer.h>
#include<QThread>





QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE



class Dialog : public QDialog
{
    Q_OBJECT



public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();


private slots:


    void on_pushButton_2_clicked();

    void on_sun_clicked();

    void on_gold_clicked();

    void on_kill_clicked();

    void on_cooldown_clicked();

    void on_sun_value_textChanged(const QString &arg1);

    void on_gold_value_textChanged(const QString &arg1);
    void time();


private:

    Ui::Dialog *ui;



};


#endif // DIALOG_H
