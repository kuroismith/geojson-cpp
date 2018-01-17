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

#ifndef GEOJSON_CPP_GEOJSON_GEOJSON_IMPL_H_
#define GEOJSON_CPP_GEOJSON_GEOJSON_IMPL_H_

#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/error/en.h>
#include <boost/variant.hpp>
#include <boost/geometry/algorithms/assign.hpp>

#include <gago/macros.h>
#include <gago/geojson/geojson.h>
#include <gago/geojson/rapid_json.h>

NS_GAGO_BEGIN
NS_GEOJSON_BEGIN

using error = std::runtime_error;
using prop_map = std::unordered_map<std::string, value>;

template<typename T>
T convert(const rapidjson_value &json);

template<>
point convert<point>(const rapidjson_value &json) {
  if (json.Size() < 2)
    throw error("coordinates array must have at least 2 numbers");

  return point(json[0].GetDouble(), json[1].GetDouble());
}

template<typename Container>
Container convert(const rapidjson_value &json) {
  Container container;
  auto size = json.Size();
  container.reserve(size);

  for (auto &element : json.GetArray()) {
    container.push_back(convert<typename Container::value_type>(element));
  }
  return container;
}

template <>
value convert<value>(const rapidjson_value &json);

template <>
prop_map convert(const rapidjson_value &json) {
  if (!json.IsObject())
    throw error("properties must be an object");

  prop_map result;
  for (auto &member : json.GetObject()) {
    result.emplace(std::string(member.name.GetString(), member.name.GetStringLength()),
                   convert<value>(member.value));
  }
  return result;
}

template <>
value convert<value>(const rapidjson_value &json) {
  switch (json.GetType()) {
    case rapidjson::kNullType:
      return null_value_t{};
    case rapidjson::kFalseType:
      return false;
    case rapidjson::kTrueType:
      return true;
    case rapidjson::kObjectType:
      return convert<prop_map>(json);
    case rapidjson::kArrayType:
      return convert<std::vector<value>>(json);
    case rapidjson::kStringType:
      return std::string(json.GetString(), json.GetStringLength());
    default:
      assert(json.GetType() == rapidjson::kNumberType);
      if (json.IsUint64())
        return std::uint64_t(json.GetUint64());
      if (json.IsInt64())
        return std::int64_t(json.GetInt64());
      return json.GetDouble();
  }
}

template <>
identifier convert<identifier>(const rapidjson_value &json) {
  switch (json.GetType()) {
    case rapidjson::kStringType:
      return std::string(json.GetString(), json.GetStringLength());
    case rapidjson::kNumberType:
      if (json.IsUint64())
        return std::uint64_t(json.GetUint64());
      if (json.IsInt64())
        return std::int64_t(json.GetInt64());
      return json.GetDouble();
    default:
      throw error("Feature id must be a string or number");
  }
}


template<>
polygon convert(const rapidjson_value &json) {
  polygon p;
  multi_linestring linestrings;
  auto size = json.Size();
  linestrings.reserve(size);

  for (auto &element : json.GetArray()) {
    linestrings.push_back(convert<linestring>(element));
  }

  boost::geometry::assign_points(p.outer(), linestrings[0]);
  for (int i = 1; i < size; i++)
    boost::geometry::assign_points(p.inners()[i - 1], linestrings[i]);

  return p;
}

template<>
geometry convert<geometry>(const rapidjson_value &json) {
  if (!json.IsObject())
    throw error("Geometry must be an object");

  const auto &json_end = json.MemberEnd();

  const auto &type_itr = json.FindMember("type");
  if (type_itr == json_end)
    throw error("Geometry must have a type property");

  const auto &type = type_itr->value;

  const auto &coords_itr = json.FindMember("coordinates");

  if (coords_itr == json_end)
    throw error(std::string(type.GetString())
                    + " geometry must have a coordinates property");

  const auto &json_coords = coords_itr->value;
  if (!json_coords.IsArray())
    throw error("coordinates property must be an array");

  if (type == "Point")
    return convert<point>(json_coords);
  if (type == "MultiPoint")
    return convert<multi_point>(json_coords);
  if (type == "LineString")
    return convert<linestring>(json_coords);
  if (type == "Polygon")
    return convert<polygon>(json_coords);
  if (type == "MultiPolygon")
    return convert<multi_polygon>(json_coords);

  throw error(std::string(type.GetString()) + " not yet implemented");
}


template <>
feature convert<feature>(const rapidjson_value &json) {
  if (!json.IsObject())
    throw error("Feature must be an object");

  auto const &json_end = json.MemberEnd();
  auto const &type_itr = json.FindMember("type");

  if (type_itr == json_end)
    throw error("Feature must have a type property");
  if (type_itr->value != "Feature")
    throw error("Feature type must be Feature");

  auto const &geom_itr = json.FindMember("geometry");

  if (geom_itr == json_end)
    throw error("Feature must have a geometry property");

  feature result{ convert<geometry>(geom_itr->value) };

  auto const &id_itr = json.FindMember("id");
  if (id_itr != json_end) {
    result.id = convert<identifier>(id_itr->value);
  }

  auto const &prop_itr = json.FindMember("properties");
  if (prop_itr != json_end) {
    const auto &json_props = prop_itr->value;
    if (!json_props.IsNull()) {
      result.properties = convert<prop_map>(json_props);
    }
  }

  return result;
}

template<>
geojson convert<geojson>(const rapidjson_value &json) {
  if (!json.IsObject())
    throw error("GeoJSON must be an object");

  const auto &type_itr = json.FindMember("type");
  const auto &json_end = json.MemberEnd();

  if (type_itr == json_end)
    throw error("GeoJSON must have a type property");

  const auto &type = type_itr->value;

  if (type == "FeatureCollection") {
    const auto &features_itr = json.FindMember("features");
    if (features_itr == json_end)
      throw error("FeatureCollection must have features property");

    const auto &json_features = features_itr->value;

    if (!json_features.IsArray())
      throw error("FeatureCollection features property must be an array");

    feature_collection collection;

    const auto &size = json_features.Size();
    collection.reserve(size);

    for (auto &feature_obj : json_features.GetArray()) {
      collection.push_back(convert<feature>(feature_obj));
    }

    return collection;
  }

  if (type == "Feature")
    return convert<feature>(json);

  return convert<geometry>(json);
}

geojson convert(const rapidjson_value &json) {
  return convert<geojson>(json);
}

NS_GEOJSON_END
NS_GAGO_END

#endif //  GEOJSON_CPP_GEOJSON_GEOJSON_IMPL_H_
