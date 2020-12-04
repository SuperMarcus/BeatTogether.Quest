#include <cstddef>

#include "modloader/shared/modloader.hpp"

#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

#include "System/Threading/Tasks/Task_1.hpp"
#include "System/Action_1.hpp"

#include "GlobalNamespace/PlatformAuthenticationTokenProvider.hpp"
#include "GlobalNamespace/AuthenticationToken.hpp"
#include "GlobalNamespace/MasterServerEndPoint.hpp"
#include "GlobalNamespace/MenuRpcManager.hpp"
#include "GlobalNamespace/BeatmapIdentifierNetSerializable.hpp"
#include "GlobalNamespace/MultiplayerLevelLoader.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionViewController.hpp"
#include "GlobalNamespace/MultiplayerModeSelectionFlowCoordinator.hpp"
#include "GlobalNamespace/OptionsViewController.hpp"
#include "GlobalNamespace/SimpleDialogPromptViewController.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Events/UnityAction.hpp"

#include "HMUI/ViewController_AnimationDirection.hpp"

#include "Polyglot/LocalizedTextMeshProUGUI.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "custom-types/shared/register.hpp"

#include "BeatTogether.h"
#include "MultiplayerOptionsViewController.hpp"
#include "MultiplayerOptionsFlowCoordinator.hpp"
#include "ConnectionFailedReason.hpp"

using namespace GlobalNamespace;

static ModInfo modInfo;

const Logger& getLogger()
{
    static const Logger logger(modInfo);
    return logger;
}

BeatTogether::ServerOptions& BeatTogether::getGlobalServerOptions() {
    static BeatTogether::ServerOptions sharedOptions;
    return sharedOptions;
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;

    // Initialize server opts, may want to save this into configs later.
    BeatTogether::getGlobalServerOptions().endpoint = HOST_NAME;
    BeatTogether::getGlobalServerOptions().port = PORT;
}

// Makes the Level ID stored in this identifer lower case if it is a custom level
void makeIdLowerCase(BeatmapIdentifierNetSerializable* identifier) {
    // Check if it is a custom level
    if (identifier->levelID->StartsWith(il2cpp_utils::createcsstr("custom_level_")))
        identifier->set_levelID(il2cpp_utils::createcsstr("custom_level_" + to_utf8(csstrtostr(identifier->levelID->Substring(13)->ToLower()))));
}

// Makes the Level ID stored in this identifer upper case if it is a custom level
void makeIdUpperCase(BeatmapIdentifierNetSerializable* identifier) {
    // Check if it is a custom level
    if (identifier->levelID->StartsWith(il2cpp_utils::createcsstr("custom_level_")))
        identifier->set_levelID(il2cpp_utils::createcsstr("custom_level_" + to_utf8(csstrtostr(identifier->levelID->Substring(13)->ToUpper()))));
}

/**
 * Present the Server Options flow coordinator from a parent flow coordinator.
 */
void presentServerOptionsFC(GlobalNamespace::MultiplayerModeSelectionFlowCoordinator* parent) {
    auto optionsFc = QuestUI::BeatSaberUI::CreateFlowCoordinator<BeatTogether::MultiplayerOptionsFlowCoordinator*>();
    parent->PresentFlowCoordinator(optionsFc, nullptr, HMUI::ViewController::AnimationDirection::Horizontal, false, false);
}

void onServerOptionsButtonClick(Il2CppObject*) {
    getLogger().info("Server Options button clicked. Presenting FlowController...");
    auto modeSelectionFC = UnityEngine::Object::FindObjectOfType<GlobalNamespace::MultiplayerModeSelectionFlowCoordinator*>();
    presentServerOptionsFC(modeSelectionFC);
}

void onOverrideErrorDialogButtonOnClick(MultiplayerModeSelectionFlowCoordinator* coordinator, int btnSelected) {
    if (btnSelected == 1) {
        presentServerOptionsFC(coordinator);
    } else {
        coordinator->DismissViewController(
            coordinator->simpleDialogPromptViewController,
            HMUI::ViewController::AnimationDirection::Vertical,
            nullptr, false
            );
    }
}

MAKE_HOOK_OFFSETLESS(PlatformAuthenticationTokenProvider_GetAuthenticationToken, System::Threading::Tasks::Task_1<GlobalNamespace::AuthenticationToken>*, PlatformAuthenticationTokenProvider* self)
{
    auto* sessionToken = Array<uint8_t>::NewLength(1);
    sessionToken->values[0] = 10;
    auto authenticationToken = AuthenticationToken(
        AuthenticationToken::Platform::OculusQuest,
        self->userId,
        self->userName,
        sessionToken
    );
    return System::Threading::Tasks::Task_1<AuthenticationToken>::New_ctor(authenticationToken);
}

MAKE_HOOK_OFFSETLESS(NetworkConfigSO_get_masterServerEndPoint, MasterServerEndPoint*, Il2CppObject* self)
{
    auto& serverOpts = BeatTogether::getGlobalServerOptions();
    getLogger().debug("Patching master server end point (EndPoint='%s:%u').", serverOpts.endpoint.data(), serverOpts.port);
    return MasterServerEndPoint::New_ctor(il2cpp_utils::createcsstr(serverOpts.endpoint), serverOpts.port);
}

MAKE_HOOK_OFFSETLESS(X509CertificateUtility_ValidateCertificateChain, void, Il2CppObject* self, Il2CppObject* certificate, Il2CppObject* certificateChain)
{
    // TODO: Support disabling the mod if official multiplayer is ever fixed
    // It'd be best if we do certificate validation here...
    // but for now we'll just skip it.
}

MAKE_HOOK_OFFSETLESS(MenuRpcManager_SelectBeatmap, void, MenuRpcManager* self, BeatmapIdentifierNetSerializable* identifier)
{
    auto* levelID = identifier->get_levelID();
    makeIdUpperCase(identifier);
    MenuRpcManager_SelectBeatmap(self, identifier);
}

MAKE_HOOK_OFFSETLESS(MenuRpcManager_InvokeSelectedBeatmap, void, MenuRpcManager* self, Il2CppString* userId, BeatmapIdentifierNetSerializable* identifier)
{
    auto* levelID = identifier->get_levelID();
    makeIdLowerCase(identifier);
    MenuRpcManager_InvokeSelectedBeatmap(self, userId, identifier);
}

MAKE_HOOK_OFFSETLESS(MenuRpcManager_StartLevel, void, MenuRpcManager* self, BeatmapIdentifierNetSerializable* identifier, GameplayModifiers* gameplayModifiers, float startTime)
{
    auto* levelID = identifier->get_levelID();
    makeIdUpperCase(identifier);
    MenuRpcManager_StartLevel(self, identifier, gameplayModifiers, startTime);
}

MAKE_HOOK_OFFSETLESS(MenuRpcManager_InvokeStartLevel, void, MenuRpcManager* self, Il2CppString* userId, BeatmapIdentifierNetSerializable* identifier, GameplayModifiers* gameplayModifiers, float startTime)
{
    makeIdLowerCase(identifier);
    MenuRpcManager_InvokeStartLevel(self, userId, identifier, gameplayModifiers, startTime);
}

MAKE_HOOK_OFFSETLESS(MultiplayerLevelLoader_LoadLevel, void, MultiplayerLevelLoader* self, BeatmapIdentifierNetSerializable* beatmapId, GameplayModifiers* gameplayModifiers, float initialStartTime) {
    // Change the ID to lower case temporarily so the level gets fetched correctly
    makeIdLowerCase(beatmapId);
    MultiplayerLevelLoader_LoadLevel(self, beatmapId, gameplayModifiers, initialStartTime);
    makeIdUpperCase(beatmapId);
}

/**
 * Hook to add the "Server Options" button to the mode selection menu.
 */
MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionViewController_DidActivate, void, MultiplayerModeSelectionViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    MultiplayerModeSelectionViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    // Inject mode selection button if is first activation
    if (firstActivation) {
        auto buttonsTransform = self->createServerButton->get_transform()->get_parent();
        auto optionsVc = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::OptionsViewController*>()->values[0];

        // Make the "Server Options" button based on settings button
        auto serverOptionsButton = UnityEngine::Object::Instantiate(optionsVc->settingsButton);
        serverOptionsButton->set_name(il2cpp_utils::createcsstr("Server Options"));
        serverOptionsButton->get_transform()->SetParent(buttonsTransform, false);
        serverOptionsButton->get_transform()->SetAsLastSibling();
        serverOptionsButton->get_onClick()->AddListener(il2cpp_utils::MakeDelegate<UnityEngine::Events::UnityAction*>(
            classof(UnityEngine::Events::UnityAction*),
            serverOptionsButton,
            onServerOptionsButtonClick
        ));
        serverOptionsButton
            ->GetComponentInChildren<Polyglot::LocalizedTextMeshProUGUI*>()
            ->set_Key(il2cpp_utils::createcsstr("Server Options"));
        serverOptionsButton->get_gameObject()->SetActive(true);
    }
}

/**
 * Add Server Options button to connection error dialog.
 */
MAKE_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_PresentConnectionErrorDialog, void, MultiplayerModeSelectionFlowCoordinator* self, BeatTogether::ConnectionFailedReason reason) {
    MultiplayerModeSelectionFlowCoordinator_PresentConnectionErrorDialog(self, reason);

    if (reason != BeatTogether::ConnectionCanceled) {
        auto errorVc = self->simpleDialogPromptViewController;
        auto serverOptsBtn = errorVc->buttons->values[1];
        QuestUI::BeatSaberUI::SetButtonText(serverOptsBtn, "Server Options");
        serverOptsBtn->get_gameObject()->SetActive(true);

        errorVc->didFinishAction = il2cpp_utils::MakeDelegate<System::Action_1<int>*>(
            classof(System::Action_1<int>*), self,
            onOverrideErrorDialogButtonOnClick
        );
    }
}

extern "C" void load()
{
    il2cpp_functions::Init();

    QuestUI::Init();
    custom_types::Register::RegisterType<BeatTogether::MultiplayerOptionsViewController>();
    custom_types::Register::RegisterType<BeatTogether::MultiplayerOptionsFlowCoordinator>();

    INSTALL_HOOK_OFFSETLESS(PlatformAuthenticationTokenProvider_GetAuthenticationToken,
        il2cpp_utils::FindMethod("", "PlatformAuthenticationTokenProvider", "GetAuthenticationToken"));
    INSTALL_HOOK_OFFSETLESS(NetworkConfigSO_get_masterServerEndPoint,
        il2cpp_utils::FindMethod("", "NetworkConfigSO", "get_masterServerEndPoint"));
    INSTALL_HOOK_OFFSETLESS(X509CertificateUtility_ValidateCertificateChain,
        il2cpp_utils::FindMethodUnsafe("", "X509CertificateUtility", "ValidateCertificateChain", 2));
    INSTALL_HOOK_OFFSETLESS(MenuRpcManager_SelectBeatmap,
        il2cpp_utils::FindMethodUnsafe("", "MenuRpcManager", "SelectBeatmap", 1));
    INSTALL_HOOK_OFFSETLESS(MenuRpcManager_InvokeSelectedBeatmap,
        il2cpp_utils::FindMethodUnsafe("", "MenuRpcManager", "InvokeSelectedBeatmap", 2));
    INSTALL_HOOK_OFFSETLESS(MenuRpcManager_StartLevel,
        il2cpp_utils::FindMethodUnsafe("", "MenuRpcManager", "StartLevel", 3));
    INSTALL_HOOK_OFFSETLESS(MenuRpcManager_InvokeStartLevel,
        il2cpp_utils::FindMethodUnsafe("", "MenuRpcManager", "InvokeStartLevel", 4));
    INSTALL_HOOK_OFFSETLESS(MultiplayerLevelLoader_LoadLevel,
        il2cpp_utils::FindMethodUnsafe("", "MultiplayerLevelLoader", "LoadLevel", 3));
    INSTALL_HOOK_OFFSETLESS(MultiplayerModeSelectionViewController_DidActivate,
        il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionViewController", "DidActivate", 3));
    INSTALL_HOOK_OFFSETLESS(MultiplayerModeSelectionFlowCoordinator_PresentConnectionErrorDialog,
        il2cpp_utils::FindMethodUnsafe("", "MultiplayerModeSelectionFlowCoordinator", "PresentConnectionErrorDialog", 1));
}
