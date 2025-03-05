#pragma once
#include <iostream>
#include <vector>
#include <glibmm.h>
#include <giomm.h>

namespace org {
namespace kde {

class StatusNotifierWatcherTypeWrap {
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

    static Glib::VariantContainerBase RegisterStatusNotifierItem_pack(
        const Glib::ustring & arg_service) {
        Glib::VariantContainerBase base;
        Glib::Variant<Glib::ustring> params =
            Glib::Variant<Glib::ustring>::create(arg_service);
        return Glib::VariantContainerBase::create_tuple(params);
    }

    static Glib::VariantContainerBase RegisterStatusNotifierHost_pack(
        const Glib::ustring & arg_service) {
        Glib::VariantContainerBase base;
        Glib::Variant<Glib::ustring> params =
            Glib::Variant<Glib::ustring>::create(arg_service);
        return Glib::VariantContainerBase::create_tuple(params);
    }
};

} // kde
} // org


