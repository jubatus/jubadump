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

#ifndef JUBATUS_DUMP_COLUMN_TABLE_HPP_
#define JUBATUS_DUMP_COLUMN_TABLE_HPP_

#include <stdint.h>

#include <string>
#include <map>
#include <vector>

#include <msgpack.hpp>
#include <jubatus/util/data/serialization.h>
#include <jubatus/util/data/unordered_map.h>

#include "types.hpp"
#include "msgpack_serializer.hpp"

namespace jubatus {
namespace dump {

struct owner {
  std::string name;

  MSGPACK_DEFINE(name);
};

struct abstract_column {
  // schema_.first:  column type (enum type_name)
  // schema_.second: bit_vector length (if column type is a bit_vector)
  std::pair<uint8_t, uint64_t> schema_;

  // Records: list of value whose type is type_.
  std::vector<msgpack::object> data_;

  MSGPACK_DEFINE(schema_, data_);
};

struct column_table {
  typedef std::pair<owner, uint64_t> version_t;
  typedef jubatus::util::data::unordered_map<std::string, uint64_t> index_table;

  std::vector<std::string> keys_;
  uint64_t tuples_;
  std::vector<version_t> versions_;
  std::vector<abstract_column> columns_;
  uint64_t clock_;
  index_table index_;

  MSGPACK_DEFINE(keys_, tuples_, versions_, columns_, clock_, index_);
};

struct column_type {
  // Copied from jubatus/core/storage/column_type.hpp

  enum type_name {
    int8_type,
    int16_type,
    int32_type,
    int64_type,
    uint8_type,
    uint16_type,
    uint32_type,
    uint64_type,
    float_type,
    double_type,
    bit_vector_type,
    string_type,
    array_type,  // not implemented yet
    invalid_type
  };

  std::string type_as_string() const {
    if (type_ == int8_type) {
      return "int8";
    } else if (type_ == int16_type) {
      return "int16";
    } else if (type_ == int32_type) {
      return "int32";
    } else if (type_ == int64_type) {
      return "int64";
    } else if (type_ == uint8_type) {
      return "uint8";
    } else if (type_ == uint16_type) {
      return "uint16";
    } else if (type_ == uint32_type) {
      return "uint32";
    } else if (type_ == uint64_type) {
      return "uint64";
    } else if (type_ == float_type) {
      return "float";
    } else if (type_ == double_type) {
      return "double";
    } else if (type_ == bit_vector_type) {
      return "bit_vector";
    } else if (type_ == string_type) {
      return "string";
    }
    throw std::out_of_range("unknown type");
  }

  type_name type_;
  int bit_vector_length_;

  template <class Ar>
  void serialize(Ar& ar) {
    std::string t = type_as_string();
    ar
        & JUBA_NAMED_MEMBER("type", t)
        & JUBA_NAMED_MEMBER("bit_vector_length", bit_vector_length_);
  }
};

struct row_entry {
  std::vector<std::vector<msgpack::object> > values;  // variant array
  std::string owner;
  uint64_t version;

  template <class Ar>
  void serialize(Ar& ar) {
    ar
        & JUBA_MEMBER(values)
        & JUBA_MEMBER(owner)
        & JUBA_MEMBER(version);
  }
};

struct column_table_dump {
  explicit column_table_dump(const column_table& table);

  std::vector<column_type> schema;
  std::map<std::string, row_entry> data;
  uint64_t clock;

  template <class Ar>
  void serialize(Ar& ar) {
    ar
        & JUBA_MEMBER(schema)
        & JUBA_MEMBER(data)
        & JUBA_MEMBER(clock);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_COLUMN_TABLE_HPP_
