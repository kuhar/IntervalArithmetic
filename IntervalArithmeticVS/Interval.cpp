/*
* Interval.cpp
*
*  Created on: 02-04-2015
*  Author: Jakub Kuderski
*/

#include "Interval.h"
#include <cmath>
#include <cfloat>

using namespace std;

namespace ean
{
	Interval::Interval(const std::string& value) noexcept
		: m_value(IntervalArithmetic::IntRead(value))
	{}

	Interval::Interval(IntervalArithmetic::interval interval) noexcept
		: m_value(interval)
	{}

	long double Interval::getWidth() const noexcept
	{
		return IntervalArithmetic::IntWidth(m_value);
	}

	Interval& Interval::operator+=(const Interval& rhs) noexcept
	{
		m_value = IntervalArithmetic::IAdd(m_value, rhs.m_value);
		return *this;
	}

	Interval Interval::operator+(const Interval& rhs) const noexcept
	{
		return {IntervalArithmetic::IAdd(m_value, rhs.m_value)};
	}

	Interval& Interval::operator-=(const Interval& rhs) noexcept
	{
		m_value = IntervalArithmetic::ISub(m_value, rhs.m_value);
		return *this;
	}

	Interval Interval::operator-(const Interval& rhs) const noexcept
	{
		return {IntervalArithmetic::ISub(m_value, rhs.m_value)};
	}

	Interval& Interval::operator*=(const Interval& rhs) noexcept
	{
		m_value = IntervalArithmetic::IMul(m_value, rhs.m_value);
		return *this;
	}

	Interval Interval::operator*(const Interval& rhs) const noexcept
	{
		return {IntervalArithmetic::IMul(m_value, rhs.m_value)};
	}

	Interval& Interval::operator/=(const Interval& rhs)
	{
		m_value = IntervalArithmetic::IDiv(m_value, rhs.m_value);
		return *this;
	}

	Interval Interval::operator/(const Interval& rhs) const
	{
		return {IntervalArithmetic::IDiv(m_value, rhs.m_value)};
	}

	bool Interval::operator==(const Interval& rhs) const
	{
		return (std::fabs(m_value.a - rhs.m_value.a) < LDBL_EPSILON)
			&& (std::fabs(m_value.b - rhs.m_value.b) < LDBL_EPSILON);
	}

	Interval& Interval::opposite() noexcept
	{
		m_value = IntervalArithmetic::Opposite(m_value);
		return *this;
	}

	Interval Interval::getOpposite() const noexcept
	{
		return {IntervalArithmetic::Opposite(m_value)};
	}

	Interval& Interval::invert() noexcept
	{
		m_value = IntervalArithmetic::Inverse(m_value);
		return *this;
	}

	Interval Interval::getInverse() const noexcept
	{
		return {IntervalArithmetic::Inverse(m_value)};
	}

	ErrorCode<Interval> Interval::sin() const noexcept
	{
		try
		{
			char st;
			const auto result = IntervalArithmetic::ISin(m_value, st);
			return {{result}, st};
		}
		catch (...)
		{
			return  ErrorCode<Interval>::createError(DivisionByZeroCode);
		}
	}

	ErrorCode<Interval> Interval::cos() const noexcept
	{
		try
		{
			char st;
			const auto result = IntervalArithmetic::ICos(m_value, st);
			return {{result}, st};
		}
		catch (...)
		{
			return  ErrorCode<Interval>::createError(DivisionByZeroCode);
		}
	}

	ErrorCode<Interval> Interval::exp() const noexcept
	{
		try
		{
			char st;
			const auto result = IntervalArithmetic::IExp(m_value, st);
			return {{result}, st};
		}
		catch (...)
		{
			return  ErrorCode<Interval>::createError(DivisionByZeroCode);
		}
	}

	ErrorCode<Interval> Interval::sqrt() const noexcept
	{
		try
		{
			char st;
			const auto result = IntervalArithmetic::ISqr(m_value, st);
			return {{result}, st};
		}
		catch (...)
		{
			return  ErrorCode<Interval>::createError(DivisionByZeroCode);
		}
	}

	std::string Interval::to_string() const
	{
		string left, right;
		IntervalArithmetic::IEndsToStrings(m_value, left, right);
		return "[" + std::move(left) + ", " + std::move(right) + "]";
	}

	std::string Interval::getLeftAsString() const
	{
		string left, right;
		IntervalArithmetic::IEndsToStrings(m_value, left, right);
		return left;
	}

	std::string Interval::getRightAsString() const
	{
		string left, right;
		IntervalArithmetic::IEndsToStrings(m_value, left, right);
		return right;
	}

} // namespace ean