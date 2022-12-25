#ifndef SRC_CALC_PLUS_GRAPH_H_
#define SRC_CALC_PLUS_GRAPH_H_

#include <QDialog>
#include <QVector>

#include "qcustomplot.h"
#include "controller.h"

namespace Ui {

class Graph;
}
namespace s21 {
class Graph : public QDialog {
    Q_OBJECT


 public:
    explicit Graph(QWidget *parent = nullptr);
    ~Graph();


 private slots:
    void print();
    void quit();


 private:
    Ui::Graph *ui;
    QString func;
    QVector<double> x, y;
    QCPGraph *a;


 public slots:
    void slot(QString func);
};
}  // namespace s21
#endif  // SRC_CALC_PLUS_GRAPH_H_
