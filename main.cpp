#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

// Функция для вычисления НОД (алгоритм Евклида)
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

// Функция для вычисления степени полинома по модулю другого полинома
vector<int> poly_pow_mod(const vector<int>& base, int exp, const vector<int>& mod, int p) {
    vector<int> result = { 1 }; // Полином x^0 = 1
    vector<int> base_copy = base;

    while (exp > 0) {
        if (exp % 2 == 1) {
            // result = (result * base_copy) % mod
            vector<int> new_result(result.size() + base_copy.size() - 1, 0);
            for (size_t i = 0; i < result.size(); ++i) {
                for (size_t j = 0; j < base_copy.size(); ++j) {
                    new_result[i + j] = (new_result[i + j] + result[i] * base_copy[j]) % p;
                }
            }
            result = new_result;

            // result = result % mod
            while (result.size() >= mod.size()) {
                if (result.back() != 0) {
                    for (size_t i = 0; i < mod.size(); ++i) {
                        result[result.size() - mod.size() + i] = (result[result.size() - mod.size() + i] - mod[i] + p) % p;
                    }
                }
                result.pop_back();
            }
        }

        // base_copy = (base_copy * base_copy) % mod
        vector<int> new_base_copy(base_copy.size() * 2 - 1, 0);
        for (size_t i = 0; i < base_copy.size(); ++i) {
            for (size_t j = 0; j < base_copy.size(); ++j) {
                new_base_copy[i + j] = (new_base_copy[i + j] + base_copy[i] * base_copy[j]) % p;
            }
        }
        base_copy = new_base_copy;

        // base_copy = base_copy % mod
        while (base_copy.size() >= mod.size()) {
            if (base_copy.back() != 0) {
                for (size_t i = 0; i < mod.size(); ++i) {
                    base_copy[base_copy.size() - mod.size() + i] = (base_copy[base_copy.size() - mod.size() + i] - mod[i] + p) % p;
                }
            }
            base_copy.pop_back();
        }

        exp /= 2;
    }

    return result;
}

// Функция для генерации случайного полинома
vector<int> generate_random_polynomial(int n, int p) {
    vector<int> polynomial(n + 1);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, p - 1);

    for (int i = 0; i < n; ++i) {
        polynomial[i] = dis(gen);
    }
    polynomial[n] = 1; // Старшая степень полинома равна 1

    return polynomial;
}

// Функция для проверки полинома на неприводимость
bool is_irreducible(const vector<int>& poly, int p) {
    int n = poly.size() - 1;
    vector<int> u = { 0, 1 }; // u(x) = x

    for (int i = 0; i < n / 2; ++i) {
        u = poly_pow_mod(u, p, poly, p);
        vector<int> u_minus_x = u;
        u_minus_x[0] = (u_minus_x[0] - 1 + p) % p;

        // Вычисляем НОД полинома и (u(x) - x)
        vector<int> d = { 1 };
        for (size_t j = 0; j < u_minus_x.size(); ++j) {
            if (u_minus_x[j] != 0) {
                d[0] = gcd(d[0], u_minus_x[j]);
            }
        }

        if (d[0] != 1) {
            return false; // Полином приводим
        }
    }

    return true; // Полином неприводим
}

int main() {
    setlocale(LC_ALL, "Russian");

    int p = 101; // Поле GF(p)
    int n = 25; // Степень полинома

    // Генерируем случайный полином
    vector<int> polynomial = generate_random_polynomial(n, p);

    // Выводим сгенерированный полином
    cout << "Сгенерированный полином: ";
    for (int i = n; i >= 0; --i) {
        cout << polynomial[i] << "x^" << i;
        if (i != 0) cout << " + ";
    }
    cout << endl;

    cout << "В другом формате: ";
    for (size_t i = 0; i < polynomial.size(); ++i) {
        cout << polynomial[i];
        if (i != polynomial.size() - 1) cout << ", ";
    }
    cout << endl;

    // Проверяем полином на неприводимость
    if (is_irreducible(polynomial, p)) {
        cout << "Полином неприводим." << endl;
    }
    else {
        cout << "Полином приводим." << endl;
    }

    return 0;
}
