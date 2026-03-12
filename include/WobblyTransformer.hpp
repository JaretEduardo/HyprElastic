#pragma once
#include <hyprland/src/render/Transformer.hpp>

class CWobblyWindow;

class CWobblyTransformer : public IWindowTransformer {
public:
    CWobblyTransformer(CWobblyWindow* pPhysics);
    virtual CFramebuffer* transform(CFramebuffer* in) override;
    virtual void preWindowRender(CSurfacePassElement::SRenderData* pRenderData) override;

private:
    CWobblyWindow* m_pPhysics;
};