import os
import json

platform_local_txt = "C:\\Arduino15\\hardware\\arduino\\avr\\platform.local.txt"

build_source_path = os.getcwd()
build_source_path_list = build_source_path.rsplit('\\')
build_project_name = (build_source_path_list[len(build_source_path_list) - 1] + ".ino")
user_settings_h = (build_source_path + "/UserSettings.h")

print(build_source_path)
print(build_source_path_list)
print(build_project_name)
print(user_settings_h)

"""
#compiler.c.extra_flags=-D BUILD_SOURCE_PATH="{build.source.path}" -D BUILD_PROJECT_NAME="{build.project_name}" -D USER_SETTINGS_H="{build.source.path}/UserSettings.h" -include "{build.source.path}/UserSettings.h"
compiler.c.extra_flags=-D BUILD_SOURCE_PATH="C:\\Users\\jmparatte\\Arduino15\\test\\ConsoleRead1" -D BUILD_PROJECT_NAME="{build.project_name}" -D USER_SETTINGS_H="C:\\Users\\jmparatte\\Arduino15\\test\\ConsoleRead1/UserSettings.h" -include "{build.source.path}/UserSettings.h"
compiler.cpp.extra_flags={compiler.c.extra_flags}
"""

BUILD_SOURCE_PATH = json.dumps(build_source_path)
BUILD_PROJECT_NAME = json.dumps(build_project_name)
USER_SETTINGS_H = json.dumps(build_source_path + "/UserSettings.h")

file = open(platform_local_txt, "w")
print( "compiler.c.extra_flags=", ("-D BUILD_SOURCE_PATH=" + BUILD_SOURCE_PATH), ("-D BUILD_PROJECT_NAME=" + BUILD_PROJECT_NAME), ("-D USER_SETTINGS_H=" + USER_SETTINGS_H), ("-include " + "\"" + user_settings_h + "\""), file=file)
print( "compiler.cpp.extra_flags={compiler.c.extra_flags}", file=file )
file.close()
