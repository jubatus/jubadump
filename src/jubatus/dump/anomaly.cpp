// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2015 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

#include "anomaly.hpp"

#include <jubatus/util/data/unordered_map.h>
#include <string>

using jubatus::util::data::unordered_map;

namespace jubatus {
namespace dump {

void lof_storage_dump::extract_from_table(const lof_table_t& table) {
  for (lof_table_t::const_iterator it = table.begin();
       it != table.end(); ++it) {
    entry[it->first] = it->second;
  }
}

lof_storage_dump::lof_storage_dump(const lof_storage& storage) {
  extract_from_table(storage.lof_table_diff_);
  extract_from_table(storage.lof_table_);
}

}  // namespace dump
}  // namespace jubatus
