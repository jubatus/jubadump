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

#include <iostream>
#include <string>
#include <exception>
#include <vector>

#include <msgpack.hpp>
#include <jubatus/core/common/big_endian.hpp>
#include <jubatus/core/common/jsonconfig/config.hpp>
#include <jubatus/core/common/jsonconfig/cast.hpp>
#include <jubatus/util/data/serialization/unordered_map.h>
#include <jubatus/util/text/json.h>
#include <jubatus/util/lang/cast.h>

#include "third_party/cmdline/cmdline.h"

#include "jubatus/dump/classifier.hpp"
#include "jubatus/dump/recommender.hpp"
#include "jubatus/dump/anomaly.hpp"
#include "jubatus/dump/nearest_neighbor.hpp"
#include "jubatus/dump/unsupported.hpp"

using std::runtime_error;
using jubatus::core::common::read_big_endian;
using jubatus::util::lang::lexical_cast;
using jubatus::util::text::json::from_json;

namespace jubatus {
namespace dump {

struct model {
  std::string type_;
  jubatus::core::common::jsonconfig::config config_;
  std::vector<char> user_data_;
};

void read(std::ifstream& ifs, model& m) {
  // TODO(unno): This implementation ignores checksums, version, and all other
  // check codes. We need to re-implemenet such process like
  // jubatus::server::framework::save_server/load_server, or to use these
  // methods to show file format errors.

  ifs.exceptions(std::ifstream::failbit);

  std::vector<char> system_data_buf;
  try {
    char header_buf[48];
    ifs.read(header_buf, 48);

    uint64_t system_data_size = read_big_endian<uint64_t>(&header_buf[32]);
    system_data_buf.resize(system_data_size);
    ifs.read(&system_data_buf[0], system_data_size);

    uint64_t user_data_size = read_big_endian<uint64_t>(&header_buf[40]);
    m.user_data_.resize(user_data_size);
    ifs.read(&m.user_data_[0], user_data_size);
  } catch (std::ios_base::failure& e) {
    throw runtime_error("Input stream reached end of file.");
  }

  if (ifs.peek() != -1) {
    std::ifstream::pos_type pos = ifs.tellg();
    throw runtime_error("Input stream remains. Position: " +
        lexical_cast<std::string>(pos));
  }
  ifs.close();

  msgpack::unpacked msg;
  msgpack::unpack(&msg, system_data_buf.data(), system_data_buf.size());
  msgpack::object system_data = msg.get();
  std::string config;
  system_data.via.array.ptr[2].convert(&m.type_);
  system_data.via.array.ptr[4].convert(&config);
  m.config_ = jubatus::core::common::jsonconfig::config(
      jubatus::util::lang::lexical_cast<
          jubatus::util::text::json::json>(config));
}

template <typename T, typename D>
void dump(model& m, jubatus::util::text::json::json& js) {
  msgpack::unpacked msg_user;
  msgpack::unpack(&msg_user, m.user_data_.data(), m.user_data_.size());

  // obj[0] is the version of the saved model file
  T data;
  msg_user.get().via.array.ptr[1].convert(&data);

  D dump(data);
  js = jubatus::util::text::json::to_json(dump);
}

int run(const std::string& path) try {
  std::ifstream ifs(path.c_str());
  if (!ifs) {
    throw runtime_error("Cannot open: " + path);
  }

  model m;
  jubatus::util::text::json::json js;

  try {
    read(ifs, m);
  } catch (const std::exception& e) {
    throw runtime_error(std::string("invalid model file structure: ") +
                        e.what());
  }

  if (m.type_ == "classifier") {
    std::string method;
    from_json<std::string>(m.config_["method"].get(), method);
    if (method != "NN") {
      dump<classifier<local_storage>,
          classifier_dump<local_storage, local_storage_dump> >(m, js);
    } else {
      throw runtime_error("classifier method \"" + method +
                          "\" is not supported for dump");
    }
  } else if (m.type_ == "regression") {
    // same model data structure as classifier
    dump<classifier<local_storage>,
        classifier_dump<local_storage, local_storage_dump> >(m, js);
  } else if (m.type_ == "recommender") {
    std::string method;
    from_json<std::string>(m.config_["method"].get(), method);
    if (method == "inverted_index") {
      dump<recommender<inverted_index>,
           recommender_dump<inverted_index_recommender_dump> >(m, js);
    } else {
      throw runtime_error("recommender method \"" + method +
                          "\" is not supported for dump");
    }
  } else if (m.type_ == "anomaly") {
    std::string method, backend_method;
    from_json<std::string>(m.config_["method"].get(), method);
    from_json<std::string>(m.config_["parameter"]["method"].get(),
                           backend_method);
    if (method == "lof") {
      if (backend_method == "inverted_index") {
        dump<
            anomaly<lof<inverted_index> >,
            anomaly_dump<lof<inverted_index>, lof_dump<
                inverted_index, inverted_index_recommender_dump> > >(m, js);
      } else {
        std::cerr << "Warning: backend recommender method \""
                  << backend_method << "\" is not supported for dump"
                  << std::endl;
        dump<
            anomaly<lof<unsupported_data> >,
            anomaly_dump<lof<unsupported_data>, lof_dump<
                unsupported_data, unsupported_data_dump> > >(m, js);
      }
    } else {
      std::cerr << "Warning: anomaly method \""
                << method << "\" is not fully supported for dump"
                << std::endl;
      dump<
          anomaly<unsupported_data>,
          anomaly_dump<unsupported_data, unsupported_data_dump> >(m, js);
    }
  } else if (m.type_ == "nearest_neighbor") {
    dump<nearest_neighbor, nearest_neighbor_dump>(m, js);
  } else {
    throw runtime_error("type \"" + m.type_ +
                        "\" is not supported for dump");
  }

  js.pretty(std::cout, true);
  return 0;
} catch (const std::exception& e) {
  std::cerr << "Error: failed to dump \"" << path
            << "\": " << e.what() << std::endl;
  return -1;
}

}  // namespace dump
}  // namespace jubatus


int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input", 'i', "Input file");
  p.parse_check(argc, argv);

  return jubatus::dump::run(p.get<std::string>("input"));
}
