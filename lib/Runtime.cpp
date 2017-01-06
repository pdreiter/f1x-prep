/*
  This file is part of f1x.
  Copyright (C) 2016  Sergey Mechtaev, Gao Xiang, Abhik Roychoudhury

  f1x is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <sstream>
#include <cstdlib>

#include <boost/filesystem/fstream.hpp>

#include <boost/log/trivial.hpp>

#include "RepairUtil.h"
#include "Runtime.h"

namespace fs = boost::filesystem;
using std::vector;
using std::unordered_set;

const std::string RUNTIME_SOURCE_FILE_NAME = "rt.cpp";
const std::string RUNTIME_HEADER_FILE_NAME = "rt.h";


Runtime::Runtime(const fs::path &workDir, const Config &cfg): 
  workDir(workDir),
  cfg(cfg) {};

void Runtime::cleanPartition(uint locId) {
  fs::path partitionFile = workDir / ("partition" + std::to_string(locId) + ".txt");
  fs::ofstream out;
  out.open(partitionFile, std::ofstream::out | std::ofstream::trunc);
  out.close();
}

unordered_set<uint> Runtime::getPartition(uint locId) {
  unordered_set<uint> result;
  unordered_set<uint> aux;
  fs::path partitionFile = workDir / ("partition" + std::to_string(locId) + ".txt");
  if (!fs::exists(partitionFile)) {
    return result;
  }
  fs::ifstream in(partitionFile);
  std::string line;
  bool firstLine = true;
  while (getline(in, line)) {
    std::swap(result, aux);
    result.clear();
    std::istringstream iss(line);
    uint id;
    if (firstLine) {
      firstLine = false;
      while (iss >> id) {
        result.insert(id);
      }
    } else {
      while (iss >> id) {
        if (aux.count(id))
          result.insert(id);
      }
    }
  }
  return result;
}

fs::path Runtime::getWorkDir() {
  return workDir;
}

boost::filesystem::path Runtime::getHeader() {
  return workDir / RUNTIME_HEADER_FILE_NAME;
}

boost::filesystem::path Runtime::getSource() {
  return workDir / RUNTIME_SOURCE_FILE_NAME;
}

bool Runtime::compile() {
  BOOST_LOG_TRIVIAL(info) << "compiling meta program runtime";
  FromDirectory dir(workDir);
  std::stringstream cmd;
  cmd << cfg.runtimeCompiler << " -O2 -fPIC"
      << " " << RUNTIME_SOURCE_FILE_NAME 
      << " -shared"
      << " -o libf1xrt.so";
  if (cfg.verbose) {
    cmd << " >&2";
  } else {
    cmd << " >/dev/null 2>&1";
  }
  BOOST_LOG_TRIVIAL(debug) << "cmd: " << cmd.str();
  uint status = std::system(cmd.str().c_str());
  return status == 0;
}
