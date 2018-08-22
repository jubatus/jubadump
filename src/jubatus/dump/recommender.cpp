// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2013 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

using jubatus::util::data::unordered_map;

namespace jubatus {
namespace dump {

namespace {

void convert_table(
    const tbl_t& tbl,
    const unordered_map<uint64_t, std::string>& id2key,
    std::map<std::string, std::map<std::string, double> >& inv) {
  for (tbl_t::const_iterator it = tbl.begin();
       it != tbl.end(); ++it) {
    const std::string& feature = it->first;

    for (row_t::const_iterator it2 = it->second.begin();
         it2 != it->second.end(); ++it2) {
      uint64_t row_id = it2->first;
      unordered_map<uint64_t, std::string>::const_iterator id_it
          = id2key.find(row_id);

      // TODO(unno): How to treat row_id when it is not found in key_manager?
      if (id_it == id2key.end()) {
        std::string message = "ID is not found. This file is broken: "
            + jubatus::util::lang::lexical_cast<std::string>(row_id);
        throw std::runtime_error(message);
      }

      std::string id = id_it->second;
      double value = it2->second;
      inv[feature][id] = value;
    }
  }
}

}  // namespace

inverted_index_dump::inverted_index_dump(
    const inverted_index_storage& storage) {
  convert_table(storage.inv_, storage.column2id_.id2key_, inv);
}

inverted_index_dump::inverted_index_dump(
    const sparse_matrix_storage& storage) {
  convert_table(storage.tbl_, storage.column2id_.id2key_, inv);
}

}  // namespace dump
}  // namespace jubatus
