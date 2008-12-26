// stringex.h: interface for the stringex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGEX_H__204B5708_D3BE_43AF_BF88_D0F0E88ECC21__INCLUDED_)
#define AFX_STRINGEX_H__204B5708_D3BE_43AF_BF88_D0F0E88ECC21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <algorithm>
#include <vector>

using namespace std;

namespace stringex
{
	void split(const string& str, vector<string>& tokens,
			   const string& delimiters = ",")
	{
		// Skip delimiters at beginning.
		string::size_type lastPos = str.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		string::size_type pos = str.find_first_of(delimiters, lastPos);

		while (string::npos != pos || string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}
}

#endif // !defined(AFX_STRINGEX_H__204B5708_D3BE_43AF_BF88_D0F0E88ECC21__INCLUDED_)
