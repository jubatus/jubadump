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

#ifndef JUBADUMP_BIT_INDEX_HPP_
#define JUBADUMP_BIT_INDEX_HPP_

#include <stdint.h>

#include <map>
#include <string>
#include <vector>

#include <jubatus/util/data/unordered_map.h>

namespace jubadump {

struct bit_vector {
  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(bits_) & JUBA_MEMBER(bit_num_);
  }

  std::vector<uint64_t> bits_;
  uint64_t bit_num_;
};

typedef jubatus::util::data::unordered_map<std::string, bit_vector> bit_table_t;

struct bit_index_storage {
  bit_table_t bitvals_;
  bit_table_t bitvals_diff_;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(bitvals_) & JUBA_MEMBER(bitvals_diff_);
  }
};

struct lsh {
  jubatus::util::data::unordered_map<std::string, std::vector<float> >
    column2baseval_;
  bit_index_storage row2lshvals_;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & column2baseval_ & row2lshvals_;
  }
};

struct bit_index_dump {
  std::map<std::string, std::string> table;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(table);
  }
};

struct lsh_dump {
};

}  // namespace jubadump

#endif  // JUBADUMP_BIT_INDEX_HPP_
