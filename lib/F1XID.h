/*
  This file is part of f1x.
  Copyright (C) 2016  Sergey Mechtaev, Abhik Roychoudhury

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

#include <assert.h>
#include "Config.h"

const uint F1XID_WIDTH = 32;
const uint F1XID_VALUE_BITS = 10;

// FIXME: single 32 value will not be enough to encode 10^9 candidates with sharing. Better to use several ids

/*
  __f1x_id is a F1XID_WIDTH bit transparent candidate ID. The left F1XID_VALUE_BITS bits of this id is the parameter value.
 */
uint f1xid(uint baseId, uint parameter) {
  assert(baseId < (1 << (F1XID_WIDTH - F1XID_VALUE_BITS)));
  uint result = parameter;
  result <<= (F1XID_WIDTH - F1XID_VALUE_BITS);
  result += baseId;
  return result;
}

/*
  __f1x_loc is a F1XID_WIDTH bit transparent location ID. The left F1XID_VALUE_BITS bits of this id is the file ID.
 */

uint f1xloc(uint baseId, uint fileId) {
  assert(baseId < (1 << (F1XID_WIDTH - F1XID_VALUE_BITS)));
  uint result = fileId;
  result <<= (F1XID_WIDTH - F1XID_VALUE_BITS);
  result += baseId;
  return result;
}

