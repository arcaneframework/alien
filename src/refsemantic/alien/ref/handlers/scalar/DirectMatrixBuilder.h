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

#pragma once

#include <alien/handlers/scalar/BaseDirectMatrixBuilder.h>

#include <alien/ref/data/scalar/Matrix.h>
#include <alien/data/utils/MatrixElement.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

    class DirectMatrixBuilder {
    public:
        using MatrixElement = MatrixElementT<Common::DirectMatrixBuilder>;

        DirectMatrixBuilder(Matrix &matrix, const Common::DirectMatrixBuilder::ResetFlag reset_flag,
                            const Common::DirectMatrixBuilder::SymmetricFlag symmetric_flag = Common::DirectMatrixBuilder::SymmetricFlag::eSymmetric) {
            m_builder = std::make_unique<Alien::Common::DirectMatrixBuilder>(matrix, reset_flag, symmetric_flag);
        }

        MatrixElement operator()(const Integer iIndex, const Integer jIndex) {
            return MatrixElement(iIndex, jIndex, *m_builder.get());
        }

        void reserve(Arccore::Integer n,
                     Common::DirectMatrixBuilder::ReserveFlag flag = Common::DirectMatrixBuilder::ReserveFlag::eResetReservation) {
            m_builder->reserve(n, flag);
        }

        void reserve(Arccore::ConstArrayView<Arccore::Integer> indices, Arccore::Integer n,
                     Common::DirectMatrixBuilder::ReserveFlag flag = Common::DirectMatrixBuilder::ReserveFlag::eResetReservation) {
            m_builder->reserve(indices, n, flag);
        }

        void allocate() {
            m_builder->allocate();
        }

        void addData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value) {
            m_builder->addData(iIndex, jIndex, value);
        }

        void addData(Arccore::Integer iIndex, Arccore::Real factor,
                     Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                     Arccore::ConstArrayView<Arccore::Real> jValues) {
            m_builder->addData(iIndex, factor, jIndexes, jValues);
        }

        void setData(Arccore::Integer iIndex, Arccore::Integer jIndex, Arccore::Real value) {
            m_builder->setData(iIndex, jIndex, value);
        }

        void setData(Arccore::Integer iIndex, Arccore::Real factor,
                     Arccore::ConstArrayView<Arccore::Integer> jIndexes,
                     Arccore::ConstArrayView<Arccore::Real> jValues) {
            m_builder->setData(iIndex, factor, jIndexes, jValues);
        }

        void finalize() {
            m_builder->finalize();
        }

        void squeeze() {
            m_builder->squeeze();
        }

        [[nodiscard]] Arccore::String stats() const {
            return m_builder->stats();
        }

        [[nodiscard]] Arccore::String stats(Arccore::IntegerConstArrayView ids) const {
            return m_builder->stats(ids);
        }

    private:
        std::unique_ptr<Alien::Common::DirectMatrixBuilder> m_builder;
    };

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
