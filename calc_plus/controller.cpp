#include "controller.h"

namespace s21 {
Controller::Controller() {
    model = new Model;
}

Controller::~Controller() {
    delete model;
}

void Controller::validation(QString* str) {
    try {
        model->pars(*str);
        model->valid();
        model->ready_PRN();
    } catch (std::exception& e) {
        *str = e.what();
    }
}

double Controller::calculate(double x) {
    model->setX(x);
    double res = model->arithmetic();
    return res;
}

void Controller::credit_calculate(double sumCredit, double time, double percent, int type, \
                      double* payMonth, double* overpay, double* all) {
    if (type == 1) {
        Model::credit_calc_annuitet(sumCredit, time, percent, payMonth, overpay, all);
    } else {
        Model::credit_calc_difference(sumCredit, time, percent, payMonth, overpay, all);
    }
}
}  // namespace s21
