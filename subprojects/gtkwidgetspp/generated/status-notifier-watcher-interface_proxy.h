#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include <status-notifier-watcher-interface_common.h>

namespace org {
namespace kde {

class StatusNotifierWatcherProxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<StatusNotifierWatcherProxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<StatusNotifierWatcherProxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void RegisterStatusNotifierItem(
        const Glib::ustring & service,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void RegisterStatusNotifierItem_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    RegisterStatusNotifierItem_sync(
        const Glib::ustring & service,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void RegisterStatusNotifierHost(
        const Glib::ustring & service,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void RegisterStatusNotifierHost_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    RegisterStatusNotifierHost_sync(
        const Glib::ustring & service,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    std::vector<Glib::ustring> RegisteredStatusNotifierItems_get(bool *ok = nullptr);
    sigc::signal<void()> &RegisteredStatusNotifierItems_changed() {
        return m_RegisteredStatusNotifierItems_changed;
    }

    bool IsStatusNotifierHostRegistered_get(bool *ok = nullptr);
    sigc::signal<void()> &IsStatusNotifierHostRegistered_changed() {
        return m_IsStatusNotifierHostRegistered_changed;
    }

    gint32 ProtocolVersion_get(bool *ok = nullptr);
    sigc::signal<void()> &ProtocolVersion_changed() {
        return m_ProtocolVersion_changed;
    }

    sigc::signal<void(Glib::ustring)> StatusNotifierItemRegistered_signal;
    sigc::signal<void(Glib::ustring)> StatusNotifierItemUnregistered_signal;
    sigc::signal<void()> StatusNotifierHostRegistered_signal;
    sigc::signal<void()> StatusNotifierHostUnregistered_signal;

    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

    StatusNotifierWatcherProxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

    sigc::signal<void()> m_RegisteredStatusNotifierItems_changed;
    sigc::signal<void()> m_IsStatusNotifierHostRegistered_changed;
    sigc::signal<void()> m_ProtocolVersion_changed;
};

}// kde
}// org

