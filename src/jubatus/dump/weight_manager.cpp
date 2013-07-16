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

#include "weight_manager.hpp"

#include <string>

namespace jubatus {
namespace dump {

weight_manager_dump::weight_manager_dump(const weight_manager& weights) {
  document_count =
      weights.diff_weights_.document_count_
      + weights.master_weights_.document_count_;
  document_frequencies = weights.master_weights_.document_frequencies_.data_;
  const pfi::data::unordered_map<std::string, unsigned>&
      diff = weights.diff_weights_.document_frequencies_.data_;
  for (pfi::data::unordered_map<std::string, unsigned>::const_iterator
           it = diff.begin(); it != diff.end(); ++it) {
    document_frequencies[it->first] += it->second;
  }
}

}  // namespace dump
}  // namespace jubatus
