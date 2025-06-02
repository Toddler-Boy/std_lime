#pragma once

#include <stdexcept>
#include <array>

//-----------------------------------------------------------------------------

namespace lime
{

template <typename T, size_t N>
class stack_vector
{
public:
	explicit stack_vector ( size_t a_size = 0 )
		: vec_capacity ( N )
		, vec_size ( a_size )
	{
		if ( a_size > vec_capacity )
			throw std::invalid_argument ( "size more than capacity" );
	}

	T& operator[] ( size_t index )				{	return arr[ index ];	}
	const T& operator[] ( size_t index ) const	{	arr[ index ];			}

	typename std::array<T, N>::iterator begin ()				{	return arr.begin ();			}
	typename std::array<T, N>::iterator end ()					{	return arr.begin () + vec_size;	}
	typename std::array<T, N>::const_iterator begin () const	{	return arr.begin ();			}
	typename std::array<T, N>::const_iterator end () const		{	return arr.begin () + vec_size;	}

	size_t size () const		{	return vec_size;		}
	size_t capacity () const	{	return vec_capacity;	}
	bool isEmpty () const		{	return vec_size == 0;	}
	bool isNotEmpty () const	{	return ! isEmpty ();	}

	void push_back ( const T& value )
	{
		if ( vec_size < vec_capacity )
		{
			arr[ vec_size++ ] = value;
			return;
		}

		throw std::overflow_error ( "overflow" );
	}

	T pop_back ()
	{
		if ( vec_size )
			return arr[ --vec_size ];

		throw std::underflow_error ( "underflow" );
	}

private:
	size_t	vec_capacity;
	size_t	vec_size;
	std::array<T, N>	arr;
};

}
//-----------------------------------------------------------------------------
