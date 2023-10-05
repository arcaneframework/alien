function(generateOptionsConfig target)

  set(options INSTALL_GENERATED_FILES)
  set(oneValueArgs)
  set(multiValueArgs)
  cmake_parse_arguments(ARGS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
  
  set(OPTIONSCONFIG2CC "${PROJECT_SOURCE_DIR}/tools/python/options/gen_baseoption.py")
  
  set(template_path "${PROJECT_SOURCE_DIR}/tools/python/options/template")
  
  set(cxxopts_path "${PROJECT_SOURCE_DIR}/third_party/cxxopts/include")

  set(options_config_path ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/options_config)

  if(NOT EXISTS ${options_config_path})
    file(MAKE_DIRECTORY ${options_config_path})
  endif()

  set(options_config_files ${ARGS_UNPARSED_ARGUMENTS})

  foreach(config_file ${options_config_files})

    get_filename_component(name ${config_file} NAME_WE)
    
    set(file_args --file ${config_file})
    set(input_args --input_dir ${CMAKE_CURRENT_LIST_DIR})
    set(output_args --output_dir ${options_config_path})
    set(template_args --template_dir ${template_path})
    
    target_include_directories(${target} PRIVATE
      $<BUILD_INTERFACE:${options_config_path}>
      $<BUILD_INTERFACE:${cxxopts_path}>
    )

    if(IS_ABSOLUTE ${config_file})
      set(file ${config_file})
    else()
      set(file ${CMAKE_CURRENT_LIST_DIR}/${config_file})
    endif()

    if(NOT EXISTS ${file})
      get_source_file_property(is_generated ${file} GENERATED)
      if(NOT ${is_generated})
        logFatalError("solver config file ${file} doesn't exist")
      endif()
    endif()

    set(COMMENT_MESSAGE)
    set(generated_files ${options_config_path}/${name}BaseOptions_config.h
                        ${options_config_path}/${name}BaseOptionsSetter_config.h
                        ${options_config_path}/${name}BaseOptionsEnum_config.h)
    set(pycmd "python ${OPTIONSCONFIG2CC} ${file_args} ${input_args} ${output_args} ${template_args}")
    set(COMMENT_MESSAGE "Building BaseOptions generated files ${name}BaseOptions_config.h ${name}BaseOptionsSetter_config.h ${name}BaseOptionsEnum_config.h ${pycmd}")
    
    add_custom_command(OUTPUT  ${generated_files}
                       DEPENDS ${file}
                       COMMAND python ${OPTIONSCONFIG2CC} 
                       ARGS    ${file_args}
                               ${input_args}
                               ${output_args}
                               ${template_args}
                       COMMENT ${COMMENT_MESSAGE}
                      )

    foreach(generated_file ${generated_files})
      set_source_files_properties(
        ${generated_file} PROPERTIES GENERATED ON
      )
    endforeach()
    
    target_sources(${target} PRIVATE ${generated_files})

    set_property(GLOBAL APPEND PROPERTY SOLVERCONFIG_DB ${file})

    if(ARGS_INSTALL_GENERATED_FILES)
      install(FILES ${generated_files} DESTINATION include/${PROJECT_NAME}/solver_config)
    endif()

  endforeach()

  target_include_directories(${target} PRIVATE
    $<BUILD_INTERFACE:${config_path}>
  )

endfunction()


function(generateOptionConfigDataBase)

  get_property(configs GLOBAL PROPERTY SOLVERCONFIG_DB)

  foreach(config ${configs})
    set(CONFIG_STR "${CONFIG_STR}${config}\n")
  endforeach()

  file(WRITE ${PROJECT_BINARY_DIR}/optionconfigdb.txt ${CONFIG_STR})

endfunction()