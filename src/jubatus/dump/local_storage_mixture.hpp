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

#ifndef JUBATUS_DUMP_LOCAL_STORAGE_MIXTURE_HPP_
#define JUBATUS_DUMP_LOCAL_STORAGE_MIXTURE_HPP_

#include <string>
#include <map>

#include <msgpack.hpp>
#include <jubatus/util/data/serialization.h>

#include "types.hpp"
#include "local_storage.hpp"

namespace jubatus {
namespace dump {

struct local_storage_mixture {
  std::map<std::string, std::map<uint64_t, val3_t> > tbl_;
  key_manager class2id_;
  std::map<std::string, std::map<uint64_t, val3_t> > tbl_diff_;

  MSGPACK_DEFINE(tbl_, class2id_, tbl_diff_);
};

struct local_storage_mixture_dump {
  explicit local_storage_mixture_dump(const local_storage_mixture& storage);

  std::map<std::string, std::map<std::string, val3_t> > weight;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(weight);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_LOCAL_STORAGE_MIXTURE_HPP_
