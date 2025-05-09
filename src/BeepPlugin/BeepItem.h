/**
   @author Kenta Suzuki
*/

#ifndef CNOID_BEEP_PLUGIN_BEEP_ITEM_H
#define CNOID_BEEP_PLUGIN_BEEP_ITEM_H

#include <cnoid/SubSimulatorItem>

namespace cnoid {

class BeepItem : public SubSimulatorItem
{
public:
    BeepItem();
    BeepItem(const BeepItem& org);
    virtual ~BeepItem();

    static void initializeClass(ExtensionManager* ext);
    virtual bool initializeSimulation(SimulatorItem* simulatorItem) override;

protected:
    virtual Item* doCloneItem(CloneMap* cloneMap) const override;
    virtual void doPutProperties(PutPropertyFunction& putProperty) override;
    virtual bool store(Archive& archive) override;
    virtual bool restore(const Archive& archive) override;

private:
    class Impl;
    Impl* impl;
};

typedef ref_ptr<BeepItem> BeepItemPtr;

}

#endif // CNOID_BEEP_PLUGIN_BEEP_ITEM_H