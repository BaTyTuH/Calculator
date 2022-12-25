#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace s21 {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_e, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_finish, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_min, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_mul, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_C, SIGNAL(clicked()), this, SLOT(press_C()));
    connect(ui->pushButton_AC, SIGNAL(clicked()), this, SLOT(press_AC()));
    connect(ui->pushButton_equal, SIGNAL(clicked()), this, SLOT(press_equal()));
    connect(ui->pushButton_pi, SIGNAL(clicked()), this, SLOT(press_button()));
    connect(ui->pushButton_graph, SIGNAL(clicked()), this, SLOT(graph()));
    connect(ui->credit_calc, SIGNAL(clicked()), this, SLOT(credit_calc()));
    ui->pushButton_equal->setCheckable(true);
    controller = new Controller;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::press_AC() {
    ui->result_str->setPlainText("0");
}

void MainWindow::press_button() {
    QPushButton *button = static_cast<QPushButton*>(sender());
    QString out_string;
    if (ui->pushButton_equal->isChecked() && (button->text() != "+" && button->text() != "-" && \
        button->text() != "*" && button->text() != "/" && button->text() != "^" && button->text() != "mod")) {
        ui->result_str->clear();
    } else if (!QString::compare(ui->result_str->toPlainText(), "0") || \
               !QString::compare(ui->result_str->toPlainText().left(6), "Ошибка")) {
        ui->result_str->clear();
    }
    ui->pushButton_equal->setChecked(false);
    out_string = ui->result_str->toPlainText() + button->text();
    ui->result_str->setPlainText(out_string);
}

void MainWindow::press_C() {
    QString out_string = ui->result_str->toPlainText();
    if (!QString::compare(ui->result_str->toPlainText().left(6), "Ошибка")) {
        out_string = "0";
    } else if (out_string.size() > 1) {
        out_string.chop(1);
    } else {
        out_string = "0";
    }
    ui->result_str->setPlainText(out_string);
}

void MainWindow::press_equal() {
    QPushButton *button = static_cast<QPushButton*>(sender());
    QString out_string = ui->result_str->toPlainText();
    controller->validation(&out_string);
    button->setChecked(true);
    if (QString::compare(out_string.left(6), "Ошибка")) {
        xData = 0.0;
        if (out_string.contains('x') || out_string.contains('X')) {
            set = new Set_x;
            set->setModal(true);
            set->show();
            connect(set, &Set_x::signal_set, this, &MainWindow::slot_set);
            set->exec();
            delete set;
        }
        if (xData == xData) {
            out_string = QString::number(controller->calculate(xData), 'g', 16);
        } else {
            button->setChecked(false);
        }
    }
    ui->result_str->setPlainText(out_string);
}

void MainWindow::slot_set(double x) {
        xData = x;
}

void MainWindow::credit_calc() {
    cr = new Credit;
    cr->setModal(true);
    cr->show();
    cr->exec();
    delete cr;
}

void MainWindow::graph() {
    gr = new Graph;
    gr->setModal(true);
    gr->show();
    connect(this, &MainWindow::print_signal, gr, &Graph::slot);
    emit print_signal(ui->result_str->toPlainText());
    gr->exec();
    delete gr;
}
}  // namespace s21
