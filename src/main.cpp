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

#include <iostream>
#include <string>

#include <pficommon/data/serialization/unordered_map.h>
#include <pficommon/text/json.h>

#include "third_party/cmdline/cmdline.h"
#include "jubatus/dump/recommender.hpp"
#include "jubatus/dump/classifier.hpp"

namespace jubatus {
namespace dump {

template <typename T, typename D>
bool read_and_dump(std::ifstream& ifs, pfi::text::json::json& js) {
  pfi::data::serialization::binary_iarchive ia(ifs);
  T data;
  ia >> data;
  if (!ia) {
    std::cerr << "Input stream reached end of file." << std::endl;
    return false;
  }
  std::ifstream::pos_type pos = ifs.tellg();
  if (ifs.get() != -1) {
    std::cerr << "Input stream remains. Position: " << pos << std::endl;
    return false;
  }

  D dump(data);
  js = pfi::text::json::to_json(dump);
  return true;
}

int run(const std::string& path, const std::string& type) {
  std::ifstream ifs(path.c_str());
  if (!ifs) {
    std::cerr << "Cannot open: " << path << std::endl;
    return -1;
  }

  pfi::text::json::json js;
  bool result;
  if (type == "classifier") {
    result = read_and_dump<classifier<local_storage>,
        classifier_dump<local_storage, local_storage_dump> >(ifs, js);
  } else if (type == "inverted_index") {
    result = read_and_dump<recommender<inverted_index_storage>,
        recommender_dump<inverted_index_storage, inverted_index_dump> >(
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
