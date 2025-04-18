block()
  set(unsupported_generators Ninja "Ninja Multi-Config")
  set(supported_generators "Visual Studio 17 2022" "Visual Studio 16 2019" "Visual Studio 15 2017" "MinGW Makefiles" "NMake Makefiles")

  if(CMAKE_GENERATOR IN_LIST unsupported_generators)
    message(FATAL_ERROR "Due to post build commands not being called when compiling unchanged targets, the ${CMAKE_GENERATOR} generator isn't supported by Internal2. Please use a different generator.")
  endif()

  if(NOT CMAKE_GENERATOR IN_LIST supported_generators)
    message(WARNING "The current generator (${CMAKE_GENERATOR}) isn't officially supported by Internal2. Issues might arise.")
  endif()
endblock()

function(internal2_setup_target target)
  macro(compute_guid)
    string(RANDOM LENGTH 32 guid)
  endmacro()

  macro(compute_source_directory)
    get_target_property(source_directory ${target} SOURCE_DIR)
    string(APPEND source_directory /source)
  endmacro()

  macro(compute_source_paths)
    file(GLOB_RECURSE source_paths RELATIVE ${source_directory} ${source_directory}/*)
  endmacro()

  macro(compute_backup_directory)
    set(backup_directory ${CMAKE_CURRENT_BINARY_DIR}/Internal2_backups/${target})
  endmacro()

  macro(create_custom_targets)
    macro(create_custom_target command)
      set(target_${command} Internal2_${command}_${target})
      add_custom_target(${target_${command}})
    endmacro()

    create_custom_target(backup)
    create_custom_target(process)
    add_dependencies(${target_process} ${target_backup})
    add_dependencies(${target} ${target_process})
    create_custom_target(restore)
  endmacro()

  function(compute_commands)
    foreach(source_path IN LISTS source_paths)
      foreach(command IN ITEMS backup process restore)
        function(add_command target)
          add_custom_command(TARGET ${target} POST_BUILD COMMAND Internal2 ${backup_directory} ${source_directory} ${guid} ${command} ${source_path})
        endfunction()
        add_command(${target_${command}})
        if(command STREQUAL "restore")
          add_command(${target})
        endif()
      endforeach()
    endforeach()
  endfunction()

  compute_guid()
  compute_source_directory()
  compute_source_paths()
  compute_backup_directory()
  create_custom_targets()
  compute_commands()
endfunction()