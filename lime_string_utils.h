#pragma once

#include <string>
#include <string_view>
#include <vector>

//-----------------------------------------------------------------------------

namespace lime::str
{

//-----------------------------------------------------------------------------

[[nodiscard]] inline bool equalIgnoreCase ( std::string_view s1, std::string_view s2 ) noexcept
{
	if ( s1.size () != s2.size () )
		return false;

	auto toLowerChar = [] ( unsigned char c ) -> unsigned char {
		return ( c >= 'A' && c <= 'Z' ) ? c + ( 'a' - 'A' ) : c;
	};

	for ( size_t i = 0; i < s1.size (); ++i )
		if ( toLowerChar ( s1[ i ] ) != toLowerChar ( s2[ i ] ) )
			return false;

	return true;
}
//-----------------------------------------------------------------------------

[[nodiscard]] inline std::string toLower ( std::string input ) noexcept
{
	for ( char& c : input )
		if ( c >= 'A' && c <= 'Z' ) c += ( 'a' - 'A' );
	return input;
}
//-----------------------------------------------------------------------------

[[nodiscard]] inline std::string toUpper ( std::string input ) noexcept
{
	for ( char& c : input )
		if ( c >= 'a' && c <= 'z' ) c -= ( 'a' - 'A' );

	return input;
}
//-----------------------------------------------------------------------------

[[nodiscard]] inline std::string_view ltrim ( std::string_view input ) noexcept
{
	const auto	pos = input.find_first_not_of ( " \t\n\r\f\v" );

	return pos == std::string_view::npos ? std::string_view {} : input.substr ( pos );
}
//-----------------------------------------------------------------------------

[[nodiscard]] inline std::string_view rtrim ( std::string_view input ) noexcept
{
	const auto	pos = input.find_last_not_of ( " \t\n\r\f\v" );

	return pos == std::string_view::npos ? std::string_view {} : input.substr ( 0, pos + 1 );
}
//-----------------------------------------------------------------------------

[[nodiscard]] inline std::string_view trim ( std::string_view input ) noexcept
{
	const auto start = input.find_first_not_of ( " \t\n\r\f\v" );

	if ( start == std::string_view::npos )
		return {};

	const auto end = input.find_last_not_of ( " \t\n\r\f\v" );
	return input.substr ( start, end - start + 1 );
}
//-----------------------------------------------------------------------------

[[ nodiscard ]] inline std::vector<std::string> split ( std::string_view input, const char delimiter )
{
	std::vector<std::string>	tokens;

	size_t	start = 0;
	while ( start <= input.size () )
	{
		const auto	end = input.find ( delimiter, start );
		const auto	len = ( end == std::string_view::npos ) ? input.size () - start : end - start;

		const auto	trimmed = trim ( input.substr ( start, len ) );
		if ( ! trimmed.empty () )
			tokens.emplace_back ( trimmed );

		if ( end == std::string_view::npos )
			break;

		start = end + 1;
	}

	return tokens;
}
//-----------------------------------------------------------------------------

} // namespace lime::str
//-----------------------------------------------------------------------------
