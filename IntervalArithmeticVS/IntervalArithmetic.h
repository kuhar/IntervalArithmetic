/*
 * IntervalArithmetic.h
 *
 *  Created on: 20-11-2012
 *  Author: Tomasz Hoffmann and Andrzej Marciniak
 *  Some utter crap fixed by Jakub Kuderski
 *
 *  Before you start use module, please install libraries:
 *  	- GNU MPFR ( http://www.mpfr.org/ )
 */

#ifndef INTERVALARITHMETIC_H_
#define INTERVALARITHMETIC_H_

#include <string>

namespace IntervalArithmetic
{
	struct interval
	{
		long double a;
		long double b;
	};

	long double IntWidth(const interval& x) noexcept;
	interval IAdd(const interval& x, const interval& y) noexcept;
	interval ISub(const interval& x, const interval& y) noexcept;
	interval IMul(const interval& x, const interval& y) noexcept;
	interval IDiv(const interval& x, const interval& y);
	long double DIntWidth(const interval& x) noexcept;
	interval Projection(const interval& x) noexcept;
	interval Opposite(const interval& x) noexcept;
	interval Inverse(const interval& x) noexcept;
	interval DIAdd(const interval& x, const interval& y) noexcept;
	interval DISub(const interval& x, const interval& y) noexcept;
	interval DIMul(const interval& x, const interval& y) noexcept;
	interval DIDiv(const interval& x, const interval& y);
	interval IntRead(const std::string& sa) noexcept;
	long double LeftRead(const std::string& sa) noexcept;
	long double RightRead(const std::string& sa) noexcept;
	interval ISin(const interval& x, char& st);
	interval ICos(const interval& x, char& st);
	interval IExp(const interval& x, char& st);
	interval ISqr(const interval& x, char& st) noexcept;
	interval ISqr2();
	interval ISqr3();
	interval IPi();
	void IEndsToStrings(const interval& i, std::string& left, std::string& right);
} // namespace IntervalArithmetic

#endif /* INTERVALARITHMETIC_H_ */
