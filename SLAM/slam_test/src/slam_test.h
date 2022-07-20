#ifndef SLAM_TEST_H
#define SLAM_TEST_H

#include <interfaces/iplugin.h>

class slam_test : public KDevelop::IPlugin
{
    Q_OBJECT

public:
    // KPluginFactory-based plugin wants constructor with this signature
    slam_test(QObject* parent, const QVariantList& args);
};

#endif // SLAM_TEST_H
