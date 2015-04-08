#include <iostream>
#include <string>

#include "Number.h"
#include "Interval.h"

using namespace std;

int main()
{
	auto wtw = ean::Interval("1.223");
	auto scnd = wtw;
	cout << (wtw.sin().get() * ean::Interval::pi()).getInverse() << endl;

	ean::Extended ex(5.0l);
	auto res = ex.cos();
	cout << res.isError() << endl;
	//res.setError(2);
	//cout << res.isError() << endl;
	cout << (res.get() * ean::Extended{2.0l}).getInverse();

	wtw = ean::Interval(-1, 1);

	return 0;
}