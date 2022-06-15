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

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef ALIEN_USE_LIBXML2
#include <libxml/parser.h>
#endif

#include "arccore/base/String.h"

#include <alien/utils/Precomp.h>

namespace Alien
{
#ifdef ALIEN_USE_LIBXML2
namespace XML2Tools {

  class XMLDocument
  {
  public :
    XMLDocument() = default ;
    ~XMLDocument()
    {
      if(m_doc!=nullptr)
        xmlFreeDoc(m_doc);
    }

    xmlDocPtr m_doc = nullptr ;
    bool isNull() const {
      return m_doc == nullptr ;
    }
  };

  struct FileNode
  {
    FileNode(int _level = 0)
    : name("")
    , path_name("")
    , level(_level)
    {}

    bool isNull() const {
      return x_id == nullptr ;
    }

    std::string name;
    std::string path_name;
    int level = 0 ;
    xmlNodePtr x_id = nullptr ;
  };


  XMLDocument openDocument(const std::string& filename)
  {
    XMLDocument document ;
    document.m_doc = xmlParseFile(filename.c_str());
    if (document.m_doc == nullptr) {
      throw Arccore::FatalErrorException(A_FUNCINFO, Arccore::String::format("Error while parsing XML file : {0} ",filename));
    }
    return document ;
  }

  FileNode openFileNode(XMLDocument const& document, const std::string& node_name)
  {
    if(document.isNull())
      throw Arccore::FatalErrorException(A_FUNCINFO, "Null XML document");

    FileNode node;
    node.x_id = xmlDocGetRootElement(document.m_doc);
    if (xmlStrcmp(node.x_id->name, (const xmlChar*)node_name.c_str())) {
        throw Arccore::FatalErrorException(A_FUNCINFO, "node not found");
    }
    return node;
  }

  FileNode openFileNode(FileNode const& parent, const std::string& name)
  {
    FileNode node;
    node.name = name;
    node.path_name = parent.path_name + "/" + name;
    node.level = parent.level + 1;
    xmlNodePtr cur = parent.x_id->xmlChildrenNode;
    while (cur != NULL) {
      if ((!xmlStrcmp(cur->name, (const xmlChar*)name.c_str()))) {
        node.x_id = cur;
        break;
      }
      cur = cur->next;
    }
    return node;
  }

  void closeFileNode(FileNode const& group)
  {
      xmlFree(group.x_id) ;
  }

  template <typename ValueT>
  bool read(FileNode const& parent_node, const std::string& node_name,
            std::vector<ValueT>& buffer)
  {
      FileNode node;

      xmlNodePtr cur = parent_node.x_id->xmlChildrenNode;
      while (cur != nullptr) {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)node_name.c_str()))) {
          node.x_id = cur;
          break;
        }
        cur = cur->next;
      }
      if(node.isNull())
        return false ;

      xmlChar* contenu = xmlNodeGetContent(node.x_id);
      std::stringstream flux;
      flux << (const xmlChar*)contenu;
      for (std::size_t i = 0; i < buffer.size(); ++i)
        flux >> buffer[i];
      xmlFree(contenu);
      return true ;
  }

  template <typename ValueT>
  bool read(FileNode const& parent_node, const std::string& node_name, ValueT& val)
  {
      FileNode node;
      xmlNodePtr cur = parent_node.x_id->xmlChildrenNode;
      while (cur != nullptr) {
        if ((!xmlStrcmp(cur->name, (const xmlChar*)node_name.c_str()))) {
          node.x_id = cur;
          break;
        }
        cur = cur->next;
      }
      if(node.isNull())
        return false ;

      xmlChar* contenu = xmlNodeGetContent(node.x_id);
      //printf("%s\n",contenu);
      std::stringstream flux;
      flux << (const xmlChar*)contenu;
      flux >> val;
      xmlFree(contenu);
      return true ;
  }

  void read(FileNode const& node, std::string& buffer)
  {
      xmlChar* contenu = xmlNodeGetContent(node.x_id);
      buffer = std::string((char*)contenu);
      xmlFree(contenu);
  }

} // end namespace XML2Tools
#endif
} // end namespace Alien
