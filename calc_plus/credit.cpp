#include "credit.h"
#include "ui_credit.h"
#include <QMessageBox>

namespace s21 {
Credit::Credit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Credit) {
    ui->setupUi(this);

    ui->ann->setChecked(true);
    ui->monthRB->setChecked(true);
    type_time = DEFOLT;
    type = DEFOLT;

    connect(ui->res, SIGNAL(clicked()), this, SLOT(on_res_clicked()));

    ui->percent->setMaxLength(5);
    QDoubleValidator *validPercent = new QDoubleValidator(0.01, 99.99, 2, this);
    validPercent->setNotation(QDoubleValidator::StandardNotation);
    validPercent->setLocale(QLocale::C);
    ui->percent->setValidator(validPercent);
    ui->sum_credit->setValidator(new QIntValidator(1, 999999999, this));
    ui->time->setValidator(new QIntValidator(1, 600, this));
}

Credit::~Credit() {
    delete ui;
}

void Credit::on_Quit_clicked() {
    close();
}

void Credit::on_ann_clicked(bool checked) {
    if (checked) {
        type = DEFOLT;
    }
}

void Credit::on_dif_clicked(bool checked) {
    if (checked) {
        type = NEW;
    }
}

void Credit::on_res_clicked() {
    double valuePercent = ui->percent->text().toDouble()/100;
    double valueSumCredit = ui->sum_credit->text().toDouble();
    double valueTime = ui->time->text().toDouble();
    if (type_time == NEW) {
        valueTime *= 12;
    }
    if (!valuePercent || !valueSumCredit || !valueTime) {
        ui->statusBar->setText("Заполните пустые строки!");
        ui->overpayment->setText("");
        ui->total->setText("");
    } else if (valuePercent > 99.99 || valuePercent < 0.01 || valueTime > 600) {
        ui->statusBar->setText("Введите корректные данные");
        ui->overpayment->setText("");
        ui->total->setText("");
    } else {
        ui->statusBar->setText("");
        double valueOverPay = 0, valueTotal = 0;
        double* valueMonthPay = new double[valueTime];
        Controller::credit_calculate(valueSumCredit, valueTime, valuePercent,
                                     type, valueMonthPay, &valueOverPay, &valueTotal);
        model = new QStandardItemModel(valueTime, 2, this);
        ui->tableView->setModel(model);
        model->setHeaderData(0, Qt::Horizontal, "Платеж");
        model->setHeaderData(1, Qt::Horizontal, "Остаток долга");
        QModelIndex index;
        double balance = valueTotal;
        for (int row = 0; row < model->rowCount(); row++) {
            balance -= *(valueMonthPay+row);
            if (balance < 0.0000001) {
                balance = 0;
            }
            for (int col = 0; col < model->columnCount(); col++) {
                index = model->index(row, col);
                model->setData(index, col ? balance : *(valueMonthPay+row));
            }
        }
        ui->overpayment->setText(QString::number(valueOverPay, 'g', 12));
        ui->total->setText(QString::number(valueTotal, 'g', 12));
        delete[] valueMonthPay;
    }
}

void Credit::on_monthRB_clicked(bool checked) {
    if (checked) {
        type_time = DEFOLT;
    }
}

void Credit::on_yearRB_clicked(bool checked) {
    if (checked) {
        type_time = NEW;
    }
}
}  // namespace s21
