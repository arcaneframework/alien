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

#include <fstream>
#include <algorithm>
#include <cctype>

#include <arccore/base/FatalErrorException.h>
#include <arccore/message_passing/IMessagePassingMng.h>

#include <alien/data/Space.h>
#include <alien/distribution/MatrixDistribution.h>

#include <alien/move/data/MatrixData.h>
#include <alien/move/handlers/scalar/DoKDirectMatrixBuilder.h>

namespace Alien::Move
{

namespace
{
  void tolower(std::string& str)
  {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  }

  class MatrixDescription
  {
   public:
    MatrixDescription() = default;

    explicit MatrixDescription(Arccore::Span<Arccore::Integer> src)
    {
      if (src.size() != 4) {
        throw Arccore::FatalErrorException("Matrix Descriptor", "Cannot deserialize array");
      }
      n_rows = src[0];
      n_cols = src[1];
      n_nnz = src[2];
      symmetric = (src[3] == 0);
    }

    Arccore::UniqueArray<Arccore::Integer> to_array()
    {
      Arccore::UniqueArray<Arccore::Integer> array(4);
      array[0] = n_rows;
      array[1] = n_cols;
      array[2] = n_nnz;
      array[3] = (symmetric) ? 0 : 1;
      return array;
    }

    int n_rows{ 0 };
    int n_cols{ 0 };
    int n_nnz{ 0 };
    bool symmetric{ true };
  };

  std::optional<MatrixDescription> readBanner(std::istream& fstream)
  {
    std::string line;

    MatrixDescription out;

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
          return std::nullopt;
          // throw Arccore::FatalErrorException("MatrixMarketReader", "format array not supported");
        }

        if ("real" != scalar) {
          return std::nullopt;
          // throw Arccore::FatalErrorException("MatrixMarketReader", "pattern not supported, only scalar is available");
        }

        if ("general" == symmetry) {
          out.symmetric = false;
        }
        else {
          out.symmetric = true;
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

        ss >> out.n_rows;
        ss >> out.n_cols;
        ss >> out.n_nnz;
        break;
      }
    }
    return std::make_optional(out);
  }

  bool readValues(std::istream& fstream, DoKDirectMatrixBuilder& builder, bool symmetric)
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
      if (symmetric) {
        builder.contribute(col, row, value);
      }
    }
    return true;
  }

  MatrixData createMatrixData(MatrixDescription desc, Arccore::MessagePassing::IMessagePassingMng* pm)
  {
    Alien::Space row_space(desc.n_rows, "RowSpace");
    Alien::Space col_space(desc.n_cols, "ColSpace");
    Alien::MatrixDistribution dist(
    row_space, col_space, pm);
    return Alien::Move::MatrixData(dist);
  }

} // namespace

MatrixData ALIEN_MOVESEMANTIC_EXPORT
readFromMatrixMarket(Arccore::MessagePassing::IMessagePassingMng* pm, const std::string& filename)
{
  if (pm->commRank() == 0) { // Only rank 0 read the file
    auto stream = std::ifstream(filename);
    if (!stream) {
      throw Arccore::FatalErrorException("readFromMatrixMarket", "Unable to read file");
    }
    auto desc = readBanner(stream);
    if (!desc) {
      throw Arccore::FatalErrorException("readFromMatrixMarket", "Invalid header");
    }
    auto ser_desc = desc.value().to_array();
    Arccore::MessagePassing::mpBroadcast(pm, ser_desc, 0);
    DoKDirectMatrixBuilder builder(createMatrixData(desc.value(), pm));
    readValues(stream, builder, desc->symmetric);
    return builder.release();
  }
  else {
    // Receive description description from rank 0
    Arccore::UniqueArray<Arccore::Integer> ser_desc(4);
    Arccore::MessagePassing::mpBroadcast(pm, ser_desc, 0);
    MatrixDescription desc(ser_desc);
    DoKDirectMatrixBuilder builder(createMatrixData(desc, pm));
    return builder.release();
  }
}

} // namespace Alien::Move
