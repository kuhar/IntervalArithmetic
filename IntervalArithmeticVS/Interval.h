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

		Interval() noexcept = default;
		Interval(long double left, long double right) noexcept
			: m_value{left, right}
		{}
		Interval(const std::string& value) noexcept;

		long double getLeft() const noexcept { return m_value.a;  }
		std::string getLeftAsString() const;
		long double getRight() const noexcept { return m_value.b; }
		std::string getRightAsString() const;
		long double getWidth() const noexcept;

		Interval& operator+=(const Interval& rhs) noexcept;
		Interval operator+(const Interval& rhs) const noexcept;

		Interval& operator-=(const Interval& rhs) noexcept;
		Interval operator-(const Interval& rhs) const noexcept;

		Interval& operator*=(const Interval& rhs) noexcept;
		Interval operator*(const Interval& rhs) const noexcept;

		Interval& operator/=(const Interval& rhs);
		Interval operator/(const Interval& rhs) const;

		bool operator==(const Interval& rhs) const;

		Interval& opposite() noexcept;
		Interval getOpposite() const noexcept;
		Interval& invert() noexcept;
		Interval getInverse() const noexcept;

		ErrorCode<Interval> sin() const noexcept;
		ErrorCode<Interval> cos() const noexcept;
		ErrorCode<Interval> exp() const noexcept;
		ErrorCode<Interval> sqrt() const noexcept;

		static Interval sqrt2() noexcept { return {IntervalArithmetic::ISqr2()}; }
		static Interval sqrt3() noexcept { return {IntervalArithmetic::ISqr3()}; }
		static Interval pi() noexcept { return {IntervalArithmetic::IPi()}; }

		std::string to_string() const;

	private:
		Interval(IntervalArithmetic::interval interval) noexcept;

		IntervalArithmetic::interval m_value = {0.0, 0.0};
	};

	inline std::ostream& operator<<(std::ostream& os, const Interval& value)
	{
		return os << value.to_string();
	}

} // namespace ean