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

#include <msgpack.hpp>
#include <jubatus/core/common/big_endian.hpp>
#include <jubatus/util/data/serialization/unordered_map.h>
#include <jubatus/util/text/json.h>

#include "third_party/cmdline/cmdline.h"
#include "jubatus/dump/recommender.hpp"
#include "jubatus/dump/classifier.hpp"

using jubatus::core::common::read_big_endian;

namespace jubatus {
namespace dump {

template <typename T, typename D>
bool read_and_dump(std::ifstream& ifs, jubatus::util::text::json::json& js) {
  // TODO(unno): This implementation ignores checksums, version, and all other
  // check codes. We need to re-implemenet such process like
  // jubatus::server::framework::save_server/load_server, or to use these
  // methods to show file format errors.

  ifs.exceptions(std::ifstream::failbit);
  uint64_t user_data_size;
  std::vector<char> user_data_buf;
  try {
    char header_buf[48];
    ifs.read(header_buf, 48);
    uint64_t system_data_size = read_big_endian<uint64_t>(&header_buf[32]);
    user_data_size = read_big_endian<uint64_t>(&header_buf[40]);

    ifs.ignore(system_data_size);
    user_data_buf.resize(user_data_size);
    ifs.read(&user_data_buf[0], user_data_size);
  } catch(std::ios_base::failure& e) {
    std::cerr << "Input stream reached end of file." << std::endl;
    return false;
  }
  if (ifs.peek() != -1) {
    std::ifstream::pos_type pos = ifs.tellg();
    std::cerr << "Input stream remains. Position: " << pos << std::endl;
    return false;
  }
  ifs.close();
  
  msgpack::unpacked msg;
  msgpack::unpack(&msg, user_data_buf.data(), user_data_buf.size());
  msgpack::object obj = msg.get();

  T data;
  // obj[0] is the version of the saved model file
  obj.via.array.ptr[1].convert(&data);

  D dump(data);
  js = jubatus::util::text::json::to_json(dump);
  return true;
}

int run(const std::string& path, const std::string& type) try {
  std::ifstream ifs(path.c_str());
  if (!ifs) {
    std::cerr << "Cannot open: " << path << std::endl;
    return -1;
  }

  jubatus::util::text::json::json js;
  bool result;
  if (type == "classifier") {
    result = read_and_dump<classifier<local_storage>,
        classifier_dump<local_storage, local_storage_dump> >(ifs, js);
  } else if (type == "inverted_index") {
    result = read_and_dump<
      recommender<inverted_index>,
      recommender_dump<inverted_index_recommender_dump> >(
            ifs, js);
  } else {
    std::cerr << "Unsupported file format: " << type << std::endl;
    return -1;
  }

  if (!result) {
    return -1;
  }
  js.pretty(std::cout, true);
  return 0;
} catch (msgpack::type_error& e) {
  std::cerr << "Cannot read the file \"" << path
            << "\" as \"" << type << "\"" << std::endl;
  return -1;
}

}  // namespace dump
}  // namespace jubatus


int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input", 'i', "Input file");
  p.add<std::string>("type", 't', "Format type", false, "classifier",
                     cmdline::oneof<std::string>("classifier",
                                                 "inverted_index"));
  p.parse_check(argc, argv);

  return jubatus::dump::run(p.get<std::string>("input"),
                            p.get<std::string>("type"));
}
