# ./options.py
# -*- coding: utf-8 -*-
# PyXB bindings for NM:e92452c8d3e28a9e27abfc9994d2007779e7f4c9
# Generated 2022-01-11 15:23:56.767145 by PyXB version 1.2.6 using Python 3.8.2.final.0
# Namespace AbsentNamespace0

from __future__ import unicode_literals
import pyxb
import pyxb.binding
import pyxb.binding.saxer
import io
import pyxb.utils.utility
import pyxb.utils.domutils
import sys
import pyxb.utils.six as _six
# Unique identifier for bindings created at the same time
_GenerationUID = pyxb.utils.utility.UniqueIdentifier('urn:uuid:1c785db8-72ea-11ec-b9af-106530a6d97b')

# Version of PyXB used to generate the bindings
_PyXBVersion = '1.2.6'
# Generated bindings are not compatible across PyXB versions
if pyxb.__version__ != _PyXBVersion:
    raise pyxb.PyXBVersionError(_PyXBVersion)

# A holder for module-level binding classes so we can access them from
# inside class definitions where property names may conflict.
_module_typeBindings = pyxb.utils.utility.Object()

# Import bindings for namespaces imported into schema
import pyxb.binding.datatypes

# NOTE: All namespace declarations are reserved within the binding
Namespace = pyxb.namespace.CreateAbsentNamespace()
Namespace.configureCategories(['typeBinding', 'elementBinding'])

def CreateFromDocument (xml_text, default_namespace=None, location_base=None):
    """Parse the given XML and use the document element to create a
    Python instance.

    @param xml_text An XML document.  This should be data (Python 2
    str or Python 3 bytes), or a text (Python 2 unicode or Python 3
    str) in the L{pyxb._InputEncoding} encoding.

    @keyword default_namespace The L{pyxb.Namespace} instance to use as the
    default namespace where there is no default namespace in scope.
    If unspecified or C{None}, the namespace of the module containing
    this function will be used.

    @keyword location_base: An object to be recorded as the base of all
    L{pyxb.utils.utility.Location} instances associated with events and
    objects handled by the parser.  You might pass the URI from which
    the document was obtained.
    """

    if pyxb.XMLStyle_saxer != pyxb._XMLStyle:
        dom = pyxb.utils.domutils.StringToDOM(xml_text)
        return CreateFromDOM(dom.documentElement, default_namespace=default_namespace)
    if default_namespace is None:
        default_namespace = Namespace.fallbackNamespace()
    saxer = pyxb.binding.saxer.make_parser(fallback_namespace=default_namespace, location_base=location_base)
    handler = saxer.getContentHandler()
    xmld = xml_text
    if isinstance(xmld, _six.text_type):
        xmld = xmld.encode(pyxb._InputEncoding)
    saxer.parse(io.BytesIO(xmld))
    instance = handler.rootObject()
    return instance

def CreateFromDOM (node, default_namespace=None):
    """Create a Python instance from the given DOM node.
    The node tag must correspond to an element declaration in this module.

    @deprecated: Forcing use of DOM interface is unnecessary; use L{CreateFromDocument}."""
    if default_namespace is None:
        default_namespace = Namespace.fallbackNamespace()
    return pyxb.binding.basis.element.AnyCreateFromDOM(node, default_namespace)


# Atomic simple type: [anonymous]
class STD_ANON (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 34, 10)
    _Documentation = None
STD_ANON._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=STD_ANON, enum_prefix=None)
STD_ANON.ustring = STD_ANON._CF_enumeration.addEnumeration(unicode_value='ustring', tag='ustring')
STD_ANON.string = STD_ANON._CF_enumeration.addEnumeration(unicode_value='string', tag='string')
STD_ANON.real = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real', tag='real')
STD_ANON.bool = STD_ANON._CF_enumeration.addEnumeration(unicode_value='bool', tag='bool')
STD_ANON.integer = STD_ANON._CF_enumeration.addEnumeration(unicode_value='integer', tag='integer')
STD_ANON.int16 = STD_ANON._CF_enumeration.addEnumeration(unicode_value='int16', tag='int16')
STD_ANON.int32 = STD_ANON._CF_enumeration.addEnumeration(unicode_value='int32', tag='int32')
STD_ANON.int64 = STD_ANON._CF_enumeration.addEnumeration(unicode_value='int64', tag='int64')
STD_ANON.real2 = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real2', tag='real2')
STD_ANON.real3 = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real3', tag='real3')
STD_ANON.real2x2 = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real2x2', tag='real2x2')
STD_ANON.real3x3 = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real3x3', tag='real3x3')
STD_ANON.string_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='string[]', tag='string_')
STD_ANON.bool_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='bool[]', tag='bool_')
STD_ANON.real_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real[]', tag='real_')
STD_ANON.real2_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real2[]', tag='real2_')
STD_ANON.real3_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real3[]', tag='real3_')
STD_ANON.real2x2_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real2x2[]', tag='real2x2_')
STD_ANON.real3x3_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='real3x3[]', tag='real3x3_')
STD_ANON.integer_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='integer[]', tag='integer_')
STD_ANON.int16_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='int16[]', tag='int16_')
STD_ANON.int32_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='int32[]', tag='int32_')
STD_ANON.int64_ = STD_ANON._CF_enumeration.addEnumeration(unicode_value='int64[]', tag='int64_')
STD_ANON._InitializeFacetMap(STD_ANON._CF_enumeration)
_module_typeBindings.STD_ANON = STD_ANON

# Atomic simple type: [anonymous]
class STD_ANON_ (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 118, 10)
    _Documentation = None
STD_ANON_._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=STD_ANON_, enum_prefix=None)
STD_ANON_.n1_0 = STD_ANON_._CF_enumeration.addEnumeration(unicode_value='1.0', tag='n1_0')
STD_ANON_._InitializeFacetMap(STD_ANON_._CF_enumeration)
_module_typeBindings.STD_ANON_ = STD_ANON_

# Atomic simple type: DataType
class DataType (pyxb.binding.datatypes.string, pyxb.binding.basis.enumeration_mixin):

    """An atomic simple type."""

    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'DataType')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 176, 2)
    _Documentation = None
DataType._CF_enumeration = pyxb.binding.facets.CF_enumeration(value_datatype=DataType, enum_prefix=None)
DataType.real = DataType._CF_enumeration.addEnumeration(unicode_value='real', tag='real')
DataType.integer = DataType._CF_enumeration.addEnumeration(unicode_value='integer', tag='integer')
DataType.int16 = DataType._CF_enumeration.addEnumeration(unicode_value='int16', tag='int16')
DataType.int32 = DataType._CF_enumeration.addEnumeration(unicode_value='int32', tag='int32')
DataType.int64 = DataType._CF_enumeration.addEnumeration(unicode_value='int64', tag='int64')
DataType.string = DataType._CF_enumeration.addEnumeration(unicode_value='string', tag='string')
DataType.real2 = DataType._CF_enumeration.addEnumeration(unicode_value='real2', tag='real2')
DataType.real2x2 = DataType._CF_enumeration.addEnumeration(unicode_value='real2x2', tag='real2x2')
DataType.real3 = DataType._CF_enumeration.addEnumeration(unicode_value='real3', tag='real3')
DataType.real3x3 = DataType._CF_enumeration.addEnumeration(unicode_value='real3x3', tag='real3x3')
DataType.bool = DataType._CF_enumeration.addEnumeration(unicode_value='bool', tag='bool')
DataType.ustring = DataType._CF_enumeration.addEnumeration(unicode_value='ustring', tag='ustring')
DataType._InitializeFacetMap(DataType._CF_enumeration)
Namespace.addCategoryObject('typeBinding', 'DataType', DataType)
_module_typeBindings.DataType = DataType

# Complex type Base with content type ELEMENT_ONLY
class Base (pyxb.binding.basis.complexTypeDefinition):
    """Complex type Base with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Base')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 6, 2)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element userclass uses Python identifier userclass
    __userclass = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'userclass'), 'userclass', '__AbsentNamespace0_Base_userclass', False, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6), )

    
    userclass = property(__userclass.value, __userclass.set, None, None)

    
    # Element name uses Python identifier name
    __name = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(Namespace, 'name'), 'name', '__AbsentNamespace0_Base_name', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 131, 2), )

    
    name = property(__name.value, __name.set, None, None)

    
    # Element defaultvalue uses Python identifier defaultvalue
    __defaultvalue = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue'), 'defaultvalue', '__AbsentNamespace0_Base_defaultvalue', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 142, 2), )

    
    defaultvalue = property(__defaultvalue.value, __defaultvalue.set, None, None)

    
    # Element description uses Python identifier description
    __description = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(Namespace, 'description'), 'description', '__AbsentNamespace0_Base_description', False, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 153, 2), )

    
    description = property(__description.value, __description.set, None, None)

    
    # Attribute name uses Python identifier name_
    __name_ = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'name'), 'name_', '__AbsentNamespace0_Base_name_', pyxb.binding.datatypes.string, required=True)
    __name_._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 13, 4)
    __name_._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 13, 4)
    
    name_ = property(__name_.value, __name_.set, None, None)

    _ElementMap.update({
        __userclass.name() : __userclass,
        __name.name() : __name,
        __defaultvalue.name() : __defaultvalue,
        __description.name() : __description
    })
    _AttributeMap.update({
        __name_.name() : __name_
    })
_module_typeBindings.Base = Base
Namespace.addCategoryObject('typeBinding', 'Base', Base)


# Complex type [anonymous] with content type SIMPLE
class CTD_ANON (pyxb.binding.basis.complexTypeDefinition):
    """Complex type [anonymous] with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.string
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 132, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.string
    
    # Attribute lang uses Python identifier lang
    __lang = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'lang'), 'lang', '__AbsentNamespace0_CTD_ANON_lang', pyxb.binding.datatypes.string, required=True)
    __lang._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 135, 10)
    __lang._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 135, 10)
    
    lang = property(__lang.value, __lang.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __lang.name() : __lang
    })
_module_typeBindings.CTD_ANON = CTD_ANON


# Complex type [anonymous] with content type SIMPLE
class CTD_ANON_ (pyxb.binding.basis.complexTypeDefinition):
    """Complex type [anonymous] with content type SIMPLE"""
    _TypeDefinition = pyxb.binding.datatypes.string
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_SIMPLE
    _Abstract = False
    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 143, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.string
    
    # Attribute category uses Python identifier category
    __category = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'category'), 'category', '__AbsentNamespace0_CTD_ANON__category', pyxb.binding.datatypes.string, required=True)
    __category._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 146, 10)
    __category._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 146, 10)
    
    category = property(__category.value, __category.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __category.name() : __category
    })
_module_typeBindings.CTD_ANON_ = CTD_ANON_


# Complex type [anonymous] with content type MIXED
class CTD_ANON_2 (pyxb.binding.basis.complexTypeDefinition):
    """Complex type [anonymous] with content type MIXED"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_MIXED
    _Abstract = False
    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 154, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    _HasWildcardElement = True
    _ElementMap.update({
        
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.CTD_ANON_2 = CTD_ANON_2


# Complex type [anonymous] with content type ELEMENT_ONLY
class CTD_ANON_3 (pyxb.binding.basis.complexTypeDefinition):
    """Complex type [anonymous] with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = None
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 163, 4)
    _ElementMap = {}
    _AttributeMap = {}
    # Base type is pyxb.binding.datatypes.anyType
    
    # Element complex uses Python identifier complex
    __complex = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'complex'), 'complex', '__AbsentNamespace0_CTD_ANON_3_complex', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 166, 6), )

    
    complex = property(__complex.value, __complex.set, None, None)

    
    # Element enumeration uses Python identifier enumeration
    __enumeration = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'enumeration'), 'enumeration', '__AbsentNamespace0_CTD_ANON_3_enumeration', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 167, 6), )

    
    enumeration = property(__enumeration.value, __enumeration.set, None, None)

    
    # Element simple uses Python identifier simple
    __simple = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'simple'), 'simple', '__AbsentNamespace0_CTD_ANON_3_simple', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 168, 6), )

    
    simple = property(__simple.value, __simple.set, None, None)

    _ElementMap.update({
        __complex.name() : __complex,
        __enumeration.name() : __enumeration,
        __simple.name() : __simple
    })
    _AttributeMap.update({
        
    })
_module_typeBindings.CTD_ANON_3 = CTD_ANON_3


# Complex type Option with content type ELEMENT_ONLY
class Option (Base):
    """Complex type Option with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = True
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Option')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 18, 2)
    _ElementMap = Base._ElementMap.copy()
    _AttributeMap = Base._AttributeMap.copy()
    # Base type is Base
    
    # Element userclass (userclass) inherited from Base
    
    # Element name (name) inherited from Base
    
    # Element defaultvalue (defaultvalue) inherited from Base
    
    # Element description (description) inherited from Base
    
    # Attribute name_ inherited from Base
    
    # Attribute optional uses Python identifier optional
    __optional = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'optional'), 'optional', '__AbsentNamespace0_Option_optional', pyxb.binding.datatypes.boolean)
    __optional._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 21, 8)
    __optional._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 21, 8)
    
    optional = property(__optional.value, __optional.set, None, None)

    
    # Attribute default uses Python identifier default
    __default = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'default'), 'default', '__AbsentNamespace0_Option_default', pyxb.binding.datatypes.string)
    __default._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 22, 8)
    __default._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 22, 8)
    
    default = property(__default.value, __default.set, None, None)

    
    # Attribute minOccurs uses Python identifier minOccurs
    __minOccurs = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'minOccurs'), 'minOccurs', '__AbsentNamespace0_Option_minOccurs', pyxb.binding.datatypes.string, unicode_default='1')
    __minOccurs._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 23, 8)
    __minOccurs._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 23, 8)
    
    minOccurs = property(__minOccurs.value, __minOccurs.set, None, None)

    
    # Attribute maxOccurs uses Python identifier maxOccurs
    __maxOccurs = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'maxOccurs'), 'maxOccurs', '__AbsentNamespace0_Option_maxOccurs', pyxb.binding.datatypes.string, unicode_default='1')
    __maxOccurs._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 24, 8)
    __maxOccurs._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 24, 8)
    
    maxOccurs = property(__maxOccurs.value, __maxOccurs.set, None, None)

    
    # Attribute allow-null uses Python identifier allow_null
    __allow_null = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'allow-null'), 'allow_null', '__AbsentNamespace0_Option_allow_null', pyxb.binding.datatypes.boolean)
    __allow_null._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 25, 8)
    __allow_null._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 25, 8)
    
    allow_null = property(__allow_null.value, __allow_null.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __optional.name() : __optional,
        __default.name() : __default,
        __minOccurs.name() : __minOccurs,
        __maxOccurs.name() : __maxOccurs,
        __allow_null.name() : __allow_null
    })
_module_typeBindings.Option = Option
Namespace.addCategoryObject('typeBinding', 'Option', Option)


# Complex type EnumValue with content type ELEMENT_ONLY
class EnumValue (Base):
    """Complex type EnumValue with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'EnumValue')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 79, 2)
    _ElementMap = Base._ElementMap.copy()
    _AttributeMap = Base._AttributeMap.copy()
    # Base type is Base
    
    # Element userclass (userclass) inherited from Base
    
    # Element name (name) inherited from Base
    
    # Element defaultvalue (defaultvalue) inherited from Base
    
    # Element description (description) inherited from Base
    
    # Attribute name_ inherited from Base
    
    # Attribute genvalue uses Python identifier genvalue
    __genvalue = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'genvalue'), 'genvalue', '__AbsentNamespace0_EnumValue_genvalue', pyxb.binding.datatypes.string, required=True)
    __genvalue._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 82, 8)
    __genvalue._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 82, 8)
    
    genvalue = property(__genvalue.value, __genvalue.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __genvalue.name() : __genvalue
    })
_module_typeBindings.EnumValue = EnumValue
Namespace.addCategoryObject('typeBinding', 'EnumValue', EnumValue)


# Complex type Service with content type ELEMENT_ONLY
class Service (Base):
    """Complex type Service with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Service')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 111, 2)
    _ElementMap = Base._ElementMap.copy()
    _AttributeMap = Base._AttributeMap.copy()
    # Base type is Base
    
    # Element userclass (userclass) inherited from Base
    
    # Element name (name) inherited from Base
    
    # Element defaultvalue (defaultvalue) inherited from Base
    
    # Element description (description) inherited from Base
    
    # Element options uses Python identifier options
    __options = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(Namespace, 'options'), 'options', '__AbsentNamespace0_Service_options', False, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 162, 2), )

    
    options = property(__options.value, __options.set, None, None)

    
    # Attribute name_ inherited from Base
    
    # Attribute version uses Python identifier version
    __version = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'version'), 'version', '__AbsentNamespace0_Service_version', _module_typeBindings.STD_ANON_)
    __version._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 117, 8)
    __version._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 117, 8)
    
    version = property(__version.value, __version.set, None, None)

    _ElementMap.update({
        __options.name() : __options
    })
    _AttributeMap.update({
        __version.name() : __version
    })
_module_typeBindings.Service = Service
Namespace.addCategoryObject('typeBinding', 'Service', Service)


# Complex type Simple with content type ELEMENT_ONLY
class Simple (Option):
    """Complex type Simple with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Simple')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 30, 2)
    _ElementMap = Option._ElementMap.copy()
    _AttributeMap = Option._AttributeMap.copy()
    # Base type is Option
    
    # Element userclass (userclass) inherited from Base
    
    # Element name (name) inherited from Base
    
    # Element defaultvalue (defaultvalue) inherited from Base
    
    # Element description (description) inherited from Base
    
    # Attribute name_ inherited from Base
    
    # Attribute optional inherited from Option
    
    # Attribute default inherited from Option
    
    # Attribute minOccurs inherited from Option
    
    # Attribute maxOccurs inherited from Option
    
    # Attribute allow_null inherited from Option
    
    # Attribute type uses Python identifier type
    __type = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'type'), 'type', '__AbsentNamespace0_Simple_type', _module_typeBindings.STD_ANON)
    __type._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 33, 8)
    __type._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 33, 8)
    
    type = property(__type.value, __type.set, None, None)

    
    # Attribute unit uses Python identifier unit
    __unit = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'unit'), 'unit', '__AbsentNamespace0_Simple_unit', pyxb.binding.datatypes.string)
    __unit._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 62, 8)
    __unit._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 62, 8)
    
    unit = property(__unit.value, __unit.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __type.name() : __type,
        __unit.name() : __unit
    })
_module_typeBindings.Simple = Simple
Namespace.addCategoryObject('typeBinding', 'Simple', Simple)


# Complex type Enumeration with content type ELEMENT_ONLY
class Enumeration (Option):
    """Complex type Enumeration with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Enumeration')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 67, 2)
    _ElementMap = Option._ElementMap.copy()
    _AttributeMap = Option._AttributeMap.copy()
    # Base type is Option
    
    # Element userclass (userclass) inherited from Base
    
    # Element enumvalue uses Python identifier enumvalue
    __enumvalue = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'enumvalue'), 'enumvalue', '__AbsentNamespace0_Enumeration_enumvalue', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 71, 10), )

    
    enumvalue = property(__enumvalue.value, __enumvalue.set, None, None)

    
    # Element name (name) inherited from Base
    
    # Element defaultvalue (defaultvalue) inherited from Base
    
    # Element description (description) inherited from Base
    
    # Attribute name_ inherited from Base
    
    # Attribute optional inherited from Option
    
    # Attribute default inherited from Option
    
    # Attribute minOccurs inherited from Option
    
    # Attribute maxOccurs inherited from Option
    
    # Attribute allow_null inherited from Option
    
    # Attribute type uses Python identifier type
    __type = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'type'), 'type', '__AbsentNamespace0_Enumeration_type', pyxb.binding.datatypes.string)
    __type._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 73, 8)
    __type._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 73, 8)
    
    type = property(__type.value, __type.set, None, None)

    _ElementMap.update({
        __enumvalue.name() : __enumvalue
    })
    _AttributeMap.update({
        __type.name() : __type
    })
_module_typeBindings.Enumeration = Enumeration
Namespace.addCategoryObject('typeBinding', 'Enumeration', Enumeration)


# Complex type Complex with content type ELEMENT_ONLY
class Complex (Option):
    """Complex type Complex with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Complex')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 88, 2)
    _ElementMap = Option._ElementMap.copy()
    _AttributeMap = Option._AttributeMap.copy()
    # Base type is Option
    
    # Element userclass (userclass) inherited from Base
    
    # Element complex uses Python identifier complex
    __complex = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'complex'), 'complex', '__AbsentNamespace0_Complex_complex', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 93, 7), )

    
    complex = property(__complex.value, __complex.set, None, None)

    
    # Element enumeration uses Python identifier enumeration
    __enumeration = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'enumeration'), 'enumeration', '__AbsentNamespace0_Complex_enumeration', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 94, 7), )

    
    enumeration = property(__enumeration.value, __enumeration.set, None, None)

    
    # Element simple uses Python identifier simple
    __simple = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'simple'), 'simple', '__AbsentNamespace0_Complex_simple', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 95, 7), )

    
    simple = property(__simple.value, __simple.set, None, None)

    
    # Element attribute uses Python identifier attribute
    __attribute = pyxb.binding.content.ElementDeclaration(pyxb.namespace.ExpandedName(None, 'attribute'), 'attribute', '__AbsentNamespace0_Complex_attribute', True, pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 96, 7), )

    
    attribute = property(__attribute.value, __attribute.set, None, None)

    
    # Element name (name) inherited from Base
    
    # Element defaultvalue (defaultvalue) inherited from Base
    
    # Element description (description) inherited from Base
    
    # Attribute name_ inherited from Base
    
    # Attribute optional inherited from Option
    
    # Attribute default inherited from Option
    
    # Attribute minOccurs inherited from Option
    
    # Attribute maxOccurs inherited from Option
    
    # Attribute allow_null inherited from Option
    
    # Attribute ref uses Python identifier ref
    __ref = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'ref'), 'ref', '__AbsentNamespace0_Complex_ref', pyxb.binding.datatypes.string)
    __ref._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 99, 8)
    __ref._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 99, 8)
    
    ref = property(__ref.value, __ref.set, None, None)

    
    # Attribute type uses Python identifier type
    __type = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'type'), 'type', '__AbsentNamespace0_Complex_type', pyxb.binding.datatypes.string)
    __type._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 100, 8)
    __type._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 100, 8)
    
    type = property(__type.value, __type.set, None, None)

    _ElementMap.update({
        __complex.name() : __complex,
        __enumeration.name() : __enumeration,
        __simple.name() : __simple,
        __attribute.name() : __attribute
    })
    _AttributeMap.update({
        __ref.name() : __ref,
        __type.name() : __type
    })
_module_typeBindings.Complex = Complex
Namespace.addCategoryObject('typeBinding', 'Complex', Complex)


# Complex type Attribute with content type ELEMENT_ONLY
class Attribute (Option):
    """Complex type Attribute with content type ELEMENT_ONLY"""
    _TypeDefinition = None
    _ContentTypeTag = pyxb.binding.basis.complexTypeDefinition._CT_ELEMENT_ONLY
    _Abstract = False
    _ExpandedName = pyxb.namespace.ExpandedName(Namespace, 'Attribute')
    _XSDLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 193, 2)
    _ElementMap = Option._ElementMap.copy()
    _AttributeMap = Option._AttributeMap.copy()
    # Base type is Option
    
    # Element userclass (userclass) inherited from Base
    
    # Element name (name) inherited from Base
    
    # Element defaultvalue (defaultvalue) inherited from Base
    
    # Element description (description) inherited from Base
    
    # Attribute name_ inherited from Base
    
    # Attribute optional inherited from Option
    
    # Attribute default inherited from Option
    
    # Attribute minOccurs inherited from Option
    
    # Attribute maxOccurs inherited from Option
    
    # Attribute allow_null inherited from Option
    
    # Attribute type uses Python identifier type
    __type = pyxb.binding.content.AttributeUse(pyxb.namespace.ExpandedName(None, 'type'), 'type', '__AbsentNamespace0_Attribute_type', pyxb.binding.datatypes.string, unicode_default='string')
    __type._DeclarationLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 197, 6)
    __type._UseLocation = pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 197, 6)
    
    type = property(__type.value, __type.set, None, None)

    _ElementMap.update({
        
    })
    _AttributeMap.update({
        __type.name() : __type
    })
_module_typeBindings.Attribute = Attribute
Namespace.addCategoryObject('typeBinding', 'Attribute', Attribute)


name = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'name'), CTD_ANON, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 131, 2))
Namespace.addCategoryObject('elementBinding', name.name().localName(), name)

defaultvalue = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue'), CTD_ANON_, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 142, 2))
Namespace.addCategoryObject('elementBinding', defaultvalue.name().localName(), defaultvalue)

description = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'description'), CTD_ANON_2, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 153, 2))
Namespace.addCategoryObject('elementBinding', description.name().localName(), description)

options = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'options'), CTD_ANON_3, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 162, 2))
Namespace.addCategoryObject('elementBinding', options.name().localName(), options)

service = pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'service'), Service, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 109, 2))
Namespace.addCategoryObject('elementBinding', service.name().localName(), service)



Base._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'userclass'), pyxb.binding.datatypes.string, scope=Base, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6)))

Base._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'name'), CTD_ANON, scope=Base, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 131, 2)))

Base._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue'), CTD_ANON_, scope=Base, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 142, 2)))

Base._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'description'), CTD_ANON_2, scope=Base, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 153, 2)))

def _BuildAutomaton ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton
    del _BuildAutomaton
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Base._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(Base._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(Base._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(Base._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    st_3._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Base._Automaton = _BuildAutomaton()




def _BuildAutomaton_ ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_
    del _BuildAutomaton_
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 156, 8))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.WildcardUse(pyxb.binding.content.Wildcard(process_contents=pyxb.binding.content.Wildcard.PC_skip, namespace_constraint=pyxb.binding.content.Wildcard.NC_any), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 156, 8))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
CTD_ANON_2._Automaton = _BuildAutomaton_()




CTD_ANON_3._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'complex'), Complex, scope=CTD_ANON_3, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 166, 6)))

CTD_ANON_3._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'enumeration'), Enumeration, scope=CTD_ANON_3, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 167, 6)))

CTD_ANON_3._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'simple'), Simple, scope=CTD_ANON_3, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 168, 6)))

def _BuildAutomaton_2 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_2
    del _BuildAutomaton_2
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 164, 6))
    counters.add(cc_0)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CTD_ANON_3._UseForTag(pyxb.namespace.ExpandedName(None, 'complex')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 166, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CTD_ANON_3._UseForTag(pyxb.namespace.ExpandedName(None, 'enumeration')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 167, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(CTD_ANON_3._UseForTag(pyxb.namespace.ExpandedName(None, 'simple')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 168, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, True) ]))
    st_2._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
CTD_ANON_3._Automaton = _BuildAutomaton_2()




def _BuildAutomaton_3 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_3
    del _BuildAutomaton_3
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Option._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(Option._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(Option._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(Option._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    st_3._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Option._Automaton = _BuildAutomaton_3()




def _BuildAutomaton_4 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_4
    del _BuildAutomaton_4
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(EnumValue._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(EnumValue._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(EnumValue._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(EnumValue._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    st_3._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
EnumValue._Automaton = _BuildAutomaton_4()




Service._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(Namespace, 'options'), CTD_ANON_3, scope=Service, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 162, 2)))

def _BuildAutomaton_5 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_5
    del _BuildAutomaton_5
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    cc_4 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 115, 10))
    counters.add(cc_4)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Service._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(Service._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(Service._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(Service._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(Service._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'options')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 115, 10))
    st_4 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_3, False) ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_4, True) ]))
    st_4._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Service._Automaton = _BuildAutomaton_5()




def _BuildAutomaton_6 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_6
    del _BuildAutomaton_6
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Simple._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(Simple._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(Simple._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(Simple._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    st_3._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Simple._Automaton = _BuildAutomaton_6()




Enumeration._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'enumvalue'), EnumValue, scope=Enumeration, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 71, 10)))

def _BuildAutomaton_7 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_7
    del _BuildAutomaton_7
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    states = []
    final_update = None
    symbol = pyxb.binding.content.ElementUse(Enumeration._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(Enumeration._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(Enumeration._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = None
    symbol = pyxb.binding.content.ElementUse(Enumeration._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    symbol = pyxb.binding.content.ElementUse(Enumeration._UseForTag(pyxb.namespace.ExpandedName(None, 'enumvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 71, 10))
    st_4 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_3, False) ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
         ]))
    st_4._set_transitionSet(transitions)
    return fac.Automaton(states, counters, False, containing_state=None)
Enumeration._Automaton = _BuildAutomaton_7()




Complex._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'complex'), Complex, scope=Complex, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 93, 7)))

Complex._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'enumeration'), Enumeration, scope=Complex, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 94, 7)))

Complex._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'simple'), Simple, scope=Complex, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 95, 7)))

Complex._AddElement(pyxb.binding.basis.element(pyxb.namespace.ExpandedName(None, 'attribute'), Attribute, scope=Complex, location=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 96, 7)))

def _BuildAutomaton_8 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_8
    del _BuildAutomaton_8
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    cc_4 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 91, 7))
    counters.add(cc_4)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(None, 'complex')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 93, 7))
    st_4 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_4)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(None, 'enumeration')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 94, 7))
    st_5 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_5)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(None, 'simple')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 95, 7))
    st_6 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_6)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_4, False))
    symbol = pyxb.binding.content.ElementUse(Complex._UseForTag(pyxb.namespace.ExpandedName(None, 'attribute')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 96, 7))
    st_7 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_7)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_2, False) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_3, False) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_3, False) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_3, False) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_3, False) ]))
    st_3._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_4, True) ]))
    st_4._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_4, True) ]))
    st_5._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_4, True) ]))
    st_6._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_4, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_5, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_6, [
        fac.UpdateInstruction(cc_4, True) ]))
    transitions.append(fac.Transition(st_7, [
        fac.UpdateInstruction(cc_4, True) ]))
    st_7._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Complex._Automaton = _BuildAutomaton_8()




def _BuildAutomaton_9 ():
    # Remove this helper function from the namespace after it is invoked
    global _BuildAutomaton_9
    del _BuildAutomaton_9
    import pyxb.utils.fac as fac

    counters = set()
    cc_0 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    counters.add(cc_0)
    cc_1 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    counters.add(cc_1)
    cc_2 = fac.CounterCondition(min=0, max=None, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    counters.add(cc_2)
    cc_3 = fac.CounterCondition(min=0, max=1, metadata=pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    counters.add(cc_3)
    states = []
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_0, False))
    symbol = pyxb.binding.content.ElementUse(Attribute._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'name')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 8, 6))
    st_0 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_0)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_1, False))
    symbol = pyxb.binding.content.ElementUse(Attribute._UseForTag(pyxb.namespace.ExpandedName(None, 'userclass')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 9, 6))
    st_1 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_1)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_2, False))
    symbol = pyxb.binding.content.ElementUse(Attribute._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'defaultvalue')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 10, 6))
    st_2 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_2)
    final_update = set()
    final_update.add(fac.UpdateInstruction(cc_3, False))
    symbol = pyxb.binding.content.ElementUse(Attribute._UseForTag(pyxb.namespace.ExpandedName(Namespace, 'description')), pyxb.utils.utility.Location('/work/gratienj/Arcane3/GitLab2GitHub/alien/tools/python/options/options.xsd', 11, 6))
    st_3 = fac.State(symbol, is_initial=True, final_update=final_update, is_unordered_catenation=False)
    states.append(st_3)
    transitions = []
    transitions.append(fac.Transition(st_0, [
        fac.UpdateInstruction(cc_0, True) ]))
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_0, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_0, False) ]))
    st_0._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_1, [
        fac.UpdateInstruction(cc_1, True) ]))
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_1, False) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_1, False) ]))
    st_1._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_2, [
        fac.UpdateInstruction(cc_2, True) ]))
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_2, False) ]))
    st_2._set_transitionSet(transitions)
    transitions = []
    transitions.append(fac.Transition(st_3, [
        fac.UpdateInstruction(cc_3, True) ]))
    st_3._set_transitionSet(transitions)
    return fac.Automaton(states, counters, True, containing_state=None)
Attribute._Automaton = _BuildAutomaton_9()

