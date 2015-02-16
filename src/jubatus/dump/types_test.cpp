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

#include <sstream>

#include <gtest/gtest.h>
#include <jubatus/core/common/key_manager.hpp>

#include "types.hpp"

namespace jubatus {
namespace dump {

TEST(key_manager, trivial) {
  jubatus::core::common::key_manager km;
  uint64_t id1 = km.get_id("key1");
  uint64_t id2 = km.get_id("key2");

  msgpack::sbuffer sbuf;
  msgpack::pack(sbuf, km);

  msgpack::unpacked msg;
  msgpack::unpack(&msg, sbuf.data(), sbuf.size());

  key_manager k;
  msg.get().convert(&k);

  ASSERT_EQ(2u, k.id2key_.size());
  EXPECT_EQ("key1", k.id2key_[id1]);
  EXPECT_EQ("key2", k.id2key_[id2]);
  EXPECT_EQ(id1, k.key2id_["key1"]);
  EXPECT_EQ(id2, k.key2id_["key2"]);
}

}  // namespace dump
}  // namespace jubatus
