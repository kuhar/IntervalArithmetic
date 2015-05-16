/*
* Interval.h
*
*  Created on: 03-04-2015
*  Author: Jakub Kuderski
*/

#pragma once

#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <cfloat>
#include <type_traits>
#include <stdexcept>

#include "ErrorCode.h"

namespace ean
{
	template<typename T>
	class Number final
	{
		static_assert(std::is_floating_point<T>::value, "T must be a floating point type");
	public:
		Number() = default;
		Number(const std::string& value) 
			: m_value(std::stold(value))
		{}

		explicit Number(T value) 
			: m_value(value)
		{}

		T getWidth() const  { return T(0); }

		Number& operator+=(const Number& rhs) 
		{
			m_value += rhs.m_value;
			return *this;
		}

		Number operator+(const Number& rhs) const 
		{
			Number temp = *this;
			temp += rhs;
			return temp;
		}

		Number& operator-=(const Number& rhs) 
		{
			m_value -= rhs.m_value;
			return *this;
		}
		Number operator-(const Number& rhs) const 
		{
			Number temp = *this;
			temp -= rhs;
			return temp;
		}

		Number& operator*=(const Number& rhs) 
		{
			m_value *= rhs.m_value;
			return *this;
		}

		Number operator*(const Number& rhs) const 
		{
			Number temp = *this;
			temp *= rhs;
			return temp;
		}

		Number& operator/=(const Number& rhs)
		{
			if (rhs.m_value == T(0)) // abs(x-y) < eps?
			{
				throw std::runtime_error("Division by 0");
			}

			m_value /= rhs.m_value;
			return *this;
		}

		Number operator/(const Number& rhs) const
		{
			Number temp = *this;
			temp /= rhs;
			return temp;
		}

		bool operator==(const Number& rhs) const
		{
			return std::fabs(m_value - rhs.m_value) < T(LDBL_EPSILON);
		}

		Number& opposite() 
		{
			m_value = -m_value;
			return *this;
		}

		Number getOpposite() const 
		{
			return Number{-m_value};
		}

		Number& invert() 
		{
			m_value = 1 / m_value;
			return *this;
		}

		Number getInverse() const 
		{
			Number temp = *this;
			temp.invert();
			return temp;
		}

		ErrorCode<Number> sin() const 
		{
			return {Number{std::sin(m_value)}};
		}
		
		ErrorCode<Number> cos() const 
		{
			return {Number{std::cos(m_value)}};
		}
		
		ErrorCode<Number> exp() const 
		{
			return {Number{std::exp(m_value)}};
		}
		
		ErrorCode<Number> sqr() const 
		{
			return {Number{m_value * m_value}};
		}

		static Number sqrt2() { return Number{T(M_SQRT2)}; }
		static Number sqrt3() { return Number{T(std::sqrt(3.0l))}; }
		static Number pi() { return Number{T(M_PI)}; }

		std::string to_string() const 
		{
			std::ostringstream oss;
			oss << std::setprecision(15) << m_value;
			return oss.str();
		}

	private:
		T m_value = 0;
	};
	
	template<typename T>
	inline std::ostream& operator<<(std::ostream& os, const Number<T>& value)
	{
		return os << value.to_string();
	}

	using Extended = Number<long double>;

} // namespace ean