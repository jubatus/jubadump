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

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <msgpack.hpp>
#include <jubatus/util/data/unordered_map.h>
#include <jubatus/core/common/unordered_map.hpp>

namespace jubatus {
namespace dump {

struct key_manager {
  jubatus::util::data::unordered_map<std::string, uint64_t> key2id_;
  jubatus::util::data::unordered_map<uint64_t, std::string> id2key_;
  uint64_t next_id_;

  MSGPACK_DEFINE(key2id_, id2key_, next_id_);
};

struct counter {
  std::map<std::string, unsigned> data_;

  MSGPACK_DEFINE(data_);
};

struct keyword_weights {
  size_t document_count_;
  counter document_frequencies_;
  std::map<std::string, float> weights_;

  MSGPACK_DEFINE(document_count_, document_frequencies_, weights_);
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBTUS_DUMP_TYPES_HPP_
