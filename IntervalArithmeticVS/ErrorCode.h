/*
* ErrorCode.h
*
*  Created on: 03-04-2015
*  Author: Jakub Kuderski
*/

#pragma once

#include <stdexcept>
#include <string>
#include <cassert>

namespace ean
{
	template<typename T>
	class ErrorCode final
	{
	public:
		ErrorCode(T result, char error)
			: m_result(std::move(result))
			, m_erorrCode(error)
		{}
		ErrorCode(T result)
			: ErrorCode(std::move(result), 0)
		{}

		static ErrorCode createError(char errorCode)
		{
			ErrorCode error;
			error.m_erorrCode = errorCode;
			return error;
		}

		ErrorCode(const ErrorCode&) = default;
#if !defined(_MSC_VER) || (_MSC_VER >= 1900) // >= VC14
		ErrorCode(ErrorCode&&) = default;
#endif
		const T& get() const
		{
			if (isError())
			{
				throw std::runtime_error("Computation error: " + std::to_string(m_erorrCode));
			}
			return m_result;
		}

		T& get()
		{
			if (isError())
			{
				throw std::runtime_error("Computation error: " + std::to_string(m_erorrCode));
			}
			return m_result;
		}

		void setError(char errorCode)  { m_erorrCode = errorCode; }
		char getError() const { return m_erorrCode; }
		bool isError() const { return m_erorrCode != 0; }

		operator bool() const { return !isError(); }

	private:
		ErrorCode() {}

		T m_result;
		char m_erorrCode;
	};

} // namespace ean