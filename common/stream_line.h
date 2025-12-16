#pragma once

#include "unwrap.h"

import std;

template<typename T>
struct line
{
	T data;

	friend std::istream& operator>>(std::istream& is, line& l)
	{
		std::string line_string;
		std::getline(is, line_string);
		std::ispanstream ss(line_string);
		ss >> l.data >> std::ws;
		if (!ss.eof()) // failed to parse entire line
		{
			is.setf(std::ios_base::failbit);
		}

		return is;
	}

	operator unwrap_result_t<T>()&&     { return unwrap(std::move(data)); }
	operator unwrap_result_t<T>()const& { return unwrap(data); }
	operator unwrap_result_t<T>()&      { return unwrap(data); }

	bool operator ==(const line<T>&) const = default;
};

template<typename T>
struct unwrap_result<line<T>>
{
	using type = unwrap_result_t<T>;
};

template<typename T>
inline unwrap_result_t<T> unwrap(line<T>&& t)
{
	return unwrap(std::move(t.data));
}
template<typename T>
inline unwrap_result_t<T>& unwrap(line<T>& t)
{
	return unwrap(t.data);
}
template<typename T>
inline unwrap_result_t<T>const& unwrap(line<T>const& t)
{
	return unwrap(t.data);
}

template<>
struct line<std::string>
{
	std::string data;

	friend std::istream& operator>>(std::istream& is, line& l)
	{
		std::getline(is, l.data);

		return is;
	}

	operator std::string& ()& { return data; }
	operator std::string && ()&& { return std::move(data); }
	operator std::string const& () const { return data; }

	bool operator ==(const line<std::string>&) const = default;
};

template<typename T>
struct line<std::vector<T>>
{
	std::vector<T> data;

	friend std::istream& operator>>(std::istream& is, line& l)
	{
		std::string line_string;
		std::getline(is, line_string);
		std::ispanstream ss(line_string);
		l.data = std::ranges::istream_view<T>(ss) | std::ranges::to<std::vector>();
		ss >> std::ws;
		if (!ss.eof()) // failed to parse entire line
		{
			is.setf(std::ios_base::failbit);
		}

		return is;
	}

	operator unwrap_result_t<std::vector<T>>()&&     { return unwrap(std::move(data)); }
	operator unwrap_result_t<std::vector<T>>()const& { return unwrap(data); }
	operator unwrap_result_t<std::vector<T>>()&      { return unwrap(data); }

	// casting operators
	template<typename U>
	operator std::vector<U>() const& { return std::vector<U>(std::from_range, data); }
	template<typename U>
	operator std::vector<U>() &&     { return std::vector<U>(std::from_range, std::move(data)); }

	bool operator ==(const line<std::vector<T>>&) const = default;
};

template<typename T>
struct line<std::optional<T>>
{
	std::optional<T> data;

	friend std::istream& operator>>(std::istream& is, line& l)
	{
		std::string line_string;
		std::getline(is, line_string);
		if (!line_string.empty())
		{
			std::ispanstream ss(line_string);
			T value;
			if (ss >> value)
			{
				l.data = std::move(value);
				ss >> std::ws;
				if (!ss.eof()) // failed to parse entire line
				{
					is.setf(std::ios_base::failbit);
				}
			}
			else
			{
				l.data = std::nullopt;
				is.setf(std::ios_base::failbit);
			}
		}
		else
		{
			l.data = std::nullopt;
		}

		return is;
	}

	operator std::optional<T>&       ()&      { return data; }
	operator std::optional<T>&&      ()&&     { return std::move(data); }
	operator std::optional<T> const& () const { return data; }

	bool operator ==(const line<std::optional<T>>&) const = default;
};
