#pragma once
#include <iostream>
#include <vector>
#include <glibmm.h>
#include <giomm.h>

namespace org {
namespace kde {

class StatusNotifierItemTypeWrap {
public:
    template<typename T>
    static void unwrapList(std::vector<T> &list, const Glib::VariantContainerBase &wrapped) {
        for (uint i = 0; i < wrapped.get_n_children(); i++) {
            Glib::Variant<T> item;
            wrapped.get_child(item, i);
            list.push_back(item.get());
        }
    }

    static std::vector<Glib::ustring> stdStringVecToGlibStringVec(const std::vector<std::string> &strv) {
        std::vector<Glib::ustring> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static std::vector<std::string> glibStringVecToStdStringVec(const std::vector<Glib::ustring> &strv) {
        std::vector<std::string> newStrv;
        for (uint i = 0; i < strv.size(); i++) {
            newStrv.push_back(strv[i]);
        }

        return newStrv;
    }

    static Glib::VariantContainerBase ContextMenu_pack(
        gint32 arg_x,
        gint32 arg_y) {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;

        Glib::Variant<gint32> x_param =
            Glib::Variant<gint32>::create(arg_x);
        params.push_back(x_param);

        Glib::Variant<gint32> y_param =
            Glib::Variant<gint32>::create(arg_y);
        params.push_back(y_param);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase Activate_pack(
        gint32 arg_x,
        gint32 arg_y) {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;

        Glib::Variant<gint32> x_param =
            Glib::Variant<gint32>::create(arg_x);
        params.push_back(x_param);

        Glib::Variant<gint32> y_param =
            Glib::Variant<gint32>::create(arg_y);
        params.push_back(y_param);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase SecondaryActivate_pack(
        gint32 arg_x,
        gint32 arg_y) {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;

        Glib::Variant<gint32> x_param =
            Glib::Variant<gint32>::create(arg_x);
        params.push_back(x_param);

        Glib::Variant<gint32> y_param =
            Glib::Variant<gint32>::create(arg_y);
        params.push_back(y_param);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase Scroll_pack(
        gint32 arg_delta,
        const Glib::ustring & arg_orientation) {
        Glib::VariantContainerBase base;
        std::vector<Glib::VariantBase> params;

        Glib::Variant<gint32> delta_param =
            Glib::Variant<gint32>::create(arg_delta);
        params.push_back(delta_param);

        Glib::Variant<Glib::ustring> orientation_param =
            Glib::Variant<Glib::ustring>::create(arg_orientation);
        params.push_back(orientation_param);
        return Glib::VariantContainerBase::create_tuple(params);
    }
};

} // kde
} // org


