#pragma once

import std;

#include "stream_required.h"
#include "unwrap.h"

template<typename T, char sep = ','>
struct delimitered_value
{
	T value;

	operator unwrap_result_t<T>()&&     { return unwrap(std::move(value)); }
	operator unwrap_result_t<T>()const& { return unwrap(value); }
	operator unwrap_result_t<T>()&      { return unwrap(value); }
};

template<typename T, char sep>
struct unwrap_result<delimitered_value<T, sep>>
{
	using type = unwrap_result_t<T>;
};

template<typename T, char sep>
inline unwrap_result_t<T> unwrap(delimitered_value<T, sep>&& t)
{
	return unwrap(std::move(t.value));
}
template<typename T, char sep>
inline unwrap_result_t<T>& unwrap(delimitered_value<T, sep>& t)
{
	return unwrap(t.value);
}
template<typename T, char sep>
inline unwrap_result_t<T>const& unwrap(delimitered_value<T, sep>const& t)
{
	return unwrap(t.value);
}

template<typename T, char sep>
std::istream& operator>>(std::istream& in, delimitered_value<T, sep>& t)
{
	if (!(in >> t.value)) //if we failed to read the int
		return in;          //return failure state
	if (in.peek() == sep) //if next character is a comma
		in.ignore();        //extract it from the stream and we're done
	else                  //if the next character is anything else
		in.clear();         //clear the EOF state, read was successful
	return in;
}

template<char sep>
std::istream& operator>>(std::istream& in, delimitered_value<std::string, sep>& t)
{
	std::getline(in, t.value, sep);
	return in;
}

template<typename T>
using comma_separated = delimitered_value<T, ','>;



template<typename T, char sep = ','>
struct delimitered_container;

template<typename T>
struct remove_delimitered
{
	using type = T;
};

template<typename T, char sep>
struct remove_delimitered<delimitered_container<T, sep>>
{
	using type = T;
};

template<typename T, typename U>
struct remove_delimitered<std::pair<T, U>>
{
	using type = std::pair<typename remove_delimitered<T>::type, typename remove_delimitered<U>::type>;
};

template<typename T, char sep>
struct delimitered_container
{
	T value;

	operator unwrap_result_t<T>()&&     { return unwrap(std::move(value)); }
	operator unwrap_result_t<T>()const& { return unwrap(value); }
	operator unwrap_result_t<T>()&      { return unwrap(value); }
	operator remove_delimitered<T>::type() const& requires !std::same_as<unwrap_result_t<T>, remove_delimitered<T>::type> { return value; }
	operator remove_delimitered<T>::type() &&     requires !std::same_as<unwrap_result_t<T>, remove_delimitered<T>::type> { return std::move(value); }
};

template<typename T, char sep>
struct unwrap_result<delimitered_container<T, sep>>
{
	using type = T;
};

template<typename T, char sep>
inline unwrap_result_t<T>&& unwrap(delimitered_container<T, sep>&& t)
{
	return unwrap(std::move(t.value));
}
template<typename T>
inline unwrap_result_t<T>& unwrap(delimitered_container<T>& t)
{
	return unwrap(t.value);
}
template<typename T>
inline unwrap_result_t<T>const& unwrap(delimitered_container<T>const& t)
{
	return unwrap(t.value);
}

template<typename T, typename U, char sep>
std::istream& operator>>(std::istream& in, delimitered_container<std::pair<T,U>, sep>& t)
{
	in >> t.value.first >> required{ sep } >> t.value.second;

	return in;
}

template<typename T, char sep, typename Alloc = std::allocator<T>>
std::istream& operator>>(std::istream& is, delimitered_container<std::vector<T, Alloc>, sep>& t)
{
	t.value.clear();
	while(1)
	{
		auto pos = is.tellg();
		std::stringbuf next;
		if(!is.get(next, sep)) //if we failed to read the value
		{
			return is;          //return failure state (stream may not be in a consistent state, can't rewind)
		}
		std::istream is2(&next);
		T value;
		if (!(is2 >> value))   //if we failed to read the value
		{
			is.seekg(pos);
			is.setf(std::ios_base::failbit);
			return is;          //return failure state
		}
		t.value.push_back(std::move(value));
		is2 >> std::ws;
		if (is2.eof())         //if next character matches the separator
		{
			if (is.eof())
				break;
			is.ignore();        //extract it from the stream and loop round
			continue;
		}
		else                   //if the next character is anything else
		{
			is.clear();         //clear the EOF state, read was successful
			is.seekg(pos + is2.tellg());
			break;
		}
	}
	return is;
}

template<typename T, size_t count, char sep>
std::istream& operator>>(std::istream& is, delimitered_container<std::array<T, count>, sep>& t)
{
	for (size_t i = 0; i < count; ++i)
	{
		auto pos = is.tellg();
		std::stringbuf next;
		if (!is.get(next, sep)) //if we failed to read the value
		{
			return is;           //return failure state (stream may not be in a consistent state, can't rewind)
		}
		std::istream is2(&next);
		T value;
		if (!(is2 >> value))    //if we failed to read the value
		{
			is.seekg(pos);
			is.setf(std::ios_base::failbit);
			return is;           //return failure state
		}
		t.value[i] = std::move(value);
		is2 >> std::ws;
		if (is2.eof())          //if next character matches the separator
		{
			if (!is.eof())
				is.ignore();     //extract it from the stream and loop round
			continue;
		}
		else                    //if the next character is anything else
		{
			is.clear();          //clear the EOF state, read was successful
			is.seekg(pos + is2.tellg());
			break;
		}
	}
	return is;
}

template<typename T, typename U, char sep=','>
using delimitered_pair = delimitered_container<std::pair<T, U>, sep>;

template<typename T, char sep = ',', typename Alloc=std::allocator<T>>
using delimitered_vector = delimitered_container<std::vector<T, Alloc>, sep>;

template<typename T, size_t count, char sep = ','>
using delimitered_array = delimitered_container<std::array<T, count>, sep>;

//------------------------------------------------------------

template<typename T, char start = '"', char end = start>
struct quoted_value
{
	T value;

	operator unwrap_result_t<T>()&&     { return unwrap(std::move(value)); }
	operator unwrap_result_t<T>()const& { return unwrap(value); }
	operator unwrap_result_t<T>()&      { return unwrap(value); }
};

template<typename T, char start, char end>
struct unwrap_result<quoted_value<T, start, end>>
{
	using type = unwrap_result_t<T>;
};

template<typename T, char start, char end>
inline unwrap_result_t<T> unwrap(quoted_value<T, start, end>&& t)
{
	return unwrap(std::move(t.value));
}
template<typename T, char start, char end>
inline unwrap_result_t<T>& unwrap(quoted_value<T, start, end>& t)
{
	return unwrap(t.value);
}
template<typename T, char start, char end>
inline unwrap_result_t<T>const& unwrap(quoted_value<T, start, end>const& t)
{
	return unwrap(t.value);
}

template<typename T, char start, char end>
std::istream& operator>>(std::istream& in, quoted_value<T, start, end>& t)
{
	if (!(in >> required{ start })) // read starting quote
		return in;                   // if we failed, return failure state
	if (!(in >> t.value))           // read the value
		return in;                   // if we failed, return failure state
	if (!(in >> required{ end }))   // read ending quote
		return in;                   // if we failed, return failure state
	return in;
}

template<char start, char end>
std::istream& operator>>(std::istream& in, quoted_value<std::string, start, end>& t)
{
	if (!(in >> required{ start })) // read starting quote
		return in;                   // if we failed, return failure state
	std::getline(in, t.value, end); // read until ending quote
	return in;
}

template<typename T, char start, char end>
std::istream& operator>>(std::istream& in, quoted_value<T&, start, end>&& t)
{
	if (!(in >> required{ start })) // read starting quote
		return in;                   // if we failed, return failure state
	if (!(in >> t.value))           // read the value
		return in;                   // if we failed, return failure state
	if (!(in >> required{ end }))   // read ending quote
		return in;                   // if we failed, return failure state
	return in;
}

template<char start, char end>
std::istream& operator>>(std::istream& in, quoted_value<std::string&, start, end>&& t)
{
	if (!(in >> required{ start })) // read starting quote
		return in;                   // if we failed, return failure state
	std::getline(in, t.value, end); // read until ending quote
	return in;
}
template<char start = '"', char end = start>
using quoted_string = quoted_value<std::string, start, end>;
