#pragma once

#include <cstring>
#include <string>
#include <string_view>
#include <utility>
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

// Functors for case-insensitive unordered containers,
// e.g. std::unordered_set<std::string, HashIgnoreCase, EqualIgnoreCase>
struct HashIgnoreCase
{
	[[ nodiscard ]] std::size_t operator() ( std::string_view key ) const;
};

struct EqualIgnoreCase
{
	[[ nodiscard ]] bool operator() ( std::string_view lhs, std::string_view rhs ) const noexcept
	{
		return equalIgnoreCase ( lhs, rhs );
	}
};
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

inline std::size_t HashIgnoreCase::operator() ( std::string_view key ) const
{
	return std::hash<std::string> {} ( toLower ( std::string ( key ) ) );
}
//-----------------------------------------------------------------------------

// Replaces the first occurrence of "from" in place; returns whether a replacement happened
inline bool replaceFirst ( std::string& str, std::string_view from, std::string_view to )
{
	const auto	pos = str.find ( from );
	if ( pos == std::string::npos )
		return false;

	str.replace ( pos, from.length (), to );
	return true;
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

// Natural sort order: runs of digits compare by numeric value ("track2" < "track10"),
// leading zeros compare fractionally ("01" < "1"), ties fall back to strcmp
[[ nodiscard ]] inline int naturalCompare ( const char* const a, const char* const b ) noexcept
{
	auto isspace = [] ( const unsigned char c ) { return c && c <= 32; };
	auto isdigit = [] ( const unsigned char c ) { return c >= '0' && c <= '9'; };
	auto compare_right = [ &isdigit ] ( const char* a, const char* b ) -> int
	{
		auto	bias = 0;

		// The longest run of digits wins. That aside, the greatest
		// value wins, but we can't know that it will until we've scanned
		// both numbers to know that they have the same magnitude, so we
		// remember it in BIAS.
		for ( ;; a++, b++ )
		{
			if ( ! isdigit ( *a ) && ! isdigit ( *b ) )	return bias;
			if ( ! isdigit ( *a ) )						return -1;
			if ( ! isdigit ( *b ) )						return +1;

			if ( *a < *b )
			{
				if ( ! bias )
					bias = -1;
			}
			else if ( *a > *b )
			{
				if ( ! bias )
					bias = +1;
			}
			else if ( ! *a && ! *b )
				return bias;
		}

		std::unreachable ();
	};
	auto compare_left = [ &isdigit ] ( const char* a, const char* b ) -> int
	{
		// Compare two left-aligned numbers: the first to have a different value wins
		for ( ;; a++, b++ )
		{
			if ( ! isdigit ( *a ) && ! isdigit ( *b ) )		return 0;
			if ( ! isdigit ( *a ) )							return -1;
			if ( ! isdigit ( *b ) )							return +1;
			if ( *a < *b )									return -1;
			if ( *a > *b )									return +1;
		}

		std::unreachable ();
	};

	auto	ai = 0;
	auto	bi = 0;

	while ( 1 )
	{
		auto	ca = a[ ai ];
		auto	cb = b[ bi ];

		// skip over leading spaces or zeros
		while ( isspace ( ca ) )	ca = a[ ++ai ];
		while ( isspace ( cb ) )	cb = b[ ++bi ];

		// process run of digits
		if ( isdigit ( ca ) && isdigit ( cb ) )
		{
			const auto	fractional = ( ca == '0' || cb == '0' );

			if ( fractional )
			{
				if ( auto result = compare_left ( a + ai, b + bi ); result != 0 )
					return result;
			}
			else
			{
				if ( auto result = compare_right ( a + ai, b + bi ); result != 0 )
					return result;
			}
		}

		if ( ! ca && ! cb )		return std::strcmp ( a, b );
		if ( ca < cb )			return -1;
		if ( ca > cb )			return +1;

		++ai;
		++bi;
	}

	std::unreachable ();
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
