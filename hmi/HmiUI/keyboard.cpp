#include "keyboard.h"
#include "ui_keyboard.h"

KeyBoard::KeyBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KeyBoard)
{
    ui->setupUi(this);
}

KeyBoard::~KeyBoard()
{
    delete ui;
}

QString KeyBoard::getContext()
{
    return ui->idLineedit->text();
}

void KeyBoard::on_keyBoard1_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "1");
}

void KeyBoard::on_keyBoard2_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "2");
}

void KeyBoard::on_keyBoard3_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "3");
}

void KeyBoard::on_keyBoard4_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "4");
}

void KeyBoard::on_keyBoard5_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "5");
}

void KeyBoard::on_keyBoard6_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "6");
}

void KeyBoard::on_keyBoard7_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "7");
}

void KeyBoard::on_keyBoard8_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "8");
}

void KeyBoard::on_keyBoard9_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "9");
}

void KeyBoard::on_keyBoard0_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text + "0");
}

void KeyBoard::on_keyBoardDelete_clicked()
{
    QString text = ui->idLineedit->text();
    ui->idLineedit->setText(text.left(text.size() - 1));
}

void KeyBoard::on_keyBoardClear_clicked()
{
    ui->idLineedit->clear();
}
