#pragma once

#include <cctype>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>

//-----------------------------------------------------------------------------

namespace lime::str
{

[[ nodiscard ]] inline bool equal ( const std::string& s1, const std::string& s2 )
{
	return std::equal ( s1.begin (), s1.end (), s2.begin (), s2.end (), [] ( char c1, char c2 ) { return std::tolower ( c1 ) == std::tolower ( c2 ); } );
}

[[ nodiscard ]] inline std::string toLower ( std::string input )
{
	std::transform ( input.begin (), input.end (), input.begin (), [] ( unsigned char c ) { return std::tolower ( c ); } );

	return input;
}

[[ nodiscard ]] inline std::string toUpper ( std::string input )
{
	std::transform ( input.begin (), input.end (), input.begin (), [] ( unsigned char c ) { return std::toupper ( c ); } );

	return input;
}

[[ nodiscard ]] inline std::string ltrim ( std::string input )
{
	input.erase ( input.begin (), std::find_if ( input.begin (), input.end (), [] ( unsigned char ch ) { return ! std::isspace ( ch ); } ) );

	return input;
}

[[ nodiscard ]] inline std::string rtrim ( std::string input )
{
	input.erase ( std::find_if ( input.rbegin (), input.rend (), [] ( unsigned char ch ) { return ! std::isspace ( ch ); } ).base (), input.end () );

	return input;
}

[[ nodiscard ]] inline std::string trim ( std::string input )
{
	// ltrim
	input.erase ( input.begin (), std::find_if ( input.begin (), input.end (), [] ( unsigned char ch ) {	return ! std::isspace ( ch );	} ) );
	// rtrim
	input.erase ( std::find_if ( input.rbegin (), input.rend (), [] ( unsigned char ch ) { return ! std::isspace ( ch );	} ).base (), input.end () );

	return input;
}

[[ nodiscard ]] std::vector<std::string> split ( const std::string& input, const char delimeter )
{
	std::vector<std::string>	tokens;

	std::stringstream	ss ( input );
	std::string			part;
	while ( std::getline ( ss, part, delimeter ) )
	{
		part = trim ( part );
		if ( ! part.empty () )
			tokens.emplace_back ( part );
	}

	return tokens;
}
//-----------------------------------------------------------------------------
} // namespace lime::str
//-----------------------------------------------------------------------------
