//
// Copyright (c) 2018 ChuiZi (wuqinchun at gagogroup.com).
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef GEOJSON_CPP_GAGO_GEOMETRY_VALUE_H_
#define GEOJSON_CPP_GAGO_GEOMETRY_VALUE_H_

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/variant.hpp>

#include <gago/macros.h>

NS_GAGO_BEGIN
NS_GEOMETRY_BEGIN

struct value;

struct null_value_t {
  constexpr null_value_t() {}
  constexpr null_value_t(std::nullptr_t) {}
};

constexpr bool operator==(const null_value_t &,
                          const null_value_t &) { return true; }
constexpr bool operator!=(const null_value_t &,
                          const null_value_t &) { return false; }
constexpr bool operator<(const null_value_t &,
                         const null_value_t &) { return false; }

constexpr null_value_t null_value = null_value_t();

using value_base = boost::variant<null_value_t, bool, uint64_t, int64_t, double, std::string,
                                  std::vector<value>,
                                  std::unordered_map<std::string, value>>;

struct value : value_base {
  using value_base::value_base;
};

using identifier = boost::variant<uint64_t, int64_t, double, std::string>;


NS_GEOMETRY_END
NS_GAGO_END

#endif //  GEOJSON_CPP_GAGO_GEOMETRY_VALUE_H_
