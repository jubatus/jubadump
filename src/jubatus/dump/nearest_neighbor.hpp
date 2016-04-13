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

#ifndef JUBATUS_DUMP_NEAREST_NEIGHBOR_HPP_
#define JUBATUS_DUMP_NEAREST_NEIGHBOR_HPP_

#include <map>
#include <string>

#include <jubatus/util/data/serialization.h>

#include "column_table.hpp"
#include "weight_manager.hpp"

namespace jubatus {
namespace dump {

struct nearest_neighbor {
  column_table table;
  weight_manager weights;

  MSGPACK_DEFINE(table, weights);
};

struct nearest_neighbor_dump {
  explicit nearest_neighbor_dump(const nearest_neighbor& nn)
      : table(nn.table),
        weights(nn.weights) {
  }

  column_table_dump table;
  weight_manager_dump weights;

  template <class Ar>
  void serialize(Ar& ar) {
    ar
        & JUBA_MEMBER(table)
        & JUBA_MEMBER(weights);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_NEAREST_NEIGHBOR_HPP_
