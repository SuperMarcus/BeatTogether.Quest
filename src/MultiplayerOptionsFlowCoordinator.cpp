#include "MultiplayerOptionsFlowCoordinator.hpp"
#include "MultiplayerOptionsViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"
#include "HMUI/ViewController_AnimationType.hpp"

#include "UnityEngine/Object.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"

using namespace HMUI;
using namespace BeatTogether;
using namespace QuestUI;
using namespace UnityEngine;
using namespace GlobalNamespace;

DEFINE_CLASS(MultiplayerOptionsFlowCoordinator);

extern const Logger& getLogger();

void MultiplayerOptionsFlowCoordinator::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation) {
        getLogger().info("MultiplayerOptionsFlowCoordinator::DidActivate");
        this->SetTitle(il2cpp_utils::createcsstr("Server Options"), ViewController::AnimationType::Out);
        this->showBackButton = true;

        if (!optionsViewController) {
            optionsViewController = BeatSaberUI::CreateViewController<MultiplayerOptionsViewController*>();
        }

        this->ProvideInitialViewControllers(
            reinterpret_cast<ViewController*>(optionsViewController),
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );
    }
}

void MultiplayerOptionsFlowCoordinator::BackButtonWasPressed(HMUI::ViewController *) {
    Object::FindObjectOfType<MultiplayerModeSelectionFlowCoordinator*>()->DismissFlowCoordinator(
        this, ViewController::AnimationDirection::Horizontal,
        nullptr, false
    );
    getLogger().info("MultiplayerOptionsFlowCoordinator::BackButtonWasPressed");
}
