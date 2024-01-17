#include "tpbatteryplugin.h"
#include "tpbattery.h"

#include <QtPlugin>

TPBatteryPlugin::TPBatteryPlugin(QObject *parent)
    : QObject(parent)
{}

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
    return QLatin1String("My Widgets工具提示");
}

QString TPBatteryPlugin::whatsThis() const
{
    return QLatin1String("My Widgets这是什么");
}

bool TPBatteryPlugin::isContainer() const
{
    return false;
}

QString TPBatteryPlugin::domXml() const
{
    return QLatin1String(R"(<widget class="TPBattery" name="tPBattery">
</widget>)");
}

QString TPBatteryPlugin::includeFile() const
{
    return QLatin1String("tpbattery.h");
}
