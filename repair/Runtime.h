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

#pragma once

#include <unordered_set>
#include <string>
#include <sstream>

#include <boost/filesystem.hpp>

#include "Config.h"
#include "RepairUtil.h"

const unsigned long MAX_PARTITION_SIZE = 1000000;
const std::string PARTITION_FILE_NAME = "/f1x_partition";
const F1XID INPUT_TERMINATOR = F1XID{0, 0, 0, 0, 0};
const F1XID OUTPUT_TERMINATOR = F1XID{0, 0, 0, 0, 1};


const std::string RUNTIME_SOURCE_FILE_NAME = "rt.cpp";
const std::string RUNTIME_HEADER_FILE_NAME = "rt.h";


class Runtime {
 public:
  Runtime(const boost::filesystem::path &workDir, const Config &cfg);

  void setPartition(std::unordered_set<F1XID> ids);
  std::unordered_set<F1XID> getPartition();
  boost::filesystem::path getSource();
  boost::filesystem::path getHeader();
  bool compile();

 private:
  boost::filesystem::path workDir;
  Config cfg;
  F1XID *partition;
};