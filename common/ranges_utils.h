#pragma once

import std;

template<std::ranges::input_range Rng, typename F, typename P = std::identity>
	requires std::indirect_unary_predicate<F, std::projected<std::ranges::iterator_t<Rng>, P>>
auto find_val_if(Rng&& rng, F pred, P proj = P{})
{
	return *std::ranges::find_if(begin(rng), end(rng), std::move(pred), std::move(proj));
}

auto index_of(std::ranges::input_range auto&& rng, const auto& value)
{
	return std::ranges::distance(rng | std::ranges::views::take_while([&value](auto&& v) { return v != value; }));
}

template<std::ranges::input_range Rng, typename F>
	requires std::indirect_unary_predicate<F, std::ranges::iterator_t<Rng>>
auto index_of_if(Rng&& rng, F pred)
{
	return std::ranges::distance(rng | std::ranges::views::take_while(std::not_fn(std::move(pred))));
}
