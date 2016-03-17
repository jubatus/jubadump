// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2016 Preferred Networks and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef JUBATUS_DUMP_MSGPACK_SERIALIZER_HPP_
#define JUBATUS_DUMP_MSGPACK_SERIALIZER_HPP_

#include <msgpack.hpp>
#include <jubatus/util/lang/cast.h>
#include <jubatus/util/text/json.h>

namespace jubatus {
namespace util {
namespace text {
namespace json {

/**
 * Extend JSON serializer so that MessagePack objects can be serialized.
 */
inline void serialize(json& js, msgpack::object& obj) {
  switch(obj.type) {
  case msgpack::type::BOOLEAN: {
    js = json(new json_bool(obj.via.boolean));
    break;
  }
  case msgpack::type::POSITIVE_INTEGER: {
    js = json(new json_integer(obj.via.i64));
    break;
  }
  case msgpack::type::NEGATIVE_INTEGER: {
    js = json(new json_integer(obj.via.u64));
    break;
  }
  case msgpack::type::DOUBLE: {
    js = json(new json_float(obj.via.dec));
    break;
  }
  case msgpack::type::RAW: {
    js = json(new json_string(std::string(obj.via.raw.ptr, obj.via.raw.size)));
    break;
  }
  case msgpack::type::ARRAY: {
    json tmp(new json_array());
    for (size_t i = 0; i < obj.via.array.size; ++i) {
      json v;
      serialize(v, obj.via.array.ptr[i]);
      tmp.add(v);
    }
    js = tmp;
    break;
  }
  case msgpack::type::MAP: {
    json tmp(new json_object());
    for (size_t i = 0; i < obj.via.map.size; ++i) {
      msgpack::object_kv ent = obj.via.map.ptr[i];
      json v;
      serialize(v, ent.val);
      tmp[jubatus::util::lang::lexical_cast<std::string>(ent.key)] = v;
    }
    js = tmp;
    break;
  }
  case msgpack::type::NIL: {
    js = json(new json_null());
    break;
  }
  }  // switch
}

}  // namespace json
}  // namespace text
}  // namespace util
}  // namespace jubatus

#endif
