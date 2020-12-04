#include <string>

#include "BeatTogether.h"
#include "MultiplayerOptionsViewController.hpp"

#include "HMUI/Touchable.hpp"
#include "TMPro/TextAlignmentOptions.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include "UnityEngine/Events/UnityAction_1.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"
#include "UnityEngine/Color.hpp"

using namespace HMUI;
using namespace BeatTogether;
using namespace QuestUI;
using namespace UnityEngine;
using namespace TMPro;

DEFINE_CLASS(MultiplayerOptionsViewController);

extern const Logger& getLogger();

void OnHostValueDidChange(MultiplayerOptionsViewController* self, Il2CppString* newValue) {
    getLogger().info("Host value changed.");
    auto& serverOpts = getGlobalServerOptions();
    serverOpts.endpoint = to_utf8(csstrtostr(newValue));
}

void OnPortValueDidChange(MultiplayerOptionsViewController* self, Il2CppString* newValue) {
    getLogger().info("Port value changed.");

    try {
        auto& serverOpts = getGlobalServerOptions();
        auto newPortStr = to_utf8(csstrtostr(newValue));
        serverOpts.port = std::stoi(newPortStr);
        self->hostErrorTextLabel->get_gameObject()->SetActive(false);
    } catch (...) {
        self->hostErrorTextLabel->set_text(il2cpp_utils::createcsstr("Port number is invalid."));
        self->hostErrorTextLabel->get_gameObject()->SetActive(true);
    }
}

void MultiplayerOptionsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (firstActivation && addedToHierarchy) {
        getLogger().info("MultiplayerOptionsViewController::DidActivate");

        auto& serverOpts = getGlobalServerOptions();

        this->get_gameObject()->AddComponent<Touchable*>();

        auto rootLayoutGroup = BeatSaberUI::CreateVerticalLayoutGroup(get_transform());
        rootLayoutGroup->get_gameObject()
            ->GetComponent<UI::ContentSizeFitter*>()
            ->set_verticalFit(UI::ContentSizeFitter::FitMode::PreferredSize);
        rootLayoutGroup->set_spacing(8);

        // Root Section - Top Label
        auto titleLabel = BeatSaberUI::CreateText(rootLayoutGroup->get_transform(), "Multiplayer Server", true);
        titleLabel->set_alignment(TextAlignmentOptions::Center);
        titleLabel->set_fontSize(8);

        // Root Section - Fields
        auto fieldsLayoutGroup = BeatSaberUI::CreateVerticalLayoutGroup(rootLayoutGroup->get_transform());
        fieldsLayoutGroup->get_gameObject()
            ->GetComponent<UI::ContentSizeFitter*>()
            ->set_verticalFit(UI::ContentSizeFitter::FitMode::PreferredSize);
        fieldsLayoutGroup->set_padding(RectOffset::New_ctor(4, 4, 4, 4));
        fieldsLayoutGroup->get_gameObject()
            ->AddComponent<QuestUI::Backgroundable*>()
            ->ApplyBackground(il2cpp_utils::createcsstr("round-rect-panel"));

        // Hostname
        auto hostLayoutGroup = BeatSaberUI::CreateHorizontalLayoutGroup(fieldsLayoutGroup->get_transform());
        hostLayoutGroup->set_spacing(16);

        BeatSaberUI::CreateText(hostLayoutGroup->get_transform(), "Server Host", true)
            ->set_alignment(TextAlignmentOptions::MidlineLeft);

        hostTextField = BeatSaberUI::CreateStringSetting(
            hostLayoutGroup->get_transform(), "Server Host", std::string(serverOpts.endpoint),
            il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<Il2CppString*>*>(
                classof(UnityEngine::Events::UnityAction_1<Il2CppString*>*),
                this,
                OnHostValueDidChange
            )
        );
        hostTextField->GetComponentInChildren<UI::LayoutElement*>()->set_preferredWidth(60);

        // Port
        auto portLayoutGroup = BeatSaberUI::CreateHorizontalLayoutGroup(fieldsLayoutGroup->get_transform());
        portLayoutGroup->set_spacing(16);

        BeatSaberUI::CreateText(portLayoutGroup->get_transform(), "Server Port", true)
            ->set_alignment(TextAlignmentOptions::MidlineLeft);

        portTextField = BeatSaberUI::CreateStringSetting(
            portLayoutGroup->get_transform(), "Server Port", std::to_string(serverOpts.port),
            il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction_1<Il2CppString*>*>(
                classof(UnityEngine::Events::UnityAction_1<Il2CppString*>*),
                this,
                OnPortValueDidChange
            )
        );
        portTextField->GetComponentInChildren<UI::LayoutElement*>()->set_preferredWidth(60);

        // Root Section - Error Label
        hostErrorTextLabel = BeatSaberUI::CreateText(rootLayoutGroup->get_transform(), "Error message goes here");
        hostErrorTextLabel->set_alignment(TextAlignmentOptions::Center);
        hostErrorTextLabel->get_gameObject()->SetActive(false);
        hostErrorTextLabel->set_color(UnityEngine::Color(0.8f, 0.0f, 0.0f, 1.0f));
    }
}

void MultiplayerOptionsViewController::DidDeactivate(bool removedFromHierarchy, bool screenSystemDisabling) { }

void MultiplayerOptionsViewController::SyncValues() { }
