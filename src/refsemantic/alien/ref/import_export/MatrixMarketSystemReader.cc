/*
 * Copyright 2020 IFPEN-CEA
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
 * SPDX-License-Identifier: Apache-2.0
 */

#include <alien/ref/import_export/MatrixMarketSystemReader.h>
#include <alien/ref/handlers/scalar/DirectMatrixBuilder.h>
#include <alien/ref/handlers/scalar/VectorWriter.h>
#include <string>

#include <alien/ref/data/scalar/Matrix.h>
#include <alien/ref/data/scalar/Vector.h>

#include <cstdio>

namespace Alien
{

class StreamFILEReader
{
 private:
  FILE* m_fdes = nullptr;
  std::string m_line;

 public:
  StreamFILEReader() = delete;
  StreamFILEReader(const StreamFILEReader&) = delete;
  StreamFILEReader& operator=(const StreamFILEReader&) = delete;

  explicit StreamFILEReader(FILE* fdes)
  : m_fdes(fdes)
  {}

  const char* line()
  {
    char* line = nullptr;
    std::size_t size = 0;
    getline(&line, &size, m_fdes);
    // TODO: find a way to directly read in m_line and avoid next copy
    m_line = line;
    std::free(line);

    return m_line.c_str();
  }

  [[nodiscard]] const char* currentLine() const
  {
    return m_line.c_str();
  }
};

template<typename ReaderT>
bool readMMHeaderFromReader(const std::string& mm_type,ReaderT& reader)
{
  // check header
  char param1[32];
  char param2[32];
  char param3[32];
  char param4[32];

  // TODO: use methods that directly read from std::string
  if(sscanf(reader.line(),"%%%%MatrixMarket %31s %31s %31s %31s"
    ,param1
    ,param2
    ,param3
    ,param4)!=4)
  {
    throw FatalErrorException(__PRETTY_FUNCTION__,"Matrix market wrong header");
  }
  if(std::string(param1)!=std::string("matrix"))
  {
    throw FatalErrorException(__PRETTY_FUNCTION__,"Matrix market wrong header 1");
  }
  if(std::string(param2) != mm_type)
  {
    throw FatalErrorException(__PRETTY_FUNCTION__,"Matrix market wrong header 2");
  }
  if(std::string(param3) != std::string("real"))
  {
    throw FatalErrorException(__PRETTY_FUNCTION__,"Matrix market wrong header 3");
  }

  // skip comments
  while(reader.line()[0]=='%');

  return std::string(param4)==std::string("symmetric");
}

template<typename ReaderT>
void loadMMMatrixFromReader(Matrix& A,ReaderT& reader)
{
  bool is_symmetric = readMMHeaderFromReader("coordinate",reader);

  // first non comment line is: n m nnz
  int n,m,nnz;
  if(sscanf(reader.currentLine(),"%d %d %d",&n,&m,&nnz) != 3)
  {
    perror("read mtx size line");
    throw FatalErrorException(__PRETTY_FUNCTION__,"IOError");
  }


  A = Matrix(n, m, n, nullptr);

  DirectMatrixOptions::SymmetricFlag sym_flag = is_symmetric ?
    DirectMatrixOptions::eSymmetric : DirectMatrixOptions::eUnSymmetric;

  DirectMatrixBuilder matrix_builder(A,DirectMatrixOptions::eResetAllocation,sym_flag);

  for(int i=0;i<nnz;++i)
  {
    int li = 0;
    int ci = 0;
    double val = 0;

    if(sscanf(reader.line(),"%d %d %lg\n",&li,&ci,&val) != 3)
    {
      perror("read mtx line");
      throw FatalErrorException(__PRETTY_FUNCTION__,"IOError");
    }
    li--;
    ci--;

    matrix_builder.addData(li,ci,val);
  }
}

template<typename ReaderT>
void loadMMRhsFromReader(Vector& rhs,ReaderT& reader)
{
  readMMHeaderFromReader("array",reader);

  // first non comment line is: n m
  int n,m;
  if(sscanf(reader.currentLine(),"%d %d",&n,&m) != 2)
  {
    perror("read mtx size line");
    throw FatalErrorException(__PRETTY_FUNCTION__,"IOError");
  }

  rhs = Vector(n,n,nullptr);

  VectorWriter vector_writer(rhs);

  for(int i = 0;i < n;++i)
  {
    double val = 0;
    if(sscanf(reader.line(),"%lg\n",&val) != 1)
    {
      perror("read mtx line");
      throw FatalErrorException(__PRETTY_FUNCTION__,"IOError");
    }

    vector_writer[i] = val;
  }
}

MatrixMarketSystemReader::MatrixMarketSystemReader(std::string const& filename)
: m_filename(filename)
{
}

MatrixMarketSystemReader::~MatrixMarketSystemReader() = default;

void MatrixMarketSystemReader::read(Matrix& A)
{
  FILE* fdes = fopen(m_filename.c_str(),"r");

  if(fdes == nullptr)
  {
    perror(m_filename.c_str());
    throw FatalErrorException(__PRETTY_FUNCTION__);
  }

  StreamFILEReader reader(fdes);
  loadMMMatrixFromReader<StreamFILEReader>(A,reader);

  if(fclose(fdes)!=0)
  {
    perror(m_filename.c_str());
    throw FatalErrorException(__PRETTY_FUNCTION__);
  }
}

void MatrixMarketSystemReader::read(Vector& rhs)
{
  FILE* fdes = fopen(m_filename.c_str(),"r");

  if(fdes == nullptr)
  {
    perror(m_filename.c_str());
    throw FatalErrorException(__PRETTY_FUNCTION__);
  }

  StreamFILEReader reader(fdes);
  loadMMRhsFromReader<StreamFILEReader>(rhs,reader);

  if(fclose(fdes)!=0)
  {
    perror(m_filename.c_str());
    throw FatalErrorException(__PRETTY_FUNCTION__);
  }
}

} /* namespace Alien */
