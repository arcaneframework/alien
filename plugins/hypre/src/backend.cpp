#include "../include/alien/hypre/backend.h"

#include "converters/simplecsr_to_hypre_matrix.h"
#include "converters/simplecsr_to_hypre_vector.h"
#include "converters/hypre_to_simplecsr_vector.h"

#include "hypre_matrix.h"
#include "hypre_vector.h"

namespace Alien
{

void Plugin::registerMatrixConverters(std::map<std::pair<BackEndId, BackEndId>, IMatrixConverter*>& converters)
{
  REGISTER_PLUGIN_MATRIX_CONVERTER("hypre", "simplecsr", SimpleCSR_to_Hypre_MatrixConverter);
}

void Plugin::registerVectorConverters(std::map<std::pair<BackEndId, BackEndId>, IVectorConverter*>& converters)
{
  REGISTER_PLUGIN_VECTOR_CONVERTER("hypre", "simplecsr", SimpleCSR_to_Hypre_VectorConverter);
  REGISTER_PLUGIN_VECTOR_CONVERTER("simplecsr", "hypre", Hypre_to_SimpleCSR_VectorConverter);
}

void Plugin::registerMatrixFactory(std::map<BackEndId, MatrixFactory>& matrixFactories)
{
  REGISTER_PLUGIN_MATRIX_FACTORY("hypre", matrixFactories, &matrix_factory);
}

void Plugin::registerVectorFactory(std::map<BackEndId, VectorFactory>& vectorFactories)
{
  REGISTER_PLUGIN_VECTOR_FACTORY("hypre", vectorFactories, &vector_factory);
}

}
