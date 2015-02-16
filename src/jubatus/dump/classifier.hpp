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

#ifndef JUBATUS_DUMP_CLASSIFIER_HPP_
#define JUBATUS_DUMP_CLASSIFIER_HPP_

#include <stdint.h>

#include <map>
#include <string>

#include <jubatus/util/data/serialization.h>

#include "types.hpp"
#include "weight_manager.hpp"

namespace jubatus {
namespace dump {

struct val3_t {
  double v1;
  double v2;
  double v3;

  MSGPACK_DEFINE(v1, v2, v3);

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(v1) & JUBA_MEMBER(v2) & JUBA_MEMBER(v3);
  }
};

struct local_storage {
  std::map<std::string, std::map<uint64_t, val3_t> > tbl_;
  key_manager class2id_;

  MSGPACK_DEFINE(tbl_, class2id_);
};

struct local_storage_dump {
  explicit local_storage_dump(const local_storage& storage);

  std::map<std::string, std::map<std::string, val3_t> > weight;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(weight);
  }
};

template <typename S>
struct classifier {
  typedef S storage_type;

  storage_type storage;
  weight_manager weights;

  MSGPACK_DEFINE(storage, weights);
};

template <typename S, typename D>
struct classifier_dump {
  typedef S storage_type;
  typedef D dump_type;

  explicit classifier_dump(const classifier<S>& classifier)
      : storage(classifier.storage),
        weights(classifier.weights) {
  }

  dump_type storage;
  weight_manager_dump weights;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(storage) & JUBA_MEMBER(weights);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_CLASSIFIER_HPP_
