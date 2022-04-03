file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "project_elf_src_esp32.c"
  "user_event_loops.bin"
  "user_event_loops.map"
  "CMakeFiles/user_event_loops.elf.dir/project_elf_src_esp32.c.obj"
  "project_elf_src_esp32.c"
  "user_event_loops.elf"
  "user_event_loops.elf.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/user_event_loops.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
