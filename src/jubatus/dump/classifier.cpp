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

#include "classifier.hpp"

#include <string>

#include <jubatus/util/data/unordered_map.h>

using jubatus::util::data::unordered_map;

namespace jubatus {
namespace dump {

local_storage_dump::local_storage_dump(const local_storage& storage) {
  for (std::map<std::string, std::map<uint64_t, val3_t> >
           ::const_iterator it = storage.tbl_.begin();
       it != storage.tbl_.end(); ++it) {
    const std::string& feature = it->first;

    for (std::map<uint64_t, val3_t>::const_iterator
             it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
      jubatus::util::data::unordered_map<uint64_t, std::string>::const_iterator
          key = storage.class2id_.id2key_.find(it2->first);
      if (key != storage.class2id_.id2key_.end()) {
        const std::string& label = key->second;
        weight[feature][label] = it2->second;
      } else {
        throw std::runtime_error("unknown key found in storage");
      }
    }
  }
}

}  // namespace dump
}  // namespace jubatus
