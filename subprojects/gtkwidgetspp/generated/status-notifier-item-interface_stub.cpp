static const char interfaceXml0[] = R"XML_DELIMITER(<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE node PUBLIC "-//freedesktop//DTD D-BUS Object Introspection 1.0//EN"
"http://www.freedesktop.org/standards/dbus/1.0/introspect.dtd">
<node>
  <interface name="org.kde.StatusNotifierItem">

    <property name="Category" type="s" access="read"/>
    <property name="Id" type="s" access="read"/>
    <property name="Title" type="s" access="read"/>
    <property name="Status" type="s" access="read"/>
    <property name="WindowId" type="i" access="read"/>
    <property name="IconThemePath" type="s" access="read"/>
    <property name="Menu" type="o" access="read"/>
    <property name="ItemIsMenu" type="b" access="read"/>
    <property name="IconName" type="s" access="read"/>
    <property name="IconPixmap" type="a(iiay)" access="read">
      <annotation name="org.qtproject.QtDBus.QtTypeName" value="IconPixmapList"/>
    </property>
    <property name="OverlayIconName" type="s" access="read"/>
    <property name="OverlayIconPixmap" type="a(iiay)" access="read">
      <annotation name="org.qtproject.QtDBus.QtTypeName" value="IconPixmapList"/>
    </property>
    <property name="AttentionIconName" type="s" access="read"/>
    <property name="AttentionIconPixmap" type="a(iiay)" access="read">
      <annotation name="org.qtproject.QtDBus.QtTypeName" value="IconPixmapList"/>
    </property>
    <property name="AttentionMovieName" type="s" access="read"/>
    <property name="ToolTip" type="(sa(iiay)ss)" access="read">
      <annotation name="org.qtproject.QtDBus.QtTypeName" value="ToolTip"/>
    </property>
    <method name="ContextMenu">
        <arg name="x" type="i" direction="in"/>
        <arg name="y" type="i" direction="in"/>
    </method>

    <method name="Activate">
        <arg name="x" type="i" direction="in"/>
        <arg name="y" type="i" direction="in"/>
    </method>

    <method name="SecondaryActivate">
        <arg name="x" type="i" direction="in"/>
        <arg name="y" type="i" direction="in"/>
    </method>

    <method name="Scroll">
      <arg name="delta" type="i" direction="in"/>
      <arg name="orientation" type="s" direction="in"/>
    </method>

    <signal name="NewTitle">
    </signal>

    <signal name="NewIcon">
    </signal>

    <signal name="NewAttentionIcon">
    </signal>

    <signal name="NewOverlayIcon">
    </signal>

    <signal name="NewToolTip">
    </signal>

    <signal name="NewStatus">
      <arg name="status" type="s"/>
    </signal>

  </interface>
</node>)XML_DELIMITER";

#include "status-notifier-item-interface_stub.h"

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

org::kde::StatusNotifierItemStub::StatusNotifierItemStub():
    m_interfaceName("org.kde.StatusNotifierItem")
{
    NewTitle_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierItemStub::NewTitle_emitter),
            std::vector<Glib::ustring>({""})) );
    NewTitle_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierItemStub::NewTitle_emitter));
    NewIcon_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierItemStub::NewIcon_emitter),
            std::vector<Glib::ustring>({""})) );
    NewIcon_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierItemStub::NewIcon_emitter));
    NewAttentionIcon_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierItemStub::NewAttentionIcon_emitter),
            std::vector<Glib::ustring>({""})) );
    NewAttentionIcon_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierItemStub::NewAttentionIcon_emitter));
    NewOverlayIcon_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierItemStub::NewOverlayIcon_emitter),
            std::vector<Glib::ustring>({""})) );
    NewOverlayIcon_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierItemStub::NewOverlayIcon_emitter));
    NewToolTip_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierItemStub::NewToolTip_emitter),
            std::vector<Glib::ustring>({""})) );
    NewToolTip_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierItemStub::NewToolTip_emitter));
    NewStatus_signal.connect(sigc::bind<0>(sigc::mem_fun(*this, &StatusNotifierItemStub::NewStatus_emitter),
            std::vector<Glib::ustring>({""})) );
    NewStatus_selectiveSignal.connect(sigc::mem_fun(*this, &StatusNotifierItemStub::NewStatus_emitter));
}

org::kde::StatusNotifierItemStub::~StatusNotifierItemStub()
{
    unregister_object();
}

guint org::kde::StatusNotifierItemStub::register_object(
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
            sigc::mem_fun(*this, &StatusNotifierItemStub::on_method_call),
            sigc::mem_fun(*this, &StatusNotifierItemStub::on_interface_get_property),
            sigc::mem_fun(*this, &StatusNotifierItemStub::on_interface_set_property));

    guint registration_id;
    try {
        registration_id = connection->register_object(object_path,
            introspection_data->lookup_interface("org.kde.StatusNotifierItem"),
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

void org::kde::StatusNotifierItemStub::unregister_object()
{
    for (const RegisteredObject &obj: m_registered_objects) {
        obj.connection->unregister_object(obj.id);
    }
    m_registered_objects.clear();
}

void org::kde::StatusNotifierItemStub::on_method_call(
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

    if (method_name.compare("ContextMenu") == 0) {
        Glib::Variant<gint32> base_x;
        parameters.get_child(base_x, 0);
        gint32 p_x = specialGetter(base_x);

        Glib::Variant<gint32> base_y;
        parameters.get_child(base_y, 1);
        gint32 p_y = specialGetter(base_y);

        MethodInvocation methodInvocation(invocation);
        ContextMenu(
            (p_x),
            (p_y),
            methodInvocation);
    }

    if (method_name.compare("Activate") == 0) {
        Glib::Variant<gint32> base_x;
        parameters.get_child(base_x, 0);
        gint32 p_x = specialGetter(base_x);

        Glib::Variant<gint32> base_y;
        parameters.get_child(base_y, 1);
        gint32 p_y = specialGetter(base_y);

        MethodInvocation methodInvocation(invocation);
        Activate(
            (p_x),
            (p_y),
            methodInvocation);
    }

    if (method_name.compare("SecondaryActivate") == 0) {
        Glib::Variant<gint32> base_x;
        parameters.get_child(base_x, 0);
        gint32 p_x = specialGetter(base_x);

        Glib::Variant<gint32> base_y;
        parameters.get_child(base_y, 1);
        gint32 p_y = specialGetter(base_y);

        MethodInvocation methodInvocation(invocation);
        SecondaryActivate(
            (p_x),
            (p_y),
            methodInvocation);
    }

    if (method_name.compare("Scroll") == 0) {
        Glib::Variant<gint32> base_delta;
        parameters.get_child(base_delta, 0);
        gint32 p_delta = specialGetter(base_delta);

        Glib::Variant<Glib::ustring> base_orientation;
        parameters.get_child(base_orientation, 1);
        Glib::ustring p_orientation = specialGetter(base_orientation);

        MethodInvocation methodInvocation(invocation);
        Scroll(
            (p_delta),
            (p_orientation),
            methodInvocation);
    }

}

void org::kde::StatusNotifierItemStub::on_interface_get_property(
    Glib::VariantBase &property,
    const Glib::RefPtr<Gio::DBus::Connection> &/* connection */,
    const Glib::ustring &/* sender */,
    const Glib::ustring &/* object_path */,
    const Glib::ustring &/* interface_name */,
    const Glib::ustring &property_name)
{
    static_cast<void>(property); // maybe unused
    static_cast<void>(property_name); // maybe unused

    if (property_name.compare("Category") == 0) {

        property = Glib::Variant<Glib::ustring>::create((Category_get()));
    }

    if (property_name.compare("Id") == 0) {

        property = Glib::Variant<Glib::ustring>::create((Id_get()));
    }

    if (property_name.compare("Title") == 0) {

        property = Glib::Variant<Glib::ustring>::create((Title_get()));
    }

    if (property_name.compare("Status") == 0) {

        property = Glib::Variant<Glib::ustring>::create((Status_get()));
    }

    if (property_name.compare("WindowId") == 0) {

        property = Glib::Variant<gint32>::create((WindowId_get()));
    }

    if (property_name.compare("IconThemePath") == 0) {

        property = Glib::Variant<Glib::ustring>::create((IconThemePath_get()));
    }

    if (property_name.compare("Menu") == 0) {

        property = Glib::Variant<Glib::DBusObjectPathString>::create((Menu_get()));
    }

    if (property_name.compare("ItemIsMenu") == 0) {

        property = Glib::Variant<bool>::create((ItemIsMenu_get()));
    }

    if (property_name.compare("IconName") == 0) {

        property = Glib::Variant<Glib::ustring>::create((IconName_get()));
    }

    if (property_name.compare("IconPixmap") == 0) {

        property = Glib::Variant<std::vector<std::tuple<gint32,gint32,std::string>>>::create((IconPixmap_get()));
    }

    if (property_name.compare("OverlayIconName") == 0) {

        property = Glib::Variant<Glib::ustring>::create((OverlayIconName_get()));
    }

    if (property_name.compare("OverlayIconPixmap") == 0) {

        property = Glib::Variant<std::vector<std::tuple<gint32,gint32,std::string>>>::create((OverlayIconPixmap_get()));
    }

    if (property_name.compare("AttentionIconName") == 0) {

        property = Glib::Variant<Glib::ustring>::create((AttentionIconName_get()));
    }

    if (property_name.compare("AttentionIconPixmap") == 0) {

        property = Glib::Variant<std::vector<std::tuple<gint32,gint32,std::string>>>::create((AttentionIconPixmap_get()));
    }

    if (property_name.compare("AttentionMovieName") == 0) {

        property = Glib::Variant<Glib::ustring>::create((AttentionMovieName_get()));
    }

    if (property_name.compare("ToolTip") == 0) {

        property = Glib::Variant<std::tuple<Glib::ustring,std::vector<std::tuple<gint32,gint32,std::string>>,Glib::ustring,Glib::ustring>>::create((ToolTip_get()));
    }

}

bool org::kde::StatusNotifierItemStub::on_interface_set_property(
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

void org::kde::StatusNotifierItemStub::NewTitle_emitter(
    const std::vector<Glib::ustring> &destination_bus_names)
{
    std::vector<Glib::VariantBase> paramsList;


    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierItem",
                    "NewTitle",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierItemStub::NewIcon_emitter(
    const std::vector<Glib::ustring> &destination_bus_names)
{
    std::vector<Glib::VariantBase> paramsList;


    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierItem",
                    "NewIcon",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierItemStub::NewAttentionIcon_emitter(
    const std::vector<Glib::ustring> &destination_bus_names)
{
    std::vector<Glib::VariantBase> paramsList;


    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierItem",
                    "NewAttentionIcon",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierItemStub::NewOverlayIcon_emitter(
    const std::vector<Glib::ustring> &destination_bus_names)
{
    std::vector<Glib::VariantBase> paramsList;


    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierItem",
                    "NewOverlayIcon",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierItemStub::NewToolTip_emitter(
    const std::vector<Glib::ustring> &destination_bus_names)
{
    std::vector<Glib::VariantBase> paramsList;


    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierItem",
                    "NewToolTip",
                    bus_name,
                    params);
        }
    }
}

void org::kde::StatusNotifierItemStub::NewStatus_emitter(
    const std::vector<Glib::ustring> &destination_bus_names,const Glib::ustring & status)
{
    std::vector<Glib::VariantBase> paramsList;

    paramsList.push_back(Glib::Variant<Glib::ustring>::create((status)));;

    const Glib::VariantContainerBase params =
        Glib::Variant<std::vector<Glib::VariantBase>>::create_tuple(paramsList);
    for (const RegisteredObject &obj: m_registered_objects) {
        for (const auto &bus_name: destination_bus_names) {
            obj.connection->emit_signal(
                    obj.object_path,
                    "org.kde.StatusNotifierItem",
                    "NewStatus",
                    bus_name,
                    params);
        }
    }
}


bool org::kde::StatusNotifierItemStub::emitSignal(
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
