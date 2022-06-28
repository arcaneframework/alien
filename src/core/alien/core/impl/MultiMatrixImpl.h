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

/*!
 * \file MultiMatrixImpl.h
 * \brief MultiMatrixImpl.h
 */

#pragma once

#include <map>

#include <alien/core/backend/BackEnd.h>
#include <alien/core/backend/MatrixConverterRegisterer.h>
#include <alien/core/block/Block.h>
#include <alien/core/block/VBlock.h>
#include <alien/core/impl/IMatrixImpl.h>
#include <alien/data/Space.h>
#include <alien/distribution/MatrixDistribution.h>
#include <alien/utils/ObjectWithLock.h>
#include <alien/utils/Precomp.h>
#include <alien/utils/UserFeatureMng.h>
#include <alien/utils/time_stamp/TimestampMng.h>
#include <alien/kernels/simple_csr/SimpleCSRMatrix.h>
#include <cstdlib>
#include <iostream>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*!
 * \ingroup impl
 * \brief Multi matrices representation container
 *
 * This class allows to store and access several implementations of the same matrix.
 * It also stores all shared information between those implementations such as
 * distribution, or block information. It provides two accessors, one to access the matrix
 * in read-only mode, that does not increase the timestamp and one in read-write mode, that can
 * increase the timestamp. The former implementation might therefore become the up to date
 * matrix.
 *
 * While requesting a matrix in a specific format, if the up to date matrix is not in that
 * specific format, the up-to-date matrix will be converted, provided a converter exists.
 */
class ALIEN_EXPORT MultiMatrixImpl : public TimestampMng
, public UserFeatureMng
, public ObjectWithTrace
, public ObjectWithLock
{
 public:
  //! Default constructor
  MultiMatrixImpl();

  /*!
   * \brief Constructor
   * \param[in] row_space The row space of the matrix
   * \param[in] col_space The column space of the matrix
   * \param[in] dist The distribution of the matrix
   */
  MultiMatrixImpl(std::shared_ptr<ISpace> row_space, std::shared_ptr<ISpace> col_space,
                  std::shared_ptr<MatrixDistribution> dist);

  //! Free resources
  virtual ~MultiMatrixImpl();

 protected:
  /*!
   * \brief Copy constructor
   * \param[in] impl The MultiMatrixImpl to copy
   */
  MultiMatrixImpl(const MultiMatrixImpl& impl);

 public:
  /*!
   * \brief Set uniform block information
   * \param[in] block_size The size of the blocks
   */
  void setBlockInfos(Arccore::Integer block_size);

  /*!
   * \brief Set uniform block information
   * \param[in] blocks The block data
   */
  void setBlockInfos(const Block* blocks);

  /*!
   * \brief Set variable block information
   * \param[in] blocks The block data
   */
  void setBlockInfos(const VBlock* blocks);

  /*!
   * \brief Set variable row block information
   * \param[in] blocks The row block data
   */
  void setRowBlockInfos(const VBlock* blocks);

  /*!
   * \brief Set variable col block information
   * \param[in] blocks The col block data
   */
  void setColBlockInfos(const VBlock* blocks);

  //! Free resources
  void free();
  //! Clear resources
  void clear();

  /*!
   * \brief Get the row space associated to the matrix
   * \returns The row space
   */
  const ISpace& rowSpace() const { return *m_row_space.get(); }

  /*!
   * \brief Get the col space associated to the matrix
   * \returns The col space
   */
  const ISpace& colSpace() const { return *m_col_space.get(); }

  /*!
   * \brief Get the matrix distribution
   * \returns The matrix distribution
   */
  const MatrixDistribution& distribution() const { return *m_distribution.get(); }

  /*!
   * \brief Get uniform block datas
   * \returns The matrix block data, or a nullptr if the matrix is scalar or has variable
   * blocks
   */
  const Block* block() const;

  /*!
   * \brief Get variable block datas
   * \returns The matrix block data, or a nullptr if the matrix is scalar or has uniform
   * blocks
   */
  const VBlock* vblock() const;

  /*!
   * \brief Get variable row block datas
   * \returns The matrix row block data, or a nullptr if the matrix is scalar or has
   * uniform blocks
   */
  const VBlock* rowBlock() const;

  /*!
   * \brief Get variable col block datas
   * \returns The matrix col block data, or a nullptr if the matrix is scalar or has
   * uniform blocks
   */
  const VBlock* colBlock() const;

 public:
  /*!
   * \brief Get a specific matrix implementation
   *
   * Might induce a conversion, depending on the up to date and requested  matrix
   * implementation
   *
   * \returns The up to date matrix in the requested implementation
   */
  const IMatrixImpl& get(BackEndId) const;

  /*!
   * \brief Get a specific matrix implementation
   *
   * Might induce a conversion, depending on the up to date and requested  matrix
   * implementation
   *
   * \param[in] update_stamp Whether or not the timestamp should be increased or not
   * \returns The up to date matrix in the requested implementation
   */
  IMatrixImpl& get(BackEndId, const bool update_stamp);

  //! Release a matrix implementation
  void release(BackEndId) const;

  /*!
   * \brief Clone this object
   * \return A clone of this object
   */
  MultiMatrixImpl* clone() const;

  // TOCHECK : should be removed or not ?
  //  void setTimestamp(Int64 stamp)
  //  {
  //    TimestampMng::setTimestamp(stamp);
  //  }

 private:
  /*!
   * \brief Get a specific matrix implementation
   * \param[in] backend The id of the specific implementation
   * \returns The matrix in the requested format
   */
  IMatrixImpl*& getImpl(BackEndId backend) const;

  /*!
   * \brief Update a matrix implementation
   * \param[in] target The targeted implementation
   */
  void updateImpl(IMatrixImpl* target) const;

 protected:
  /*!
   * \brief Insert a matrix implementation in the multi matrix container
   * \param[in] backend The implementation backend id
   * \param[in] m The matrix to insert
   */
  void insert(BackEndId backend, IMatrixImpl* m);

 private:
  //! The matrix row space
  std::shared_ptr<ISpace> m_row_space;
  //! The matrix column space
  std::shared_ptr<ISpace> m_col_space;
  //! The matrix distribution
  std::shared_ptr<MatrixDistribution> m_distribution;
  //! The type of the matrix container
  typedef std::map<BackEndId, IMatrixImpl*> MultiMatrixImplMap;
  //! The matrices container
  mutable MultiMatrixImplMap m_impls2;
  //! The uniform block datas
  std::shared_ptr<Block> m_block;
  //! The variable row block datas
  std::shared_ptr<VBlock> m_rows_block;
  //! The variable col block datas
  std::shared_ptr<VBlock> m_cols_block;




  class MatrixConverters : public std::map<std::pair<BackEndId, BackEndId>, IMatrixConverter*>
  {
    public:
      MatrixConverters();
  };

  class MatrixFactories : public std::map<BackEndId, BackEnd::MatrixFactory>
  {
    public:
      MatrixFactories();
  };

// FIXME: private, access through functions
 public:
  static MatrixConverters m_matrixConverters;
  static MatrixFactories m_matrixFactories;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
