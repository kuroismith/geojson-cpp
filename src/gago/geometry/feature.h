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

#ifndef GEOJSON_CPP_GAGO_GEOMETRY_FEATURE_H_
#define GEOJSON_CPP_GAGO_GEOMETRY_FEATURE_H_

#include <unordered_map>
#include <experimental/optional>

#include <gago/macros.h>
#include <gago/geometry/geometry.h>
#include <gago/geometry/value.h>

NS_GAGO_BEGIN
NS_GEOMETRY_BEGIN

using property_map = std::unordered_map<std::string, value>;

template<class T>
struct feature {
  using geometry_type = geometry<T>;

  geometry_type geometry;
  property_map properties{};
  std::experimental::optional<identifier> id{};

  feature(geometry_type geometry_,
          property_map properties_ = property_map {},
          std::experimental::optional<identifier> id_ = std::experimental::optional<
              identifier> {})
      : geometry(std::move(geometry_)),
        properties(std::move(properties_)),
        id(std::move(id_)) {}
};

template<class T>
constexpr bool operator==(feature<T> const &lhs, feature<T> const &rhs) {
  return lhs.id == rhs.id && lhs.geometry == rhs.geometry
      && lhs.properties == rhs.properties;
}

template<class T>
constexpr bool operator!=(feature<T> const &lhs, feature<T> const &rhs) {
  return !(lhs == rhs);
}

template<
    class T,
    template<typename...> class Container = std::vector
>
struct feature_collection : Container<feature<T>> {
  using feature_type = feature<T>;
  using container_type = Container<feature_type>;
  using container_type::container_type;
};

NS_GEOMETRY_END
NS_GAGO_END

#endif //  GEOJSON_CPP_GAGO_GEOMETRY_FEATURE_H_
