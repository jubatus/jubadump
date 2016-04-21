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

#ifndef JUBATUS_DUMP_LABELS_HPP_
#define JUBATUS_DUMP_LABELS_HPP_

#include <stdint.h>

#include <string>

#include <msgpack.hpp>
#include <jubatus/util/data/unordered_map.h>
#include <jubatus/util/data/serialization.h>
#include <jubatus/util/data/serialization/unordered_map.h>

#include "types.hpp"

using jubatus::util::data::unordered_map;

namespace jubatus {
namespace dump {

struct labels {
  unordered_map<std::string, uint64_t> master_;
  unordered_map<std::string, uint64_t> diff_;
  version version_;

  MSGPACK_DEFINE(master_, diff_, version_);
};

struct labels_dump {
  explicit labels_dump(const labels& data);

  uint64_t version_number;
  unordered_map<std::string, uint64_t> label_count;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(version_number) & JUBA_MEMBER(label_count);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_LABELS_HPP_
