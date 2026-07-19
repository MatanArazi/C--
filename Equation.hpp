#ifndef EQUATION_HPP
#define EQUATION_HPP

#include <cstddef>
#include <iosfwd>

class Equation {
public:
	Equation(double a, double b, double c);

	Equation(const Equation& other);
	Equation& operator=(const Equation& other);

	Equation(Equation&& other) noexcept;
	Equation& operator=(Equation&& other) noexcept;

	~Equation();

	double get_a() const;
	double get_b() const;
	double get_c() const;

	void set_a(double value);
	void set_b(double value);
	void set_c(double value);

	std::size_t get_solutions_size() const;
	const double* get_solutions() const;

private:
	double a;
	double b;
	double c;
	double* solutions;
	std::size_t solutions_size;

	void update_solutions();
	void clear_solutions();
	void copy_solutions_from(const Equation& other);
};

std::ostream& operator<<(std::ostream& os, const Equation& equation);

Equation operator+(const Equation& lhs, const Equation& rhs);
Equation operator+(const Equation& lhs, double value);
Equation operator+(double value, const Equation& rhs);

#endif
