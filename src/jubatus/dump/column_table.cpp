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

#include "column_table.hpp"

#include <string>

#include <msgpack.hpp>

namespace jubatus {
namespace dump {

column_table_dump::column_table_dump(const column_table& table) {
  // Extract schema
  for (size_t col_id = 0; col_id < table.columns_.size(); ++col_id) {
    column_type t;
    t.type_ = static_cast<column_type::type_name>(table.columns_[col_id].schema_.first);
    t.bit_vector_length_ = table.columns_[col_id].schema_.second;
    schema.push_back(t);
  }

  // Extract table contents
  for (size_t row_id = 0; row_id < table.keys_.size(); ++row_id) {
    std::string key = table.keys_[row_id];

    column_table::index_table::const_iterator it = table.index_.find(key);
    if (it == table.index_.end()) {
      throw std::runtime_error("index is broken");
    }

    uint64_t row_index = it->second;
    row_entry row;
    for (size_t col_id = 0; col_id < table.columns_.size(); ++col_id) {
      std::vector<msgpack::object> v;

      /* For bit_vector values, multiple uint64_t values (blocks) are used to
         record bit_vector.  For other type of values (e.g., int, string, ...)
         only consumes 1 block (note that bit_vector_length_ is 0 for them.) */
      size_t blocks = ((schema[col_id].bit_vector_length_ - 1) / 64) + 1;
      for (size_t i = 0; i < blocks; ++i) {
        v.push_back(table.columns_[col_id].data_[row_index + i]);
      }
      row.values.push_back(v);
    }
    row.owner = table.versions_[row_id].first.name;
    row.version = table.versions_[row_id].second;

    data[key] = row;
  }

  // Extract clock
  clock = table.clock_;
}

}  // namespace dump
}  // namespace jubatus
