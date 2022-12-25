#include "set_x.h"
#include "controller.h"
#include "ui_set_x.h"

namespace s21 {
Set_x::Set_x(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Set_x) {
    ui->setupUi(this);
    ui->condition->clear();
    controller = new Controller;
}

Set_x::~Set_x() {
    if (!CheckOKWithExpression) emit signal_set(NAN);
    delete controller;
    delete ui;
}

void Set_x::on_button_ok_clicked() {
    QString x = ui->setx->text();
    if (!x.size()) {
        ui->condition->setText("Введите выражение");
    } else {
        controller->validation(&x);
        if (x.contains('x') || x.contains('X') || !QString::compare(x.left(6), "Ошибка")) {
            ui->condition->setText("Неверный ввод");
        } else {
            emit signal_set(controller->calculate(0.0));
            ui->setx->clear();
            CheckOKWithExpression = true;
            close();
        }
    }
}
}  // namespace s21
