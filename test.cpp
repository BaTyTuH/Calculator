#include <gtest/gtest.h>
#include <queue>
#include <cmath>
#include <stack>

void assign_group(int& group, std::pair<double, int>& current, int* next) {
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
    } else if (current.second < 0) {  // x
        group = 2;
        next[2] = next[4] = 1;
    } else if (current.second == 2 || current.second == 4) {  // */^mod
        group = 3;
        next[1] = next[3] = next[5] = 1;
    } else if (current.second == 5) {  // sin, ...
        group = 4;
        next[1] = next[5] = next[3] = 1;
    } else if (current.first == 19) {  // )
        group = 5;
        next[2] = next[4] = next[3] = 1;
    } else if (current.first == 18) {  // (
        group = 6;
        next[3] = next[5] = next[1] = 1;
    }
}

void valid(std::queue<std::pair<double, int>>& out) {
    int flag_bracket = 0;
    std::pair<double, int> current;
    std::queue<std::pair<double, int>> res;
    int group = 0;
    int now[6] = {1, 1, 0, 1, 0, 1};
    int next[6] = {0};
    while (out.size()) {
        assign_group(group, out.front(), next);
        current = out.front();
        if (group == 5)
            flag_bracket--;
        if (!now[group-1]) throw std::invalid_argument("Ошибка: невозможное выражение!");
        if (flag_bracket < 0) throw std::invalid_argument("Ошибка: нет открывающей скобки!");
        if (group == 6)
            flag_bracket++;
        for (int i = 0; i < 6; i++)
            now[i] = next[i];
        res.push(current);
        out.pop();
    }
    if (group == 1 || group == 3 || group == 4)
        throw std::invalid_argument("Ошибка: неоконченное выражение!");
    if (flag_bracket) throw std::invalid_argument("Ошибка: незакрытая скобка!");
    out.swap(res);
}

void ready_PRN(std::queue<std::pair<double, int>>& out) {
    std::queue<std::pair<double, int>> res;
    std::stack<std::pair<double, int>> buf;
    std::pair<double, int> elem;
    while (!out.empty()) {
        elem = out.front();
        out.pop();
        if (elem.second < 0) {
            res.push(elem);
        } else if (elem.first == 18 || elem.second == 5 || elem.second == 3 || buf.empty()) {
            buf.push(elem);
        } else if (elem.first == 19) {
            while (buf.top().first != 18) {
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
    out.swap(res);
}

void unary_operation(std::pair<double, int> current, std::stack<std::pair<double, int>>& numbers) {
    if (current.first == 8) {
        numbers.top().first *= -1;
    } else if (current.first == 9) {
        numbers.top().first = cos(numbers.top().first);
    } else if (current.first == 10) {
        numbers.top().first = sin(numbers.top().first);
    } else if (current.first == 11) {
        numbers.top().first = tan(numbers.top().first);
    } else if (current.first == 12) {
        numbers.top().first = acos(numbers.top().first);
    } else if (current.first == 13) {
        numbers.top().first = asin(numbers.top().first);
    } else if (current.first == 14) {
        numbers.top().first = atan(numbers.top().first);
    } else if (current.first == 15) {
        numbers.top().first = log(numbers.top().first);
    } else if (current.first == 16) {
        numbers.top().first = log10(numbers.top().first);
    } else if (current.first == 17) {
        numbers.top().first = sqrt(numbers.top().first);
    }
}

void binary_operation(std::pair<double, int> current, \
                             std::stack<std::pair<double, int>>& numbers, double number2) {
    if (current.first == 1) {
        numbers.top().first = numbers.top().first + number2;
    } else if (current.first == 2) {
        numbers.top().first = numbers.top().first - number2;
    } else if (current.first == 3) {
        numbers.top().first = numbers.top().first * number2;
    } else if (current.first == 4) {
        numbers.top().first = numbers.top().first / number2;
    } else if (current.first == 5) {
        numbers.top().first = fmod(numbers.top().first, number2);
    } else if (current.first == 6) {
        numbers.top().first = pow(numbers.top().first, number2);
    }
}

void operation(std::pair<double, int> current, std::stack<std::pair<double, int>>& numbers) {
    if (current.second == 3 || current.second == 5) {
        unary_operation(current, numbers);
    } else {
        std::pair<double, int> buf = numbers.top();
        numbers.pop();
        binary_operation(current, numbers, buf.first);
    }
}

double arithmetic(std::queue<std::pair<double, int>>& myExpression) {
    std::pair<double, int> current;
    std::stack<std::pair<double, int>> numbers;
    while (!myExpression.empty()) {
        current = myExpression.front();
        // std::cout<<current.first<<' '<<current.second<<'\n';
        myExpression.pop();
        if (current.second < 0) {
            numbers.push(current);
        } else {
            operation(current, numbers);
        }
    }
    current = numbers.top();
    return current.first;
}


TEST(normal_value_1, true) {
    std::queue<std::pair<double, int>> test;
    test.push({18, 6});  // (
    test.push({-14, -1});  // -14
    test.push({7, 1});  // +
    test.push({123, -1});  // 123
    test.push({19, 6});  // )
    test.push({4, 2});  // /
    test.push({9, 5});  // cos
    test.push({M_PI, -1});  // π
    test.push({8, 1});  // -
    test.push({17, 5});  // sqrt
    test.push({4, -1});  // 4
    test.push({7, 1});  // +
    test.push({2, -1});  // 2
    test.push({3, 2});  // *
    test.push({2, -1});  // 2
    test.push({8, 1});  // -
    test.push({18, 6});  // (
    test.push({18, 6});  // (
    test.push({2, -1});  // 2
    test.push({19, 6});  // )
    test.push({19, 6});  // )
    test.push({7, 1});  // +
    test.push({2, -1});  // 2
    test.push({6, 4});  // ^
    test.push({3, -1});  // 3
    test.push({5, 2});  // mod
    test.push({3, -1});  // 3
    valid(test);
    ready_PRN(test);
    ASSERT_EQ(arithmetic(test), -107);
}

TEST(normal_value_2, true) {
    std::queue<std::pair<double, int>> test;
    test.push({9, 5});  // cos
    test.push({18, 6});  // (
    test.push({12, 5});  // acos
    test.push({1, -1});  // 1
    test.push({19, 6});  // )
    test.push({7, 1});  // +
    test.push({10, 5});  // sin
    test.push({18, 6});  // (
    test.push({13, 5});  // asin
    test.push({1, -1});  // 1
    test.push({19, 6});  // )
    test.push({7, 1});  // +
    test.push({11, 5});  // tan
    test.push({18, 6});  // (
    test.push({14, 5});  // atan
    test.push({0, -1});  // 0
    test.push({19, 6});  // )
    test.push({7, 1});  // +
    test.push({15, 5});  // ln
    test.push({M_E, -1});  // e
    test.push({7, 1});  // +
    test.push({16, 5});  // log
    test.push({10, -1});  // 10
    valid(test);
    ready_PRN(test);
    double x = arithmetic(test);
    ASSERT_EQ(x, 4);
}

TEST(unnormal1, exception) {
    std::queue<std::pair<double, int>> test;
    test.push({9, 5});  // cos
    test.push({18, 6});  // (
    test.push({18, 6});  // (
    test.push({18, 6});  // (
    test.push({18, 6});  // (
    test.push({0, -1});  // 0
    test.push({19, 6});  // )
    test.push({19, 6});  // )
    ASSERT_THROW(valid(test), std::invalid_argument);
}

TEST(unnormal2, exception) {
    std::queue<std::pair<double, int>> test;
    test.push({9, 5});  // cos
    test.push({18, 6});  // (
    test.push({18, 6});  // (
    test.push({0, -1});  // 0
    test.push({19, 6});  // )
    test.push({19, 6});  // )
    test.push({8, 1});  // -
    test.push({3, 2});  // *
    test.push({5, -1});  // )
    ASSERT_THROW(valid(test), std::invalid_argument);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
