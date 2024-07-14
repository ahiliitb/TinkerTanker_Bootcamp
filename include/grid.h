#pragma once

#include "json/position.h"
#include <ranges>
#include <tuple>
#include <vector>
#include <iostream>
#include <functional>

namespace miningbots {

  using json::Position;

  [[maybe_unused]]
  static bool positionInDistance(const Position &p1, const Position &p2, const Kilometer &dist) {
    return abs((int)p1.x - p2.x) * abs((int)p1.x - p2.x) + abs((int)p1.y - p2.y) * abs((int)p1.y - p2.y) <= (int)dist * dist;
  }

  template<typename T>
  struct make_not_bool {
    typedef T type;
  };
  
  template<> struct make_not_bool<bool> {
  public:                             
    typedef char type;                   
  };

  template <typename R>
  concept Range = std::ranges::range<R>;

  // TODO: fix this using concepts and stuff

  template<Range R, auto filter>
  static auto filterPosition(R range) {
    using RV = std::ranges::range_reference_t<R>;
    using T = std::tuple_element<1, RV>::type;
    static_assert(std::is_convertible_v<decltype(filter),
		  std::function<bool(const T)>>,
		  "filter must be of type bool(const T&)");
    static_assert(std::is_convertible_v<typename std::tuple_element<0, RV>::type,
		  const Position>,
		  "first element of range_type should be Position");
    return range |
      std::ranges::views::filter([](RV tup) {
	return filter(std::get<1>(tup));
      }) |
      std::ranges::views::transform([](RV tup) {
	return std::move(std::get<0>(tup));
      });
  }

  template <Range R>
  static auto filterPositionByValue(R range) {
    using RV = std::ranges::range_reference_t<R>;
    using T = std::tuple_element<1, RV>::type;
    static_assert(std::is_convertible_v<typename std::tuple_element<0, RV>::type,
		  const Position>,
		  "first element of range_type should be Position");
    return filterPosition<R, [](const T e) { return (bool)(e); }>(range);
  }


  template<typename _T>
  struct Grid {

    using T = make_not_bool<_T>::type;
    
    Grid(size_t x, size_t y):
      data(x, std::vector<T>(y))
    { }
    
    Grid(size_t x, size_t y, T def):
      data(x, std::vector<T>(y, def))
    { }
  
    T& operator[](Position pos) {
      return data[pos.x][pos.y];
    }
    
    const T& operator[](Position pos) const {
      return data[pos.x][pos.y];
    }

    Kilometer max_x() const {
      return data.size();
    }
    
    Kilometer max_y() const {
      return data[0].size();
    }

    auto cells() {
      return data | std::ranges::views::join;
    }

    template<auto printfun>
    void print() const {
      static_assert(std::is_convertible_v<decltype(printfun),
		    std::function<void(const T&)>>,
		    "print function must work as void(const T&)");
      for (int i = 0; i < max_x(); i ++) {
	for (int j = 0; j < max_y(); j ++) {
	  printfun(data[i][j]);
	}
	std::cout << std::endl;
      }
    }
    
    auto cells_indexed() {
      return data | std::ranges::views::enumerate | 
	std::ranges::views::transform([=](std::tuple<const long int, std::vector<T>&> tup1)
	{
	  auto &v = std::get<1>(tup1);
	  return v | std::views::enumerate |
	    std::ranges::views::transform([=](std::tuple<const long int, T&> tup2) {
	      const Position x = {(Kilometer)(std::move(std::get<0>(tup1))), (Kilometer)(std::move(std::get<0>(tup2)))};
	      return std::tuple<const Position, T&>(std::move(x), std::get<1>(tup2));
	    });
	}) | std::ranges::views::join;
    }


    auto square(Position p, Kilometer r) {

      auto [beg_x, x_diff, beg_y, y_diff] = calc_start_end_square(p, r);
      
      return data |
	std::ranges::views::drop(beg_x) |
	std::ranges::views::take(x_diff) |
	std::ranges::views::transform([=](std::vector<T> &v)
	{ return v | std::ranges::views::drop(beg_y) | std::ranges::views::take(y_diff); }) |
	std::ranges::views::join;
    }

    auto square_indexed(Position p, Kilometer r) {

      auto [beg_x, x_diff, beg_y, y_diff] = calc_start_end_square(p, r);

      return data | std::ranges::views::enumerate | 
	std::ranges::views::drop(beg_x) |
	std::ranges::views::take(x_diff) |
	std::ranges::views::transform([=](std::tuple<const long int, std::vector<T>&> tup1)
	{
	  auto &v = std::get<1>(tup1);
	  return v | std::views::enumerate |
	    std::ranges::views::drop(beg_y) |
	    std::ranges::views::take(y_diff) |
	    std::ranges::views::transform([=](std::tuple<const long int, T&> tup2) {
	      const Position x = {(Kilometer)(std::move(std::get<0>(tup1))), (Kilometer)(std::move(std::get<0>(tup2)))};
	      return std::tuple<const Position, T&>(std::move(x), std::get<1>(tup2));
	    });
	}) | std::ranges::views::join;
    }

    auto circle(Position p, Kilometer r) {

      auto [beg_x, x_diff, beg_y, y_diff] = calc_start_end_square(p, r);
      
      return data | std::ranges::views::enumerate | 
	std::ranges::views::drop(beg_x) |
	std::ranges::views::take(x_diff) |
	std::ranges::views::transform([=](std::tuple<const long int, std::vector<T>&> tup1)
	{
	  auto &v = std::get<1>(tup1);
	  return v | std::views::enumerate |
	    std::ranges::views::drop(beg_y) |
	    std::ranges::views::take(y_diff) |
	    std::ranges::views::filter([=](std::tuple<const long int, T&> tup2) {
	      long int x = std::get<0>(tup1);
	      long int y = std::get<0>(tup2);
	      return abs(x - p.x) * abs(x - p.x) + abs(y - p.y) * abs(y - p.y) <= ((long int)r) * r;
	    }) |
	    std::ranges::views::transform([=](std::tuple<const long int, T&> tup2) -> T& {
	      return std::get<1>(tup2);
	    });
	}) | std::ranges::views::join;
    }


    auto circle_indexed(Position p, Kilometer r) {

      auto [beg_x, x_diff, beg_y, y_diff] = calc_start_end_square(p, r);
      
      return data | std::ranges::views::enumerate | 
	std::ranges::views::drop(beg_x) |
	std::ranges::views::take(x_diff) |
	std::ranges::views::transform([=](std::tuple<const long int, std::vector<T>&> tup1)
	{
	  auto &v = std::get<1>(tup1);
	  return v | std::views::enumerate |
	    std::ranges::views::drop(beg_y) |
	    std::ranges::views::take(y_diff) |
	    std::ranges::views::filter([=](std::tuple<const long int, T&> tup2) {
	      long int x = std::get<0>(tup1);
	      long int y = std::get<0>(tup2);
	      return abs(x - p.x) * abs(x - p.x) + abs(y - p.y) * abs(y - p.y) <= ((long int)r) * r;
	    }) |
	    std::ranges::views::transform([=](std::tuple<const long int, T&> tup2) {
	      const Position x = {(Kilometer)(std::move(std::get<0>(tup1))), (Kilometer)(std::move(std::get<0>(tup2)))};
	      return std::tuple<const Position, T&>(std::move(x), std::get<1>(tup2));
	    });
	}) | std::ranges::views::join;
    }
    
  private:
    
    std::tuple<int, int, int, int> calc_start_end_square (Position p, Kilometer r) {
      int beg_x = 0;
      if (p.x > r) beg_x = p.x - r;

      int end_x = p.x + r;
      if (end_x >= (int)max_x()) end_x = max_x() - 1;

      int x_diff = end_x - beg_x + 1;

      int beg_y = 0;
      if (p.y > r) beg_y = p.y - r;
      
      int end_y = p.y + r;
      if (end_y >= (int)max_y()) end_y = max_y() - 1;

      int y_diff = end_y - beg_y + 1;

      return {beg_x, x_diff, beg_y, y_diff};
    }

    
    std::vector<std::vector<T>> data;
  };

  
}
