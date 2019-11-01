
#ifndef _COLOR_H
#define _COLOR_H

#include "Reals.h"

#define INV_255 0.003921568627450980392157		//	1.0 / 255.0
#define INV_255F 0.003921568627450980392157f	//	1.0f / 255.0f

namespace zg {
	class ZG_API Color
	{
	public:
		float r, g, b, a;
		Color()											noexcept;
		Color(float r_, float g_, float b_)				noexcept;
		Color(float r_, float g_, float b_, float a_)	noexcept;
		Color(int r_, int g_, int b_)					noexcept;
		Color(int r_, int g_, int b_, int a_)			noexcept;

		//getters
		int GetR() const;	//get int r
		int GetG() const;	//get int g
		int GetB() const;	//get int b
		int GetA() const;	//get int a
		//setters
		void Set(int r_, int g_, int b_);
		void Set(int r_, int g_, int b_, int a_);
		//operators
		bool   operator==(const Color& c_) const;
		Color& operator+=(const Color& c_);
		Color& operator-=(const Color& c_);
		Color& operator*=(float f_);
		Color& operator/=(float f_);
#ifdef USING_SOL2
		Color operator-()					const;
		Color operator-(float f_)			const;
		Color operator-(const Color &c_)	const;
		Color operator+(float f_)			const;
		Color operator+(const Color &c_)	const;
		Color operator*(float f_)			const;
		Color operator*(const Color &c_)	const;
		Color operator/(float f_)			const;
		static void BindLua(sol::state& lua);
#else
		ZG_API friend Color operator- (const Color& c0_);					//negate
		ZG_API friend Color operator- (const Color& c0_, const Color& c1_);	//minus
		ZG_API friend Color operator- (const Color& c0_, float f_);			//minus
		ZG_API friend Color operator- (float f_, const Color& c0_);			//minus
		ZG_API friend Color operator+ (const Color& c0_, const Color& c1_);	//plus
		ZG_API friend Color operator+ (const Color& c0_, float f_);			//plus
		ZG_API friend Color operator+ (float f_, const Color& c0_);			//plus
		ZG_API friend Color operator* (const Color& c0_, const Color& c1_);	//multiply
		ZG_API friend Color operator* (const Color& c0_, float f_);			//multiply
		ZG_API friend Color operator* (float f_, const Color& c0_);			//multiply
		ZG_API friend Color operator/ (const Color& c0_, float f_);			//division
#endif
		ZG_API friend std::ostream& operator<<(std::ostream& os, const Color& m);

	};

	ZG_API Color Lerp(const Color& start_, const Color& end_, float time_);

} //namespace zg

#define CC_INDIAN_RED                zg::Color{176, 23 , 31 , 255 }
#define CC_CRIMSON                   zg::Color{220, 20 , 60 , 255 }
#define CC_LIGHTPINK                 zg::Color{255, 182, 193, 255 }
#define CC_LIGHTPINK_1               zg::Color{255, 174, 185, 255 }
#define CC_LIGHTPINK_2               zg::Color{238, 162, 173, 255 }
#define CC_LIGHTPINK_3               zg::Color{205, 140, 149, 255 }
#define CC_LIGHTPINK_4               zg::Color{139, 95 , 101, 255 }
#define CC_PINK                      zg::Color{255, 192, 203, 255 }
#define CC_PINK_1                    zg::Color{255, 181, 197, 255 }
#define CC_PINK_2                    zg::Color{238, 169, 184, 255 }
#define CC_PINK_3                    zg::Color{205, 145, 158, 255 }
#define CC_PINK_4                    zg::Color{139, 99 , 108, 255 }
#define CC_PALEVIOLETRED             zg::Color{219, 112, 147, 255 }
#define CC_PALEVIOLETRED_1           zg::Color{255, 130, 171, 255 }
#define CC_PALEVIOLETRED_2           zg::Color{238, 121, 159, 255 }
#define CC_PALEVIOLETRED_3           zg::Color{205, 104, 137, 255 }
#define CC_PALEVIOLETRED_4           zg::Color{139, 71 , 93 , 255 }
#define CC_LAVENDERBLUSH_1           zg::Color{255, 240, 245, 255 }
#define CC_LAVENDERBLUSH_2           zg::Color{238, 224, 229, 255 }
#define CC_LAVENDERBLUSH_3           zg::Color{205, 193, 197, 255 }
#define CC_LAVENDERBLUSH_4           zg::Color{139, 131, 134, 255 }
#define CC_VIOLETRED_1               zg::Color{255, 62 , 150, 255 }
#define CC_VIOLETRED_2               zg::Color{238, 58 , 140, 255 }
#define CC_VIOLETRED_3               zg::Color{205, 50 , 120, 255 }
#define CC_VIOLETRED_4               zg::Color{139, 34 , 82 , 255 }
#define CC_HOTPINK                   zg::Color{255, 105, 180, 255 }
#define CC_HOTPINK_1                 zg::Color{255, 110, 180, 255 }
#define CC_HOTPINK_2                 zg::Color{238, 106, 167, 255 }
#define CC_HOTPINK_3                 zg::Color{205, 96 , 144, 255 }
#define CC_HOTPINK_4                 zg::Color{139, 58 , 98 , 255 }
#define CC_RASPBERRY                 zg::Color{135, 38 , 87 , 255 }
#define CC_DEEPPINK_1                zg::Color{255, 20 , 147, 255 }
#define CC_DEEPPINK_2                zg::Color{238, 18 , 137, 255 }
#define CC_DEEPPINK_3                zg::Color{205, 16 , 118, 255 }
#define CC_DEEPPINK_4                zg::Color{139, 10 , 80 , 255 }
#define CC_MAROON_1                  zg::Color{255, 52 , 179, 255 }
#define CC_MAROON_2                  zg::Color{238, 48 , 167, 255 }
#define CC_MAROON_3                  zg::Color{205, 41 , 144, 255 }
#define CC_MAROON_4                  zg::Color{139, 28 , 98 , 255 }
#define CC_MEDIUMVIOLETRED           zg::Color{199, 21 , 133, 255 }
#define CC_VIOLETRED                 zg::Color{208, 32 , 144, 255 }
#define CC_ORCHID                    zg::Color{218, 112, 214, 255 }
#define CC_ORCHID_1                  zg::Color{255, 131, 250, 255 }
#define CC_ORCHID_2                  zg::Color{238, 122, 233, 255 }
#define CC_ORCHID_3                  zg::Color{205, 105, 201, 255 }
#define CC_ORCHID_4                  zg::Color{139, 71 , 137, 255 }
#define CC_THISTLE                   zg::Color{216, 191, 216, 255 }
#define CC_THISTLE_1                 zg::Color{255, 225, 255, 255 }
#define CC_THISTLE_2                 zg::Color{238, 210, 238, 255 }
#define CC_THISTLE_3                 zg::Color{205, 181, 205, 255 }
#define CC_THISTLE_4                 zg::Color{139, 123, 139, 255 }
#define CC_PLUM_1                    zg::Color{255, 187, 255, 255 }
#define CC_PLUM_2                    zg::Color{238, 174, 238, 255 }
#define CC_PLUM_3                    zg::Color{205, 150, 205, 255 }
#define CC_PLUM_4                    zg::Color{139, 102, 139, 255 }
#define CC_PLUM                      zg::Color{221, 160, 221, 255 }
#define CC_VIOLET                    zg::Color{238, 130, 238, 255 }
#define CC_FUCHSIA                   zg::Color{255, 0  , 255, 255 }
#define CC_MAGENTA_2                 zg::Color{238, 0  , 238, 255 }
#define CC_MAGENTA_3                 zg::Color{205, 0  , 205, 255 }
#define CC_DARKMAGENTA               zg::Color{139, 0  , 139, 255 }
#define CC_PURPLE                    zg::Color{128, 0  , 128, 255 }
#define CC_MEDIUMORCHID              zg::Color{186, 85 , 211, 255 }
#define CC_MEDIUMORCHID_1            zg::Color{224, 102, 255, 255 }
#define CC_MEDIUMORCHID_2            zg::Color{209, 95 , 238, 255 }
#define CC_MEDIUMORCHID_3            zg::Color{180, 82 , 205, 255 }
#define CC_MEDIUMORCHID_4            zg::Color{122, 55 , 139, 255 }
#define CC_DARKVIOLET                zg::Color{148, 0  , 211, 255 }
#define CC_DARKORCHID                zg::Color{153, 50 , 204, 255 }
#define CC_DARKORCHID_1              zg::Color{191, 62 , 255, 255 }
#define CC_DARKORCHID_2              zg::Color{178, 58 , 238, 255 }
#define CC_DARKORCHID_3              zg::Color{154, 50 , 205, 255 }
#define CC_DARKORCHID_4              zg::Color{104, 34 , 139, 255 }
#define CC_INDIGO                    zg::Color{75 , 0  , 130, 255 }
#define CC_BLUEVIOLET                zg::Color{138, 43 , 226, 255 }
#define CC_PURPLE_1                  zg::Color{155, 48 , 255, 255 }
#define CC_PURPLE_2                  zg::Color{145, 44 , 238, 255 }
#define CC_PURPLE_3                  zg::Color{125, 38 , 205, 255 }
#define CC_PURPLE_4                  zg::Color{85 , 26 , 139, 255 }
#define CC_MEDIUMPURPLE              zg::Color{147, 112, 219, 255 }
#define CC_MEDIUMPURPLE_1            zg::Color{171, 130, 255, 255 }
#define CC_MEDIUMPURPLE_2            zg::Color{159, 121, 238, 255 }
#define CC_MEDIUMPURPLE_3            zg::Color{137, 104, 205, 255 }
#define CC_MEDIUMPURPLE_4            zg::Color{93 , 71 , 139, 255 }
#define CC_DARKSLATEBLUE             zg::Color{72 , 61 , 139, 255 }
#define CC_LIGHTSLATEBLUE            zg::Color{132, 112, 255, 255 }
#define CC_MEDIUMSLATEBLUE           zg::Color{123, 104, 238, 255 }
#define CC_SLATEBLUE                 zg::Color{106, 90 , 205, 255 }
#define CC_SLATEBLUE_1               zg::Color{131, 111, 255, 255 }
#define CC_SLATEBLUE_2               zg::Color{122, 103, 238, 255 }
#define CC_SLATEBLUE_3               zg::Color{105, 89 , 205, 255 }
#define CC_SLATEBLUE_4               zg::Color{71 , 60 , 139, 255 }
#define CC_GHOSTWHITE                zg::Color{248, 248, 255, 255 }
#define CC_LAVENDER                  zg::Color{230, 230, 250, 255 }
#define CC_BLUE                      zg::Color{0  , 0  , 255, 255 }
#define CC_BLUE_2                    zg::Color{0  , 0  , 238, 255 }
#define CC_MEDIUMBLUE                zg::Color{0  , 0  , 205, 255 }
#define CC_DARKBLUE                  zg::Color{0  , 0  , 139, 255 }
#define CC_NAVY                      zg::Color{0  , 0  , 128, 255 }
#define CC_MIDNIGHTBLUE              zg::Color{25 , 25 , 112, 255 }
#define CC_COBALT                    zg::Color{61 , 89 , 171, 255 }
#define CC_ROYALBLUE                 zg::Color{65 , 105, 225, 255 }
#define CC_ROYALBLUE_1               zg::Color{72 , 118, 255, 255 }
#define CC_ROYALBLUE_2               zg::Color{67 , 110, 238, 255 }
#define CC_ROYALBLUE_3               zg::Color{58 , 95 , 205, 255 }
#define CC_ROYALBLUE_4               zg::Color{39 , 64 , 139, 255 }
#define CC_CORNFLOWERBLUE            zg::Color{100, 149, 237, 255 }
#define CC_LIGHTSTEELBLUE            zg::Color{176, 196, 222, 255 }
#define CC_LIGHTSTEELBLUE_1          zg::Color{202, 225, 255, 255 }
#define CC_LIGHTSTEELBLUE_2          zg::Color{188, 210, 238, 255 }
#define CC_LIGHTSTEELBLUE_3          zg::Color{162, 181, 205, 255 }
#define CC_LIGHTSTEELBLUE_4          zg::Color{110, 123, 139, 255 }
#define CC_LIGHTSLATEGRAY            zg::Color{119, 136, 153, 255 }
#define CC_SLATEGRAY                 zg::Color{112, 128, 144, 255 }
#define CC_SLATEGRAY_1               zg::Color{198, 226, 255, 255 }
#define CC_SLATEGRAY_2               zg::Color{185, 211, 238, 255 }
#define CC_SLATEGRAY_3               zg::Color{159, 182, 205, 255 }
#define CC_SLATEGRAY_4               zg::Color{108, 123, 139, 255 }
#define CC_DODGERBLUE                zg::Color{30 , 144, 255, 255 }
#define CC_DODGERBLUE_2              zg::Color{28 , 134, 238, 255 }
#define CC_DODGERBLUE_3              zg::Color{24 , 116, 205, 255 }
#define CC_DODGERBLUE_4              zg::Color{16 , 78 , 139, 255 }
#define CC_ALICEBLUE                 zg::Color{240, 248, 255, 255 }
#define CC_STEELBLUE                 zg::Color{70 , 130, 180, 255 }
#define CC_STEELBLUE_1               zg::Color{99 , 184, 255, 255 }
#define CC_STEELBLUE_2               zg::Color{92 , 172, 238, 255 }
#define CC_STEELBLUE_3               zg::Color{79 , 148, 205, 255 }
#define CC_STEELBLUE_4               zg::Color{54 , 100, 139, 255 }
#define CC_LIGHTSKYBLUE              zg::Color{135, 206, 250, 255 }
#define CC_LIGHTSKYBLUE_1            zg::Color{176, 226, 255, 255 }
#define CC_LIGHTSKYBLUE_2            zg::Color{164, 211, 238, 255 }
#define CC_LIGHTSKYBLUE_3            zg::Color{141, 182, 205, 255 }
#define CC_LIGHTSKYBLUE_4            zg::Color{96 , 123, 139, 255 }
#define CC_SKYBLUE_1                 zg::Color{135, 206, 255, 255 }
#define CC_SKYBLUE_2                 zg::Color{126, 192, 238, 255 }
#define CC_SKYBLUE_3                 zg::Color{108, 166, 205, 255 }
#define CC_SKYBLUE_4                 zg::Color{74 , 112, 139, 255 }
#define CC_SKYBLUE                   zg::Color{135, 206, 235, 255 }
#define CC_DEEPSKYBLUE               zg::Color{0  , 191, 255, 255 }
#define CC_DEEPSKYBLUE_2             zg::Color{0  , 178, 238, 255 }
#define CC_DEEPSKYBLUE_3             zg::Color{0  , 154, 205, 255 }
#define CC_DEEPSKYBLUE_4             zg::Color{0  , 104, 139, 255 }
#define CC_PEACOCK                   zg::Color{51 , 161, 201, 255 }
#define CC_LIGHTBLUE                 zg::Color{173, 216, 230, 255 }
#define CC_LIGHTBLUE_1               zg::Color{191, 239, 255, 255 }
#define CC_LIGHTBLUE_2               zg::Color{178, 223, 238, 255 }
#define CC_LIGHTBLUE_3               zg::Color{154, 192, 205, 255 }
#define CC_LIGHTBLUE_4               zg::Color{104, 131, 139, 255 }
#define CC_POWDERBLUE                zg::Color{176, 224, 230, 255 }
#define CC_CADETBLUE_1               zg::Color{152, 245, 255, 255 }
#define CC_CADETBLUE_2               zg::Color{142, 229, 238, 255 }
#define CC_CADETBLUE_3               zg::Color{122, 197, 205, 255 }
#define CC_CADETBLUE_4               zg::Color{83 , 134, 139, 255 }
#define CC_TURQUOISE_1               zg::Color{0  , 245, 255, 255 }
#define CC_TURQUOISE_2               zg::Color{0  , 229, 238, 255 }
#define CC_TURQUOISE_3               zg::Color{0  , 197, 205, 255 }
#define CC_TURQUOISE_4               zg::Color{0  , 134, 139, 255 }
#define CC_CADETBLUE                 zg::Color{95 , 158, 160, 255 }
#define CC_DARKTURQUOISE             zg::Color{0  , 206, 209, 255 }
#define CC_AZURE                     zg::Color{240, 255, 255, 255 }
#define CC_AZURE_2                   zg::Color{224, 238, 238, 255 }
#define CC_AZURE_3                   zg::Color{193, 205, 205, 255 }
#define CC_AZURE_4                   zg::Color{131, 139, 139, 255 }
#define CC_LIGHTCYAN                 zg::Color{224, 255, 255, 255 }
#define CC_LIGHTCYAN_2               zg::Color{209, 238, 238, 255 }
#define CC_LIGHTCYAN_3               zg::Color{180, 205, 205, 255 }
#define CC_LIGHTCYAN_4               zg::Color{122, 139, 139, 255 }
#define CC_PALETURQUOISE_1           zg::Color{187, 255, 255, 255 }
#define CC_PALETURQUOISE             zg::Color{174, 238, 238, 255 }
#define CC_PALETURQUOISE_3           zg::Color{150, 205, 205, 255 }
#define CC_PALETURQUOISE_4           zg::Color{102, 139, 139, 255 }
#define CC_DARKSLATEGRAY             zg::Color{47 , 79 , 79 , 255 }
#define CC_DARKSLATEGRAY_1           zg::Color{151, 255, 255, 255 }
#define CC_DARKSLATEGRAY_2           zg::Color{141, 238, 238, 255 }
#define CC_DARKSLATEGRAY_3           zg::Color{121, 205, 205, 255 }
#define CC_DARKSLATEGRAY_4           zg::Color{82 , 139, 139, 255 }
#define CC_AQUA						 zg::Color{0  , 255, 255, 255 }
#define CC_CYAN						 zg::Color{0  , 255, 255, 255 }
#define CC_CYAN_2                    zg::Color{0  , 238, 238, 255 }
#define CC_CYAN_3                    zg::Color{0  , 205, 205, 255 }
#define CC_DARKCYAN                  zg::Color{0  , 139, 139, 255 }
#define CC_TEAL                      zg::Color{0  , 128, 128, 255 }
#define CC_MEDIUMTURQUOISE           zg::Color{72 , 209, 204, 255 }
#define CC_LIGHTSEAGREEN             zg::Color{32 , 178, 170, 255 }
#define CC_MANGANESEBLUE             zg::Color{3  , 168, 158, 255 }
#define CC_TURQUOISE                 zg::Color{64 , 224, 208, 255 }
#define CC_COLDGREY                  zg::Color{128, 138, 135, 255 }
#define CC_TURQUOISEBLUE             zg::Color{0  , 199, 140, 255 }
#define CC_AQUAMARINE                zg::Color{127, 255, 212, 255 }
#define CC_AQUAMARINE_2              zg::Color{118, 238, 198, 255 }
#define CC_MEDIUMAQUAMARINE          zg::Color{102, 205, 170, 255 }
#define CC_AQUAMARINE_4              zg::Color{69 , 139, 116, 255 }
#define CC_MEDIUMSPRINGGREEN         zg::Color{0  , 250, 154, 255 }
#define CC_MINTCREAM                 zg::Color{245, 255, 250, 255 }
#define CC_SPRINGGREEN               zg::Color{0  , 255, 127, 255 }
#define CC_SPRINGGREEN_1             zg::Color{0  , 238, 118, 255 }
#define CC_SPRINGGREEN_2             zg::Color{0  , 205, 102, 255 }
#define CC_SPRINGGREEN_3             zg::Color{0  , 139, 69 , 255 }
#define CC_MEDIUMSEAGREEN            zg::Color{60 , 179, 113, 255 }
#define CC_SEAGREEN_1                zg::Color{84 , 255, 159, 255 }
#define CC_SEAGREEN_2                zg::Color{78 , 238, 148, 255 }
#define CC_SEAGREEN_3                zg::Color{67 , 205, 128, 255 }
#define CC_SEAGREEN                  zg::Color{46 , 139, 87 , 255 }
#define CC_EMERALDGREEN              zg::Color{0  , 201, 87 , 255 }
#define CC_MINT                      zg::Color{189, 252, 201, 255 }
#define CC_COBALTGREEN               zg::Color{61 , 145, 64 , 255 }
#define CC_HONEYDEW                  zg::Color{240, 255, 240, 255 }
#define CC_HONEYDEW_2                zg::Color{224, 238, 224, 255 }
#define CC_HONEYDEW_3                zg::Color{193, 205, 193, 255 }
#define CC_HONEYDEW_4                zg::Color{131, 139, 131, 255 }
#define CC_DARKSEAGREEN              zg::Color{143, 188, 143, 255 }
#define CC_DARKSEAGREEN_1            zg::Color{193, 255, 193, 255 }
#define CC_DARKSEAGREEN_2            zg::Color{180, 238, 180, 255 }
#define CC_DARKSEAGREEN_3            zg::Color{155, 205, 155, 255 }
#define CC_DARKSEAGREEN_4            zg::Color{105, 139, 105, 255 }
#define CC_PALEGREEN                 zg::Color{152, 251, 152, 255 }
#define CC_PALEGREEN_1               zg::Color{154, 255, 154, 255 }
#define CC_LIGHTGREEN                zg::Color{144, 238, 144, 255 }
#define CC_PALEGREEN_3               zg::Color{124, 205, 124, 255 }
#define CC_PALEGREEN_4               zg::Color{84 , 139, 84 , 255 }
#define CC_LIMEGREEN                 zg::Color{50 , 205, 50 , 255 }
#define CC_FORESTGREEN               zg::Color{34 , 139, 34 , 255 }
#define CC_LIME                      zg::Color{0  , 255, 0  , 255 }
#define CC_GREEN_2                   zg::Color{0  , 238, 0  , 255 }
#define CC_GREEN_3                   zg::Color{0  , 205, 0  , 255 }
#define CC_GREEN_4                   zg::Color{0  , 139, 0  , 255 }
#define CC_GREEN                     zg::Color{0  , 128, 0  , 255 }
#define CC_DARKGREEN                 zg::Color{0  , 100, 0  , 255 }
#define CC_SAPGREEN                  zg::Color{48 , 128, 20 , 255 }
#define CC_LAWNGREEN                 zg::Color{124, 252, 0  , 255 }
#define CC_CHARTREUSE                zg::Color{127, 255, 0  , 255 }
#define CC_CHARTREUSE_2              zg::Color{118, 238, 0  , 255 }
#define CC_CHARTREUSE_3              zg::Color{102, 205, 0  , 255 }
#define CC_CHARTREUSE_4              zg::Color{69 , 139, 0  , 255 }
#define CC_GREENYELLOW               zg::Color{173, 255, 47 , 255 }
#define CC_DARKOLIVEGREEN_1          zg::Color{202, 255, 112, 255 }
#define CC_DARKOLIVEGREEN_2          zg::Color{188, 238, 104, 255 }
#define CC_DARKOLIVEGREEN_3          zg::Color{162, 205, 90 , 255 }
#define CC_DARKOLIVEGREEN_4          zg::Color{110, 139, 61 , 255 }
#define CC_DARKOLIVEGREEN            zg::Color{85 , 107, 47 , 255 }
#define CC_OLIVEDRAB                 zg::Color{107, 142, 35 , 255 }
#define CC_OLIVEDRAB_1               zg::Color{192, 255, 62 , 255 }
#define CC_OLIVEDRAB_2               zg::Color{179, 238, 58 , 255 }
#define CC_YELLOWGREEN               zg::Color{154, 205, 50 , 255 }
#define CC_OLIVEDRAB_4               zg::Color{105, 139, 34 , 255 }
#define CC_IVORY                     zg::Color{255, 255, 240, 255 }
#define CC_IVORY_2                   zg::Color{238, 238, 224, 255 }
#define CC_IVORY_3                   zg::Color{205, 205, 193, 255 }
#define CC_IVORY_4                   zg::Color{139, 139, 131, 255 }
#define CC_BEIGE                     zg::Color{245, 245, 220, 255 }
#define CC_LIGHTYELLOW               zg::Color{255, 255, 224, 255 }
#define CC_LIGHTYELLOW_2             zg::Color{238, 238, 209, 255 }
#define CC_LIGHTYELLOW_3             zg::Color{205, 205, 180, 255 }
#define CC_LIGHTYELLOW_4             zg::Color{139, 139, 122, 255 }
#define CC_LIGHTGOLDENRODYELLOW      zg::Color{250, 250, 210, 255 }
#define CC_YELLOW                    zg::Color{255, 255, 0  , 255 }
#define CC_YELLOW_2                  zg::Color{238, 238, 0  , 255 }
#define CC_YELLOW_3                  zg::Color{205, 205, 0  , 255 }
#define CC_YELLOW_4                  zg::Color{139, 139, 0  , 255 }
#define CC_WARMGREY                  zg::Color{128, 128, 105, 255 }
#define CC_OLIVE                     zg::Color{128, 128, 0  , 255 }
#define CC_DARKKHAKI                 zg::Color{189, 183, 107, 255 }
#define CC_KHAKI_1                   zg::Color{255, 246, 143, 255 }
#define CC_KHAKI_2                   zg::Color{238, 230, 133, 255 }
#define CC_KHAKI_3                   zg::Color{205, 198, 115, 255 }
#define CC_KHAKI_4                   zg::Color{139, 134, 78 , 255 }
#define CC_KHAKI                     zg::Color{240, 230, 140, 255 }
#define CC_PALEGOLDENROD             zg::Color{238, 232, 170, 255 }
#define CC_LEMONCHIFFON              zg::Color{255, 250, 205, 255 }
#define CC_LEMONCHIFFON_2            zg::Color{238, 233, 191, 255 }
#define CC_LEMONCHIFFON_3            zg::Color{205, 201, 165, 255 }
#define CC_LEMONCHIFFON_4            zg::Color{139, 137, 112, 255 }
#define CC_LIGHTGOLDENROD_1          zg::Color{255, 236, 139, 255 }
#define CC_LIGHTGOLDENROD_2          zg::Color{238, 220, 130, 255 }
#define CC_LIGHTGOLDENROD_3          zg::Color{205, 190, 112, 255 }
#define CC_LIGHTGOLDENROD_4          zg::Color{139, 129, 76 , 255 }
#define CC_BANANA                    zg::Color{227, 207, 87 , 255 }
#define CC_GOLD                      zg::Color{255, 215, 0  , 255 }
#define CC_GOLD_2                    zg::Color{238, 201, 0  , 255 }
#define CC_GOLD_3                    zg::Color{205, 173, 0  , 255 }
#define CC_GOLD_4                    zg::Color{139, 117, 0  , 255 }
#define CC_CORNSILK                  zg::Color{255, 248, 220, 255 }
#define CC_CORNSILK_2                zg::Color{238, 232, 205, 255 }
#define CC_CORNSILK_3                zg::Color{205, 200, 177, 255 }
#define CC_CORNSILK_4                zg::Color{139, 136, 120, 255 }
#define CC_GOLDENROD                 zg::Color{218, 165, 32 , 255 }
#define CC_GOLDENROD_1               zg::Color{255, 193, 37 , 255 }
#define CC_GOLDENROD_2               zg::Color{238, 180, 34 , 255 }
#define CC_GOLDENROD_3               zg::Color{205, 155, 29 , 255 }
#define CC_GOLDENROD_4               zg::Color{139, 105, 20 , 255 }
#define CC_DARKGOLDENROD             zg::Color{184, 134, 11 , 255 }
#define CC_DARKGOLDENROD_1           zg::Color{255, 185, 15 , 255 }
#define CC_DARKGOLDENROD_2           zg::Color{238, 173, 14 , 255 }
#define CC_DARKGOLDENROD_3           zg::Color{205, 149, 12 , 255 }
#define CC_DARKGOLDENROD_4           zg::Color{139, 101, 8  , 255 }
#define CC_ORANGE_1			         zg::Color{255, 165, 0  , 255 }
#define CC_ORANGE_2                  zg::Color{238, 154, 0  , 255 }
#define CC_ORANGE_3                  zg::Color{205, 133, 0  , 255 }
#define CC_ORANGE_4                  zg::Color{139, 90 , 0  , 255 }
#define CC_FLORALWHITE               zg::Color{255, 250, 240, 255 }
#define CC_OLDLACE                   zg::Color{253, 245, 230, 255 }
#define CC_WHEAT                     zg::Color{245, 222, 179, 255 }
#define CC_WHEAT_1                   zg::Color{255, 231, 186, 255 }
#define CC_WHEAT_2                   zg::Color{238, 216, 174, 255 }
#define CC_WHEAT_3                   zg::Color{205, 186, 150, 255 }
#define CC_WHEAT_4                   zg::Color{139, 126, 102, 255 }
#define CC_MOCCASIN                  zg::Color{255, 228, 181, 255 }
#define CC_PAPAYAWHIP                zg::Color{255, 239, 213, 255 }
#define CC_BLANCHEDALMOND            zg::Color{255, 235, 205, 255 }
#define CC_NAVAJOWHITE               zg::Color{255, 222, 173, 255 }
#define CC_NAVAJOWHITE_2             zg::Color{238, 207, 161, 255 }
#define CC_NAVAJOWHITE_3             zg::Color{205, 179, 139, 255 }
#define CC_NAVAJOWHITE_4             zg::Color{139, 121, 94 , 255 }
#define CC_EGGSHELL                  zg::Color{252, 230, 201, 255 }
#define CC_TAN                       zg::Color{210, 180, 140, 255 }
#define CC_BRICK                     zg::Color{156, 102, 31 , 255 }
#define CC_CADMIUMYELLOW             zg::Color{255, 153, 18 , 255 }
#define CC_ANTIQUEWHITE              zg::Color{250, 235, 215, 255 }
#define CC_ANTIQUEWHITE_1            zg::Color{255, 239, 219, 255 }
#define CC_ANTIQUEWHITE_2            zg::Color{238, 223, 204, 255 }
#define CC_ANTIQUEWHITE_3            zg::Color{205, 192, 176, 255 }
#define CC_ANTIQUEWHITE_4            zg::Color{139, 131, 120, 255 }
#define CC_BURLYWOOD                 zg::Color{222, 184, 135, 255 }
#define CC_BURLYWOOD_1               zg::Color{255, 211, 155, 255 }
#define CC_BURLYWOOD_2               zg::Color{238, 197, 145, 255 }
#define CC_BURLYWOOD_3               zg::Color{205, 170, 125, 255 }
#define CC_BURLYWOOD_4               zg::Color{139, 115, 85 , 255 }
#define CC_BISQUE                    zg::Color{255, 228, 196, 255 }
#define CC_BISQUE_2                  zg::Color{238, 213, 183, 255 }
#define CC_BISQUE_3                  zg::Color{205, 183, 158, 255 }
#define CC_BISQUE_4                  zg::Color{139, 125, 107, 255 }
#define CC_MELON                     zg::Color{227, 168, 105, 255 }
#define CC_CARROT                    zg::Color{237, 145, 33 , 255 }
#define CC_DARKORANGE                zg::Color{255, 140, 0  , 255 }
#define CC_DARKORANGE_1              zg::Color{255, 127, 0  , 255 }
#define CC_DARKORANGE_2              zg::Color{238, 118, 0  , 255 }
#define CC_DARKORANGE_3              zg::Color{205, 102, 0  , 255 }
#define CC_DARKORANGE_4              zg::Color{139, 69 , 0  , 255 }
#define CC_ORANGE                    zg::Color{255, 128, 0  , 255 }
#define CC_TAN_1                     zg::Color{255, 165, 79 , 255 }
#define CC_TAN_2                     zg::Color{238, 154, 73 , 255 }
#define CC_PERU                      zg::Color{205, 133, 63 , 255 }
#define CC_TAN_4                     zg::Color{139, 90 , 43 , 255 }
#define CC_LINEN                     zg::Color{250, 240, 230, 255 }
#define CC_PEACHPUFF                 zg::Color{255, 218, 185, 255 }
#define CC_PEACHPUFF_2               zg::Color{238, 203, 173, 255 }
#define CC_PEACHPUFF_3               zg::Color{205, 175, 149, 255 }
#define CC_PEACHPUFF_4               zg::Color{139, 119, 101, 255 }
#define CC_SEASHELL                  zg::Color{255, 245, 238, 255 }
#define CC_SEASHELL_2                zg::Color{238, 229, 222, 255 }
#define CC_SEASHELL_3                zg::Color{205, 197, 191, 255 }
#define CC_SEASHELL_4                zg::Color{139, 134, 130, 255 }
#define CC_SANDYBROWN                zg::Color{244, 164, 96 , 255 }
#define CC_RAWSIENNA                 zg::Color{199, 97 , 20 , 255 }
#define CC_CHOCOLATE                 zg::Color{210, 105, 30 , 255 }
#define CC_CHOCOLATE_1               zg::Color{255, 127, 36 , 255 }
#define CC_CHOCOLATE_2               zg::Color{238, 118, 33 , 255 }
#define CC_CHOCOLATE_3               zg::Color{205, 102, 29 , 255 }
#define CC_SADDLEBROWN               zg::Color{139, 69 , 19 , 255 }
#define CC_IVORYBLACK                zg::Color{41 , 36 , 33 , 255 }
#define CC_FLESH                     zg::Color{255, 125, 64 , 255 }
#define CC_CADMIUMORANGE             zg::Color{255, 97 , 3  , 255 }
#define CC_BURNTSIENNA               zg::Color{138, 54 , 15 , 255 }
#define CC_SIENNA                    zg::Color{160, 82 , 45 , 255 }
#define CC_SIENNA_1                  zg::Color{255, 130, 71 , 255 }
#define CC_SIENNA_2                  zg::Color{238, 121, 66 , 255 }
#define CC_SIENNA_3                  zg::Color{205, 104, 57 , 255 }
#define CC_SIENNA_4                  zg::Color{139, 71 , 38 , 255 }
#define CC_LIGHTSALMON               zg::Color{255, 160, 122, 255 }
#define CC_LIGHTSALMON_2             zg::Color{238, 149, 114, 255 }
#define CC_LIGHTSALMON_3             zg::Color{205, 129, 98 , 255 }
#define CC_LIGHTSALMON_4             zg::Color{139, 87 , 66 , 255 }
#define CC_CORAL                     zg::Color{255, 127, 80 , 255 }
#define CC_ORANGERED                 zg::Color{255, 69 , 0  , 255 }
#define CC_ORANGERED_2               zg::Color{238, 64 , 0  , 255 }
#define CC_ORANGERED_3               zg::Color{205, 55 , 0  , 255 }
#define CC_ORANGERED_4               zg::Color{139, 37 , 0  , 255 }
#define CC_SEPIA                     zg::Color{94 , 38 , 18 , 255 }
#define CC_DARKSALMON                zg::Color{233, 150, 122, 255 }
#define CC_SALMON_1                  zg::Color{255, 140, 105, 255 }
#define CC_SALMON_2                  zg::Color{238, 130, 98 , 255 }
#define CC_SALMON_3                  zg::Color{205, 112, 84 , 255 }
#define CC_SALMON_4                  zg::Color{139, 76 , 57 , 255 }
#define CC_CORAL_1                   zg::Color{255, 114, 86 , 255 }
#define CC_CORAL_2                   zg::Color{238, 106, 80 , 255 }
#define CC_CORAL_3                   zg::Color{205, 91 , 69 , 255 }
#define CC_CORAL_4                   zg::Color{139, 62 , 47 , 255 }
#define CC_BURNTUMBER                zg::Color{138, 51 , 36 , 255 }
#define CC_TOMATO                    zg::Color{255, 99 , 71 , 255 }
#define CC_TOMATO_2                  zg::Color{238, 92 , 66 , 255 }
#define CC_TOMATO_3                  zg::Color{205, 79 , 57 , 255 }
#define CC_TOMATO_4                  zg::Color{139, 54 , 38 , 255 }
#define CC_SALMON                    zg::Color{250, 128, 114, 255 }
#define CC_MISTYROSE                 zg::Color{255, 228, 225, 255 }
#define CC_MISTYROSE_2               zg::Color{238, 213, 210, 255 }
#define CC_MISTYROSE_3               zg::Color{205, 183, 181, 255 }
#define CC_MISTYROSE_4               zg::Color{139, 125, 123, 255 }
#define CC_SNOW                      zg::Color{255, 250, 250, 255 }
#define CC_SNOW_2                    zg::Color{238, 233, 233, 255 }
#define CC_SNOW_3                    zg::Color{205, 201, 201, 255 }
#define CC_SNOW_4                    zg::Color{139, 137, 137, 255 }
#define CC_ROSYBROWN                 zg::Color{188, 143, 143, 255 }
#define CC_ROSYBROWN_1               zg::Color{255, 193, 193, 255 }
#define CC_ROSYBROWN_2               zg::Color{238, 180, 180, 255 }
#define CC_ROSYBROWN_3               zg::Color{205, 155, 155, 255 }
#define CC_ROSYBROWN_4               zg::Color{139, 105, 105, 255 }
#define CC_LIGHTCORAL                zg::Color{240, 128, 128, 255 }
#define CC_INDIANRED                 zg::Color{205, 92 , 92 , 255 }
#define CC_INDIANRED_1               zg::Color{255, 106, 106, 255 }
#define CC_INDIANRED_2               zg::Color{238, 99 , 99 , 255 }
#define CC_INDIANRED_4               zg::Color{139, 58 , 58 , 255 }
#define CC_INDIANRED_3               zg::Color{205, 85 , 85 , 255 }
#define CC_BROWN                     zg::Color{165, 42 , 42 , 255 }
#define CC_BROWN_1                   zg::Color{255, 64 , 64 , 255 }
#define CC_BROWN_2                   zg::Color{238, 59 , 59 , 255 }
#define CC_BROWN_3                   zg::Color{205, 51 , 51 , 255 }
#define CC_BROWN_4                   zg::Color{139, 35 , 35 , 255 }
#define CC_FIREBRICK                 zg::Color{178, 34 , 34 , 255 }
#define CC_FIREBRICK_1               zg::Color{255, 48 , 48 , 255 }
#define CC_FIREBRICK_2               zg::Color{238, 44 , 44 , 255 }
#define CC_FIREBRICK_3               zg::Color{205, 38 , 38 , 255 }
#define CC_FIREBRICK_4               zg::Color{139, 26 , 26 , 255 }
#define CC_RED                       zg::Color{255, 0  , 0  , 255 }
#define CC_RED_2                     zg::Color{238, 0  , 0  , 255 }
#define CC_RED_3                     zg::Color{205, 0  , 0  , 255 }
#define CC_DARKRED                   zg::Color{139, 0  , 0  , 255 }
#define CC_MAROON                    zg::Color{128, 0  , 0  , 255 }
#define CC_SGI_BEET                  zg::Color{142, 56 , 142, 255 }
#define CC_SGI_SLATEBLUE             zg::Color{113, 113, 198, 255 }
#define CC_SGI_LIGHTBLUE             zg::Color{125, 158, 192, 255 }
#define CC_SGI_TEAL                  zg::Color{56 , 142, 142, 255 }
#define CC_SGI_CHARTREUSE            zg::Color{113, 198, 113, 255 }
#define CC_SGI_OLIVEDRAB             zg::Color{142, 142, 56 , 255 }
#define CC_SGI_BRIGHTGRAY            zg::Color{197, 193, 170, 255 }
#define CC_SGI_SALMON                zg::Color{198, 113, 113, 255 }
#define CC_SGI_DARKGRAY              zg::Color{85 , 85 , 85 , 255 }
#define CC_SGI_GRAY_12               zg::Color{30 , 30 , 30 , 255 }
#define CC_SGI_GRAY_16               zg::Color{40 , 40 , 40 , 255 }
#define CC_SGI_GRAY_32               zg::Color{81 , 81 , 81 , 255 }
#define CC_SGI_GRAY_36               zg::Color{91 , 91 , 91 , 255 }
#define CC_SGI_GRAY_52               zg::Color{132, 132, 132, 255 }
#define CC_SGI_GRAY_56               zg::Color{142, 142, 142, 255 }
#define CC_SGI_LIGHTGRAY             zg::Color{170, 170, 170, 255 }
#define CC_SGI_GRAY_72               zg::Color{183, 183, 183, 255 }
#define CC_SGI_GRAY_76               zg::Color{193, 193, 193, 255 }
#define CC_SGI_GRAY_92               zg::Color{234, 234, 234, 255 }
#define CC_SGI_GRAY_96               zg::Color{244, 244, 244, 255 }
#define CC_WHITE                     zg::Color{255, 255, 255, 255 }
#define CC_WHITE_SMOKE               zg::Color{245, 245, 245, 255 }
#define CC_GAINSBORO                 zg::Color{220, 220, 220, 255 }
#define CC_LIGHTGREY                 zg::Color{211, 211, 211, 255 }
#define CC_SILVER                    zg::Color{192, 192, 192, 255 }
#define CC_DARKGRAY                  zg::Color{169, 169, 169, 255 }
#define CC_GRAY                      zg::Color{128, 128, 128, 255 }
#define CC_DIMGRAY                   zg::Color{105, 105, 105, 255 }
#define CC_BLACK                     zg::Color{0  , 0  , 0  , 255 }
#define CC_GRAY_99                   zg::Color{252, 252, 252, 255 }
#define CC_GRAY_98                   zg::Color{250, 250, 250, 255 }
#define CC_GRAY_97                   zg::Color{247, 247, 247, 255 }
#define CC_WHITE_SMOKE               zg::Color{245, 245, 245, 255 }
#define CC_GRAY_95                   zg::Color{242, 242, 242, 255 }
#define CC_GRAY_94                   zg::Color{240, 240, 240, 255 }
#define CC_GRAY_93                   zg::Color{237, 237, 237, 255 }
#define CC_GRAY_92                   zg::Color{235, 235, 235, 255 }
#define CC_GRAY_91                   zg::Color{232, 232, 232, 255 }
#define CC_GRAY_90                   zg::Color{229, 229, 229, 255 }
#define CC_GRAY_89                   zg::Color{227, 227, 227, 255 }
#define CC_GRAY_88                   zg::Color{224, 224, 224, 255 }
#define CC_GRAY_87                   zg::Color{222, 222, 222, 255 }
#define CC_GRAY_86                   zg::Color{219, 219, 219, 255 }
#define CC_GRAY_85                   zg::Color{217, 217, 217, 255 }
#define CC_GRAY_84                   zg::Color{214, 214, 214, 255 }
#define CC_GRAY_83                   zg::Color{212, 212, 212, 255 }
#define CC_GRAY_82                   zg::Color{209, 209, 209, 255 }
#define CC_GRAY_81                   zg::Color{207, 207, 207, 255 }
#define CC_GRAY_80                   zg::Color{204, 204, 204, 255 }
#define CC_GRAY_79                   zg::Color{201, 201, 201, 255 }
#define CC_GRAY_78                   zg::Color{199, 199, 199, 255 }
#define CC_GRAY_77                   zg::Color{196, 196, 196, 255 }
#define CC_GRAY_76                   zg::Color{194, 194, 194, 255 }
#define CC_GRAY_75                   zg::Color{191, 191, 191, 255 }
#define CC_GRAY_74                   zg::Color{189, 189, 189, 255 }
#define CC_GRAY_73                   zg::Color{186, 186, 186, 255 }
#define CC_GRAY_72                   zg::Color{184, 184, 184, 255 }
#define CC_GRAY_71                   zg::Color{181, 181, 181, 255 }
#define CC_GRAY_70                   zg::Color{179, 179, 179, 255 }
#define CC_GRAY_69                   zg::Color{176, 176, 176, 255 }
#define CC_GRAY_68                   zg::Color{173, 173, 173, 255 }
#define CC_GRAY_67                   zg::Color{171, 171, 171, 255 }
#define CC_GRAY_66                   zg::Color{168, 168, 168, 255 }
#define CC_GRAY_65                   zg::Color{166, 166, 166, 255 }
#define CC_GRAY_64                   zg::Color{163, 163, 163, 255 }
#define CC_GRAY_63                   zg::Color{161, 161, 161, 255 }
#define CC_GRAY_62                   zg::Color{158, 158, 158, 255 }
#define CC_GRAY_61                   zg::Color{156, 156, 156, 255 }
#define CC_GRAY_60                   zg::Color{153, 153, 153, 255 }
#define CC_GRAY_59                   zg::Color{150, 150, 150, 255 }
#define CC_GRAY_58                   zg::Color{148, 148, 148, 255 }
#define CC_GRAY_57                   zg::Color{145, 145, 145, 255 }
#define CC_GRAY_56                   zg::Color{143, 143, 143, 255 }
#define CC_GRAY_55                   zg::Color{140, 140, 140, 255 }
#define CC_GRAY_54                   zg::Color{138, 138, 138, 255 }
#define CC_GRAY_53                   zg::Color{135, 135, 135, 255 }
#define CC_GRAY_52                   zg::Color{133, 133, 133, 255 }
#define CC_GRAY_51                   zg::Color{130, 130, 130, 255 }
#define CC_GRAY_50                   zg::Color{127, 127, 127, 255 }
#define CC_GRAY_49                   zg::Color{125, 125, 125, 255 }
#define CC_GRAY_48                   zg::Color{122, 122, 122, 255 }
#define CC_GRAY_47                   zg::Color{120, 120, 120, 255 }
#define CC_GRAY_46                   zg::Color{117, 117, 117, 255 }
#define CC_GRAY_45                   zg::Color{115, 115, 115, 255 }
#define CC_GRAY_44                   zg::Color{112, 112, 112, 255 }
#define CC_GRAY_43                   zg::Color{110, 110, 110, 255 }
#define CC_GRAY_42                   zg::Color{107, 107, 107, 255 }
#define CC_DIMGRAY                   zg::Color{105, 105, 105, 255 }
#define CC_GRAY_40                   zg::Color{102, 102, 102, 255 }
#define CC_GRAY_39                   zg::Color{99 , 99 , 99 , 255 }
#define CC_GRAY_38                   zg::Color{97 , 97 , 97 , 255 }
#define CC_GRAY_37                   zg::Color{94 , 94 , 94 , 255 }
#define CC_GRAY_36                   zg::Color{92 , 92 , 92 , 255 }
#define CC_GRAY_35                   zg::Color{89 , 89 , 89 , 255 }
#define CC_GRAY_34                   zg::Color{87 , 87 , 87 , 255 }
#define CC_GRAY_33                   zg::Color{84 , 84 , 84 , 255 }
#define CC_GRAY_32                   zg::Color{82 , 82 , 82 , 255 }
#define CC_GRAY_31                   zg::Color{79 , 79 , 79 , 255 }
#define CC_GRAY_30                   zg::Color{77 , 77 , 77 , 255 }
#define CC_GRAY_29                   zg::Color{74 , 74 , 74 , 255 }
#define CC_GRAY_28                   zg::Color{71 , 71 , 71 , 255 }
#define CC_GRAY_27                   zg::Color{69 , 69 , 69 , 255 }
#define CC_GRAY_26                   zg::Color{66 , 66 , 66 , 255 }
#define CC_GRAY_25                   zg::Color{64 , 64 , 64 , 255 }
#define CC_GRAY_24                   zg::Color{61 , 61 , 61 , 255 }
#define CC_GRAY_23                   zg::Color{59 , 59 , 59 , 255 }
#define CC_GRAY_22                   zg::Color{56 , 56 , 56 , 255 }
#define CC_GRAY_21                   zg::Color{54 , 54 , 54 , 255 }
#define CC_GRAY_20                   zg::Color{51 , 51 , 51 , 255 }
#define CC_GRAY_19                   zg::Color{48 , 48 , 48 , 255 }
#define CC_GRAY_18                   zg::Color{46 , 46 , 46 , 255 }
#define CC_GRAY_17                   zg::Color{43 , 43 , 43 , 255 }
#define CC_GRAY_16                   zg::Color{41 , 41 , 41 , 255 }
#define CC_GRAY_15                   zg::Color{38 , 38 , 38 , 255 }
#define CC_GRAY_14                   zg::Color{36 , 36 , 36 , 255 }
#define CC_GRAY_13                   zg::Color{33 , 33 , 33 , 255 }
#define CC_GRAY_12                   zg::Color{31 , 31 , 31 , 255 }
#define CC_GRAY_11                   zg::Color{28 , 28 , 28 , 255 }
#define CC_GRAY_10                   zg::Color{26 , 26 , 26 , 255 }
#define CC_GRAY_9                    zg::Color{23 , 23 , 23 , 255 }
#define CC_GRAY_8                    zg::Color{20 , 20 , 20 , 255 }
#define CC_GRAY_7                    zg::Color{18 , 18 , 18 , 255 }
#define CC_GRAY_6                    zg::Color{15 , 15 , 15 , 255 }
#define CC_GRAY_5                    zg::Color{13 , 13 , 13 , 255 }
#define CC_GRAY_4                    zg::Color{10 , 10 , 10 , 255 }
#define CC_GRAY_3                    zg::Color{8  , 8  , 8  , 255 }
#define CC_GRAY_2                    zg::Color{5  , 5  , 5  , 255 }
#define CC_GRAY_1                    zg::Color{3  , 3  , 3  , 255 }

#endif //_COLOR_H
