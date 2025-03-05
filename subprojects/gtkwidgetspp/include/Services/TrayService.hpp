#pragma once

#include <gtkmm-4.0/gtkmm.h>
#include <status-notifier-item-interface_stub.h>
#include <status-notifier-watcher-interface_stub.h>

#include <Utils/GLibUtil.hpp>
#include <algorithm>
#include <format>
#include <map>
#include <vector>

#include "giomm/asyncresult.h"
#include "giomm/dbusconnection.h"
#include "giomm/dbusmenumodel.h"
#include "giomm/dbusproxy.h"
#include "glibmm/object.h"
#include "glibmm/objectbase.h"
#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "glibmm/value.h"
#include "glibmm/variant.h"
#include "sigc++/functors/mem_fun.h"
#include "status-notifier-item-interface_proxy.h"

class StatusNotifierItem : public org::kde::StatusNotifierItemProxy, virtual Glib::Object {
    MAKE_PROPERTY(Glib::RefPtr<Gio::DBus::MenuModel>, menu);

protected:
    Glib::RefPtr<Gio::DBus::Connection> _connection;

    void onDBusConnectionGet(const Glib::RefPtr<Gio::AsyncResult>& res) {
        _connection = Gio::DBus::Connection::get_finish(res);
        _property_menu.set_value(Gio::DBus::MenuModel::get(_connection, this->dbusProxy()->get_name_owner(), Menu_get()));
    }

    StatusNotifierItem(Glib::RefPtr<Gio::DBus::Proxy> proxy)
        : Glib::ObjectBase(typeid(StatusNotifierItem))
        , org::kde::StatusNotifierItemProxy(proxy)
        , _property_menu(*this, "menu") {
        Gio::DBus::Connection::get(Gio::DBus::BusType::SESSION, sigc::mem_fun(*this, &StatusNotifierItem::onDBusConnectionGet));
    }

public:
    virtual ~StatusNotifierItem() {}

    static void createForBus(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string& name,
        const std::string& objectPath,
        const Gio::SlotAsyncReady& slot,
        const Glib::RefPtr<Gio::Cancellable>& cancellable
    ) {
        Gio::DBus::Proxy::create_for_bus(busType, name, objectPath, "org.kde.StatusNotifierItem", slot, cancellable, Glib::RefPtr<Gio::DBus::InterfaceInfo>(), proxyFlags);
    }

    static Glib::RefPtr<StatusNotifierItem> createForBusFinish(const Glib::RefPtr<Gio::AsyncResult>& result) {
        Glib::RefPtr<Gio::DBus::Proxy> proxy = Gio::DBus::Proxy::create_for_bus_finish(result);
        StatusNotifierItem* p                = new StatusNotifierItem(proxy);

        return Glib::RefPtr<StatusNotifierItem>(p);
    }

    Glib::RefPtr<StatusNotifierItem> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string& name,
        const std::string& objectPath,
        const Glib::RefPtr<Gio::Cancellable>& cancellable
    ) {
        Glib::RefPtr<Gio::DBus::Proxy> proxy = Gio::DBus::Proxy::create_for_bus_sync(busType, name, objectPath, "org.kde.StatusNotifierItem", cancellable, Glib::RefPtr<Gio::DBus::InterfaceInfo>(), proxyFlags);
        StatusNotifierItem* p                = new StatusNotifierItem(proxy);

        return Glib::RefPtr<StatusNotifierItem>(p);
    }
};

//

//

class TrayItem : public Glib::Object {
private:
    Glib::RefPtr<Gio::DBus::Proxy> _proxy;

    void handle_signal(
        const Glib::ustring& sender_name,
        const Glib::ustring& signal_name,
        const Glib::VariantContainerBase& parameters
    ) {
        printf("signal emitted: %s\n", signal_name.c_str());
    }

    void handle_properties_changed(
        const Gio::DBus::Proxy::MapChangedProperties& changed_properties,
        const std::vector<Glib::ustring>& invalidated_properties
    ) {
        printf("properties changed\n");
    }

public:
    ~TrayItem() {}
    TrayItem(std::string busName, std::string objectPath)
        : Glib::ObjectBase(typeid(TrayItem)) {
        _proxy = Gio::DBus::Proxy::create_for_bus_sync(
            Gio::DBus::BusType::SESSION,
            busName,
            objectPath,
            "org.kde.StatusNotifierItem",
            Glib::RefPtr<Gio::DBus::InterfaceInfo>(),
            Gio::DBus::ProxyFlags::NONE
        );

        _proxy->signal_signal().connect(sigc::mem_fun(*this, &TrayItem::handle_signal));
        _proxy->signal_properties_changed().connect(sigc::mem_fun(*this, &TrayItem::handle_properties_changed));
    }

    std::string getId() {
        Glib::Variant<Glib::ustring> id;
        _proxy->get_cached_property(id, "Id");

        return id.get();
    }

    std::string getIconName() {
        Glib::Variant<Glib::ustring> iconName;
        _proxy->get_cached_property(iconName, "IconName");

        return iconName.get();
    }

    std::string getIconThemePath() {
        Glib::Variant<Glib::ustring> iconThemePath;
        _proxy->get_cached_property(iconThemePath, "IconThemePath");

        return iconThemePath.get();
    }

    Glib::RefPtr<Gio::DBus::Proxy> getProxy() { return _proxy; }
};

#define DEBUG_DBUS
class StatusNotifierWatcher : public org::kde::StatusNotifierWatcherStub {
    MAKE_SIGNAL(item_added, Glib::RefPtr<TrayItem>);
    MAKE_SIGNAL(item_removed, Glib::RefPtr<TrayItem>);

private:
    std::vector<Glib::RefPtr<TrayItem>> items;

public:
    ~StatusNotifierWatcher() {}
    StatusNotifierWatcher() {}

    // methods
    void RegisterStatusNotifierItem(const Glib::ustring& service, MethodInvocation& invocation) {
        std::string busName    = invocation.getMessage()->get_sender();
        std::string objectPath = service;

        if(!objectPath.starts_with("/")) {
            busName    = service;
            objectPath = "/StatusNotifierItem";
        }

        items.push_back(Glib::make_refptr_for_instance(new TrayItem(busName, objectPath)));

        _signal_item_added.emit(items.back());
        StatusNotifierItemRegistered_signal.emit("path");

        // org::kde::StatusNotifierItemProxy::createForBus(
        //     Gio::DBus::BusType::SESSION,
        //     Gio::DBus::ProxyFlags::NONE,
        //     busName,
        //     objectPath,
        //     sigc::mem_fun(*this, &StatusNotifierWatcher::itemProxyCreated)
        // );

#ifdef DEBUG_DBUS
        printf("register status notifier item called for %s\n", service.c_str());
        printf("sender: %s\n", invocation.getMessage()->get_sender().c_str());
#endif

        invocation.ret();
    }

    void RegisterStatusNotifierHost(const Glib::ustring& service, MethodInvocation& invocation) {
#ifdef DEBUG_DBUS
        printf("register status notifier host called\n");
#endif
        invocation.ret();
    }

    // getters
    std::vector<Glib::ustring> RegisteredStatusNotifierItems_get() {
#ifdef DEBUG_DBUS
        printf("registered status notifier items get called\n");
#endif
        return {};
    }

    bool IsStatusNotifierHostRegistered_get() {
#ifdef DEBUG_DBUS
        printf("is status notifier host registered get called\n");
#endif
        return true;
    }

    gint32 ProtocolVersion_get() { return 0; }
};

namespace Services::Tray {

class Service : public Glib::Object {
    std::map<TrayItem*, std::string> pathLookups;

    MAKE_PROPERTY(std::vector<Glib::RefPtr<TrayItem>>, items);

    MAKE_SIGNAL(item_added, Glib::RefPtr<TrayItem>);
    MAKE_SIGNAL(item_removed, Glib::RefPtr<TrayItem>);

private:
    StatusNotifierWatcher watcher;

    Service();
    ~Service();

    void itemAdded(Glib::RefPtr<TrayItem> proxy);
    void itemRemoved(Glib::RefPtr<TrayItem> proxy);

public:
    static Service* getInstance();
    static void closeInstance();
};

};  // namespace Services::Tray