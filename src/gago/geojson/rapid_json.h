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

#ifndef GEOJSON_CPP_GAGO_GEOJSON_RAPID_JSON_H_
#define GEOJSON_CPP_GAGO_GEOJSON_RAPID_JSON_H_

#include <rapidjson/document.h>

#include <gago/macros.h>
#include <gago/geojson/geojson.h>

NS_GAGO_BEGIN
NS_GEOJSON_BEGIN

using rapidjson_allocator = rapidjson::CrtAllocator;
using rapidjson_document = rapidjson::GenericDocument<rapidjson::UTF8<>, rapidjson_allocator>;
using rapidjson_value = rapidjson::GenericValue<rapidjson::UTF8<>, rapidjson_allocator>;

NS_GEOJSON_END
NS_GAGO_END


#endif //  GEOJSON_CPP_GAGO_GEOJSON_RAPID_JSON_H_
