#ifndef __TRADE_CONVERTER_HPP__
#define __TRADE_CONVERTER_HPP__

#include <boost/lexical_cast.hpp>

namespace trade
{

template<class T, bool U>
struct ConverterImpl
{
	static inline T convert(const std::string& s) { }
};

template<class T>
struct ConverterImpl<T, true>
{
	static inline T convert(const std::string& s)
   	{
		if (s.size() && s[0] == '-')
		{
			throw boost::bad_lexical_cast();
		}

		return ConverterImpl<T, false>::convert(s);
   	}
};

template<class T>
struct ConverterImpl<T, false>
{
	static inline T convert(const std::string& s)
   	{
		return boost::lexical_cast<T>(s);
   	}
};
	


/*
 * Converter<T>::convert(const std::string& s) throw exception if T must be always possible
 * but s start from '-'
 */
template<class T>
struct Converter
{
	static inline T convert(const std::string& s)
	{
		constexpr bool u = (std::numeric_limits<T>::lowest() >= 0);
		return ConverterImpl<T, u>::convert(s);
	}
};

} // namespace david

#endif
