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
#include "local_storage_mixture.hpp"
#include "weight_manager.hpp"
#include "labels.hpp"

namespace jubatus {
namespace dump {

struct linear_classifier {
  local_storage_mixture storage;
  labels label_storage;

  MSGPACK_DEFINE(storage, label_storage);
};

struct linear_classifier_dump {
  explicit linear_classifier_dump(const linear_classifier& classifier)
      : storage(classifier.storage),
        label(classifier.label_storage) {
  }

  local_storage_mixture_dump storage;
  labels_dump label;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(storage) & JUBA_MEMBER(label);
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
