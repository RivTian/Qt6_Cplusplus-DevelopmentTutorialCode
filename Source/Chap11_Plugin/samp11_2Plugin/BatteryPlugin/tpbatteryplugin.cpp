#include "tpbattery.h"
#include "tpbatteryplugin.h"

#include <QtPlugin>

TPBatteryPlugin::TPBatteryPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void TPBatteryPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool TPBatteryPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *TPBatteryPlugin::createWidget(QWidget *parent)
{
    return new TPBattery(parent);
}

QString TPBatteryPlugin::name() const
{
    return QLatin1String("TPBattery");
}

QString TPBatteryPlugin::group() const
{
    return QLatin1String("My Widgets");
}

QIcon TPBatteryPlugin::icon() const
{
    return QIcon(QLatin1String(":/44.ico"));
}

QString TPBatteryPlugin::toolTip() const
{
    return QLatin1String("");
}

QString TPBatteryPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool TPBatteryPlugin::isContainer() const
{
    return false;
}

QString TPBatteryPlugin::domXml() const
{
    return QLatin1String("<widget class=\"TPBattery\" name=\"tPBattery\">\n</widget>\n");
}

QString TPBatteryPlugin::includeFile() const
{
    return QLatin1String("tpbattery.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(tpbatteryplugin, TPBatteryPlugin)
#endif // QT_VERSION < 0x050000
