#pragma once
#include <string>
#include <tuple>
#include <vector>
#include <glibmm.h>
#include <giomm.h>
#include <status-notifier-item-interface_common.h>

namespace org {
namespace kde {

class StatusNotifierItemProxy : public Glib::ObjectBase {
public:
    static void createForBus(Gio::DBus::BusType busType,
                             Gio::DBus::ProxyFlags proxyFlags,
                             const std::string &name,
                             const std::string &objectPath,
                             const Gio::SlotAsyncReady &slot,
                             const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    static Glib::RefPtr<StatusNotifierItemProxy> createForBusFinish (const Glib::RefPtr<Gio::AsyncResult> &result);

    static Glib::RefPtr<StatusNotifierItemProxy> createForBus_sync(
        Gio::DBus::BusType busType,
        Gio::DBus::ProxyFlags proxyFlags,
        const std::string &name,
        const std::string &objectPath,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {});

    Glib::RefPtr<Gio::DBus::Proxy> dbusProxy() const { return m_proxy; }

    void ContextMenu(
        gint32 x,
        gint32 y,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void ContextMenu_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    ContextMenu_sync(
        gint32 x,        gint32 y,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Activate(
        gint32 x,
        gint32 y,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Activate_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    Activate_sync(
        gint32 x,        gint32 y,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void SecondaryActivate(
        gint32 x,
        gint32 y,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void SecondaryActivate_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    SecondaryActivate_sync(
        gint32 x,        gint32 y,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Scroll(
        gint32 delta,
        const Glib::ustring & orientation,
        const Gio::SlotAsyncReady &slot,
        const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    void Scroll_finish (
        const Glib::RefPtr<Gio::AsyncResult> &res);

    void
    Scroll_sync(
        gint32 delta,        const Glib::ustring & orientation,const Glib::RefPtr<Gio::Cancellable> &cancellable = {},
        int timeout_msec = -1);

    Glib::ustring Category_get(bool *ok = nullptr);
    sigc::signal<void()> &Category_changed() {
        return m_Category_changed;
    }

    Glib::ustring Id_get(bool *ok = nullptr);
    sigc::signal<void()> &Id_changed() {
        return m_Id_changed;
    }

    Glib::ustring Title_get(bool *ok = nullptr);
    sigc::signal<void()> &Title_changed() {
        return m_Title_changed;
    }

    Glib::ustring Status_get(bool *ok = nullptr);
    sigc::signal<void()> &Status_changed() {
        return m_Status_changed;
    }

    gint32 WindowId_get(bool *ok = nullptr);
    sigc::signal<void()> &WindowId_changed() {
        return m_WindowId_changed;
    }

    Glib::ustring IconThemePath_get(bool *ok = nullptr);
    sigc::signal<void()> &IconThemePath_changed() {
        return m_IconThemePath_changed;
    }

    Glib::DBusObjectPathString Menu_get(bool *ok = nullptr);
    sigc::signal<void()> &Menu_changed() {
        return m_Menu_changed;
    }

    bool ItemIsMenu_get(bool *ok = nullptr);
    sigc::signal<void()> &ItemIsMenu_changed() {
        return m_ItemIsMenu_changed;
    }

    Glib::ustring IconName_get(bool *ok = nullptr);
    sigc::signal<void()> &IconName_changed() {
        return m_IconName_changed;
    }

    std::vector<std::tuple<gint32,gint32,std::string>> IconPixmap_get(bool *ok = nullptr);
    sigc::signal<void()> &IconPixmap_changed() {
        return m_IconPixmap_changed;
    }

    Glib::ustring OverlayIconName_get(bool *ok = nullptr);
    sigc::signal<void()> &OverlayIconName_changed() {
        return m_OverlayIconName_changed;
    }

    std::vector<std::tuple<gint32,gint32,std::string>> OverlayIconPixmap_get(bool *ok = nullptr);
    sigc::signal<void()> &OverlayIconPixmap_changed() {
        return m_OverlayIconPixmap_changed;
    }

    Glib::ustring AttentionIconName_get(bool *ok = nullptr);
    sigc::signal<void()> &AttentionIconName_changed() {
        return m_AttentionIconName_changed;
    }

    std::vector<std::tuple<gint32,gint32,std::string>> AttentionIconPixmap_get(bool *ok = nullptr);
    sigc::signal<void()> &AttentionIconPixmap_changed() {
        return m_AttentionIconPixmap_changed;
    }

    Glib::ustring AttentionMovieName_get(bool *ok = nullptr);
    sigc::signal<void()> &AttentionMovieName_changed() {
        return m_AttentionMovieName_changed;
    }

    std::tuple<Glib::ustring,std::vector<std::tuple<gint32,gint32,std::string>>,Glib::ustring,Glib::ustring> ToolTip_get(bool *ok = nullptr);
    sigc::signal<void()> &ToolTip_changed() {
        return m_ToolTip_changed;
    }

    sigc::signal<void()> NewTitle_signal;
    sigc::signal<void()> NewIcon_signal;
    sigc::signal<void()> NewAttentionIcon_signal;
    sigc::signal<void()> NewOverlayIcon_signal;
    sigc::signal<void()> NewToolTip_signal;
    sigc::signal<void(Glib::ustring)> NewStatus_signal;

    void reference() const override {}
    void unreference() const override {}

protected:
    Glib::RefPtr<Gio::DBus::Proxy> m_proxy;

    StatusNotifierItemProxy(const Glib::RefPtr<Gio::DBus::Proxy> &proxy);

    void handle_signal(const Glib::ustring &sender_name,
                       const Glib::ustring &signal_name,
                       const Glib::VariantContainerBase &parameters);

    void handle_properties_changed(const Gio::DBus::Proxy::MapChangedProperties &changed_properties,
                                   const std::vector<Glib::ustring> &invalidated_properties);

    sigc::signal<void()> m_Category_changed;
    sigc::signal<void()> m_Id_changed;
    sigc::signal<void()> m_Title_changed;
    sigc::signal<void()> m_Status_changed;
    sigc::signal<void()> m_WindowId_changed;
    sigc::signal<void()> m_IconThemePath_changed;
    sigc::signal<void()> m_Menu_changed;
    sigc::signal<void()> m_ItemIsMenu_changed;
    sigc::signal<void()> m_IconName_changed;
    sigc::signal<void()> m_IconPixmap_changed;
    sigc::signal<void()> m_OverlayIconName_changed;
    sigc::signal<void()> m_OverlayIconPixmap_changed;
    sigc::signal<void()> m_AttentionIconName_changed;
    sigc::signal<void()> m_AttentionIconPixmap_changed;
    sigc::signal<void()> m_AttentionMovieName_changed;
    sigc::signal<void()> m_ToolTip_changed;
};

}// kde
}// org

