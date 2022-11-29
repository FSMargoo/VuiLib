/*
 * File name	: vcolor.h
 * Author		: Margoo
 * Date			: 11/11/2022
 * Description	: This head defines the color class in vuilib's renderer
*/
#pragma once

#include "vrenderbasic.h"

VLIB_BEGIN_NAMESPACE

namespace Core {
    /*
     * VColor class:
     *  Description : This class describe a color in renderer
    */
    class VColor {
     public:
         /*
          * ColorEnum enum:
          *     Description : This enum collection include some basic color as UINT
         */
         enum ColorEnum {
            AliceBlue = 0xF0F8FF,
            AntiqueWhite = 0xFAEBD7,
            Aqua = 0x00FFFF,
            Aquamarine = 0x7FFFD4,
            Azure = 0xF0FFFF,
            Beige = 0xF5F5DC,
            Bisque = 0xFFE4C4,
            Black = 0x000000,
            BlanchedAlmond = 0xFFEBCD,
            Blue = 0x0000FF,
            BlueViolet = 0x8A2BE2,
            Brown = 0xA52A2A,
            BurlyWood = 0xDEB887,
            CadetBlue = 0x5F9EA0,
            Chartreuse = 0x7FFF00,
            Chocolate = 0xD2691E,
            Coral = 0xFF7F50,
            CornflowerBlue = 0x6495ED,
            Cornsilk = 0xFFF8DC,
            Crimson = 0xDC143C,
            Cyan = 0x00FFFF,
            DarkBlue = 0x00008B,
            DarkCyan = 0x008B8B,
            DarkGoldenrod = 0xB8860B,
            DarkGray = 0xA9A9A9,
            DarkGreen = 0x006400,
            DarkKhaki = 0xBDB76B,
            DarkMagenta = 0x8B008B,
            DarkOliveGreen = 0x556B2F,
            DarkOrange = 0xFF8C00,
            DarkOrchid = 0x9932CC,
            DarkRed = 0x8B0000,
            DarkSalmon = 0xE9967A,
            DarkSeaGreen = 0x8FBC8F,
            DarkSlateBlue = 0x483D8B,
            DarkSlateGray = 0x2F4F4F,
            DarkTurquoise = 0x00CED1,
            DarkViolet = 0x9400D3,
            DeepPink = 0xFF1493,
            DeepSkyBlue = 0x00BFFF,
            DimGray = 0x696969,
            DodgerBlue = 0x1E90FF,
            Firebrick = 0xB22222,
            FloralWhite = 0xFFFAF0,
            ForestGreen = 0x228B22,
            Fuchsia = 0xFF00FF,
            Gainsboro = 0xDCDCDC,
            GhostWhite = 0xF8F8FF,
            Gold = 0xFFD700,
            Goldenrod = 0xDAA520,
            Gray = 0x808080,
            Green = 0x008000,
            GreenYellow = 0xADFF2F,
            Honeydew = 0xF0FFF0,
            HotPink = 0xFF69B4,
            IndianRed = 0xCD5C5C,
            Indigo = 0x4B0082,
            Ivory = 0xFFFFF0,
            Khaki = 0xF0E68C,
            Lavender = 0xE6E6FA,
            LavenderBlush = 0xFFF0F5,
            LawnGreen = 0x7CFC00,
            LemonChiffon = 0xFFFACD,
            LightBlue = 0xADD8E6,
            LightCoral = 0xF08080,
            LightCyan = 0xE0FFFF,
            LightGoldenrodYellow = 0xFAFAD2,
            LightGreen = 0x90EE90,
            LightGray = 0xD3D3D3,
            LightPink = 0xFFB6C1,
            LightSalmon = 0xFFA07A,
            LightSeaGreen = 0x20B2AA,
            LightSkyBlue = 0x87CEFA,
            LightSlateGray = 0x778899,
            LightSteelBlue = 0xB0C4DE,
            LightYellow = 0xFFFFE0,
            Lime = 0x00FF00,
            LimeGreen = 0x32CD32,
            Linen = 0xFAF0E6,
            Magenta = 0xFF00FF,
            Maroon = 0x800000,
            MediumAquamarine = 0x66CDAA,
            MediumBlue = 0x0000CD,
            MediumOrchid = 0xBA55D3,
            MediumPurple = 0x9370DB,
            MediumSeaGreen = 0x3CB371,
            MediumSlateBlue = 0x7B68EE,
            MediumSpringGreen = 0x00FA9A,
            MediumTurquoise = 0x48D1CC,
            MediumVioletRed = 0xC71585,
            MidnightBlue = 0x191970,
            MintCream = 0xF5FFFA,
            MistyRose = 0xFFE4E1,
            Moccasin = 0xFFE4B5,
            NavajoWhite = 0xFFDEAD,
            Navy = 0x000080,
            OldLace = 0xFDF5E6,
            Olive = 0x808000,
            OliveDrab = 0x6B8E23,
            Orange = 0xFFA500,
            OrangeRed = 0xFF4500,
            Orchid = 0xDA70D6,
            PaleGoldenrod = 0xEEE8AA,
            PaleGreen = 0x98FB98,
            PaleTurquoise = 0xAFEEEE,
            PaleVioletRed = 0xDB7093,
            PapayaWhip = 0xFFEFD5,
            PeachPuff = 0xFFDAB9,
            Peru = 0xCD853F,
            Pink = 0xFFC0CB,
            Plum = 0xDDA0DD,
            PowderBlue = 0xB0E0E6,
            Purple = 0x800080,
            Red = 0xFF0000,
            RosyBrown = 0xBC8F8F,
            RoyalBlue = 0x4169E1,
            SaddleBrown = 0x8B4513,
            Salmon = 0xFA8072,
            SandyBrown = 0xF4A460,
            SeaGreen = 0x2E8B57,
            SeaShell = 0xFFF5EE,
            Sienna = 0xA0522D,
            Silver = 0xC0C0C0,
            SkyBlue = 0x87CEEB,
            SlateBlue = 0x6A5ACD,
            SlateGray = 0x708090,
            Snow = 0xFFFAFA,
            SpringGreen = 0x00FF7F,
            SteelBlue = 0x4682B4,
            Tan = 0xD2B48C,
            Teal = 0x008080,
            Thistle = 0xD8BFD8,
            Tomato = 0xFF6347,
            Turquoise = 0x40E0D0,
            Violet = 0xEE82EE,
            Wheat = 0xF5DEB3,
            White = 0xFFFFFF,
            WhiteSmoke = 0xF5F5F5,
            Yellow = 0xFFFF00,
            YellowGreen = 0x9ACD32,
        };

     public:
          /*
           * VColor constructor:
           *    Description : The constructor without parameters will build a black color
          */
          VColor();
          /*
           * VColor constructor:
           *    Description : The constructor will build class from a VColor object
          */
          VColor(const VColor& ColorObject);
          /*
           * VColor constructor:
           *    Description : The constructor will build class from a UINT32 variable,
           *                  and specify it's alpha value
           *    Tips        : The RGB value by UINT32 should applies the format likes 0xrrggbb
          */
          VColor(const UINT32& RGB, const float& AlphaValue);
          /*
           * VColor constructor:
           *    Description : The constructor will build class from ColorEnum value
           *    Tips        : Since this function won't do the judgment of the Color parameter's legitimacy,
           *                  it's better for you to avoid use this constructor as possible as you can,
           *                  even though it won't make any problem on the API Compatibility.
          */
          VColor(const ColorEnum& Color, const float& AlphaValue = 1.f);
          /*
           * VColor constructor:
           *    Description : The constructor will build class from following R G B A value
           *    Tips        : The R, G, B, A, value shouldn't be a invalid value like negative
          */
          VColor(const float& R, const float& G, const float& B, const float& A);
    
     public:
          /*
           * GetDxObject function:
           *		Description : Get the reference of DirectX2D object
           *		Tips		: The reference which returned is unchangeable
          */
          D2D1::ColorF GetDxObject() VRENDER_HELPER const;
    
     public:
          /*
           * GetR function:
           *    Description : Get the red value in RGBA of this color
          */
          float GetR() const;
          /*
           * GetG function:
           *    Description : Get the green value in RGBA of this color
          */
          float GetG() const;
          /*
           * GetB function:
           *    Description : Get the blue value in RGBA of this color
          */
          float GetB() const;
          /*
           * GetA function:
           *    Description : Get the alpha value in RGBA of this color
          */
          float GetA() const;
          
          /*
           * SetR function:
           *    Description : Set the red value in RGBA of this color
           *    Tips        : The specified value shouldn't be a invalid value like negative
          */
          void  SetR(const float& R);
          /*
           * SetG function:
           *    Description : Set the green value in RGBA of this color
           *    Tips        : The specified value shouldn't be a invalid value like negative
          */
          void  SetG(const float& G);
          /*
           * SetB function:
           *    Description : Set the blue value in RGBA of this color
           *    Tips        : The specified value shouldn't be a invalid value like negative
          */
          void  SetB(const float& B);
          /*
           * SetA function:
           *    Description : Set the alpha value in RGBA of this color
           *    Tips        : The specified value shouldn't be a invalid value like negative
          */
          void  SetA(const float& A);
          
          /*
           * FromBYTERGBA static function:
           *    Description : Make a VColor object from R, G, B, A value
           *    Tips        : 1> Not same as build a VColor through constructor directly, use this function
           *                     allow you to build a VColor from BYTE value instead of float
           *                  2> Since this function won't do the judgment of the Color parameter's legitimacy,
           *                     you should make sure the value is valid
          */
          static VColor FromBYTERGBA(BYTE R, BYTE G, BYTE B, BYTE A) {
              return VColor(R / 255.f, G / 255.f, B / 255.f, A / 255.f);
          }
          /*
           * FromBYTERGB static function:
           *    Description : Make a VColor object from R, G, B value
           *    Tips        : 1> Not same as build a VColor through constructor directly, use this function
           *                     allow you to build a VColor from BYTE value instead of float
           *                  2> Since this function won't do the judgment of the Color parameter's legitimacy,
           *                     you should make sure the value is valid
           *                  3> The alpha value will be set as 255 (that is 1.f)
          */
          static VColor FromBYTERGBA(BYTE R, BYTE G, BYTE B) {
              return VColor(R / 255.f, G / 255.f, B / 255.f, 1.f);
          }

      private:
          D2D1::ColorF DxColorObject;
    };
}

VLIB_END_NAMESPACE