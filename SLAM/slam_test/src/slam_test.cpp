#include "slam_test.h"

#include <debug.h>

#include <KPluginFactory>

K_PLUGIN_FACTORY_WITH_JSON(slam_testFactory, "slam_test.json", registerPlugin<slam_test>(); )

slam_test::slam_test(QObject *parent, const QVariantList& args)
    : KDevelop::IPlugin(QStringLiteral("slam_test"), parent)
{
    Q_UNUSED(args);

    qCDebug(PLUGIN_SLAM_TEST) << "Hello world, my plugin is loaded!";
}

// needed for QObject class created from K_PLUGIN_FACTORY_WITH_JSON
#include "slam_test.moc"
