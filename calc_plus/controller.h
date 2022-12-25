#ifndef SRC_CALC_PLUS_CONTROLLER_H_
#define SRC_CALC_PLUS_CONTROLLER_H_

#include "model.h"

namespace s21 {
class Controller {
 public:
    Controller();
    ~Controller();
    double calculate(double x);
    static void credit_calculate(double sumCredit, double time, double percent, int type, \
                          double* payMonth, double* overpay, double* all);
    void validation(QString* str);

 private:
    Model* model;
};
}  // namespace s21
#endif  // SRC_CALC_PLUS_CONTROLLER_H_
