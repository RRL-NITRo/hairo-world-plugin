/**
   \file
   \author  Kenta Suzuki
*/

#include "VirtualJoystickView2.h"
#include <cnoid/MenuManager>
#include <cnoid/ViewManager>
#include <QBoxLayout>
#include <QStackedLayout>
#include "VirtualJoystickWidget.h"
#include "OnScreenJoystickWidget.h"
#include "gettext.h"

using namespace std;
using namespace cnoid;

namespace cnoid {

class VirtualJoystickView2::Impl
{
public:
    VirtualJoystickView2* self;

    Impl(VirtualJoystickView2* self);

    bool isOnScreenJoystickEnabled;
    QStackedLayout* stackedLayout;
};

}


void VirtualJoystickView2::initializeClass(ExtensionManager* ext)
{
    ext->viewManager().registerClass<VirtualJoystickView2>(
        "VirtualJoystickView2", N_("Virtual Joystick2"), ViewManager::SINGLE_OPTIONAL);
}


VirtualJoystickView2::VirtualJoystickView2()
{
    impl = new Impl(this);
}


VirtualJoystickView2::Impl::Impl(VirtualJoystickView2* self)
    : self(self),
      isOnScreenJoystickEnabled(false)
{
    self->setDefaultLayoutArea(View::BottomCenterArea);

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(new VirtualJoystickWidget);
    // stackedLayout->addWidget(new OnScreenJoystickWidget);

    auto vbox = new QVBoxLayout;
    vbox->addLayout(stackedLayout);
    self->setLayout(vbox);
}


VirtualJoystickView2::~VirtualJoystickView2()
{
    delete impl;
}


void VirtualJoystickView2::onAttachedMenuRequest(MenuManager& menuManager)
{
    // auto screenCheck = menuManager.addCheckItem(_("On-screen Joystick"));
    // screenCheck->setChecked(impl->isOnScreenJoystickEnabled);
    // screenCheck->sigToggled().connect([&](bool checked){
    //     impl->isOnScreenJoystickEnabled = checked;
    //     impl->stackedLayout->setCurrentIndex(checked ? 1 : 0);
    // });
}


bool VirtualJoystickView2::storeState(Archive& archive)
{
    return true;
}


bool VirtualJoystickView2::restoreState(const Archive& archive)
{
    return true;
}