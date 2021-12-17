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

#include <ginkgo/ginkgo.hpp>
#include <iomanip>

// Utility function which returns the first element (position [0, 0]) from a
// given gko::matrix::Dense matrix / vector.
template <typename ValueType>
ValueType get_first_element(const gko::matrix::Dense<ValueType>* mtx)
{
  // Copy the matrix / vector to the host device before accessing the value in
  // case it is stored in a GPU.
  return mtx->get_executor()->copy_val_to_host(mtx->get_const_values());
}

// Utility function which computes the norm of a Ginkgo gko::matrix::Dense
// vector.
template <typename ValueType>
gko::remove_complex<ValueType> compute_norm(
const gko::matrix::Dense<ValueType>* b)
{
  // Get the executor of the vector
  auto exec = b->get_executor();
  // Initialize a result scalar containing the value 0.0.
  auto b_norm =
  gko::initialize<gko::matrix::Dense<gko::remove_complex<ValueType>>>(
  { 0.0 }, exec);
  // Use the dense `compute_norm2` function to compute the norm.
  b->compute_norm2(gko::lend(b_norm));
  // Use the other utility function to return the norm contained in `b_norm`
  return get_first_element(gko::lend(b_norm));
}

// Custom logger class which intercepts the residual norm scalar and solution
// vector in order to print a table of real vs recurrent (internal to the
// solvers) residual norms.
template <typename ValueType>
struct ResidualLogger : gko::log::Logger
{
  using RealValueType = gko::remove_complex<ValueType>;
  // Output the logger's data in a table format
  void write() const
  {
    // Print a header for the table
    std::cout << "Recurrent vs true vs implicit residual norm:"
              << std::endl;
    std::cout << '|' << std::setw(10) << "Iteration" << '|' << std::setw(25)
              << "Recurrent Residual Norm" << '|' << std::setw(25)
              << "True Residual Norm" << '|' << std::setw(25)
              << "Implicit Residual Norm" << '|' << std::endl;
    // Print a separation line. Note that for creating `10` characters
    // `std::setw()` should be set to `11`.
    std::cout << '|' << std::setfill('-') << std::setw(11) << '|'
              << std::setw(26) << '|' << std::setw(26) << '|'
              << std::setw(26) << '|' << std::setfill(' ') << std::endl;
    // Print the data one by one in the form
    std::cout << std::scientific;
    for (std::size_t i = 0; i < iterations.size(); i++) {
      std::cout << '|' << std::setw(10) << iterations[i] << '|'
                << std::setw(25) << recurrent_norms[i] << '|'
                << std::setw(25) << real_norms[i] << '|' << std::setw(25)
                << implicit_norms[i] << '|' << std::endl;
    }
    // std::defaultfloat could be used here but some compilers
    // do not support it properly, e.g. the Intel compiler
    std::cout.unsetf(std::ios_base::floatfield);
    // Print a separation line
    std::cout << '|' << std::setfill('-') << std::setw(11) << '|'
              << std::setw(26) << '|' << std::setw(26) << '|'
              << std::setw(26) << '|' << std::setfill(' ') << std::endl;
  }

  using gko_dense = gko::matrix::Dense<ValueType>;
  using gko_real_dense = gko::matrix::Dense<RealValueType>;

  // This overload is necessary to avoid interface breaks for Ginkgo 2.0
  void on_iteration_complete(const gko::LinOp* solver,
                             const gko::size_type& iteration,
                             const gko::LinOp* residual,
                             const gko::LinOp* solution,
                             const gko::LinOp* residual_norm) const override
  {
    this->on_iteration_complete(solver, iteration, residual, solution,
                                residual_norm, nullptr);
  }

  // Customize the logging hook which is called everytime an iteration is
  // completed
  void on_iteration_complete(
  const gko::LinOp*, const gko::size_type& iteration,
  const gko::LinOp* residual, const gko::LinOp* solution,
  const gko::LinOp* residual_norm,
  const gko::LinOp* implicit_sq_residual_norm) const override
  {
    // If the solver shares a residual norm, log its value
    if (residual_norm) {
      auto dense_norm = gko::as<gko_real_dense>(residual_norm);
      // Add the norm to the `recurrent_norms` vector
      recurrent_norms.push_back(get_first_element(gko::lend(dense_norm)));
      // Otherwise, use the recurrent residual vector
    }
    else {
      auto dense_residual = gko::as<gko_dense>(residual);
      // Compute the residual vector's norm
      auto norm = compute_norm(gko::lend(dense_residual));
      // Add the computed norm to the `recurrent_norms` vector
      recurrent_norms.push_back(norm);
    }

    // If the solver shares the current solution vector
    if (solution) {
      // Store the matrix's executor
      auto exec = matrix->get_executor();
      // Create a scalar containing the value 1.0
      auto one = gko::initialize<gko_dense>({ 1.0 }, exec);
      // Create a scalar containing the value -1.0
      auto neg_one = gko::initialize<gko_dense>({ -1.0 }, exec);
      // Instantiate a temporary result variable
      auto res = gko::clone(b);
      // Compute the real residual vector by calling apply on the system
      // matrix
      matrix->apply(gko::lend(one), gko::lend(solution),
                    gko::lend(neg_one), gko::lend(res));

      // Compute the norm of the residual vector and add it to the
      // `real_norms` vector
      real_norms.push_back(compute_norm(gko::lend(res)));
    }
    else {
      // Add to the `real_norms` vector the value -1.0 if it could not be
      // computed
      real_norms.push_back(-1.0);
    }

    if (implicit_sq_residual_norm) {
      auto dense_norm =
      gko::as<gko_real_dense>(implicit_sq_residual_norm);
      // Add the norm to the `implicit_norms` vector
      implicit_norms.push_back(
      std::sqrt(get_first_element(gko::lend(dense_norm))));
    }
    else {
      // Add to the `implicit_norms` vector the value -1.0 if it could not
      // be computed
      implicit_norms.push_back(-1.0);
    }

    // Add the current iteration number to the `iterations` vector
    iterations.push_back(iteration);
  }

  // Construct the logger and store the system matrix and b vectors
  ResidualLogger(std::shared_ptr<const gko::Executor> exec,
                 const gko::LinOp* matrix, const gko_dense* b)
  : gko::log::Logger(exec, gko::log::Logger::iteration_complete_mask)
  , matrix{ matrix }
  , b{ b }
  {}

 private:
  // Pointer to the system matrix
  const gko::LinOp* matrix;
  // Pointer to the right hand sides
  const gko_dense* b;
  // Vector which stores all the recurrent residual norms
  mutable std::vector<RealValueType> recurrent_norms{};
  // Vector which stores all the real residual norms
  mutable std::vector<RealValueType> real_norms{};
  // Vector which stores all the implicit residual norms
  mutable std::vector<RealValueType> implicit_norms{};
  // Vector which stores all the iteration numbers
  mutable std::vector<std::size_t> iterations{};
};