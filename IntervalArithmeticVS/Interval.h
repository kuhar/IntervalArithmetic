/*
* Interval.h
*
*  Created on: 02-04-2015
*  Author: Jakub Kuderski
*/

#pragma once

#include <ostream>

#include "IntervalArithmetic.h"
#include "ErrorCode.h"

namespace ean
{
	class Interval final
	{
	public:
		static const char DivisionByZeroCode = 3;

		Interval() = default;
		Interval(long double left, long double right) 
		{
			m_value = {left, right};
		}

		Interval(const std::string& value);

		long double getLeft() const { return m_value.a;  }
		std::string getLeftAsString() const;
		long double getRight() const { return m_value.b; }
		std::string getRightAsString() const;
		long double getWidth() const;

		Interval& operator+=(const Interval& rhs);
		Interval operator+(const Interval& rhs) const;

		Interval& operator-=(const Interval& rhs);
		Interval operator-(const Interval& rhs) const;

		Interval& operator*=(const Interval& rhs);
		Interval operator*(const Interval& rhs) const;

		Interval& operator/=(const Interval& rhs);
		Interval operator/(const Interval& rhs) const;

		bool operator==(const Interval& rhs) const;

		Interval& opposite();
		Interval getOpposite() const;
		Interval& invert();
		Interval getInverse() const;

		ErrorCode<Interval> sin() const;
		ErrorCode<Interval> cos() const;
		ErrorCode<Interval> exp() const;
		ErrorCode<Interval> sqr() const;

		static Interval sqrt2() { return {IntervalArithmetic::ISqr2()}; }
		static Interval sqrt3() { return {IntervalArithmetic::ISqr3()}; }
		static Interval pi() { return {IntervalArithmetic::IPi()}; }

		std::string to_string() const;

	private:
		Interval(IntervalArithmetic::interval interval);

		IntervalArithmetic::interval m_value;
	};

	inline std::ostream& operator<<(std::ostream& os, const Interval& value)
	{
		return os << value.to_string();
	}

} // namespace ean