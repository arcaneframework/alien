﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2022 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* StringView.cc                                               (C) 2000-2021 */
/*                                                                           */
/* Vue sur une chaîne de caractères UTF-8.                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arccore/base/StringView.h"

#include <iostream>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arccore
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

void StringView::
writeBytes(std::ostream& o) const
{
  o.write((const char*)m_v.data(),m_v.size());
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

std::ostream&
operator<<(std::ostream& o,const StringView& str)
{
  str.writeBytes(o);
  return o;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool
operator==(StringView a,StringView b)
{
  bool is_equal = (a.toStdStringView()==b.toStdStringView());
  //std::cout << "COMPARE: a=" << a.length() << " '" << a << "'"
  //          << " b=" << b.length() << " '" << b << "' v=" << is_equal << '\n';
  return is_equal;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool operator==(const char* a,StringView b)
{
  return operator==(StringView(a),b);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool operator==(StringView a,const char* b)
{
  return operator==(a,StringView(b));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

bool operator<(StringView a,StringView b)
{
  return a.toStdStringView()<b.toStdStringView();
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arccore

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
