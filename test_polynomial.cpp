#include <gtest/gtest.h>
#include <sstream>
#include <cmath>
#include "polynomial.h"

// 1. Проверка конструктора по умолчанию (инвариант пустого объекта: degree == -1)
TEST(PolynomialBasics, DefaultConstructor) {
    Polynomial p;
    std::stringstream ss;
    p.print(ss);
    EXPECT_EQ(ss.str(), "0\n");
    EXPECT_DOUBLE_EQ(p.evaluate(5.0), 0.0);
}

// 2. Инициализация через список (initializer_list) и степень константы
TEST(PolynomialBasics, InitializerList) {
    Polynomial p = {1.0, 2.0, 3.0}; // 1 + 2x + 3x^2
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 1.0 + 2.0 * 2.0 + 3.0 * 4.0); // 17.0
}

// 3. Вычисление значения схемы Горнера (evaluate) в разных точках
TEST(PolynomialAlgorithm, Evaluate) {
    Polynomial p = {5.0, -2.0, 4.0}; // 5 - 2x + 4x^2
    EXPECT_DOUBLE_EQ(p.evaluate(0.0), 5.0);
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 7.0);
    EXPECT_DOUBLE_EQ(p.evaluate(-1.0), 11.0);
}

// 4. Правило пяти: Конструктор глубокого копирования (deep copy)
TEST(PolynomialRuleOfFive, CopyConstructor) {
    Polynomial p1 = {1.0, 2.0};
    Polynomial p2 = p1; // Вызов конструктора копирования
    
    // Проверяем независимость копии: меняем p1 через сложение
    p1.add(Polynomial{0.0, 5.0});
    EXPECT_DOUBLE_EQ(p2.evaluate(1.0), 3.0); // p2 не должен измениться
}

// 5. Правило пяти: Перемещающий конструктор (Move constructor, обнуление источника)
TEST(PolynomialRuleOfFive, MoveConstructor) {
    Polynomial p1 = {1.0, 3.0, 5.0};
    Polynomial p2 = std::move(p1);

    EXPECT_DOUBLE_EQ(p2.evaluate(1.0), 9.0);
    // Донор должен стать пустым полиномом (инвариант degree == -1, evaluate == 0)
    EXPECT_DOUBLE_EQ(p1.evaluate(1.0), 0.0);
}

// 6. Правило пяти: Оператор присваивания Copy-and-Swap (копирующее присваивание)
TEST(PolynomialRuleOfFive, CopyAssignment) {
    Polynomial p1 = {2.0, 4.0};
    Polynomial p2 = {10.0, 20.0, 30.0};
    p2 = p1;

    EXPECT_DOUBLE_EQ(p2.evaluate(2.0), 10.0);
}

TEST(PolynomialRuleOfFive, MoveAssignment) {
    Polynomial p;
    p = Polynomial{1.0, -1.0, 2.0}; // Временный объект rvalue
    EXPECT_DOUBLE_EQ(p.evaluate(2.0), 1.0 - 2.0 + 2.0 * 4.0); // 7.0
}

//self-assignment
TEST(PolynomialEdgeCases, SelfAssignment) {
    Polynomial p = {3.0, 2.0, 1.0};
    p = p;
    EXPECT_DOUBLE_EQ(p.evaluate(1.0), 6.0);
}

TEST(PolynomialEdgeCases, TrimLeadingZeros) {
    Polynomial p = {1.0, 2.0, 0.0, 0.0};
    std::stringstream ss;
    p.print(ss);
    EXPECT_EQ(ss.str(), "1 + 2x\n");
}

TEST(PolynomialMethods, AddDifferentDegrees) {
    Polynomial p1 = {1.0, 2.0};        // 1 + 2x
    Polynomial p2 = {0.0, 1.0, 1.0};   // x + x^2
    p1.add(p2);                        // 1 + 3x + x^2

    std::stringstream ss;
    p1.print(ss);
    EXPECT_EQ(ss.str(), "1 + 3x + x^2\n");
    EXPECT_DOUBLE_EQ(p1.evaluate(2.0), 11.0);
}

TEST(PolynomialMethods, AddCancellation) {
    Polynomial p1 = {2.0, 3.0, 4.0};   // 2 + 3x + 4x^2
    Polynomial p2 = {1.0, 0.0, -4.0};  // 1 - 4x^2
    p1.add(p2);                        // Старший коэффициент 4 - 4 = 0 зануляется

    std::stringstream ss;
    p1.print(ss);
    EXPECT_EQ(ss.str(), "3 + 3x\n");
}


TEST(PolynomialVariant10, DefiniteIntegralParabola) {
    Polynomial p = {0.0, 0.0, 1.0};
    
    double result = p.definite_integral(0.0, 2.0);
    EXPECT_NEAR(result, 8.0 / 3.0, 1e-4);
}

TEST(PolynomialVariant10, DefiniteIntegralLinear) {
    Polynomial p = {2.0, 6.0};
    // Первообразная: 2x + 3x^2. На отрезке [1, 3]: (2*3 + 3*9) - (2*1 + 3*1) = 33 - 5 = 28
    double result = p.definite_integral(1.0, 3.0);
    EXPECT_DOUBLE_EQ(result, 28.0);
}

TEST(PolynomialVariant10, DefiniteIntegralEdgeCases) {
    Polynomial empty_poly;
    EXPECT_DOUBLE_EQ(empty_poly.definite_integral(0.0, 10.0), 0.0);

    Polynomial p = {1.0, 2.0, 3.0};
    EXPECT_DOUBLE_EQ(p.definite_integral(2.5, 2.5), 0.0);
}