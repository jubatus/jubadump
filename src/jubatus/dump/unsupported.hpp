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

#ifndef JUBATUS_DUMP_UNSUPPORTED_HPP_
#define JUBATUS_DUMP_UNSUPPORTED_HPP_

#include <jubatus/util/data/serialization.h>

namespace jubatus {
namespace dump {

struct unsupported_data {
  MSGPACK_DEFINE();
};

struct unsupported_data_dump {
  const jubatus::util::data::optional<int> null_ptr;

  explicit unsupported_data_dump(const unsupported_data&) {
  }

  template <class Ar>
  void serialize(Ar& ar) {
    ar & JUBA_NAMED_MEMBER("(dump not supported)", null_ptr);
  }
};

}  // namespace dump
}  // namespace jubatus

#endif  // JUBATUS_DUMP_UNSUPPORTED_HPP_
