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

#include <sstream>

#include <gtest/gtest.h>

#include <jubatus/core/driver/classifier.hpp>
#include <jubatus/core/storage/storage_factory.hpp>

#include "classifier.hpp"

using std::stringstream;

namespace jubatus {
namespace dump {

TEST(classifier, trivial) {
  msgpack::sbuffer sbuf;
  {
    jubatus::util::lang::shared_ptr<jubatus::core::storage::storage_base> s
        = jubatus::core::storage::storage_factory::create_storage("local");

    s->set3("f1", "k1", jubatus::core::storage::val3_t(1.0, 2.0, 3.0));

    msgpack::packer<msgpack::sbuffer> pk(sbuf);
    s->pack(pk);
  }

  {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());
    
    local_storage s;
    msg.get().convert(&s);

    EXPECT_EQ(1u, s.tbl_.size());
    EXPECT_EQ(1u, s.tbl_["f1"].size());
    EXPECT_EQ(1.0, s.tbl_["f1"][0].v1);
    EXPECT_EQ(2.0, s.tbl_["f1"][0].v2);
    EXPECT_EQ(3.0, s.tbl_["f1"][0].v3);
  }
}

}  // namespace dump
}  // namespace jubatus
