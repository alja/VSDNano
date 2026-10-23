#ifndef FWColorPalette_h
#define FWColorPalette_h

// Colors that the web client can show, and darker variants for the dark mode
// (EventManager::setDarkMode). In the spirit of Fireworks' palettes for white
// and black backgrounds, but done by choosing other color indices: the client
// cannot be given new colors.

#include "TColor.h"
#include "TMath.h"
#include "TROOT.h"

#include <vector>

// Nearest ROOT color that the web client knows. The client looks up colors
// by index in JSROOT's copy of ROOT's predefined colors, so an index that
// TColor::GetColor() creates for a new RGB value (1000 and up) shows as
// white. These are the basic colors, the color wheel and the grays.
inline Color_t nearestWebColor(int r, int g, int b)
{
    static const int wheel[] = {kRed, kPink, kMagenta, kViolet, kBlue, kAzure,
                                kCyan, kTeal, kGreen, kSpring, kYellow, kOrange};
    std::vector<int> cands = {kWhite, kBlack, kGray, kGray + 1, kGray + 2, kGray + 3};
    for (int w : wheel)
        for (int o = -10; o <= 10; ++o)
            cands.push_back(w + o);

    Color_t best = kBlack;
    long best_d = -1;
    for (int ci : cands)
    {
        TColor *c = gROOT->GetColor(ci);
        if (!c)
            continue;
        long dr = r - long(255 * c->GetRed() + 0.5);
        long dg = g - long(255 * c->GetGreen() + 0.5);
        long db = b - long(255 * c->GetBlue() + 0.5);
        long d = dr * dr + dg * dg + db * db;
        if (best_d < 0 || d < best_d)
        {
            best_d = d;
            best = ci;
        }
    }
    return best;
}

// The web-known color nearest to color c with its RGB values scaled by f
// (f < 1 darkens).
inline Color_t scaledColor(Color_t c, float f)
{
    TColor *tc = gROOT->GetColor(c);
    if (!tc)
        return c;
    auto chan = [f](float v) { return TMath::Nint(255 * TMath::Min(1.f, v * f)); };
    return nearestWebColor(chan(tc->GetRed()), chan(tc->GetGreen()), chan(tc->GetBlue()));
}

#endif
