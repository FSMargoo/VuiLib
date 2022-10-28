#include "../../../include/core/object/vapplication.h"

VLIB_BEGIN_NAMESPACE

namespace Core {

VApplication* VLib_Application = nullptr;

VMessage *VApplication::PatchEvent() {
    VMessage* ResultEvent = nullptr;

    ExMessage EasyXMessage{};
    peekmessage(&EasyXMessage);

    switch (EasyXMessage.message) {
        case WM_LBUTTONUP: {
            ResultEvent = new VMouseClickedMessage(EasyXMessage.x, EasyXMessage.y,
                                                   VMouseClickedFlag::Up, VMouseKeyFlag::Left);

            return ResultEvent;
        }
        case WM_LBUTTONDOWN: {
            ResultEvent = new VMouseClickedMessage(EasyXMessage.x, EasyXMessage.y,
                                                   VMouseClickedFlag::Down, VMouseKeyFlag::Left);

            return ResultEvent;
        }
        case WM_RBUTTONUP: {
            ResultEvent = new VMouseClickedMessage(EasyXMessage.x, EasyXMessage.y,
                                                   VMouseClickedFlag::Up, VMouseKeyFlag::Right);

            return ResultEvent;
        }
        case WM_RBUTTONDOWN: {
            ResultEvent = new VMouseClickedMessage(EasyXMessage.x, EasyXMessage.y,
                                                   VMouseClickedFlag::Down, VMouseKeyFlag::Right);

            return ResultEvent;
        }
        case WM_MBUTTONUP: {
            ResultEvent = new VMouseClickedMessage(EasyXMessage.x, EasyXMessage.y,
                                                   VMouseClickedFlag::Up, VMouseKeyFlag::Middle);

            return ResultEvent;
        }
        case WM_MBUTTONDOWN: {
            ResultEvent = new VMouseClickedMessage(EasyXMessage.x, EasyXMessage.y,
                                                   VMouseClickedFlag::Down, VMouseKeyFlag::Middle);

            return ResultEvent;
        }
        case WM_MOUSEMOVE: {
            ResultEvent = new VMouseMoveMessage(EasyXMessage.x, EasyXMessage.y);

            return ResultEvent;
        }
        case WM_MOUSEWHEEL: {
            ResultEvent = new VMouseWheelMessage(EasyXMessage.x, EasyXMessage.y, EasyXMessage.wheel);

            return ResultEvent;
        }
        case WM_KEYUP: {
            ResultEvent = new VKeyClickedMessage(EasyXMessage.vkcode, EasyXMessage.prevdown, EasyXMessage.extended, VkeyClickedFlag::Up);

            return ResultEvent;
        }
        case WM_KEYDOWN: {
            ResultEvent = new VKeyClickedMessage(EasyXMessage.vkcode, EasyXMessage.prevdown, EasyXMessage.extended, VkeyClickedFlag::Down);

            return ResultEvent;
        }
        case WM_CHAR: {
            ResultEvent = new VIMECharMessage(EasyXMessage.ch);

            return ResultEvent;
        }
    }

    return nullptr;
}
VApplication::VApplication() : VUIObject(nullptr) {
    if (VLib_Application == nullptr) {
        InitTheme();

        VLib_Application = this;
    } else {
        VLIB_REPORT_ERROR(L"Application can only init as once.");
    }
}

void VApplication::InitTheme() {
    ThemeList.push_back(new VPushButtonTheme);
    ThemeList.push_back(new VMainWindowTheme);
    ThemeList.push_back(new VImageLabelTheme);
    ThemeList.push_back(new VTextLabelTheme);
    ThemeList.push_back(new VRadioButtonTheme);
    ThemeList.push_back(new VCircleScrollBarTheme);
    ThemeList.push_back(new VSliderTheme);
    ThemeList.push_back(new VBlurLabelTheme);
    ThemeList.push_back(new VIconButtonTheme);
    ThemeList.push_back(new VTextEditorTheme);
    ThemeList.push_back(new VViewScrollerButtonTheme);
    ThemeList.push_back(new VViewScrollerTheme);
    ThemeList.push_back(new VViewLabelTheme);
}
void VApplication::ProcessEvent(Core::VMessage* PatchedMessage) {
    PatchedMessage = PatchEvent();

    while (PatchedMessage != nullptr) {
        SysProcessMessage(PatchedMessage);

        delete PatchedMessage;

        PatchedMessage = PatchEvent();
    }
}

std::vector<VBasicUITheme*> VApplication::GetApplicationTheme() {
    return ThemeList;
}

int VApplication::Exec() {
    VMessage* PatchedMessage = nullptr;

    while (true) {
        ProcessEvent(PatchedMessage);
        CheckAllFrame(true);

        VTimeHelper::Sleep(1);
    }

    return -1;
}
void VApplication::SetTheme(VBasicUITheme* Theme) {
    for (int Count = 0; Count < ThemeList.size(); ++Count) {
        if (ThemeList[Count]->GetThemeType() == Theme->GetThemeType()) {
            delete ThemeList.at(Count);
            ThemeList.erase(ThemeList.begin() + Count);
            ThemeList.insert(ThemeList.begin() + Count, Theme);

            for (auto& Objects : ObjectKernel.ChildObjectContainer) {
                Objects->Update();
            }

            break;
        }
    }
}

}
VLIB_END_NAMESPACE