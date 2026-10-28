#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <utility>

class Polynomial{
    double* coefs_array_;
    int degree_;

public:

    Polynomial();

    explicit Polynomial(int degree);
    
    Polynomial(int degree, double* values);

    Polynomial(std::initializer_list<double> values);

    Polynomial(const Polynomial& other);

    Polynomial(Polynomial&& other) noexcept;

    friend void swap(Polynomial& a, Polynomial& b) noexcept {
        using std::swap;
        swap(a.degree_, b.degree_);
        swap(a.coefs_array_, b.coefs_array_);
    };


    Polynomial& operator=(Polynomial other) {
        swap(*this, other); 
        return *this;
    }

    void trim();

    double evaluate(double x) const;

    double definite_integral(double a, double b) const;

    void resize(int degree);

    void add(const Polynomial& b);

    void print(std::ostream& out) const;

    ~Polynomial();
};