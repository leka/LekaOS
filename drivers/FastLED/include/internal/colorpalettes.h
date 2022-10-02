#ifndef __INC_COLORPALETTES_H
#define __INC_COLORPALETTES_H

#include "colorutils.h"
#include "fastled_progmem.h"
#include "pixeltypes.h"

///@file colorpalettes.h
/// contains definitions for the predefined color palettes supplied by FastLED.

// FASTLED_NAMESPACE_BEGIN

///@defgroup Colorpalletes Pre-defined color palletes
/// These schemes are all declared as "PROGMEM", meaning
/// that they won't take up SRAM on AVR chips until used.
/// Furthermore, the compiler won't even include these
/// in your PROGMEM (flash) storage unless you specifically
/// use each one, so you only 'pay for' those you actually use.

///@{

// typedef uint32_t TProgmemRGBPalette16[16];
// typedef uint32_t TProgmemHSVPalette16[16];

/// Cloudy color pallete
extern const TProgmemRGBPalette16 CloudColors_p;
/// Lava colors
extern const TProgmemRGBPalette16 LavaColors_p;
/// Ocean colors, blues and whites
extern const TProgmemRGBPalette16 OceanColors_p;
/// Forest colors, greens
extern const TProgmemRGBPalette16 ForestColors_p;

/// HSV Rainbow
extern const TProgmemRGBPalette16 RainbowColors_p;

#define RainbowStripesColors_p RainbowStripeColors_p
/// HSV Rainbow colors with alternatating stripes of black
extern const TProgmemRGBPalette16 RainbowStripeColors_p;

/// HSV color ramp: blue purple ping red orange yellow (and back)
/// Basically, everything but the greens, which tend to make
/// people's skin look unhealthy.  This palette is good for
/// lighting at a club or party, where it'll be shining on people.
extern const TProgmemRGBPalette16 PartyColors_p;

/// Approximate "black body radiation" palette, akin to
/// the FastLED 'HeatColor' function.
/// Recommend that you use values 0-240 rather than
/// the usual 0-255, as the last 15 colors will be
/// 'wrapping around' from the hot end to the cold end,
/// which looks wrong.
extern const TProgmemRGBPalette16 HeatColors_p;

// extern const TProgmemRGBGradientPalette_byte Rainbow_gp
DECLARE_GRADIENT_PALETTE(Rainbow_gp);

// FASTLED_NAMESPACE_END

///@}
#endif
