#ifndef SRC_CALC_PLUS_CREDIT_H_
#define SRC_CALC_PLUS_CREDIT_H_

#include <QDialog>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QStandardItemModel>

#include "controller.h"

namespace Ui {
class Credit;
}

namespace s21 {
class Credit : public QDialog {
    Q_OBJECT

 public:
    explicit Credit(QWidget *parent = nullptr);
    ~Credit();
    static constexpr int DEFOLT = 1;
    static constexpr int NEW = 2;

 private slots:
    void on_Quit_clicked();
    void on_ann_clicked(bool checked);
    void on_dif_clicked(bool checked);
    void on_res_clicked();
    void on_monthRB_clicked(bool checked);
    void on_yearRB_clicked(bool checked);

 private:
    Ui::Credit *ui;
    int type, type_time;
    QStandardItemModel *model;
};
}  // namespace s21
#endif  // SRC_CALC_PLUS_CREDIT_H_
