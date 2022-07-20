set(slam_test_SRCS
    slam_test.cpp
)

ecm_qt_declare_logging_category(slam_test_SRCS
    HEADER debug.h
    IDENTIFIER PLUGIN_SLAM_TEST
    CATEGORY_NAME "kdevelop.plugin.slam_test"
)

kdevplatform_add_plugin(slam_test
    JSON slam_test.json
    SOURCES ${slam_test_SRCS}
)

target_link_libraries(slam_test
    KDev::Interfaces
)
