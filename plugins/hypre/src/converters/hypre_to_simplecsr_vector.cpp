#include "hypre_to_simplecsr_vector.h"

void Hypre_to_SimpleCSR_VectorConverter::convert(
const Alien::IVectorImpl* sourceImpl, Alien::IVectorImpl* targetImpl) const
{
  const auto& v = cast<Alien::Vector>(sourceImpl, sourceBackend());
  auto& v2 = cast<Alien::SimpleCSRVector<Arccore::Real>>(targetImpl, targetBackend());

  alien_debug([&] {
    cout() << "Converting Hypre::Vector: " << &v << " to Alien::SimpleCSRVector " << &v2;
  });
  auto values = v2.values();

  v.getValues(values);
}
