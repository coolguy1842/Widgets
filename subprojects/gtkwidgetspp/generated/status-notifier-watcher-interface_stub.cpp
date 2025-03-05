static const char interfaceXml0[] = R"XML_DELIMITER(<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"
"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">
<node>
  <interface name="org.kde.StatusNotifierWatcher">
    <annotation name="org.gtk.GDBus.C.Name" value="WatcherV0Gen" />

    <method name="RegisterStatusNotifierItem">
      <annotation name="org.gtk.GDBus.C.Name" value="RegisterItem" />
      <arg type="s" direction="in" name="service" />
    </method>

    <method name="RegisterStatusNotifierHost">
      <annotation name="org.gtk.GDBus.C.Name" value="RegisterHost" />
      <arg type="s" direction="in" name="service" />
    </method>

    <property name="RegisteredStatusNotifierItems" type="as" access="read">
      <annotation name="org.gtk.GDBus.C.Name" value="RegisteredItems" />
    </property>

    <property name="IsStatusNotifierHostRegistered" type="b" access="read">
      <annotation name="org.gtk.GDBus.C.Name" value="IsHostRegistered" />
    </property>

    <property name="ProtocolVersion" type="i" access="read" />

    <signal name="StatusNotifierItemRegistered">
      <annotation name="org.gtk.GDBus.C.Name" value="ItemRegistered" />
      <arg type="s" direction="out" name="service" />
    </signal>

    <signal name="StatusNotifierItemUnregistered">
      <annotation name="org.gtk.GDBus.C.Name" value="ItemUnregistered" />
      <arg type="s" direction="out" name="service" />
    </signal>

    <signal name="StatusNotifierHostRegistered">
      <annotation name="org.gtk.GDBus.C.Name" value="HostRegistered" />
    </signal>

    <signal name="StatusNotifierHostUnregistered">
      <annotation name="org.gtk.GDBus.C.Name" value="HostUnegistered" />
    </signal>
  </interface>
</node>)XML_DELIMITER";

#include "status-notifier-watcher-interface_stub.h"

template<class T>
inline T specialGetter(Glib::Variant<T> variant)
{
    return variant.get();
}

template<>
inline std::string specialGetter(Glib::Variant<std::string> variant)
{
    // String is not guaranteed to be null-terminated, so don't use ::get()
    gsize n_elem;
    gsize elem_size = sizeof(char);
    char* data = (char*)g_variant_get_fixed_array(variant.gobj(), &n_elem, elem_size);

    return std::string(data, n_elem);
}

org::kde::StatusNotifierWatcherStub::StatusNotifierWatcherStub():
    m_interfaceName("org.kde.StatusNotifierWatcher")
{
    StatusNotifierItemRegistered_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierItemRegistered_emitter),
            std::vector<Glib::ustring>({""})) );
    StatusNotifierItemRegistered_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierItemRegistered_emitter));
    StatusNotifierItemUnregistered_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierItemUnregistered_emitter),
            std::vector<Glib::ustring>({""})) );
    StatusNotifierItemUnregistered_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierItemUnregistered_emitter));
    StatusNotifierHostRegistered_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierHostRegistered_emitter),
            std::vector<Glib::ustring>({""})) );
    StatusNotifierHostRegistered_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierHostRegistered_emitter));
    StatusNotifierHostUnregistered_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierHostUnregistered_emitter),
            std::vector<Glib::ustring>({""})) );
    StatusNotifierHostUnregistered_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierWatcherStub::StatusNotifierHostUnregistered_emitter));
}

org::kde::StatusNotifierWatcherStub::~StatusNotifierWatcherStub()
{
    unregister_object();
}

guint org::kde::StatusNotifierWatcherStub::register_object(
    const Glib::RefPtr<Gio::DBus::Connection> &connection,
    const Glib::ustring &object_path)
{
    if (!introspection_data) {
        try {
            introspection_data = Gio::DBus::NodeInfo::create_for_xml(interfaceXml0);
        } catch(const Glib::Error& ex) {
            g_warning("Unable to create introspection data for %s: %s", object_path.c_str(), ex.what());
            return 0;
        }
    }

    Gio::DBus::InterfaceVTable *interface_vtable =
        new Gio::DBus::InterfaceVTable(
            sigc::mem_fun(*this, &StatusNotifierWatcherStub::on_method_call),
            sigc::mem_fun(*this, &StatusNotifierWatcherStub::on_interface_get_property),
            sigc::mem_fun(*this, &StatusNotifierWatcherStub::on_interface_set_property));

    guint registration_id;
    try {
        registration_id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.kde.StatusNotifierWatcher"),
            *interface_vtable);
    } catch(const Glib::Error &ex) {
        g_warning("Registration of object %s failed: %s", object_path.c_str(), ex.what());
        return 0;
    }

    m_registered_objects.emplace_back(RegisteredObject {
        registration_id,
        connection,
        object_path
    });

    return registration_id;
}

void org::kde::StatusNotifierWatcherStub::unregister_object()
{
    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->unregister_object(obj.id);
    }
    m_registered_objects.clear();
}

void org::kde::StatusNotifierWatcherStub::on_method_call(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &method_name,
    const Glib::VariantContainerBase &parameters,
    const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation)
{
    static_cast<void>(method_name); // maybe unused
    static_cast<void>(parameters); // maybe unused
    static_cast<void>(invocation); // maybe unused

    if (method_name.compare("RegisterStatusNotifierItem") == 0) {
        Glib::Variant<Glib::ustring> base_service;
        parameters.get_child(base_service, 0);
        Glib::ustring p_service = specialGetter(base_service);

        MethodInvocation methodInvocation(invocation);
        RegisterStatusNotifierItem(
            (p_service),
            methodInvocation);
    }

    if (method_name.compare("RegisterStatusNotifierHost") == 0) {
        Glib::Variant<Glib::ustring> base_service;
        parameters.get_child(base_service, 0);
        Glib::ustring p_service = specialGetter(base_service);

        MethodInvocation methodInvocation(invocation);
        RegisterStatusNotifierHost(
            (p_service),
            methodInvocation);
    }

}

void org::kde::StatusNotifierWatcherStub::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name)
{
    static_cast<void>(property); // maybe unused
    static_cast<void>(property_name); // maybe unused

    if (property_name.compare("RegisteredStatusNotifierItems") == 0) {

        property = Glib::Variant<std::vector<Glib::ustring>>::create((RegisteredStatusNotifierItems_get()));
    }

    if (property_name.compare("IsStatusNotifierHostRegistered") == 0) {

        property = Glib::Variant<bool>::create((IsStatusNotifierHostRegistered_get()));
    }

    if (property_name.compare("ProtocolVersion") == 0) {

        property = Glib::Variant<gint32>::create((ProtocolVersion_get()));
    }

}

bool org::kde::StatusNotifierWatcherStub::on_interface_set_property(
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name,
    const Glib::VariantBase &value)
{
    static_cast<void>(property_name); // maybe unused
    static_cast<void>(value); // maybe unused

    return true;
}

void org::kde::StatusNotifierWatcherStub::StatusNotifierItemRegistered_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,const Glib::ustring & service)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<Glib::ustring>::create((service)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierWatcher",
                    "StatusNotifierItemRegistered",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierWatcherStub::StatusNotifierItemUnregistered_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,const Glib::ustring & service)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<Glib::ustring>::create((service)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierWatcher",
                    "StatusNotifierItemUnregistered",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierWatcherStub::StatusNotifierHostRegistered_emitter(
    const std::vector<Glib::ustring> &destination_bus_names)
{
    std::vector<Glib::VariantBase> paramsList;


    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierWatcher",
                    "StatusNotifierHostRegistered",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierWatcherStub::StatusNotifierHostUnregistered_emitter(
    const std::vector<Glib::ustring> &destination_bus_names)
{
    std::vector<Glib::VariantBase> paramsList;


    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierWatcher",
                    "StatusNotifierHostUnregistered",
                    bus_name,
                    params);
        }
    }
}


bool org::kde::StatusNotifierWatcherStub::emitSignal(
    const std::string &propName,
    Glib::VariantBase &value)
{
    std::map<Glib::ustring, Glib::VariantBase> changedProps;
    std::vector<Glib::ustring> changedPropsNoValue;

    changedProps[propName] = value;

    Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>> changedPropsVar =
        Glib::Variant<std::map<Glib::ustring, Glib::VariantBase>>::create(changedProps);
    Glib::Variant<std::vector<Glib::ustring>> changedPropsNoValueVar =
        Glib::Variant<std::vector<Glib::ustring>>::create(changedPropsNoValue);
    std::vector<Glib::VariantBase> ps;
    ps.push_back(Glib::Variant<Glib::ustring>::create(m_interfaceName));
    ps.push_back(changedPropsVar);
    ps.push_back(changedPropsNoValueVar);
    Glib::VariantContainerBase propertiesChangedVariant =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(ps);

    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->emit_signal(
            obj.object_path,
            "org.freedesktop.DBus.Properties",
            "PropertiesChanged",
            Glib::ustring(),
            propertiesChangedVariant);
    }

    return true;
}
