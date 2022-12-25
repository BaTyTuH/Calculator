#ifndef SRC_CALC_PLUS_MAINWINDOW_H_
#define SRC_CALC_PLUS_MAINWINDOW_H_

#include <QMainWindow>

#include "controller.h"
#include "set_x.h"
#include "graph.h"
#include "credit.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
    Q_OBJECT

 public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void slot_set(double);

 private:
    Ui::MainWindow *ui;
    Set_x *set;
    Graph *gr;
    Credit *cr;
    double xData;
    Controller* controller;

 signals:
    void print_signal(QString func);

 private slots:
    void press_button();
    void press_C();
    void press_AC();
    void press_equal();
    void graph();
    void credit_calc();
};
}  // namespace s21
#endif  // SRC_CALC_PLUS_MAINWINDOW_H_
