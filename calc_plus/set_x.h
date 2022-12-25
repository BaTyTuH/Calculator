#ifndef SRC_CALC_PLUS_SET_X_H_
#define SRC_CALC_PLUS_SET_X_H_

#include <QDialog>

#include "controller.h"

namespace Ui {
class Set_x;
}
namespace s21 {
class Set_x : public QDialog {
    Q_OBJECT

 public:
    explicit Set_x(QWidget *parent = nullptr);
    ~Set_x();

 private:
    Ui::Set_x *ui;
    bool CheckOKWithExpression = false;
    Controller* controller;

 signals:
    void signal_set(double x);

 private slots:
    void on_button_ok_clicked();
};
}  // namespace s21
#endif  // SRC_CALC_PLUS_SET_X_H_
