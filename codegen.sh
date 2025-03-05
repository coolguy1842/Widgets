#!/bin/bash

cd gdbus-codegen-glibmm 
./gdbus-codegen-glibmm.py --generate-cpp-code=status-notifier-watcher-interface ../subprojects/gtkwidgetspp/data/introspect/org.kde.StatusNotifierWatcher.xml
./gdbus-codegen-glibmm.py --generate-cpp-code=status-notifier-item-interface ../subprojects/gtkwidgetspp/data/introspect/org.kde.StatusNotifierItem.xml

mv *.h ../subprojects/gtkwidgetspp/generated
mv *.cpp ../subprojects/gtkwidgetspp/generated
cd ..