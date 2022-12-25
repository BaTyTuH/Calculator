#ifndef SRC_CALC_PLUS_MODEL_H_
#define SRC_CALC_PLUS_MODEL_H_

#include <QString>
#include <queue>
#include <stack>

namespace s21 {
class Model {
 public:
    Model();
    ~Model();

    static void credit_calc_annuitet(double sumCredit, double time, double percent, \
                                double* payMonth, double* overpay, double* all);
    static void credit_calc_difference(double sumCredit, double time, double percent, \
                                double* payMonth, double* overpay, double* all);
    void pars(QString str);
    void trigonometry(QString& str);
    void fromQstrToNumber(QString& str);
    void assign_group(int& group, std::pair<double, int>& current, int* next);
    void valid();
    void ready_PRN();
    void binary_operation(std::pair<double, int> current, \
                                 std::stack<std::pair<double, int>>& numbers, double number2);
    void unary_operation(std::pair<double, int> current, std::stack<std::pair<double, int>>& numbers);
    double arithmetic();
    void operation(std::pair<double, int> current, std::stack<std::pair<double, int>>& numbers);
    void setX(double var) {x = var;}

    static constexpr double SUM = 1;  // приоритет 1
    static constexpr double SUB = 2;

    static constexpr double MUL = 3;  // приоритет 2
    static constexpr double DIV = 4;
    static constexpr double MOD = 5;

    static constexpr double PLUS = 7;  // приоритет 3
    static constexpr double MIN = 8;

    static constexpr double POW = 6;  // приоритет 4

    static constexpr double COS = 9;  // приоритет 5
    static constexpr double SIN = 10;
    static constexpr double TAN = 11;
    static constexpr double ACOS = 12;
    static constexpr double ASIN = 13;
    static constexpr double ATAN = 14;
    static constexpr double LN = 15;
    static constexpr double LOG = 16;
    static constexpr double SQRT = 17;

    static constexpr double START = 18;  // приоритет 6
    static constexpr double FINISH = 19;

// private:
    double x;
    std::queue<std::pair<double, int>> *expression;
};
}  // namespace s21
#endif  // SRC_CALC_PLUS_MODEL_H_
