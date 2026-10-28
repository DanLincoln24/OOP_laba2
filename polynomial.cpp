#include <iostream>
#include <polynomial.h>
#include <algorithm>
#include <stdexcept>
#include <math.h>

Polynomial::Polynomial() : coefs_array_(nullptr), degree_(-1){}

Polynomial::Polynomial(int degree)
    : coefs_array_(degree > -1 ? new double[degree + 1]{} : nullptr),
      degree_(degree) {}

Polynomial::Polynomial(int degree, double* array)
    : Polynomial(degree) {
    for (int i = 0; i <= degree; ++i) {
        coefs_array_[i] = array[i];
    }
}
Polynomial::Polynomial(const Polynomial& other)
    : coefs_array_(other.degree_ > -1 ? new double[other.degree_ + 1]{} : nullptr),
    degree_(other.degree_) {
    std::copy(other.coefs_array_, other.coefs_array_ + other.degree_ + 1, coefs_array_);
}

Polynomial::Polynomial(Polynomial&& other) noexcept : degree_(other.degree_), coefs_array_(other.coefs_array_) {
        other.coefs_array_ = nullptr;
        other.degree_ = -1;
}

Polynomial::Polynomial(std::initializer_list<double> values) {
    if(values.size() == 0) {
        coefs_array_ = nullptr;
        degree_ = -1;
        return;
    }
    degree_ = values.size() - 1;
    coefs_array_ = new double[values.size()];
    std::copy(values.begin(), values.end(), coefs_array_);
    trim();
}

Polynomial::~Polynomial() {
    delete[] coefs_array_;
}

double Polynomial::evaluate(double x) const { // cхема горнера
    if(degree_ == -1 || coefs_array_ == nullptr) return 0.0;
    double res = coefs_array_[degree_];
    for(int i = degree_ - 1; i >= 0; --i) {
        res = res * x + coefs_array_[i];
    }
    return res;
}

double Polynomial::definite_integral(double a, double b) const {
    if(degree_ == -1 || coefs_array_ == nullptr) return 0.0;
    Polynomial integral(degree_ + 1);
    integral.coefs_array_[0] = 0.0;
    for(int i = 1; i <= degree_ + 1; ++i) {
        integral.coefs_array_[i] = coefs_array_[i - 1] / static_cast<double>(i); 
    }
    return integral.evaluate(b) - integral.evaluate(a);
}

void Polynomial::resize(int degree) {
    double* new_arr = new double[degree + 1]{};
    if(coefs_array_ != nullptr) {
        for(int i = 0; i <= std::min(degree_, degree); ++i) {
            new_arr[i] = coefs_array_[i];
        }
    }
    delete[] coefs_array_;
    coefs_array_ = new_arr;
    degree_ = degree;
}

void Polynomial::trim() {
    for(int i = degree_; i >= 0; --i) {
        if(coefs_array_[i] != 0.0) break;
        degree_--;
        if(i == 0) {
            delete[] coefs_array_;
            coefs_array_ = nullptr;
        }
    }
}

void Polynomial::add(const Polynomial& b) {
    if(b.degree_ > degree_) {
        resize(b.degree_);
    }
    for(int i = 0; i <= b.degree_; ++i) {
        coefs_array_[i] += b.coefs_array_[i];
    }
    trim();
}

void Polynomial::print(std::ostream& out) const {
    if (degree_ < 0 || coefs_array_ == nullptr) {
        out << 0 << '\n';
        return;
    }

    bool first = true;

    for (int i = 0; i <= degree_; ++i) {
        double c = coefs_array_[i];
        if (c == 0.0) continue;

        if (first) {
            if (c < 0) out << "-";
        } else {
            out << (c < 0 ? " - " : " + ");
        }

        double abs_c = (c < 0) ? -c : c;
        if (i == 0 || abs_c != 1.0) {
            out << abs_c;
        }

        if (i == 1) {
            out << "x";
        } else if (i > 1) {
            out << "x^" << i;
        }

        first = false;
    }

    if (first) {
        out << 0;
    }
    out << '\n';
}

