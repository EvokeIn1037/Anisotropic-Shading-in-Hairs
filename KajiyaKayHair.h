#if defined(_MSC_VER)
#define NOMINMAX
#pragma once
#endif

#ifndef PBRT_MATERIALS_KajiyaKayHAIR_H
#define PBRT_MATERIALS_KajiyaKayHAIR_H

#include "material.h"
#include "pbrt.h"
#include "reflection.h"
#include <array>

namespace pbrt {

    // HairMaterial Declarations
    class KajiyaKayHairMaterial : public Material {
    public:
        // HairMaterial Public Methods
        KajiyaKayHairMaterial(const std::shared_ptr<Texture<Spectrum>>& kd,
            const std::shared_ptr<Texture<Spectrum>>& ks,
            const std::shared_ptr<Texture<Float>>& roughness)
            : Kd(kd), Ks(ks), roughness(roughness) {}
        void ComputeScatteringFunctions(SurfaceInteraction* si, MemoryArena& arena,
            TransportMode mode,
            bool allowMultipleLobes) const;

    private:
        // HairMaterial Private Data
        std::shared_ptr<Texture<Spectrum>> Kd, Ks;
        std::shared_ptr<Texture<Float>> roughness;
    };

    KajiyaKayHairMaterial* CreateKajiyaKayHairMaterial(const TextureParams& mp);

    // HairBSDF Declarations
    class KajiyaKayHairBSDF : public BxDF {
    public:
        // KajiyaKay Public Methods
        KajiyaKayHairBSDF(const Spectrum& kd, const Spectrum& ks, Float roughness)
            : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE | BSDF_GLOSSY)),
            Kd(kd),
            Ks(ks) {
            Float e = (Float)1. / roughness;
            if (e > 10000.f || std::isnan(e)) e = 10000.f;
            exponent = e;
        }
        Spectrum f(const Vector3f& wo, const Vector3f& wi) const;
        std::string ToString() const;

    private:
        // KajiyaKay Private Data
        Spectrum Kd, Ks;
        Float exponent;
    };
}

#endif
