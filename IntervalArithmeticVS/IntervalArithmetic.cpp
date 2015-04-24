/*
* IntervalArithmetic.cpp
*
*  Created on: 20-11-2012
*  Author: Tomasz Hoffmann and Andrzej Marciniak
*  Some utter crap fixed by Jakub Kuderski
*/

#include "IntervalArithmetic.h"

#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cfenv>
#include <cstdlib>
#include <cstdint>
#include <climits>
#include <cmath>

#include "mpfr.h"

using namespace std;
using namespace IntervalArithmetic;


// store the original rounding mode
const int originalRounding = fegetround();

long double IntervalArithmetic::IntWidth(const interval& x) 
{
	fesetround(FE_UPWARD);
	long double w = x.b - x.a;
	fesetround(FE_TONEAREST);
	return w;
}

IntervalArithmetic::interval IntervalArithmetic::IAdd(const interval& x, const interval& y) 
{
	interval r;
	fesetround(FE_DOWNWARD);
	r.a = x.a + y.a;
	fesetround(FE_UPWARD);
	r.b = x.b + y.b;
	fesetround(FE_TONEAREST);
	return r;
}

IntervalArithmetic::interval IntervalArithmetic::ISub(const interval& x, const interval& y) 
{
	interval r;
	fesetround(FE_DOWNWARD);
	r.a = x.a - y.b;
	fesetround(FE_UPWARD);
	r.b = x.b - y.a;
	fesetround(FE_TONEAREST);
	return r;
}

IntervalArithmetic::interval IntervalArithmetic::IMul(const interval& x, const interval& y) 
{
	interval r;
	long double x1y1, x1y2, x2y1;

	fesetround(FE_DOWNWARD);
	x1y1 = x.a * y.a;
	x1y2 = x.a * y.b;
	x2y1 = x.b * y.a;
	r.a = x.b * y.b;
	if (x2y1 < r.a)
		r.a = x2y1;
	if (x1y2 < r.a)
		r.a = x1y2;
	if (x1y1 < r.a)
		r.a = x1y1;

	fesetround(FE_UPWARD);
	x1y1 = x.a * y.a;
	x1y2 = x.a * y.b;
	x2y1 = x.b * y.a;

	r.b = x.b * y.b;
	if (x2y1 > r.b)
		r.b = x2y1;
	if (x1y2 > r.b)
		r.b = x1y2;
	if (x1y1 > r.b)
		r.b = x1y1;
	fesetround(FE_TONEAREST);
	return r;
}

interval IntervalArithmetic::IDiv(const interval& x, const interval& y)
{
	interval r;
	long double x1y1, x1y2, x2y1, t;

	if ((y.a <= 0) && (y.b >= 0))
	{
		throw runtime_error("Division by an interval containing 0");
	}
	else
	{
		fesetround(FE_DOWNWARD);
		x1y1 = x.a / y.a;
		x1y2 = x.a / y.b;
		x2y1 = x.b / y.a;
		r.a = x.b / y.b;
		t = r.a;
		if (x2y1 < t)
			r.a = x2y1;
		if (x1y2 < t)
			r.a = x1y2;
		if (x1y1 < t)
			r.a = x1y1;

		fesetround(FE_UPWARD);
		x1y1 = x.a / y.a;
		x1y2 = x.a / y.b;
		x2y1 = x.b / y.a;

		r.b = x.b / y.b;
		t = r.b;
		if (x2y1 > t)
			r.b = x2y1;
		if (x1y2 > t)
			r.b = x1y2;
		if (x1y1 > t)
			r.b = x1y1;
	}
	fesetround(FE_TONEAREST);
	return r;
}

long double IntervalArithmetic::DIntWidth(const interval& x) 
{
	long double w1, w2;

	fesetround(FE_UPWARD);
	w1 = x.b - x.a;
	if (w1 < 0)
		w1 = -w1;
	fesetround(FE_DOWNWARD);
	w2 = x.b - x.a;
	if (w2 < 0)
		w2 = -w2;
	fesetround(FE_TONEAREST);
	if (w1 > w2)
		return w1;
	else
		return w2;
}

IntervalArithmetic::interval IntervalArithmetic::Projection(const interval& x) 
{
	interval r;
	r = x;
	if (x.a > x.b)
	{
		r.a = x.b;
		r.b = x.a;
	}
	return r;
}

IntervalArithmetic::interval IntervalArithmetic::Opposite(const interval& x) 
{
	interval r;
	r.a = -x.a;
	r.b = -x.b;
	return r;
}

IntervalArithmetic::interval IntervalArithmetic::Inverse(const interval& x) 
{
	interval z1, z2;

	fesetround(FE_DOWNWARD);
	z1.a = 1 / x.a;
	z2.b = 1 / x.b;
	fesetround(FE_UPWARD);
	z1.b = 1 / x.b;
	z2.a = 1 / x.a;
	fesetround(FE_TONEAREST);
	if (DIntWidth(z1) >= DIntWidth(z2))
		return z1;
	else
		return z2;
}

IntervalArithmetic::interval IntervalArithmetic::DIAdd(const interval& x, const interval& y) 
{
	interval z1, z2;
	if ((x.a <= x.b) && (y.a <= y.b))
	{
		return IAdd(x, y);
	}
	else
	{
		fesetround(FE_DOWNWARD);
		z1.a = x.a + y.a;
		z2.b = x.b + y.b;
		fesetround(FE_UPWARD);
		z1.b = x.b + y.b;
		z2.a = x.a + y.a;
		fesetround(FE_TONEAREST);
		if (DIntWidth(z1) >= DIntWidth(z2))
			return z1;
		else
			return z2;
	}
}

IntervalArithmetic::interval IntervalArithmetic::DISub(const interval& x, const interval& y) 
{
	interval z1, z2;
	if ((x.a <= x.b) && (y.a <= y.b))
	{
		return ISub(x, y);
	}
	else
	{
		fesetround(FE_DOWNWARD);
		z1.a = x.a - y.b;
		z2.b = x.b - y.a;
		fesetround(FE_UPWARD);
		z1.b = x.b - y.a;
		z2.a = x.a - y.b;
		fesetround(FE_TONEAREST);
		if (DIntWidth(z1) >= DIntWidth(z2))
			return z1;
		else
			return z2;
	}
}

IntervalArithmetic::interval IntervalArithmetic::DIMul(const interval& x, const interval& y) 
{
	interval z1, z2, r;
	long double z;
	bool xn, xp, yn, yp, zero;

	if ((x.a <= x.b) && (y.a <= y.b))
		r = IMul(x, y);
	else
	{
		xn = (x.a < 0) && (x.b < 0);
		xp = (x.a > 0) && (x.b > 0);
		yn = (y.a < 0) && (y.b < 0);
		yp = (y.a > 0) && (y.b > 0);
		zero = false;
		// A, B in H-T
		if ((xn || xp) && (yn || yp))
			if (xp && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a * y.a;
				z2.b = x.b * y.b;
				fesetround(FE_UPWARD);
				z1.b = x.b * y.b;
				z2.a = x.a * y.a;
			}
			else if (xp && yn)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b * y.a;
				z2.b = x.a * y.b;
				fesetround(FE_UPWARD);
				z1.b = x.a * y.b;
				z2.a = x.b * y.a;
			}
			else if (xn && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a * y.b;
				z2.b = x.b * y.a;
				fesetround(FE_UPWARD);
				z1.b = x.b * y.a;
				z2.a = x.a * y.b;
			}
			else
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b * y.b;
				z2.b = x.a * y.a;
				fesetround(FE_UPWARD);
				z1.b = x.a * y.a;
				z2.a = x.b * y.b;
			}
			// A in H-T, B in T
		else if ((xn || xp) && (((y.a <= 0) && (y.b >= 0)) || ((y.a >= 0) && (y.b <= 0))))
			if (xp && (y.a <= y.b))
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b * y.a;
				z2.b = x.b * y.b;
				fesetround(FE_UPWARD);
				z1.b = x.b * y.b;
				z2.a = x.b * y.a;
			}
			else if (xp && (y.a > y.b))
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a * y.a;
				z2.b = x.a * y.b;
				fesetround(FE_UPWARD);
				z1.b = x.a * y.b;
				z2.a = x.a * y.a;
			}
			else if (xn && (y.a <= y.b))
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a * y.b;
				z2.b = x.a * y.a;
				fesetround(FE_UPWARD);
				z1.b = x.a * y.a;
				z2.a = x.a * y.b;
			}
			else
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b * y.b;
				z2.b = x.b * y.a;
				fesetround(FE_UPWARD);
				z1.b = x.b * y.a;
				z2.a = x.b * y.b;
			}
			// A in T, B in H-T
		else if ((((x.a <= 0) && (x.b >= 0)) || ((x.a >= 0) && (x.b <= 0))) && (yn || yp))
			if ((x.a <= x.b) && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a * y.b;
				z2.b = x.b * y.b;
				fesetround(FE_UPWARD);
				z1.b = x.b * y.b;
				z2.a = x.a * y.b;
			}
			else if ((x.a <= 0) && yn)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b * y.a;
				z2.b = x.a * y.a;
				fesetround(FE_UPWARD);
				z1.b = x.a * y.a;
				z2.a = x.b * y.a;
			}
			else if ((x.a > x.b) && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a * y.a;
				z2.b = x.b * y.a;
				fesetround(FE_UPWARD);
				z1.b = x.b * y.a;
				z2.a = x.a * y.a;
			}
			else
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b * y.b;
				z2.b = x.a * y.b;
				fesetround(FE_UPWARD);
				z1.b = x.a * y.b;
				z2.a = x.b * y.b;
			}
			// A, B in Z-
		else if ((x.a >= 0) && (x.b <= 0) && (y.a >= 0) && (y.b <= 0))
		{
			fesetround(FE_DOWNWARD);
			z1.a = x.a * y.a;
			z = x.b * y.b;
			if (z1.a < z)
				z1.a = z;
			z2.b = x.a * y.b;
			z = x.b * y.a;
			if (z < z2.b)
				z2.b = z;
			fesetround(FE_UPWARD);
			z1.b = x.a * y.b;
			z = x.b * y.a;
			if (z < z1.b)
				z1.b = z;
			z2.a = x.a * y.a;
			z = x.b * y.b;
			if (z2.a < z)
				z2.a = z;
		}
		// A in Z and B in Z- or A in Z- and B in Z
		else
			zero = true;
		if (zero)
		{
			r.a = 0;
			r.b = 0;
		}
		else if (DIntWidth(z1) >= DIntWidth(z2))
			r = z1;
		else
			r = z2;
	}

	fesetround(FE_TONEAREST);
	return r;
}

interval IntervalArithmetic::DIDiv(const interval& x, const interval& y)
{

	interval z1, z2, r;
	bool xn, xp, yn, yp, zero;

	if ((x.a <= x.b) && (y.a <= y.b))
		r = IDiv(x, y);
	else
	{
		xn = (x.a < 0) && (x.b < 0);
		xp = (x.a > 0) && (x.b > 0);
		yn = (y.a < 0) && (y.b < 0);
		yp = (y.a > 0) && (y.b > 0);
		zero = false;
		// A, B in H-T
		if ((xn || xp) && (yn || yp))
			if (xp && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a / y.b;
				z2.b = x.b / y.a;
				fesetround(FE_UPWARD);
				z1.b = x.b / y.a;
				z2.a = x.a / y.b;
			}
			else if (xp && yn)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b / y.b;
				z2.b = x.a / y.a;
				fesetround(FE_UPWARD);
				z1.b = x.a / y.a;
				z2.a = x.b / y.b;
			}
			else if (xn && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a / y.a;
				z2.b = x.b / y.b;
				fesetround(FE_UPWARD);
				z1.b = x.b / y.b;
				z2.a = x.a / y.a;
			}
			else
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b / y.a;
				z2.b = x.a / y.b;
				fesetround(FE_UPWARD);
				z1.b = x.a / y.b;
				z2.a = x.b / y.a;
			}
			// A in T, B in H-T
		else if (((x.a <= 0) && (x.b >= 0)) || (((x.a >= 0) && (x.b <= 0)) && (yn || yp)))
			if ((x.a <= x.b) && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a / y.a;
				z2.b = x.b / y.a;
				fesetround(FE_UPWARD);
				z1.b = x.b / y.a;
				z2.a = x.a / y.a;
			}
			else if ((x.a <= x.b) && yn)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b / y.b;
				z2.b = x.a / y.b;
				fesetround(FE_UPWARD);
				z1.b = x.a / y.b;
				z2.a = x.b / y.b;
			}
			else if ((x.a > x.b) && yp)
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.a / y.b;
				z2.b = x.b / y.b;
				fesetround(FE_UPWARD);
				z1.b = x.b / y.b;
				z2.a = x.a / y.b;
			}
			else
			{
				fesetround(FE_DOWNWARD);
				z1.a = x.b / y.a;
				z2.b = x.a / y.a;
				fesetround(FE_UPWARD);
				z1.b = x.a / y.a;
				z2.a = x.b / y.a;
			}
		else
			zero = true;
		if (zero)
			throw runtime_error("Division by an interval containing 0.");
		else if (DIntWidth(z1) >= DIntWidth(z2))
			r = z1;
		else
			r = z2;
		fesetround(FE_TONEAREST);
	}
	return r;
}

IntervalArithmetic::interval IntervalArithmetic::IntRead(const std::string& sa) 
{
	interval r;
	mpfr_t rop;
	mpfr_init2(rop, 80);
	mpfr_set_str(rop, sa.c_str(), 10, MPFR_RNDD);

	long double le = mpfr_get_ld(rop, MPFR_RNDD);
	mpfr_set_str(rop, sa.c_str(), 10, MPFR_RNDU);
	long double re = mpfr_get_ld(rop, MPFR_RNDU);
	fesetround(FE_TONEAREST);

	r.a = le;
	r.b = re;
	return r;
}

long double IntervalArithmetic::LeftRead(const std::string& sa) 
{
	interval int_number;
	int_number = IntRead(sa);
	return int_number.a;
}

long double IntervalArithmetic::RightRead(const std::string& sa) 
{
	interval int_number;
	int_number = IntRead(sa);
	return int_number.b;
}

IntervalArithmetic::interval IntervalArithmetic::ISin(const interval& x, char& st)
{
	bool is_even, finished = false;
	int k;
	interval d, s, w, w1, x2;
	if (x.a > x.b)
		st = 1;
	else
	{
		s = x;
		w = x;
		x2 = IMul(x, x);
		k = 1;
		is_even = true;
		finished = false;
		st = 0;

		do
		{
			d.a = (k + 1) * (k + 2);
			d.b = d.a;
			s = IMul(s, IDiv(x2, d));
			if (is_even)
				w1 = ISub(w, s);
			else
				w1 = IAdd(w, s);
			if ((w.a != 0) && (w.b != 0))
			{
				if ((std::fabs(w.a - w1.a) / std::fabs(w.a) < 1e-18) && (std::fabs(w.b - w1.b) / std::fabs(w.b) < 1e-18))
					finished = true;
			}

			else if ((w.a == 0) && (w.b != 0))
			{
				if ((std::fabs(w.a - w1.a) < 1e-18) && (std::fabs(w.b - w1.b) / std::fabs(w.b) < 1e-18))
					finished = true;
			}

			else if (w.a != 0)
			{
				if ((std::fabs(w.a - w1.a) / std::fabs(w.a) < 1e-18) & (std::fabs(w.b - w1.b) < 1e-18))
					finished = true;
				else if ((std::fabs(w.a - w1.a) < 1e-18) & (std::fabs(w.b - w1.b) < 1e-18))
					finished = true;
			}

			if (finished)
			{
				if (w1.b > 1)
				{
					w1.b = 1;
					if (w1.a > 1)
						w1.a = 1;
				}
				if (w1.a < -1)
				{
					w1.a = -1;
					if (w1.b < -1)
						w1.b = -1;
				}
				return w1;
			}
			else
			{
				w = w1;
				k = k + 2;
				is_even = !is_even;
			}
		} while (!(finished || (k > INT_MAX / 2)));
	}
	if (!finished)
		st = 2;

	interval r;
	r.a = 0;
	r.b = 0;
	return r;
}
IntervalArithmetic::interval IntervalArithmetic::ICos(const interval& x, char& st)
{
	bool is_even, finished = false;
	int k;
	interval d, c, w, w1, x2;
	if (x.a > x.b)
		st = 1;
	else
	{
		c.a = 1;
		c.b = 1;
		w = c;
		x2 = IMul(x, x);
		k = 1;
		is_even = true;
		finished = false;
		st = 0;

		do
		{
			d.a = k * (k + 1);
			d.b = d.a;
			c = IMul(c, IDiv(x2, d));
			if (is_even)
				w1 = ISub(w, c);
			else
				w1 = IAdd(w, c);

			if ((w.a != 0) && (w.b != 0))
			{
				if ((std::fabs(w.a - w1.a) / std::fabs(w.a) < 1e-18) && (std::fabs(w.b - w1.b) / std::fabs(w.b) < 1e-18))
					finished = true;
			}

			else if ((w.a == 0) && (w.b != 0))
			{
				if ((std::fabs(w.a - w1.a) < 1e-18) && (std::fabs(w.b - w1.b) / std::fabs(w.b) < 1e-18))
					finished = true;
			}

			else if (w.a != 0)
			{
				if ((std::fabs(w.a - w1.a) / std::fabs(w.a) < 1e-18) & (std::fabs(w.b - w1.b) < 1e-18))
					finished = true;
				else if ((std::fabs(w.a - w1.a) < 1e-18) & (std::fabs(w.b - w1.b) < 1e-18))
					finished = true;
			}

			if (finished)
			{
				if (w1.b > 1)
				{
					w1.b = 1;
					if (w1.a > 1)
						w1.a = 1;
				}
				if (w1.a < -1)
				{
					w1.a = -1;
					if (w1.b < -1)
						w1.b = -1;
				}
				return w1;
			}
			else
			{
				w = w1;
				k = k + 2;
				is_even = !is_even;
			}
		} while (!(finished || (k > INT_MAX / 2)));
	}
	if (!finished)
		st = 2;

	interval r;
	r.a = 0;
	r.b = 0;
	return r;
}
IntervalArithmetic::interval IntervalArithmetic::IExp(const interval& x, char& st)
{
	bool finished;
	int k;
	interval d, e, w, w1;
	if (x.a > x.b)
		st = 1;
	else
	{
		e.a = 1;
		e.b = 1;
		w = e;
		k = 1;
		finished = false;
		st = 0;
		do
		{
			d.a = k;
			d.b = k;
			e = IMul(e, IDiv(x, d));
			w1 = IAdd(w, e);
			if ((std::fabs(w.a - w1.a) / std::fabs(w.a) < 1e-18) && (std::fabs(w.b - w1.b) / std::fabs(w.b) < 1e-18))
			{
				finished = true;
				return w1;
			}
			else
			{
				w = w1;
				k = k + 1;
			}
		} while (!(finished || (k > INT_MAX / 2)));
		if (!finished)
			st = 2;
	}
	interval r;
	r.a = 0;
	r.b = 0;
	return r;
}

IntervalArithmetic::interval IntervalArithmetic::ISqr(const interval& x, char& st) 
{
	long double minx, maxx;
	interval r;
	r.a = 0;
	r.b = 0;
	if (x.a > x.b)
		st = 1;
	else
	{
		st = 0;
		if ((x.a <= 0) && (x.b >= 0))
			minx = 0;
		else if (x.a > 0)
			minx = x.a;
		else
			minx = x.b;
		if (std::fabs(x.a) > std::fabs(x.b))
			maxx = std::fabs(x.a);
		else
			maxx = std::fabs(x.b);
		fesetround(FE_DOWNWARD);
		r.a = minx * minx;
		fesetround(FE_UPWARD);
		r.b = maxx * maxx;
		fesetround(FE_TONEAREST);
	}
	return r;
}

interval IntervalArithmetic::ISqr2()
{
	string i2;
	interval r;
	i2 = "1.414213562373095048";
	r.a = LeftRead(i2);
	i2 = "1.414213562373095049";
	r.b = RightRead(i2);
	return r;
}

interval IntervalArithmetic::ISqr3()
{
	string i2;
	interval r;
	i2 = "1.732050807568877293";
	r.a = LeftRead(i2);
	i2 = "1.732050807568877294";
	r.b = RightRead(i2);
	return r;
}
interval IntervalArithmetic::IPi()
{
	string i2;
	interval r;
	i2 = "3.141592653589793238";
	r.a = LeftRead(i2);
	i2 = "3.141592653589793239";
	r.b = RightRead(i2);
	return r;
}

void IntervalArithmetic::IEndsToStrings(const interval& i, string& left, string& right)
{
	ostringstream oss;
	oss << std::scientific << i.a;
	left = oss.str();

	auto complementExponent = [](string& x)
	{
		auto pos = x.find('e');
		if (pos == string::npos) return;
		x[pos] = 'E';
		if (x[pos + 1] == '+' || x[pos + 1] == '-')
		{
			auto width = x.size() - pos - 2;
			if (width < 4)
			{
				x.insert(pos + 2, string(4 - width, '0'));
			}
		}
	};
	complementExponent(left);

	ostringstream oss2;
	oss2 << std::scientific << i.b;
	right = oss2.str();

	complementExponent(right);
}