#pragma once

//-----------------------------------------------------------------------------

namespace lime
{

//-----------------------------------------------------------------------------

template<typename InX, typename InMin, typename InMax, typename OutMin = float, typename OutMax = float, typename Out = OutMin>
Out remap ( const InX x, const InMin inMin, const InMax inMax, const OutMin outMin = 0.0f, const OutMax outMax = 1.0f )
{
	const auto	xf = static_cast<Out>( x - inMin );
	const auto	maxF = static_cast<Out>( inMax - inMin );

	return static_cast<Out>( outMin + ( xf / maxF ) * ( outMax - outMin ) );
}
//-----------------------------------------------------------------------------

template<typename InX, typename InMin, typename InMax, typename OutMin = float, typename OutMax = float, typename Out = OutMin>
Out remapClamp ( const InX x, const InMin inMin, const InMax inMax, const OutMin outMin = 0.0f, const OutMax outMax = 1.0f )
{
	const auto	xf = static_cast<Out>( x - inMin );
	const auto	maxF = static_cast<Out>( inMax - inMin );
	const auto	result = static_cast<Out>( outMin ) + ( xf / maxF ) * ( static_cast<Out>( outMax ) - static_cast<Out>( outMin ) );

	const auto	lo = outMin < outMax ? static_cast<Out> ( outMin ) : static_cast<Out> ( outMax );
	const auto	hi = outMin < outMax ? static_cast<Out> ( outMax ) : static_cast<Out> ( outMin );

	return result < lo ? lo : ( result > hi ? hi : result );
}
//-----------------------------------------------------------------------------

} // namespace lime
//-----------------------------------------------------------------------------
