#include "graph.h"
#include "ui_graph.h"

namespace s21 {
Graph::Graph(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Graph) {
    ui->setupUi(this);
    connect(ui->print, SIGNAL(clicked()), this, SLOT(print()));
    connect(ui->quit, SIGNAL(clicked()), this, SLOT(quit()));
}

Graph::~Graph() {
    delete ui;
}

void Graph::slot(QString f) {
    func = f;
    ui->function->setText(f);
}


void Graph::print() {
    ui->function->setText(func);
    double x_start = ui->xMin_value->toPlainText().toDouble();
    double x_fin = ui->xMax_value->toPlainText().toDouble();
    double y_start = ui->yMin_value->toPlainText().toDouble();
    double y_fin = ui->yMax_value->toPlainText().toDouble();
    if (x_fin <= x_start || y_fin <= y_start) {
        ui->function->setText("Некорректная область определения!");
    } else {
        double step = x_fin-x_start > 100 ? (x_fin-x_start)/10000 : 0.01;
        std::string buf1 = ui->function->text().toStdString();
        ui->graphik->xAxis->setRange(x_start, x_fin);
        ui->graphik->yAxis->setRange(y_start, y_fin);
        Controller graphCalculation;
        graphCalculation.validation(&func);
        if (!QString::compare(func.left(6), "Ошибка")) {
            ui->function->setText("Некорректная функция!");
        } else {
            double Ybuf;
            for (double Xbuf = x_start; Xbuf <= x_fin+step; Xbuf += step) {
                Ybuf = graphCalculation.calculate(Xbuf);
                x.push_back(Xbuf);
                y.push_back(Ybuf);
            }
            ui->graphik->addGraph();
            ui->graphik->graph(0)->addData(x, y);
            ui->graphik->replot();
            x.clear();
            y.clear();
            ui->graphik->clearPlottables();
        }
    }
}


void Graph::quit() {
    close();
}
}  // namespace s21
