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

#ifndef JUBATUS_DUMP_RECOMMENDER_HPP_
#define JUBATUS_DUMP_RECOMMENDER_HPP_

#include <stdint.h>

#include <map>
#include <string>

#include <pficommon/data/serialization.h>

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
      ar & MEMBER(column) & MEMBER(norm);
    }
  };

  std::map<std::string, std::map<std::string, float> > inv;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(inv);
  }
};

template <typename S>
struct recommender_base {
  typedef S storage_type;

  sparse_matrix_storage original;
  storage_type storage;

  MSGPACK_DEFINE(original, storage);
};

template <typename S>
struct recommender {
  recommender_base<S> base;
  weight_manager weights;

  // TODO(unno):
  // The current implementation (278e17c89687acc6039e1db8d64b9e81a2e51389)
  // does not read 'base.original'.  We need to fix jubatus.
  MSGPACK_DEFINE(base.storage, weights);
};

template <typename S, typename D>
struct recommender_dump {
  typedef S storage_type;
  typedef D dump_type;

  explicit recommender_dump(const recommender<storage_type>& recommender)
      : original(recommender.base.original),
        index(recommender.base.storage),
        weights(recommender.weights) {
  }

  inverted_index_dump original;
  dump_type index;
  weight_manager_dump weights;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(original) & MEMBER(index) & MEMBER(weights);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_RECOMMENDER_HPP_
