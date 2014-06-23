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

#ifndef JUBATUS_DUMP_WEIGHT_MANAGER_HPP_
#define JUBATUS_DUMP_WEIGHT_MANAGER_HPP_

#include <stdint.h>

#include <string>

#include <msgpack.hpp>
#include <jubatus/util/data/unordered_map.h>
#include <jubatus/util/data/serialization.h>
#include <jubatus/util/data/serialization/unordered_map.h>

#include "types.hpp"

namespace jubatus {
namespace dump {

struct weight_manager {
  version version_;
  keyword_weights diff_weights_;
  keyword_weights master_weights_;

  MSGPACK_DEFINE(version_, diff_weights_, master_weights_);
};

struct weight_manager_dump {
  explicit weight_manager_dump(const weight_manager& weights);

  uint64_t version_number;
  uint32_t document_count;
  std::map<std::string, double> document_frequencies;

  template <class Ar>
  void serialize(Ar& ar) {
    ar
        & JUBA_MEMBER(version_number)
        & JUBA_MEMBER(document_count)
        & JUBA_MEMBER(document_frequencies);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_WEIGHT_MANAGER_HPP_
