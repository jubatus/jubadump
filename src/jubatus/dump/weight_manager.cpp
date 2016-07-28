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

  // group_records (diff)
  {
    const std::map<std::string, double>& master_group_freq =
        weights.master_weights_.group_frequencies_.data_;
    const std::map<std::string, double>& master_group_length =
        weights.master_weights_.group_total_lengths_.data_;

    for (std::map<std::string, double>::const_iterator
         it = master_group_freq.begin(); it != master_group_freq.end(); ++it) {
      // frequency
      group_frequencies[it->first] += it->second;

      // total length (keys in group_freq must be in group_length)
      group_total_lengths[it->first] += master_group_length.at(it->first);
    }
  }

  // group records (master)
  {
    const std::map<std::string, double>& diff_group_freq =
        weights.diff_weights_.group_frequencies_.data_;
    const std::map<std::string, double>& diff_group_length =
        weights.diff_weights_.group_total_lengths_.data_;

    for (std::map<std::string, double>::const_iterator
         it = diff_group_freq.begin(); it != diff_group_freq.end(); ++it) {
      // frequency
      group_frequencies[it->first] += it->second;

      // total length (keys in group_freq must be in group_length)
      group_total_lengths[it->first] += diff_group_length.at(it->first);
    }
  }

  // version_number
  version_number = weights.version_.version_number_;
}

}  // namespace dump
}  // namespace jubatus
