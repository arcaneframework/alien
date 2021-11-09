/*
 * Copyright 2021 IFPEN-CEA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *  SPDX-License-Identifier: Apache-2.0
 */

#include "MatrixMarketReader.h"

#include <fstream>
#include <algorithm>
#include <cctype>

#include <arccore/base/FatalErrorException.h>

namespace Alien::Move
{

namespace
{
  void tolower(std::string& str)
  {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  }

  class MMReader
  {
   public:
    explicit MMReader(std::ifstream& fstream)
    {
      read_banner(fstream);
    }

    bool read_banner(std::istream& fstream)
    {
      std::string line;
      while (std::getline(fstream, line)) {

        if ('%' == line[0] && '%' == line[1]) {

          std::string _; // junk
          std::string format; // (coordinate, array)
          std::string scalar; // (pattern, real, complex, integer)
          std::string symmetry; // (general, symmetric, skew-symmetric, hermitian)

          // get matrix kind
          std::stringstream ss;
          ss << line;
          ss >> _; // skip '%%MatrixMarket
          ss >> _; // skip matrix
          ss >> format;
          ss >> scalar;
          ss >> symmetry;

          tolower(format);
          tolower(scalar);
          tolower(symmetry);

          if ("coordinate" != format) {
            throw Arccore::FatalErrorException("MatrixMarketReader", "format array not supported");
          }

          if ("real" != scalar) {
            throw Arccore::FatalErrorException("MatrixMarketReader", "pattern not supported, only scalar is available");
          }

          if ("general" == symmetry) {
            m_symmetric = false;
          }
          else {
            m_symmetric = true;
          }
        }
        else if ('%' == line[0]) {
          // skip comment
          continue;
        }
        else {
          //first line is matrix size, then done with banner
          std::stringstream ss;
          ss << line;
          ss >> m_rows;
          ss >> m_cols;
          ss >> m_nnz;
          break;
        }
      }
      return true;
    }

    bool read_values(std::istream& fstream, DoKDirectMatrixBuilder& builder) const
    {
      std::string line;
      while (std::getline(fstream, line)) {

        if ('%' == line[0]) {
          continue;
        }

        int row, col;
        double value;
        std::stringstream ss;
        ss << line;
        ss >> row >> col >> value;
        builder.contribute(row, col, value);
        if (m_symmetric) {
          builder.contribute(col, row, value);
        }
      }
      return true;
    }

   private:
    int m_rows{};
    int m_cols{};
    int m_nnz{};
    bool m_symmetric{};
  };
} // namespace

MatrixMarketReader::MatrixMarketReader(MatrixData&& src)
: m_builder(std::move(src))
{
}

MatrixData&& MatrixMarketReader::read(std::string filename)
{
  auto fstream = std::ifstream(filename);
  MMReader reader(fstream);
  reader.read_values(fstream, m_builder);
  fstream.close();
  return m_builder.release();
}
} // namespace Alien::Move
