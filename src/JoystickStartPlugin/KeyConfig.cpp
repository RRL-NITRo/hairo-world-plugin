/**
   @author Kenta Suzuki
*/

#include "KeyConfig.h"
#include <cnoid/Button>
#include <cnoid/ComboBox>
#include <cnoid/Dialog>
#include <cnoid/Joystick>
#include <cnoid/Separator>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include "gettext.h"

using namespace std;
using namespace cnoid;

namespace {

struct ItemInfo {
    const char* name;
    int id;
};

ItemInfo axisInfo[] = {
    { "L_STICK_H_AXIS",                 Joystick::L_STICK_H_AXIS },
    { "L_STICK_V_AXIS",                 Joystick::L_STICK_V_AXIS },
    { "R_STICK_H_AXIS",                 Joystick::R_STICK_H_AXIS },
    { "R_STICK_V_AXIS",                 Joystick::R_STICK_V_AXIS },
    { "DIRECTIONAL_PAD_H_AXIS", Joystick::DIRECTIONAL_PAD_H_AXIS },
    { "DIRECTIONAL_PAD_V_AXIS", Joystick::DIRECTIONAL_PAD_V_AXIS },
    { "L_TRIGGER_AXIS",                 Joystick::L_TRIGGER_AXIS },
    { "R_TRIGGER_AXIS",                 Joystick::R_TRIGGER_AXIS }
};

ItemInfo buttonInfo[] = {
    { "A_BUTTON",             Joystick::A_BUTTON },
    { "B_BUTTON",             Joystick::B_BUTTON },
    { "X_BUTTON",             Joystick::X_BUTTON },
    { "Y_BUTTON",             Joystick::Y_BUTTON },
    { "L_BUTTON",             Joystick::L_BUTTON },
    { "R_BUTTON",             Joystick::R_BUTTON },
    { "SELECT_BUTTON",   Joystick::SELECT_BUTTON },
    { "START_BUTTON",     Joystick::START_BUTTON },
    { "L_STICK_BUTTON", Joystick::L_STICK_BUTTON },
    { "R_STICK_BUTTON", Joystick::R_STICK_BUTTON },
    { "LOGO_BUTTON",       Joystick::LOGO_BUTTON }
};

}

namespace cnoid {

class KeyConfig::Impl : public Dialog
{
public:

    Impl();

    ComboBox* axisCombos[Joystick::NUM_STD_AXES];
    ComboBox* buttonCombos[Joystick::NUM_STD_BUTTONS];
};

}


KeyConfig::KeyConfig()
{
    impl = new Impl;
}


KeyConfig::Impl::Impl()
    : Dialog()
{
    setWindowTitle(_("KeyConfig"));

    auto gridLayout = new QGridLayout;
    gridLayout->addWidget(new QLabel("Axis"), 0, 0);
    gridLayout->addWidget(new QLabel("ID"), 0, 1);
    for(int i = 0; i < Joystick::NUM_STD_AXES; ++i) {
        axisCombos[i] = new ComboBox;
        for(int j = 0; j < Joystick::NUM_STD_AXES; ++j) {
            ItemInfo info = axisInfo[j];
            axisCombos[i]->addItem(info.name);
        }
        axisCombos[i]->setCurrentIndex(i);
        gridLayout->addWidget(new QLabel(axisInfo[i].name));
        gridLayout->addWidget(axisCombos[i], i + 1, 1);
    }
    auto avbox = new QVBoxLayout;
    avbox->addLayout(gridLayout);
    avbox->addStretch();

    auto gridLayout2 = new QGridLayout;
    gridLayout2->addWidget(new QLabel("Button"), 0, 0);
    gridLayout2->addWidget(new QLabel("ID"), 0, 1);
    for(int i = 0; i < Joystick::NUM_STD_BUTTONS; ++i) {
        buttonCombos[i] = new ComboBox;
        for(int j = 0; j < Joystick::NUM_STD_BUTTONS; ++j) {
            ItemInfo info = buttonInfo[j];
            buttonCombos[i]->addItem(info.name);
        }
        buttonCombos[i]->setCurrentIndex(i);
        gridLayout2->addWidget(new QLabel(buttonInfo[i].name));
        gridLayout2->addWidget(buttonCombos[i], i + 1, 1);
    }
    auto bvbox = new QVBoxLayout;
    bvbox->addLayout(gridLayout2);
    bvbox->addStretch();

    auto buttonBox = new QDialogButtonBox(this);
    auto okButton = new PushButton(_("&Ok"));
    buttonBox->addButton(okButton, QDialogButtonBox::AcceptRole);
    connect(buttonBox, &QDialogButtonBox::accepted, [this](){ this->accept(); });

    auto hbox = new QHBoxLayout;
    hbox->addLayout(avbox);
    hbox->addWidget(new VSeparator);
    hbox->addLayout(bvbox);
    auto vbox = new QVBoxLayout;
    vbox->addLayout(hbox);
    vbox->addStretch();
    vbox->addWidget(new HSeparator);
    vbox->addWidget(buttonBox);
    setLayout(vbox);
}


KeyConfig::~KeyConfig()
{
    delete impl;
}


int KeyConfig::axisID(const int& axis)
{
    int index = 0;
    for(int i = 0; i < Joystick::NUM_STD_AXES; ++i) {
        ItemInfo info = axisInfo[i];
        if(info.id == axis) {
            index = i;
        }
    }
    return impl->axisCombos[index]->currentIndex();
}


int KeyConfig::buttonID(const int& button)
{
    int index = 0;
    for(int i = 0; i < Joystick::NUM_STD_BUTTONS; ++i) {
        ItemInfo info = buttonInfo[i];
        if(info.id == button) {
            index = i;
        }
    }
    return impl->buttonCombos[index]->currentIndex();
}


void KeyConfig::showConfig()
{
    impl->show();
}
