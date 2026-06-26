#include "SKSE/SKSE.h"
#include "REL/Relocation.h"
#include "RE/T/TESDataHandler.h"
#include "RE/T/TESObjectARMA.h"
#include "RE/T/TESModelTextureSwap.h"

using namespace SKSE;
using namespace RE;

SKSEPluginInfo(
    .Version = REL::Version{ 1, 0, 0, 0 },
    .Name = "JoP Patch",
    .Author = "",
    .SupportEmail = "",
    .StructCompatibility = SKSE::StructCompatibility::Independent,
    .RuntimeCompatibility = SKSE::VersionIndependence::AddressLibrary,
    .MinimumSKSEVersion = REL::Version{ 0, 0, 0, 0 }
)

namespace
{
    class ARMAPatcher
    {
    public:
        static void Install()
        {
            auto* messaging = GetMessagingInterface();
            if (!messaging) {
                stl::report_and_fail("Failed to get messaging interface");
            }
            messaging->RegisterListener("SKSE", OnDataLoaded);
        }

    private:
        static void OnDataLoaded(MessagingInterface::Message* msg)
        {
            if (msg->type != MessagingInterface::kDataLoaded) {
                return;
            }

            auto* dataHandler = TESDataHandler::GetSingleton();
            if (!dataHandler) {
                log::warn("TESDataHandler not found");
                return;
            }

            auto& armas = dataHandler->GetFormArray<TESObjectARMA>();
            log::info("Found {} ARMA records", armas.size());

            uint32_t patched = 0;
            for (auto* arma : armas) {
                if (!arma) continue;

                auto& maleModel = arma->bipedModels[SEXES::kMale];
                auto& male1st = arma->bipedModel1stPersons[SEXES::kMale];
                if (male1st.GetModel() && male1st.GetModel()[0]) {
                    if (maleModel.GetModel() && maleModel.GetModel()[0]) {
                        male1st.SetModel(maleModel.GetModel());
                        male1st.alternateTextures = maleModel.alternateTextures;
                        male1st.numAlternateTextures = maleModel.numAlternateTextures;
                        patched++;
                    }
                }

                auto& femaleModel = arma->bipedModels[SEXES::kFemale];
                auto& female1st = arma->bipedModel1stPersons[SEXES::kFemale];
                if (female1st.GetModel() && female1st.GetModel()[0]) {
                    if (femaleModel.GetModel() && femaleModel.GetModel()[0]) {
                        female1st.SetModel(femaleModel.GetModel());
                        female1st.alternateTextures = femaleModel.alternateTextures;
                        female1st.numAlternateTextures = femaleModel.numAlternateTextures;
                        patched++;
                    }
                }
            }

            log::info("Patched {} ARMA 1st-person model slots", patched);
        }
    };
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
    SKSE::Init(skse);
    log::info("JoP Patch plugin loaded");
    ARMAPatcher::Install();
    return true;
}
