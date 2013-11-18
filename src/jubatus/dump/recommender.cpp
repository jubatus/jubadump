// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
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

#include "recommender.hpp"

#include <string>

#include <jubatus/util/lang/cast.h>

namespace jubatus {
namespace dump {

inverted_index_dump::inverted_index_dump(
    const inverted_index_storage& storage) {
  for (tbl_t::const_iterator it = storage.inv_.begin();
       it != storage.inv_.end(); ++it) {
    const std::string& feature = it->first;

    for (row_t::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
      uint64_t row_id = it2->first;
      float value = it2->second;
      inv[feature][jubatus::util::lang::lexical_cast<std::string>(row_id)] = value;
    }
  }
}

inverted_index_dump::inverted_index_dump(
    const sparse_matrix_storage& storage) {
  for (tbl_t::const_iterator it = storage.tbl_.begin();
       it != storage.tbl_.end(); ++it) {
    const std::string& feature = it->first;

    for (row_t::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
      uint64_t row_id = it2->first;
      float value = it2->second;
      inv[feature][jubatus::util::lang::lexical_cast<std::string>(row_id)] = value;
    }
  }
}

}  // namespace dump
}  // namespace jubatus
