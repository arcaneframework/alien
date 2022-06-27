#include "simplecsr_to_hypre_vector.h"

void SimpleCSR_to_Hypre_VectorConverter::convert(const Alien::IVectorImpl* sourceImpl,
                                                 Alien::IVectorImpl* targetImpl) const
{
  const auto& v = cast<Alien::SimpleCSRVector<Arccore::Real>>(sourceImpl, sourceBackend());
  auto& v2 = cast<Alien::Vector>(targetImpl, targetBackend());

  alien_debug([&] {
    cout() << "Converting Alien::SimpleCSRVector: " << &v << " to Hypre::Vector " << &v2;
  });

  auto block_size = 1;
  const auto* block = v2.block();
  if (v2.block())
    block_size *= block->size();
  else if (v2.vblock())
    throw Arccore::FatalErrorException(A_FUNCINFO, "Not implemented yet");

  const auto localOffset = v2.distribution().offset();
  const auto localSize = v2.distribution().localSize();
  const auto ilower = localOffset * block_size;
  const auto iupper = ilower + localSize * block_size - 1;

  alien_debug([&] {
    cout() << "Vector range : "
           << "[" << ilower << ":" << iupper << "]";
  });

  auto values = v.values();

  v2.setValues(values);

  v2.assemble();
}
