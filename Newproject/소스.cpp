#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <cstring>
#include <queue>
#pragma warning(disable:4996)

using namespace std;
class Strategy {
public:
    virtual double doOperation(double num1, double num2) = 0;
};
class OperationAdd : public Strategy {
    double doOperation(double num1, double num2) {
        return num1 + num2;
    }
};
class OperationSub : public Strategy {
    double doOperation(double num1, double num2) {
        return num1 - num2;
    }
};
class OperationMul : public Strategy {
    double doOperation(double num1, double num2) {
        return num1 * num2;
    }
};
class OperationDiv : public Strategy {
    double doOperation(double num1, double num2) {
        return num1 / num2;
    }
};
class Context {
private:
    Strategy* strategy;
public:
    void setStrategy(Strategy* strategy) {
        this->strategy = strategy;
    }
    double executeStrategy(double num1, double num2) {
        return (strategy->doOperation(num1, num2));
    }
};
bool checkline(string sentence) {
    string str = sentence;
    istringstream iss(str);
    string stringBuffer;
    queue<string> queue;
    string result;
    int startparenthesis = 0;
    int endparenthesis = 0;
    int num_of_operator = 0;
    int num_of_operand = 0;
    while (getline(iss, stringBuffer, ' ')) {
        queue.push(stringBuffer);
    }
    while (!queue.empty()) {
        if (queue.front() == "(") {
            startparenthesis++;
        }
        else if (queue.front() == ")") {
            endparenthesis++;
        }
        else if (queue.front() == "*" || queue.front() == "/" ||
            queue.front() == "+" || queue.front() == "-") {
            num_of_operator++;
        }
        else {
            num_of_operand++;
        }
        queue.pop();
    }
    if (startparenthesis != endparenthesis ||
        num_of_operand - num_of_operator != 1) {
        return false;
    }
    return true;
}
double hexToDec(string d) {
    double result = 0;
    char array[1024];
    strcpy(array, d.c_str());
    result = (int)strtol(array, NULL, 16);
    return result;
}
//2����(string)�� 10���� double������ ��ȯ�ϴ� �Լ�
double binToDec(string d) {
    double result = 0;
    int e = 1;
    for (int i = d.length() - 1; i >= 0; i--) {
        for (int j = 0; j < d.length() - i - 1; j++) {
            e *= 2;
        }if (d[i] == '1') {
            result += e;
        }
        e = 1;
    }
    return result;
}
string conToDecimal(string sentence) {
    string str = sentence;
    istringstream iss(str);
    string stringBuffer;
    queue<string> queue;
    string result;
    while (getline(iss, stringBuffer, ' ')) {
        queue.push(stringBuffer);
    }
    while (!queue.empty()) {
        int operand = 0;
        string oper = queue.front();
        if (oper.at(0) == '0' && oper.at(1) == 'x') {
            operand = hexToDec(oper.substr(2, oper.length() - 2));
            oper = to_string(operand);
        }
        else if (oper.at(0) == '0' && oper.at(1) == 'b') {
            operand = binToDec(oper.substr(2, oper.length() - 2));
            oper = to_string(operand);
        }
        result += oper;
        result += " ";
        queue.pop();
    }
    return result;
}
struct oper {
    int p; // �������� �켱����
    string o; // ������
};

stack<double> num; // �ǿ����� ����
stack<oper> op; // ������ ����

void calc() {
    Context* c = new Context();
    Strategy* str;
    double a, b, result;
    b = num.top();
    num.pop();
    a = num.top();
    num.pop();
    string oper = op.top().o;
    op.pop();

    if (oper == "*") {
        //result = a * b;
        str = new OperationMul();
        c->setStrategy(str);
        result = c->executeStrategy(a, b);
    }
    else if (oper == "/") {
        //result = a / b;
        try {
            if (b == 0) {
                throw b;
            }
            str = new OperationDiv();
            c->setStrategy(str);
            result = c->executeStrategy(a, b);
        }
        catch (int& b) {
            cout << "Exception: divided by 0" << endl;
        }
    }
    else if (oper == "+") {
        //result = a + b;
        str = new OperationAdd();
        c->setStrategy(str);
        result = c->executeStrategy(a, b);
    }
    else if (oper == "-") {
        //result = a - b;
        str = new OperationSub();
        c->setStrategy(str);
        result = c->executeStrategy(a, b);
    }
    // ��� �� ���ÿ� �ٽ� ����
    num.push(result);
}

int main() {

    string str;
    cout << "Calculation(�������� ����): ";
    getline(cin, str);
    if (checkline(str)) {
        string answer = conToDecimal(str);
    }
    else {
        cout << "Wrong Format" << endl;
        return 0;
    }
    string answer = conToDecimal(str);

    stringstream ss(answer);

    // ������ �켱������ ���� ���ÿ� push
    // ( ) -> 0
    // + - -> 1
    // * / -> 2
    string tok;
    while (ss >> tok) {

        if (tok == "(") {
            op.push({ 0, tok });
        }
        else if (tok == ")") {
            while (op.top().o != "(") {
                calc();
            }
            op.pop();
        }
        else if (tok == "*" || tok == "/" || tok == "+" || tok == "-") {
            int prior;
            if (tok == "*")
                prior = 2;
            else if (tok == "/")
                prior = 2;
            else if (tok == "+")
                prior = 1;
            else if (tok == "-")
                prior = 1;

            while (!op.empty() && prior <= op.top().p) {
                calc();
            }

            op.push({ prior, tok });
        }
        else
            num.push(stoi(tok));
    }
    while (!op.empty()) {
        calc();
    }
    cout << num.top();
    return 0;
}
