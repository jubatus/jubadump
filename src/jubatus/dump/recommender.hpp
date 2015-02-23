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

#ifndef JUBATUS_DUMP_RECOMMENDER_HPP_
#define JUBATUS_DUMP_RECOMMENDER_HPP_

#include <stdint.h>

#include <map>
#include <string>

#include <jubatus/util/data/serialization.h>

#include "types.hpp"
#include "weight_manager.hpp"

namespace jubatus {
namespace dump {

typedef std::map<uint64_t, float> row_t;
typedef std::map<std::string, row_t> tbl_t;

typedef std::map<uint64_t, float> imap_float_t;

struct sparse_matrix_storage {
  tbl_t tbl_;
  key_manager column2id_;

  MSGPACK_DEFINE(tbl_, column2id_);
};

struct inverted_index_storage {
  tbl_t inv_;
  tbl_t inv_diff_;
  imap_float_t column2norm_;
  imap_float_t column2norm_diff_;
  key_manager column2id_;

  MSGPACK_DEFINE(inv_, inv_diff_, column2norm_, column2norm_diff_, column2id_);
};

struct inverted_index_dump {
  explicit inverted_index_dump(const inverted_index_storage& storage);

  explicit inverted_index_dump(const sparse_matrix_storage& storage);

  struct column {
    std::map<std::string, float> column;
    float norm;

    template <class Ar>
    void serialize(Ar& ar) {
      ar & JUBA_MEMBER(column) & JUBA_MEMBER(norm);
    }
  };

  std::map<std::string, std::map<std::string, float> > inv;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(inv);
  }
};

struct inverted_index {
  sparse_matrix_storage original;
  inverted_index_storage storage;

  MSGPACK_DEFINE(original, storage);
};

template <typename R>
struct recommender {
  R index;
  weight_manager weights;

  MSGPACK_DEFINE(index, weights);
};

struct inverted_index_recommender_dump {
  inverted_index_dump original;
  inverted_index_dump storage;

  explicit
  inverted_index_recommender_dump(const inverted_index& index)
      : original(index.original),
        storage(index.storage) {
  }

  template <typename Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(original) & JUBA_MEMBER(storage);
  }
};

template <typename D>
struct recommender_dump {
  typedef D dump_type;

  template <typename R>
  explicit recommender_dump(const recommender<R>& recommender)
      : index(recommender.index),
        weights(recommender.weights) {
  }

  dump_type index;
  weight_manager_dump weights;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(index) & JUBA_MEMBER(weights);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_RECOMMENDER_HPP_
