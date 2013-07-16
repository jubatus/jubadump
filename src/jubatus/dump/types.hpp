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

#ifndef JUBATUS_DUMP_TYPES_HPP_
#define JUBATUS_DUMP_TYPES_HPP_

#include <stdint.h>
#include <string>
#include <vector>

#include <pficommon/data/serialization.h>
#include <pficommon/data/serialization/unordered_map.h>
#include <pficommon/data/unordered_map.h>

namespace jubatus {
namespace dump {

struct key_manager {
  pfi::data::unordered_map<std::string, uint64_t> key2id_;
  std::vector<std::string> id2key_;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(key2id_) & MEMBER(id2key_);
  }
};

struct counter {
  pfi::data::unordered_map<std::string, unsigned> data_;

  template<class Archiver>
  void serialize(Archiver& ar) {
    ar & MEMBER(data_);
  }
};

struct keyword_weights {
  size_t document_count_;
  counter document_frequencies_;
  pfi::data::unordered_map<std::string, float> weights_;

  template<class Archiver>
  void serialize(Archiver& ar) {
    ar
        & MEMBER(document_count_)
        & MEMBER(document_frequencies_)
        & MEMBER(weights_);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBTUS_DUMP_TYPES_HPP_
