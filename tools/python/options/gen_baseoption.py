import options
#import mako
import os


if __name__ == "__main__":
    import argparse
    import json
    from mako.template import Template


    parser = argparse.ArgumentParser(description='BaseOptions generator')
    parser.add_argument("--file",            type=str, default="sample-config.xml",help="Solver option config")
    parser.add_argument("--input_dir",       type=str, default="./input",          help="Input dir")
    parser.add_argument("--output_dir",      type=str, default="./output",         help="Output dir")
    parser.add_argument("--template_dir",    type=str, default="./template",       help="Template dir")
    args = parser.parse_args()

    filename = os.path.join(args.input_dir,args.file)
    print("INPUT FILE NAME :",filename)
    xml = open(filename).read()
    opts = options.CreateFromDocument(xml)
    print("SERVICE INSTANCE :",opts.name_)
    options_set = opts.options
    #print("Options SET:",options_set)
    for simple in options_set.simple:
        s = simple
        #print("SIMPLE OPT : ",s)

    for en in options_set.enumeration:
        e = en
        #print("ENUMERATION OPT : ",e,e.type)
        for ev in e.enumvalue:
            v =ev
            #print("ENUM VALUE",ev)

    for comp in options_set.complex:
        c = comp
        #print("COMPLEXE OPT : ", c)

    base_options_template =  os.path.join(args.template_dir,"BaseOptions.h.template")
    t = Template(filename=base_options_template)
    base_options = t.render(options=opts)
    #print("GENERATED CLASS : ",base_options)
    cxx_base_options_filename = os.path.join(args.output_dir,f"{opts.name_}BaseOptions_config.h")
    print("GENERATED CLASS : ",cxx_base_options_filename)
    with open(cxx_base_options_filename,'w') as f0:
        f0.write(base_options)

    base_options_setter_template =  os.path.join(args.template_dir,"BaseOptionsSetter.h.template")
    t = Template(filename=base_options_setter_template)
    base_options_setter = t.render(options=opts)
    #print("GENERATED CLASS : ",base_options)
    cxx_base_options_setter_filename = os.path.join(args.output_dir,f"{opts.name_}BaseOptionsSetter_config.h")
    print("GENERATED CLASS : ",cxx_base_options_setter_filename)
    with open(cxx_base_options_setter_filename,'w') as f1:
        f1.write(base_options_setter)

    base_options_enum_template =  os.path.join(args.template_dir,"BaseOptionsEnum.h.template")
    t = Template(filename=base_options_enum_template)
    base_options_enum = t.render(options=opts)
    #print("GENERATED CLASS : ",base_options)
    cxx_base_options_enum_filename = os.path.join(args.output_dir,f"{opts.name_}BaseOptionsEnum_config.h")
    print("GENERATED CLASS : ",cxx_base_options_enum_filename)
    with open(cxx_base_options_enum_filename,'w') as f2:
        f2.write(base_options_enum)