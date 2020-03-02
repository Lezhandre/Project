#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <algorithm>

using namespace std;

const double e = 2.718282;
const double pi = 3.141593;

inline short priority(string s){
    if (s == "+")
        return 0;
    if (s == "-")
        return 1;
    if (s == "*")
        return 2;
    if (s == "/")
        return 3;
    if (s == "int")
        return 4;
    if (s == "^")
        return 5;
    if (s == "log" || s == "sin" || s == "cos")
        return 6;
    return 7;
}

inline double strtodbl(string s){
    int t = 1;
    if (s[0] == '-'){
        t = -1;
    }
    unsigned long i;
    double a, b;
    for (i = 0; i < s.size() && s[i] != '.'; ++i){
        if (s[i] >= '0' && s[i] <= '9'){
            a = a * 10 + s[i] - '0';
        }
        else
            return 0.0 / 0.0;
    }
    unsigned long n = 0;
    if (i < s.size()){
        n = s.size() - i - 1;
        for (++i; i < s.size(); ++i){
            if (s[i] >= '0' && s[i] <= '9'){
                a = a * 10 + s[i] - '0';
            }
            else
                return 0.0 / 0.0;
        }
    }
    for (i = 0; i < n; ++i){a /= 10;}
    return t * a;
}

class fun{
public:
    string s;
    fun *left, *med, *right;
    double eps;

private:
    string exc_str(string str){
        int i, j;
        for (i = 0, j = 0; i < str.size(); ++i){
            if (str[i] != ' ')
                str[j] = str[i], ++j;
        }
        str.resize(j);
        return str;
    }

    void function(string f){
        unsigned i;
        stack <char> pars;
        unsigned t = 0;
        for (i = 0; i < f.size() && f[i] == '('; ++i){
            ++t;
            pars.push(')');
        }
        for (; i < f.size(); ++i){
            if (pars.size() != 0 && pars.top() == f[i])
                pars.pop();
            else if (f[i] == '(')
                pars.push(')');
            if (pars.size() < t && i < f.size() - t) //integral(0 ; x ; x^2)
                --t;
        }
        if (pars.size() != 0){
            s = "ooops!";
            return;
        }
        if (t){
            f.resize(f.size() - t);
            f = f.substr(t);
        }

        string h;
        short pr = 7;
        unsigned pos = 0;
        for (i = 0; i < f.size(); ++i){
            if (pars.size() != 0 && pars.top() == f[i])
                pars.pop();
            else if (f[i] == '(')
                pars.push(')');
            if (pars.size() == 0){
                if ((h.resize(1), h[0] = f[i], priority(h) < pr) || priority(h) == pr && h == "/"){
                    s = h;
                    pr = priority(h);
                    pos = i;
                }
                else if (i + 3 < f.size() && (h.resize(3), h[1] = f[i + 1], h[2] = f[i + 2], priority(h) < pr) && h != "int"){
                    s = h;
                    pr = priority(h);
                    pos = i;
                }
                else if (i + 7 < f.size()){
                    if (priority(h) < pr){
                        s = h;
                        pr = priority(h);
                        pos = i;
                    }
                }
            }
        }

        if (pr != 7)
            left = new fun;
        if (pr <= 5)
            right = new fun;
        if (pr < 4 || pr == 5){
            left->function(f.substr(0, pos));
            right->function(f.substr(pos + 1));
        }
        else if (pr == 6){
            left->function(f.substr(pos + 3));
        }
        else if (pr == 4){
            med = new fun;
            unsigned j = pos + 4;
            for (i = j; i < f.size() && f[i] != ';' && f[i] != ','; ++i){
                h.resize(i + 1 - j);
                h[i - j] = f[i];
            }
            left->function(h);
            for (++i, j = i; i < f.size() && f[i] != ';' && f[i] != ','; ++i){
                h.resize(i + 1 - j);
                h[i - j] = f[i];
            }
            med->function(h);
            for (++i, j = i; f.size() && f[i] != ')'; ++i){
                h.resize(i + 1 - j);
                h[i - j] = f[i];
            }
            right->function(h);
        }
        else
            s = f;
    }

public:
    void get_fun(string f){
        f = exc_str(f);
        function(f);
    }

    double fun_val(double x){
        if (s == "x" || s == "y")
            return x;
        if (s == "-")
            return left->fun_val(x) - right->fun_val(x);
        if (s == "+")
            return left->fun_val(x) + right->fun_val(x);
        if (s == "*")
            return left->fun_val(x) * right->fun_val(x);
        if (s == "/")
            return left->fun_val(x) / right->fun_val(x);
        if (s == "pi")
            return pi;
        if (s == "^")
            return pow(left->fun_val(x), right->fun_val(x));
        if (s == "exp")
            return e;
        if (s == "log")
            return log(left->fun_val(x));
        if (s == "sin")
            return sin(left->fun_val(x));
        if (s == "cos")
            return cos(left->fun_val(x));
        if (s == "int"){
            long long n = 2;
            double a, b, sn, s2n;
            a = left->fun_val(x);
            b = med->fun_val(x);
            eps = 0.00001 * n / fabs(b - a);
            s2n = (right->fun_val(a) + right->fun_val(b)) / 2;
            do {
                sn = s2n;
                for (long long i = 1; i < n; i += 2){
                    s2n += right->fun_val(a + (b - a) * i / n);
                }
                n *= 2; eps *= 2;
            } while(abs(s2n - 2 * sn) > eps);
            double val = 2 * s2n / n * (b - a);
            return val;
        }
        if (s == "")
            return 0;
        return strtodbl(s);
    }

    double min(double a, double b){
        double m = this->fun_val(a), step = (b - a) / 540;
        for (double i = a + step; i <= b; i += step){
            m = m <= this->fun_val(i) ? m: this->fun_val(i);
        }
        return m;
    }

    double max(double a, double b){
        double m = this->fun_val(a), step = (b - a) / 540;
        for (double i = a + step; i <= b; i += step){
            m = m >= this->fun_val(i) ? m: this->fun_val(i);
        }
        return m;
    }
};

#endif // FUNCTION_H
