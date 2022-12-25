#include <cmath>
#include "model.h"

namespace s21 {
Model::Model() {
    x = 0;
    expression = new std::queue<std::pair<double, int>>;
}

Model::~Model() {
    delete expression;
}

void Model::credit_calc_annuitet(double sumCredit, double time, double percent, \
                            double* payMonth, double* overpay, double* all) {
    percent /= 12;
    *payMonth = sumCredit*percent*pow((1+percent), time)/(pow((1+percent), time)-1);
    for (int i = 1; i < time; i++) {
        *(payMonth+i) = *payMonth;
     }
    *all = *payMonth*time;
    *overpay = *all-sumCredit;
}

void Model::credit_calc_difference(double sumCredit, double time, double percent, \
                            double* payMonth, double* overpay, double* all) {
    percent /= 12;
    double ost = sumCredit;
    double dif = sumCredit/time;
    for (int i = 0; i < time; i++) {
        *(payMonth+i) = dif+ost*percent;
        *all += *(payMonth+i);
        ost -= dif;
    }
    *overpay = *all-sumCredit;
}


void Model::pars(QString str) {
    while (!expression->empty()) expression->pop();
    while (str.length()) {
        if (str.at(0) == 'x' || str.at(0) == 'X') {
            expression->push({x, 0});
        } else if ((str.at(0) >= '0' && str.at(0) <= '9') || str.at(0) == '.') {
            fromQstrToNumber(str);
        } else if (str.at(0) == '+') {
            expression->push({PLUS, 3});
        } else if (str.at(0) == '-') {
            expression->push({MIN, 3});
        } else if (str.at(0) == '*') {
            expression->push({MUL, 2});
        } else if (str.at(0) == '%') {
            expression->push({MOD, 2});
        } else if (str.at(0) == '/') {
            expression->push({DIV, 2});
        } else if (str.at(0) == '^') {
            expression->push({POW, 4});
        } else if (str.at(0) == '(') {
            expression->push({START, 6});
        } else if (str.at(0) == ')') {
            expression->push({FINISH, 6});
        } else if (str.at(0) == 'e') {
            expression->push({M_E, -1});
        } else if (str.at(0) == ' ' || str.at(0) == '\t' || str.at(0) == '\n') {
        } else {
            trigonometry(str);
        }
        str.remove(0, 1);
    }
}

void Model::trigonometry(QString& str) {
    if (str.left(3) == "sin") {
        expression->push({SIN, 5});
        str.remove(0, 2);
    } else if (str.left(3) == "cos") {
        expression->push({COS, 5});
        str.remove(0, 2);
    } else if (str.left(3) == "tan") {
        expression->push({TAN, 5});
        str.remove(0, 2);
    } else if (str.left(4) == "asin") {
        expression->push({ASIN, 5});
        str.remove(0, 3);
    } else if (str.left(4) == "acos") {
        expression->push({ACOS, 5});
        str.remove(0, 3);
    } else if (str.left(4) == "atan") {
        expression->push({ATAN, 5});
        str.remove(0, 3);
    } else if (str.left(2) == "ln") {
        expression->push({LN, 5});
        str.remove(0, 1);
    } else if (str.left(3) == "log") {
        expression->push({LOG, 5});
        str.remove(0, 2);
    } else if (str.left(4) == "sqrt") {
        expression->push({SQRT, 5});
        str.remove(0, 3);
    } else if (str.left(3) == "mod") {
        expression->push({MOD, 2});
        str.remove(0, 2);
    } else if (str.left(1) == "π") {
        expression->push({M_PI, -1});
    } else {
       throw std::invalid_argument("Ошибка: неопознанная функция!");
    }
}

void Model::fromQstrToNumber(QString& str) {
    size_t i_number = 0, check = 0;
    size_t max = str.size();
    while (max-i_number > 0 && ((str.at(i_number) >= '0' && str.at(i_number) <= '9') \
                                                    || str.at(i_number) == '.')) {
        if (str.at(i_number) == '.') {
            if (!check) check++;
            else
                break;
        }
        i_number++;
    }
    if (max-i_number && str.at(i_number) == 'e') {
        i_number++;
        if (max-i_number && (str.at(i_number) == '+' || str.at(i_number) == '-')) {
            i_number++;
        }
        size_t trigger = i_number;
        while (max-i_number && (str.at(i_number) >= '0' && str.at(i_number) <= '9')) {
            i_number++;
        }
        if (trigger == i_number) throw std::invalid_argument("Ошибка: неправильная экспоненциальная запись!");
    }
    expression->push({str.left(i_number).toDouble(), -1});
    str.remove(0, i_number-1);
}

void Model::assign_group(int& group, std::pair<double, int>& current, int* next) {
    for (int i = 1; i < 6; i++)
        next[i] = 0;
    next[0] = 1;
    if (current.second == 3 || current.second == 1) {  // +-
        if (group == 2 || group == 5) {
            current.second = 1;
            current.first -= 6;
        }
        group = 1;
        next[1] = next[3] = next[5] = 1;
    } else if (current.second <= 0) {  // x
        group = 2;
        next[2] = next[4] = 1;
    } else if (current.second == 2 || current.second == 4) {  // */^mod
        group = 3;
        next[1] = next[3] = next[5] = 1;
    } else if (current.second == 5) {  // sin, ...
        group = 4;
        next[1] = next[5] = next[3] = 1;
    } else if (current.first == FINISH) {  // )
        group = 5;
        next[2] = next[4] = next[3] = 1;
    } else if (current.first == START) {  // (
        group = 6;
        next[3] = next[5] = next[1] = 1;
    }
}

void Model::valid() {
    int flag_bracket = 0;
    std::pair<double, int> current;
    std::queue<std::pair<double, int>> res;
    int group = 0;
    int now[6] = {1, 1, 0, 1, 0, 1};
    int next[6] = {0};
    while (expression->size()) {
        assign_group(group, expression->front(), next);
        current = expression->front();
        if (group == 5)
            flag_bracket--;
        if (!now[group-1]) throw std::invalid_argument("Ошибка: невозможное выражение!");
        if (flag_bracket < 0) throw std::invalid_argument("Ошибка: нет открывающей скобки!");
        if (group == 6)
            flag_bracket++;
        for (int i = 0; i < 6; i++)
            now[i] = next[i];
        res.push(current);
        expression->pop();
    }
    if (group == 1 || group == 3 || group == 4)
        throw std::invalid_argument("Ошибка: неоконченное выражение!");
    if (flag_bracket) throw std::invalid_argument("Ошибка: незакрытая скобка!");
    expression->swap(res);
}

void Model::ready_PRN() {
    std::queue<std::pair<double, int>> res;
    std::stack<std::pair<double, int>> buf;
    std::pair<double, int> elem;
    while (!expression->empty()) {
        elem = expression->front();
        expression->pop();
        if (elem.second <= 0) {
            res.push(elem);
        } else if (elem.first == START || elem.second == 5 || elem.second == 3 || buf.empty()) {
            buf.push(elem);
        } else if (elem.first == FINISH) {
            while (buf.top().first != START) {
                res.push(buf.top());
                buf.pop();
            }
            buf.pop();
        } else {
            while (buf.top().second >= elem.second && elem.second != 4 && buf.top().second != 6) {
                res.push(buf.top());
                buf.pop();
                if (buf.empty()) break;
            }
            buf.push(elem);
        }
    }
    while (!buf.empty()) {
        res.push(buf.top());
        buf.pop();
    }
    expression->swap(res);
}

double Model::arithmetic() {
    std::pair<double, int> current;
    std::stack<std::pair<double, int>> numbers;
    std::queue<std::pair<double, int>> myExpression(*expression);
    while (!myExpression.empty()) {
        current = myExpression.front();
        myExpression.pop();
        if (current.second <= 0) {
            if (current.second == 0) current.first = x;
            numbers.push(current);
        } else {
            operation(current, numbers);
        }
    }
    current = numbers.top();
    return current.first;
}

void Model::operation(std::pair<double, int> current, std::stack<std::pair<double, int>>& numbers) {
    if (current.second == 3 || current.second == 5) {
        unary_operation(current, numbers);
    } else {
        std::pair<double, int> buf = numbers.top();
        numbers.pop();
        binary_operation(current, numbers, buf.first);
    }
}

void Model::unary_operation(std::pair<double, int> current, std::stack<std::pair<double, int>>& numbers) {
    if (current.first == MIN) {
        numbers.top().first *= -1;
    } else if (current.first == COS) {
        numbers.top().first = cos(numbers.top().first);
    } else if (current.first == SIN) {
        numbers.top().first = sin(numbers.top().first);
    } else if (current.first == TAN) {
        numbers.top().first = tan(numbers.top().first);
    } else if (current.first == ACOS) {
        numbers.top().first = acos(numbers.top().first);
    } else if (current.first == ASIN) {
        numbers.top().first = asin(numbers.top().first);
    } else if (current.first == ATAN) {
        numbers.top().first = atan(numbers.top().first);
    } else if (current.first == LN) {
        numbers.top().first = log(numbers.top().first);
    } else if (current.first == LOG) {
        numbers.top().first = log10(numbers.top().first);
    } else if (current.first == SQRT) {
        numbers.top().first = sqrt(numbers.top().first);
    }
}

void Model::binary_operation(std::pair<double, int> current, \
                             std::stack<std::pair<double, int>>& numbers, double number2) {
    if (current.first == SUM) {
        numbers.top().first = numbers.top().first + number2;
    } else if (current.first == SUB) {
        numbers.top().first = numbers.top().first - number2;
    } else if (current.first == MUL) {
        numbers.top().first = numbers.top().first * number2;
    } else if (current.first == DIV) {
        numbers.top().first = numbers.top().first / number2;
    } else if (current.first == MOD) {
        numbers.top().first = fmod(numbers.top().first, number2);
    } else if (current.first == POW) {
        numbers.top().first = pow(numbers.top().first, number2);
    }
}
}  // namespace s21
