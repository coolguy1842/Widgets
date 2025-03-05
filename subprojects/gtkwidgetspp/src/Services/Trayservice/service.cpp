#include <fmt/format.h>

#include <Services/TrayService.hpp>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "glibmm/refptr.h"
#include "glibmm/ustring.h"
#include "sigc++/functors/mem_fun.h"

void Services::Tray::Service::itemAdded(Glib::RefPtr<TrayItem> proxy) {
    printf("item added\n");
    pathLookups[proxy.get()] = fmt::format("{}{}", proxy->getProxy()->get_name().c_str(), proxy->getProxy()->get_object_path().c_str());

    auto newItems = get_items();
    newItems.push_back(proxy);

    _property_items.set_value(newItems);
    _signal_item_added.emit(proxy);
}

void Services::Tray::Service::itemRemoved(Glib::RefPtr<TrayItem> proxy) {
    printf("item removed\n");
    pathLookups.erase(proxy.get());

    auto newItems = get_items();
    auto it       = std::find_if(
        newItems.begin(),
        newItems.end(),
        [&](const Glib::RefPtr<TrayItem>& itemProxy) {
            return itemProxy.get() == proxy.get();
        }
    );

    if(it != newItems.end()) {
        newItems.erase(it);
    }

    _property_items.set_value(newItems);
    _signal_item_removed.emit(proxy);
}

Services::Tray::Service::~Service() {}
Services::Tray::Service::Service()
    : Glib::ObjectBase(typeid(Services::Tray::Service))
    , _property_items(*this, "items") {
    uint32_t owner_id = Gio::DBus::own_name(
        Gio::DBus::BusType::SESSION,
        "org.kde.StatusNotifierWatcher",
        [&](const Glib::RefPtr<Gio::DBus::Connection>& con, Glib::ustring name) {
            // con->call(
            //     "/org/ayatana/NotificationItem/nm_applet",
            //     "org.freedesktop.DBus.Properties",
            //     "GetAll",
            //     Glib::VariantContainerBase::create_tuple(Glib::Variant<Glib::ustring>::create("org.kde.StatusNotifierItem")),
            //     [con](const Glib::RefPtr<Gio::AsyncResult>& res) {
            //         auto variant = con->call_finish(res);
            //     },
            //     Glib::RefPtr<Gio::Cancellable>(),
            //     ":1.6651",
            //     -1,
            //     Gio::DBus::CallFlags::NONE,
            //     Glib::VariantType::create_tuple({ Glib::VariantType::create_array(Glib::VariantType::create_dict_entry(Glib::VARIANT_TYPE_STRING, Glib::VARIANT_TYPE_VARIANT)) })
            // );
        },
        [&](const Glib::RefPtr<Gio::DBus::Connection>& con, Glib::ustring name) {
            printf("name acquired on %s\n", name.c_str());

            if(watcher.register_object(con, "/StatusNotifierWatcher") == 0) {
                printf("ERROR: Failed to register object on /StatusNotifierWatcher\n");

                Gio::DBus::unown_name(owner_id);
            }
            else {
                printf("successfully registered object\n");
            }
        },
        [&](const Glib::RefPtr<Gio::DBus::Connection>& con, Glib::ustring name) {
            printf("ERROR: Lost name for %s\n", name.c_str());
        },
        Gio::DBus::BusNameOwnerFlags::REPLACE
    );

    watcher.signal_item_added().connect(sigc::mem_fun(*this, &Service::itemAdded));
    watcher.signal_item_removed().connect(sigc::mem_fun(*this, &Service::itemRemoved));
}

#pragma region __SINGLETON__

static Services::Tray::Service* instance = nullptr;
Services::Tray::Service* Services::Tray::Service::getInstance() {
    if(instance != nullptr) {
        return instance;
    }

    instance = new Services::Tray::Service();
    return instance;
}

void Services::Tray::Service::closeInstance() {
    if(instance == nullptr) {
        return;
    }

    delete instance;
    instance = nullptr;
}

#pragma endregion