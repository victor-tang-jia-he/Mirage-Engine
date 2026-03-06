add_library(Core
    Private/Logs/Log.cpp
    Private/ThreadWorker.cpp
    Private/ThreadsManager.cpp

    Public/Logs/Log.h
    Public/ThreadsManager.h
    Public/ThreadWorker.h
    Public/Events/Event.h
    Public/Events/EventCallback.h
 "Public/Logs/LogMacros.h" "Private/Logs/LogMacros.cpp")

target_include_directories(Core
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/Public"
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/Private"
)

set_target_properties(Core PROPERTIES
    CXX_STANDARD 17
)