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

#include "labels.hpp"

#include <string>

namespace jubatus {
namespace dump {

labels_dump::labels_dump(const labels& labels) {
  label_count = labels.master_;
  for (unordered_map<std::string, uint64_t>::const_iterator
      it = labels.diff_.begin(); it != labels.diff_.end(); ++it) {
    label_count[it->first] += it->second;
  }
  version_number = labels.version_.version_number_;
}

}  // namespace dump
}  // namespace jubatus
