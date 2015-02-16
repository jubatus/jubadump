// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2014 Preferred Networks and Nippon Telegraph and Telephone Corporation.
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

#include <gtest/gtest.h>


#include <jubatus/core/fv_converter/weight_manager.hpp>
#include <jubatus/core/framework/stream_writer.hpp>

#include "weight_manager.hpp"

namespace jubatus {
namespace dump {

TEST(weight_manager, trivial) {
  msgpack::sbuffer buf;
  {
    jubatus::core::fv_converter::weight_manager wm;
    jubatus::core::common::sfv_t fv;
    fv.push_back(std::make_pair("a", 1));
    wm.update_weight(fv);

    jubatus::core::framework::stream_writer<msgpack::sbuffer> st(buf);
    jubatus::core::framework::jubatus_packer jp(st);
    jubatus::core::framework::packer packer(jp);
    wm.pack(packer);
  }

  {
    msgpack::unpacked msg;
    msgpack::unpack(&msg, buf.data(), buf.size());

    weight_manager wm;
    msg.get().convert(&wm);

    EXPECT_EQ(0u, wm.version_.version_number_);
    EXPECT_EQ(1u, wm.diff_weights_.document_count_);
    EXPECT_EQ(1u, wm.diff_weights_.document_frequencies_.data_.count("a"));
  }
}

}  // namespace dump
}  // namespace jubatus
