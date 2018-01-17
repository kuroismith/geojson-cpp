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

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>

#include <boost/geometry.hpp>

#include <gago/geojson.h>

using namespace gago::geojson;

enum class geometry_type {
  POINT = 0,
  MULTIPOINT,
  LINESTRING,
  MULTILINESTRING,
  POLYGON,
  MULTIPOLYGON
};
enum class geojson_type {
  GEOMETRY = 0,
  FEATURE,
  FEATURECOLLECTION
};

enum class value_type {
  NIL = 0,
  BOOL,
  UINT,
  INT,
  DOUBLE,
  STRING,
  VECTOR,
  MAP
};

geojson readGeoJSON(const std::string &path) {
  std::ifstream t(path.c_str());
  std::stringstream buffer;
  buffer << t.rdbuf();
  rapidjson_document d;
  d.Parse<0>(buffer.str().c_str());
  return convert(d);
}

static void testPoint() {
  const auto &data = readGeoJSON("test/data/point.json");
  assert(data.which() == int(geojson_type::GEOMETRY));

  const auto& geom = boost::get<geometry >(data);
  assert(geom.which() == (int)geometry_type::POINT);

  const auto& p = boost::get<point >(geom);
  assert(p.x() == 30.5);
  assert(p.y() == 50.5);
}

static void testMultiPoint() {
  const auto &data = readGeoJSON("test/data/multi-point.json");
  assert(data.which() == int(geojson_type::GEOMETRY));

  const auto& geom = boost::get<geometry >(data);
  assert(geom.which() == (int)geometry_type::MULTIPOINT);

  const auto& points = boost::get<multi_point >(geom);
  assert(points.size() == 2);
}

static void testLineString() {
  const auto &data = readGeoJSON("test/data/linestring.json");
  assert(data.which() == int(geojson_type::GEOMETRY));

  const auto& geom = boost::get<geometry >(data);
  assert(geom.which() == (int)geometry_type::LINESTRING);

  const auto &points = boost::get<linestring>(geom);
  assert(points.size() == 2);
}

static void testPolygon() {
  const auto &data = readGeoJSON("test/data/polygon.json");
  assert(data.which() == int(geojson_type::GEOMETRY));

  const auto& geom = boost::get<geometry >(data);
  assert(geom.which() == (int)geometry_type::POLYGON);

  const auto &rings = boost::get<polygon>(geom);
  assert(rings.outer().size() == 5);
  assert(boost::geometry::equals(rings.outer()[0], rings.outer()[4]));
}

static void testMultiPolygon() {
  const auto &data = readGeoJSON("test/data/multi-polygon.json");
  assert(data.which() == int(geojson_type::GEOMETRY));

  const auto& geom = boost::get<geometry >(data);
  assert(geom.which() == (int)geometry_type::MULTIPOLYGON);

  const auto &polygons = boost::get<multi_polygon >(geom);
  assert(polygons.size() == 1);
  assert(polygons[0].outer().size() == 5);
  assert(boost::geometry::equals(polygons[0].outer()[0], polygons[0].outer()[4]));
}

static void testFeature() {
  const auto &data = readGeoJSON("test/data/feature.json");
  assert(data.which() == int(geojson_type::FEATURE));

  const auto& f = boost::get<feature>(data);
  assert(f.geometry.which() == (int)geometry_type::POINT);

  assert(f.properties.at("bool").which() == (int)value_type::BOOL);
  assert(boost::get<bool>(f.properties.at("bool")) == true);
  assert(f.properties.at("string").which() == (int)value_type::STRING);
  assert(boost::get<std::string>(f.properties.at("string")) == "foo");
  assert(f.properties.at("double").which() == (int)value_type::DOUBLE);
  assert(boost::get<double>(f.properties.at("double")) == 2.5);
  assert(f.properties.at("uint").which() == (int)value_type::UINT);
  assert(boost::get<uint64_t >(f.properties.at("uint")) == 10);
  assert(f.properties.at("int").which() == (int)value_type::INT);
  assert(boost::get<int64_t >(f.properties.at("int")) == -10);
  assert(f.properties.at("null").which() == (int)value_type::NIL);
  assert(boost::get<null_value_t >(f.properties.at("null")) == null_value_t{});

  using values   = std::vector<value>;
  using prop_map = std::unordered_map<std::string, value>;

  const auto &nested = f.properties.at("nested");
  assert(nested.which() == (int)value_type::VECTOR);

  const auto& vec = boost::get<values>(nested);

  assert(vec.at(0).which() == (int)value_type::UINT);
  assert(boost::get<uint64_t >(vec.at(0)) == 5);
  assert(vec.at(1).which() == (int)value_type::MAP);
  assert(boost::get<prop_map >(vec.at(1)).at("foo").which() == (int)value_type::STRING);
  assert(boost::get<std::string>(boost::get<prop_map >(vec.at(1)).at("foo")) == "bar");
}

static void testFeatureCollection() {
  const auto &data = readGeoJSON("test/data/feature-collection.json");
  assert(data.which() == int(geojson_type::FEATURECOLLECTION));

  const auto& features = boost::get<feature_collection>(data);

  assert(features.size() == 2);
}

void testAll() {
  testPoint();
  testMultiPoint();
  testLineString();
  testPolygon();
  testMultiPolygon();
  testFeature();
  testFeatureCollection();
}

int main() {
  testAll();
  return 0;
}
