#pragma once

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "HMUI/ViewController.hpp"
#include "HMUI/NavigationController.hpp"
#include "HMUI/FlowCoordinator.hpp"
#include "HMUI/InputFieldView.hpp"

#include "TMPro/TextMeshProUGUI.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"

DECLARE_CLASS_CODEGEN(BeatTogether, MultiplayerOptionsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(HMUI::InputFieldView*, hostTextField);
    DECLARE_INSTANCE_FIELD(HMUI::InputFieldView*, portTextField);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, hostErrorTextLabel);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, DidDeactivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidDeactivate", 2), bool removedFromHierarchy, bool screenSystemDisabling);

    DECLARE_METHOD(void, SyncValues);

    REGISTER_FUNCTION(MultiplayerOptionsViewController,
        REGISTER_FIELD(hostTextField);
        REGISTER_FIELD(portTextField);
        REGISTER_FIELD(hostErrorTextLabel);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(DidDeactivate);
        REGISTER_METHOD(SyncValues);
    )
)
