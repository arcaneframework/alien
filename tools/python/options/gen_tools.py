
cxx_type = { "real"        : "Arccore::Real",
             "integer"     : "Arccore::Integer",
             "bool"        : "bool",
             "ustring"     : "Arccore::String",
             "string"      : "Arccore::String",
             "std::string" : "std::string",
             "string[]"    : "std::vector<Arccore::String>",
        	 "bool[]"      : "std::vector<bool>",
           	 "real[]"      : "std::vector<Arccore::Real>",
           	 "integer[]"   : "std::vector<Arccore::Integer>" }

def tabs(level):
    if level == 0:
        return '\t'
    if level == 1:
        return '\t\t'
    if level == 2:
        return '\t\t\t'
    if level == 3:
        return '\t\t\t\t'
    if level == 4:
        return '\t\t\t\t\t'
    if level == 5:
        return '\t\t\t\t\t\t'
    return '\t\t\t\t\t\t\t\t'

def toClassName(xml_name):
    return ''.join([ s.capitalize() for s in xml_name.split("-")])

def toMethodName(xml_name):
    return ''.join([ s if i==0 else s.capitalize() for (i,s) in enumerate(xml_name.split("-"))])

def toEnvName(xml_name):
    return "_".join([ s.upper() for s in xml_name.split("-")])

def toCxxType(xml_type):
    #print("TOCXX TYPE",xml_type)
    if xml_type in cxx_type.keys():
        return cxx_type[xml_type]
    else:
        return xml_type

def isArray(minOccurs,maxOccurs):
    if minOccurs == None :
        return False
    else:
        if minOccurs == "1" :
            if maxOccurs == "1" :
                return False
            else:
                return True
        else:
            return True

def toCxxArrayType(xml_type,minOccurs,maxOccurs) :
    #print("TOCXX ARRAYTYPE",xml_type)
    if minOccurs == None :
        if xml_type in cxx_type.keys():
            return cxx_type[xml_type]
        else:
            return xml_type
    else:
        if minOccurs == "1" :
            if maxOccurs == "1" :
                if xml_type in cxx_type.keys():
                    return cxx_type[xml_type]
                else:
                    return xml_type
            else:
                if xml_type in cxx_type.keys():
                    return f"std::vector<{cxx_type[xml_type]}>"
                else:
                    return f"std::vector<{xml_type}>"
        else:
            if xml_type in cxx_type.keys():
                return f"std::vector<{cxx_type[xml_type]}>"
            else:
                return f"std::vector<{xml_type}>"

def defaultValue(value):
    if value == None:
        return ""
    else:
        return f"->default_value(\"{value}\")"

def isMandatory(opt):
    if opt.default == None:
        if opt.minOccurs == None:
            return True
        else:
            if opt.minOccurs == 0 :
                return False
            else :
                return True
    else:
        return False

