#include "KajiyaKayHair.h"
#include <array>
#include <numeric>
#include "interaction.h"
#include "materials/hair.h"
#include "paramset.h"
#include "reflection.h"
#include "sampling.h"
#include "spectrum.h"
#include "texture.h"
#include "textures/constant.h"

namespace pbrt {

    void KajiyaKayHairMaterial::ComputeScatteringFunctions(SurfaceInteraction* si,
        MemoryArena& arena,
        TransportMode mode,
        bool allowMultipleLobes) const {
        si->bsdf = ARENA_ALLOC(arena, BSDF)(*si);

        Spectrum kd = Kd->Evaluate(*si).Clamp();
        Spectrum ks = Ks->Evaluate(*si).Clamp();
        Float rough = roughness->Evaluate(*si);
        if (!kd.IsBlack() || !ks.IsBlack())
            si->bsdf->Add(ARENA_ALLOC(arena, KajiyaKayHairBSDF)(kd, ks, rough));
    }

    KajiyaKayHairMaterial* CreateKajiyaKayHairMaterial(const TextureParams& mp) {
        std::shared_ptr<Texture<Spectrum>> Kd =
            mp.GetSpectrumTexture("Kd", Spectrum(0.5f));
        std::shared_ptr<Texture<Spectrum>> Ks =
            mp.GetSpectrumTexture("Ks", Spectrum(0.5f));
        std::shared_ptr<Texture<Float>> roughness =
            mp.GetFloatTexture("roughness", .1f);
        return new KajiyaKayHairMaterial(Kd, Ks, roughness);
    }

    Spectrum KajiyaKayHairBSDF::f(const Vector3f& wo, const Vector3f& wi) const {
        Spectrum diffuse(0.f), specular(0.f);
        if (!Ks.IsBlack()) {
            // Compute specular Kajiya-Kay term
            Vector3f wh = wi + wo;
            if (!(wh.x == 0 && wh.y == 0 && wh.z == 0)) {
                wh = Normalize(wh);
#if 1
                Float cosThetaH = Dot(wo, wh);
                Float sinThetaH = SafeSqrt(1 - cosThetaH * cosThetaH);
                Float cosThetaO = CosTheta(wo), sinThetaO = SinTheta(wo);
                Float spec = std::pow(cosThetaO * cosThetaH + sinThetaO * sinThetaH,
                    exponent);
#else
                Float tdoth = wh.x;
                Float spec = std::pow(
                    SafeSqrt(1 - tdoth * tdoth),
                    exponent);
#endif
                specular = spec * Ks;
            }
        }
        // Compute diffuse Kajiya-Kay term
        diffuse = Kd * SafeSqrt(1. - wi.x * wi.x);
        return (InvPi / AbsCosTheta(wi)) * (diffuse + specular);
    }

    std::string KajiyaKayHairBSDF::ToString() const {
        return StringPrintf(
            "[ Hair h: %f ks: %f kd:", Ks, Kd) + std::string("  ]");
    }
}
