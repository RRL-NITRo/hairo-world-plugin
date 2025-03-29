/**
   @author Kenta Suzuki
*/

#include "JoystickTester.h"
#include <cnoid/Buttons>
#include <cnoid/Dialog>
#include <cnoid/ExtensionManager>
#include <cnoid/Format>
#include <cnoid/JoystickCapture>
#include <cnoid/MainMenu>
#include <cnoid/Separator>
#include <cnoid/stdx/filesystem>
#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QProgressBar>
#include <fcntl.h>
#include <linux/joystick.h>
#include <map>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include "gettext.h"

using namespace std;
using namespace cnoid;
namespace filesystem = stdx::filesystem;

namespace {

const map<string, int> modelIdMap = {
    { "Sony Computer Entertainment Wireless Controller", 0 },
    { "Sony Interactive Entertainment Wireless Controller", 0 },
    { "Sony Interactive Entertainment DUALSHOCK®4 USB Wireless Adaptor", 0 },
    { "Sony PLAYSTATION(R)3 Controller", 0 },
    { "Microsoft X-Box 360 pad", 0 },
    { "Microsoft X-Box One pad", 0 },
    { "Logitech Gamepad F310", 0 },
    { "Logicool Logicool Dual Action", 0 }
};

class TesterDialog : public QDialog
{
public:
    TesterDialog(QWidget* parent = nullptr);

private:
    void onAxis(int id, const double& position);
    void onButton(int id, bool isPressed);

    JoystickCapture joystick;
    vector<QProgressBar*> bars;
    vector<QPushButton*> buttons;
    QDialogButtonBox* buttonBox;
};

}


void JoystickTester::initializeClass(ExtensionManager* ext)
{
    static TesterDialog* dialog = nullptr;

    if(!dialog) {
        dialog = ext->manage(new TesterDialog);

        MainMenu::instance()->add_Tools_Item(
            _("Joystick Tester"), [](){ dialog->show(); });
    }
}


JoystickTester::JoystickTester()
{

}


JoystickTester::~JoystickTester()
{

}


TesterDialog::TesterDialog(QWidget* parent)
    : QDialog(parent)
{
    joystick.setDevice("/dev/input/js0");

    joystick.sigAxis().connect(
        [&](int id, double position){ onAxis(id, position); });

    joystick.sigButton().connect(
        [&](int id, bool isPressed){ onButton(id, isPressed); });

    QGroupBox* groupBox = new QGroupBox(_("Axes"));
    auto vbox = new QVBoxLayout;
    auto gridLayout = new QGridLayout;
    for(int i = 0; i < joystick.numAxes(); ++i) {
        QProgressBar* bar = new QProgressBar;
        bar->setValue(0);
        bar->setRange(-100, 100);
        bar->setFormat(formatC("{0:.3}%", 0.0).c_str());
        bars.push_back(bar);
        const string label = "Axis " + to_string(i) + ":";
        gridLayout->addWidget(new QLabel(label.c_str()), i, 0);
        gridLayout->addWidget(bar, i, 1);
    }
    vbox->addLayout(gridLayout);
    vbox->addStretch();
    groupBox->setLayout(vbox);

    QGroupBox* groupBox2 = new QGroupBox(_("Buttons"));
    vbox = new QVBoxLayout;
    for(int i = 0; i < joystick.numButtons(); ++i) {
        PushButton* button = new PushButton(to_string(i).c_str());
        buttons.push_back(button);
        vbox->addWidget(button);
    }
    vbox->addStretch();
    groupBox2->setLayout(vbox);

    auto layout = new QHBoxLayout;
    layout->addWidget(groupBox);
    layout->addWidget(groupBox2);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, &QDialogButtonBox::accepted, [&](){ accept(); });

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addStretch();
    mainLayout->addWidget(new HSeparator);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(_("Joystick Tester"));

    int id = 0;
    while(true) {
        string file = formatC("/dev/input/js{}", id);
        if(!filesystem::exists(filesystem::path(file))) {
            break;
        }
        int tmpfd = ::open(file.c_str(), O_RDONLY | O_NONBLOCK);
        if(tmpfd < 0) {
            break;
        }

        // check the josytick model name
        char buf[1024];
        if(ioctl(tmpfd, JSIOCGNAME(sizeof(buf)), buf) < 0) {
            ::close(tmpfd);
            break;
        }
        string identifier(buf);

        int ret = -1;
        auto iter = modelIdMap.find(identifier);
        if(iter != modelIdMap.end()) {
            ret = iter->second;
        }

        identifier += ret < 0 ? " (Unsupported)" : " (Supported)";
        setWindowTitle(identifier.c_str());
        ++id;
        break;
    }
}


void TesterDialog::onAxis(int id, const double& position)
{
    QProgressBar* bar = bars[id];
    double value = 100.0 * position;
    bar->setValue(value);
    bar->setFormat(formatC("{0:.3}%", value).c_str());
}


void TesterDialog::onButton(int id, bool isPressed)
{
    QPalette palette;
    if(isPressed) {
        palette.setColor(QPalette::Button, QColor(Qt::red));
    }
    buttons[id]->setPalette(palette);
}