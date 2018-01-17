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
#pragma once

#ifndef GEOJSON_CPP_GAGO_GEOJSON_GEOJSON_H_
#define GEOJSON_CPP_GAGO_GEOJSON_GEOJSON_H_

#include <gago/macros.h>
#include <gago/geometry.h>


NS_GAGO_BEGIN
NS_GEOJSON_BEGIN

using value = gago::geometry::value;
using null_value_t = gago::geometry::null_value_t;
using identifier = gago::geometry::identifier;
using point = gago::geometry::point<double>;
using multi_point = gago::geometry::multi_point<double>;
using linestring = gago::geometry::linestring<double>;
using multi_linestring = gago::geometry::multi_linestring<double>;
using polygon = gago::geometry::polygon<double>;
using multi_polygon = gago::geometry::multi_polygon<double>;
using geometry = gago::geometry::geometry<double>;
using feature = gago::geometry::feature<double>;
using feature_collection = gago::geometry::feature_collection<double>;

template<class T>
T parse(const std::string &);

using geojson = boost::variant<geometry , feature, feature_collection>;

geojson parse(const std::string &);


NS_GEOJSON_END
NS_GAGO_END

#endif //  GEOJSON_CPP_GAGO_GEOJSON_GEOJSON_H_
