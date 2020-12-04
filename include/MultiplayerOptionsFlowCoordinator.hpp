#pragma once

#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"

#include "HMUI/ViewController.hpp"
#include "HMUI/NavigationController.hpp"
#include "HMUI/FlowCoordinator.hpp"

#include "custom-types/shared/macros.hpp"
#include "custom-types/shared/register.hpp"

#include "MultiplayerOptionsViewController.hpp"

DECLARE_CLASS_CODEGEN(BeatTogether, MultiplayerOptionsFlowCoordinator, HMUI::FlowCoordinator,
    DECLARE_INSTANCE_FIELD(MultiplayerOptionsViewController*, optionsViewController);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    DECLARE_OVERRIDE_METHOD(void, BackButtonWasPressed, il2cpp_utils::FindMethodUnsafe("HMUI", "FlowCoordinator", "BackButtonWasPressed", 1), HMUI::ViewController* topViewController);

    REGISTER_FUNCTION(MultiplayerOptionsFlowCoordinator,
        REGISTER_FIELD(optionsViewController);

        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(BackButtonWasPressed);
    )
)
