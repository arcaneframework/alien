/*
 * Copyright 2022 IFPEN-CEA
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

#include <gtest/gtest.h>

#include <algorithm>

#include <Environment.h>

#include <alien/index_manager/functional/DefaultAbstractFamily.h>
#include <alien/index_manager/functional/AbstractItemFamily.h>
#include <alien/index_manager/IAbstractFamily.h>
#include <random>

namespace
{
template <typename Family>
struct FamilyChecker
{
 public:
  Arccore::MessagePassing::IMessagePassingMng* pm;
  Alien::UniqueArray<Arccore::Int64> uid;
  Alien::UniqueArray<Arccore::Int32> lid;
  Alien::UniqueArray<Arccore::Integer> owners;
  Arccore::Int64 problem_size = 100;
  std::unique_ptr<Family> family = nullptr;

  explicit FamilyChecker(Arccore::MessagePassing::IMessagePassingMng* pm, bool shuffle = false)
  : pm(pm)
  {
    auto rank = pm->commRank();
    auto procs = pm->commSize();

    auto local_problem = problem_size / procs;

    auto start = local_problem * rank;

    if (rank == procs - 1) { // last processor
      local_problem = problem_size - start;
    }

    // Add local data
    for (auto i = 0; i < local_problem / 2; i++) {
      uid.add(i + start);
      lid.add(i);
      owners.add(rank);
    }

    for (auto i = 0; i < procs; i++) {
      if (i != rank) {
        auto offset = local_problem * i;
        uid.add(offset);
        lid.add(i + local_problem / 2);
        owners.add(i);
      }
    } // Add some ghosts

    // Resume local data
    for (auto i = local_problem / 2; i < local_problem; i++) {
      uid.add(i + start);
      lid.add(i + procs - 1);
      owners.add(rank);
    }

    if (shuffle) {
      std::vector<size_t> perm(local_problem);
      auto i = 0;
      for (auto& p : perm) {
        p = i++;
      }

      auto seed = 42;
      std::shuffle(perm.begin(), perm.end(), std::default_random_engine(seed));
      auto rand_func = [&perm](int id) { return perm[id]; };

      std::random_shuffle(uid.begin(), uid.end(), rand_func);
      std::random_shuffle(lid.begin(), lid.end(), rand_func);
      std::random_shuffle(owners.begin(), owners.end(), rand_func);
    }
    family = std::make_unique<Family>(uid, owners, pm);
  }
};

} // namespace

TEST(TestFamilies, DefaultAbstractFamily)
{
  auto* pm = AlienTest::Environment::parallelMng();
  auto test_case = FamilyChecker<Alien::DefaultAbstractFamily>(pm);

  auto local_size = test_case.lid.size();
  auto sub_lid = test_case.lid.subView(0, local_size);
  auto test_owners = test_case.family->owners(sub_lid);
  auto test_uids = test_case.family->uids(sub_lid);
  Arccore::UniqueArray<Arccore::Int32> test_lid(local_size, 0);
  test_case.family->uniqueIdToLocalId(test_lid, test_case.uid.subView(0, local_size));

  for (auto i = 0; i < sub_lid.size(); i++) {
    ASSERT_EQ(test_owners[i], test_case.owners[i]);
    ASSERT_EQ(test_uids[i], test_case.uid[i]);
    ASSERT_EQ(test_owners[i], test_case.owners[i]);
    ASSERT_EQ(test_lid[i], test_case.lid[i]);
  }
}

TEST(TestFamilies, AbstractItemFamily)
{
  auto* pm = AlienTest::Environment::parallelMng();
  auto test_case = FamilyChecker<Alien::AbstractItemFamily>(pm);

  auto local_size = test_case.lid.size();
  auto sub_lid = test_case.lid.subView(0, local_size);
  auto test_owners = test_case.family->owners(sub_lid);
  auto test_uids = test_case.family->uids(sub_lid);
  Arccore::UniqueArray<Arccore::Int32> test_lid(local_size, 0);
  test_case.family->uniqueIdToLocalId(test_lid, test_case.uid.subView(0, local_size));

  for (auto i = 0; i < sub_lid.size(); i++) {
    ASSERT_EQ(test_owners[i], test_case.owners[i]);
    ASSERT_EQ(test_uids[i], test_case.uid[i]);
    ASSERT_EQ(test_owners[i], test_case.owners[i]);
    ASSERT_EQ(test_lid[i], test_case.lid[i]);
  }
}
