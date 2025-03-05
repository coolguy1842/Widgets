#pragma once
#include <string>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include <status-notifier-item-interface_common.h>

namespace org {
namespace kde {

class StatusNotifierItemStub : public sigc::trackable {
public:
    StatusNotifierItemStub();
    virtual ~StatusNotifierItemStub();

    StatusNotifierItemStub(const StatusNotifierItemStub &other) = delete;
    StatusNotifierItemStub(StatusNotifierItemStub &&other) = delete;
    StatusNotifierItemStub &operator=(const StatusNotifierItemStub &other) = delete;
    StatusNotifierItemStub &operator=(StatusNotifierItemStub &&other) = delete;

    guint register_object(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                          const Glib::ustring &object_path);
    void unregister_object();

    unsigned int usage_count() const {
        return static_cast<unsigned int>(m_registered_objects.size());
    }

    class MethodInvocation;

protected:
    virtual void ContextMenu(
        gint32 x,
        gint32 y,
        MethodInvocation &invocation) = 0;
    virtual void Activate(
        gint32 x,
        gint32 y,
        MethodInvocation &invocation) = 0;
    virtual void SecondaryActivate(
        gint32 x,
        gint32 y,
        MethodInvocation &invocation) = 0;
    virtual void Scroll(
        gint32 delta,
        const Glib::ustring & orientation,
        MethodInvocation &invocation) = 0;

    virtual Glib::ustring Category_get() = 0;

    virtual Glib::ustring Id_get() = 0;

    virtual Glib::ustring Title_get() = 0;

    virtual Glib::ustring Status_get() = 0;

    virtual gint32 WindowId_get() = 0;

    virtual Glib::ustring IconThemePath_get() = 0;

    virtual Glib::DBusObjectPathString Menu_get() = 0;

    virtual bool ItemIsMenu_get() = 0;

    virtual Glib::ustring IconName_get() = 0;

    virtual std::vector<std::tuple<gint32,gint32,std::string>> IconPixmap_get() = 0;

    virtual Glib::ustring OverlayIconName_get() = 0;

    virtual std::vector<std::tuple<gint32,gint32,std::string>> OverlayIconPixmap_get() = 0;

    virtual Glib::ustring AttentionIconName_get() = 0;

    virtual std::vector<std::tuple<gint32,gint32,std::string>> AttentionIconPixmap_get() = 0;

    virtual Glib::ustring AttentionMovieName_get() = 0;

    virtual std::tuple<Glib::ustring,std::vector<std::tuple<gint32,gint32,std::string>>,Glib::ustring,Glib::ustring> ToolTip_get() = 0;
    void NewTitle_emitter(const std::vector<Glib::ustring> &destination_bus_names);
    sigc::signal<void()> NewTitle_signal;
    sigc::signal<void(const std::vector<Glib::ustring>&)> NewTitle_selectiveSignal;
    void NewIcon_emitter(const std::vector<Glib::ustring> &destination_bus_names);
    sigc::signal<void()> NewIcon_signal;
    sigc::signal<void(const std::vector<Glib::ustring>&)> NewIcon_selectiveSignal;
    void NewAttentionIcon_emitter(const std::vector<Glib::ustring> &destination_bus_names);
    sigc::signal<void()> NewAttentionIcon_signal;
    sigc::signal<void(const std::vector<Glib::ustring>&)> NewAttentionIcon_selectiveSignal;
    void NewOverlayIcon_emitter(const std::vector<Glib::ustring> &destination_bus_names);
    sigc::signal<void()> NewOverlayIcon_signal;
    sigc::signal<void(const std::vector<Glib::ustring>&)> NewOverlayIcon_selectiveSignal;
    void NewToolTip_emitter(const std::vector<Glib::ustring> &destination_bus_names);
    sigc::signal<void()> NewToolTip_signal;
    sigc::signal<void(const std::vector<Glib::ustring>&)> NewToolTip_selectiveSignal;
    void NewStatus_emitter(const std::vector<Glib::ustring> &destination_bus_names, const Glib::ustring &);
    sigc::signal<void(const Glib::ustring &)> NewStatus_signal;
    sigc::signal<void(const std::vector<Glib::ustring>&, const Glib::ustring &)> NewStatus_selectiveSignal;

    void on_method_call(const Glib::RefPtr<Gio::DBus::Connection> &connection,
                        const Glib::ustring &sender,
                        const Glib::ustring &object_path,
                        const Glib::ustring &interface_name,
                        const Glib::ustring &method_name,
                        const Glib::VariantContainerBase &parameters,
                        const Glib::RefPtr<Gio::DBus::MethodInvocation> &invocation);

    void on_interface_get_property(Glib::VariantBase& property,
                                   const Glib::RefPtr<Gio::DBus::Connection> &connection,
                                   const Glib::ustring &sender,
                                   const Glib::ustring &object_path,
                                   const Glib::ustring &interface_name,
                                   const Glib::ustring &property_name);

    bool on_interface_set_property(
        const Glib::RefPtr<Gio::DBus::Connection> &connection,
        const Glib::ustring &sender,
        const Glib::ustring &object_path,
        const Glib::ustring &interface_name,
        const Glib::ustring &property_name,
        const Glib::VariantBase &value);

private:
    bool emitSignal(const std::string &propName, Glib::VariantBase &value);

    struct RegisteredObject {
        guint id;
        Glib::RefPtr<Gio::DBus::Connection> connection;
        std::string object_path;
    };

    Glib::RefPtr<Gio::DBus::NodeInfo> introspection_data;
    std::vector<RegisteredObject> m_registered_objects;
    std::string m_interfaceName;
};

class StatusNotifierItemStub::MethodInvocation {
public:
    MethodInvocation(const Glib::RefPtr<Gio::DBus::MethodInvocation> &msg):
        m_message(msg) {}

    const Glib::RefPtr<Gio::DBus::MethodInvocation> getMessage() {
        return m_message;
    }

    void ret(Glib::Error error) {
        m_message->return_error(error);
    }

    void returnError(const Glib::ustring &domain, int code, const Glib::ustring &message) {
        m_message->return_error(domain, code, message);
    }

    void ret() {
        std::vector<Glib::VariantBase> vlist;

        m_message->return_value(Glib::Variant<Glib::VariantBase>::create_tuple(vlist));
    }

private:
    Glib::RefPtr<Gio::DBus::MethodInvocation> m_message;
};

} // kde
} // org

