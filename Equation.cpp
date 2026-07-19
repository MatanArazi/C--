#include "Equation.hpp"

#include <cmath>
#include <iomanip>
#include <ostream>
#include <stdexcept>

Equation::Equation(double a_value, double b_value, double c_value)
	: a(a_value), b(b_value), c(c_value), solutions(nullptr), solutions_size(0) {
	if (a_value == 0.0) {
		throw std::invalid_argument("a must not be zero");
	}

	update_solutions();
}

Equation::Equation(const Equation& other)
	: a(other.a), b(other.b), c(other.c), solutions(nullptr), solutions_size(0) {
	copy_solutions_from(other);
}

Equation& Equation::operator=(const Equation& other) {
	if (this == &other) {
		return *this;
	}

	// Copy dynamic memory first so allocation failure leaves *this unchanged.
	copy_solutions_from(other);
	a = other.a;
	b = other.b;
	c = other.c;
	return *this;
}

Equation::Equation(Equation&& other) noexcept
	: a(other.a),
	  b(other.b),
	  c(other.c),
	  solutions(other.solutions),
	  solutions_size(other.solutions_size) {
	// Keep moved-from object valid and safe to destroy.
	other.a = 1.0;
	other.b = 0.0;
	other.c = 0.0;
	other.solutions = nullptr;
	other.solutions_size = 0;
}

Equation& Equation::operator=(Equation&& other) noexcept {
	if (this == &other) {
		return *this;
	}

	clear_solutions();

	a = other.a;
	b = other.b;
	c = other.c;
	solutions = other.solutions;
	solutions_size = other.solutions_size;

	other.a = 1.0;
	other.b = 0.0;
	other.c = 0.0;
	other.solutions = nullptr;
	other.solutions_size = 0;

	return *this;
}

Equation::~Equation() {
	clear_solutions();
}

double Equation::get_a() const {
	return a;
}

double Equation::get_b() const {
	return b;
}

double Equation::get_c() const {
	return c;
}

void Equation::set_a(double value) {
	if (value == 0.0) {
		throw std::invalid_argument("a must not be zero");
	}

	a = value;
	update_solutions();
}

void Equation::set_b(double value) {
	b = value;
	update_solutions();
}

void Equation::set_c(double value) {
	c = value;
	update_solutions();
}

std::size_t Equation::get_solutions_size() const {
	return solutions_size;
}

const double* Equation::get_solutions() const {
	return solutions;
}

void Equation::update_solutions() {
	clear_solutions();

	const double discriminant = (b * b) - (4.0 * a * c);
	const double denominator = 2.0 * a;

	if (discriminant > 0.0) {
		solutions_size = 2;
		solutions = new double[2];
		const double root_part = std::sqrt(discriminant);
		solutions[0] = (-b + root_part) / denominator;
		solutions[1] = (-b - root_part) / denominator;
	} else if (discriminant == 0.0) {
		solutions_size = 1;
		solutions = new double[1];
		solutions[0] = -b / denominator;
	} else {
		solutions_size = 0;
		solutions = nullptr;
	}
}

void Equation::clear_solutions() {
	delete[] solutions;
	solutions = nullptr;
	solutions_size = 0;
}

void Equation::copy_solutions_from(const Equation& other) {
	if (other.solutions_size == 0) {
		clear_solutions();
		return;
	}

	double* new_solutions = new double[other.solutions_size];
	for (std::size_t i = 0; i < other.solutions_size; ++i) {
		new_solutions[i] = other.solutions[i];
	}

	clear_solutions();
	solutions = new_solutions;
	solutions_size = other.solutions_size;
}

std::ostream& operator<<(std::ostream& os, const Equation& equation) {
	os << std::fixed << std::setprecision(1) << equation.get_a() << " X^2 + " << equation.get_b()
	   << " X + " << equation.get_c() << " = 0";
	return os;
}

Equation operator+(const Equation& lhs, const Equation& rhs) {
	return Equation(lhs.get_a() + rhs.get_a(), lhs.get_b() + rhs.get_b(), lhs.get_c() + rhs.get_c());
}

Equation operator+(const Equation& lhs, double value) {
	return Equation(lhs.get_a(), lhs.get_b(), lhs.get_c() + value);
}

Equation operator+(double value, const Equation& rhs) {
	return Equation(rhs.get_a(), rhs.get_b(), rhs.get_c() + value);
}
