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

#include "weight_manager.hpp"

#include <string>

namespace jubatus {
namespace dump {

weight_manager_dump::weight_manager_dump(const weight_manager& weights) {
  // document_count
  document_count =
      weights.diff_weights_.document_count_
      + weights.master_weights_.document_count_;

  // document_frequencies
  document_frequencies = weights.master_weights_.document_frequencies_.data_;
  const std::map<std::string, double>&
      diff = weights.diff_weights_.document_frequencies_.data_;
  for (std::map<std::string, double>::const_iterator
           it = diff.begin(); it != diff.end(); ++it) {
    document_frequencies[it->first] += it->second;
  }

  // group_frequencies
  group_frequencies = weights.master_weights_.group_frequencies_.data_;
  {
    const std::map<std::string, double>& diff_freq =
        weights.diff_weights_.group_frequencies_.data_;
    for (std::map<std::string, double>::const_iterator
        it = diff_freq.begin(); it != diff_freq.end(); ++it) {
      group_frequencies[it->first] += it->second;
    }
  }

  // group_total_lengths
  group_total_lengths = weights.master_weights_.group_total_lengths_.data_;
  {
    const std::map<std::string, double>& diff_lengths =
        weights.diff_weights_.group_total_lengths_.data_;
    for (std::map<std::string, double>::const_iterator
         it = diff_lengths.begin(); it != diff_lengths.end(); ++it) {
      group_total_lengths[it->first] += it->second;
    }
  }

  // version_number
  version_number = weights.version_.version_number_;
}

}  // namespace dump
}  // namespace jubatus
