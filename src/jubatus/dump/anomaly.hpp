// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2015 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

#ifndef JUBATUS_DUMP_ANOMALY_HPP_
#define JUBATUS_DUMP_ANOMALY_HPP_

#include <jubatus/util/data/serialization.h>

#include <stdint.h>
#include <map>
#include <string>

#include "recommender.hpp"

namespace jubatus {
namespace dump {

struct lof_entry {
  double kdist;
  double lrd;

  MSGPACK_DEFINE(kdist, lrd);

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(kdist) & JUBA_MEMBER(lrd);
  }
};

typedef jubatus::util::data::unordered_map<std::string, lof_entry> lof_table_t;

struct lof_storage {
  lof_table_t lof_table_;
  lof_table_t lof_table_diff_;

  // Note: the following values are persisted but actually
  // configuration values.
  uint32_t neighbor_num_;
  uint32_t reverse_nn_num_;

  MSGPACK_DEFINE(lof_table_, lof_table_diff_, neighbor_num_, reverse_nn_num_);
};

struct lof_storage_dump {
  explicit lof_storage_dump(const lof_storage& storage);
  void extract_from_table(const lof_table_t&);

  std::map<std::string, lof_entry> entry;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(entry);
  }
};

template <typename B>
struct lof {
  typedef B backend_type;

  lof_storage storage;
  backend_type nn_storage;

  MSGPACK_DEFINE(storage, nn_storage);
};

template <typename B, typename D>
struct lof_dump {
  typedef B backend_type;
  typedef D backend_dump_type;

  explicit lof_dump(const lof<backend_type>& method)
      : lof_records(method.storage),
        nn_records(method.nn_storage) {
  }

  lof_storage_dump lof_records;
  backend_dump_type nn_records;

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_MEMBER(lof_records) & JUBA_MEMBER(nn_records);
  }
};

template <typename M>
struct anomaly {
  typedef M anomaly_method_type;

  anomaly_method_type storage;
  weight_manager weights;

  MSGPACK_DEFINE(storage, weights);
};

template <typename M, typename D>
struct anomaly_dump {
  typedef D dump_type;

  explicit anomaly_dump(const anomaly<M>& anomaly)
      : storage(anomaly.storage),
        weights(anomaly.weights) {
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

#endif  // JUBATUS_DUMP_ANOMALY_HPP_
