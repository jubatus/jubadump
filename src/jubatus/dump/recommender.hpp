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
#include <pficommon/data/serialization/unordered_map.h>
#include <pficommon/data/unordered_map.h>

#include "types.hpp"
#include "weight_manager.hpp"

namespace jubatus {
namespace dump {

typedef pfi::data::unordered_map<uint64_t, float> row_t;
typedef pfi::data::unordered_map<std::string, row_t> tbl_t;

typedef std::map<uint64_t, float> imap_float_t;

struct sparse_matrix_storage {
  template <class Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(tbl_) & MEMBER(column2id_);
  }

  tbl_t tbl_;
  key_manager column2id_;
};

struct inverted_index_storage {
  template <class Ar>
  void serialize(Ar& ar) {
    ar
        & MEMBER(inv_)
        & MEMBER(inv_diff_)
        & MEMBER(column2norm_)
        & MEMBER(column2norm_diff_)
        & MEMBER(column2id_);
  }

  tbl_t inv_;
  tbl_t inv_diff_;
  imap_float_t column2norm_;
  imap_float_t column2norm_diff_;
  key_manager column2id_;
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
struct recommender {
  typedef S storage_type;

  sparse_matrix_storage original;
  storage_type storage;
  weight_manager weights;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & MEMBER(original) & MEMBER(storage) & MEMBER(weights);
  }
};

template <typename S, typename D>
struct recommender_dump {
  typedef S storage_type;
  typedef D dump_type;

  explicit recommender_dump(const recommender<storage_type>& recommender)
      : original(recommender.original),
        index(recommender.storage),
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
