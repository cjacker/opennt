/***************************************************************************\
* Module Name: KBDGR1.C
*
* keyboard layout for German_IBM
*
* Copyright (c) 1985-95, Microsoft Corporation
*
* History:
* KBDTOOL v1.03 - Created  Sun Mar 15 00:58:13 2020
\***************************************************************************/

#include <windows.h>
#include "vkoem.h"
#include "kbd.h"
#include "kbdGR1.h"

/***************************************************************************\
* ausVK[] - Virtual Scan Code to Virtual Key conversion table for German_IBM
\***************************************************************************/

static USHORT ausVK[] = {
    T00, T01, T02, T03, T04, T05, T06, T07,
    T08, T09, T0A, T0B, T0C, T0D, T0E, T0F,
    T10, T11, T12, T13, T14, T15, T16, T17,
    T18, T19, T1A, T1B, T1C, T1D, T1E, T1F,
    T20, T21, T22, T23, T24, T25, T26, T27,
    T28, T29, T2A, T2B, T2C, T2D, T2E, T2F,
    T30, T31, T32, T33, T34, T35,

    /*
     * Right-hand Shift key must have KBDEXT bit set.
     */
    T36 | KBDEXT,

    T37 | KBDMULTIVK,               // numpad_* + Shift/Alt -> SnapShot

    T38, T39, T3A, T3B, T3C, T3D, T3E,
    T3F, T40, T41, T42, T43, T44,

    /*
     * NumLock Key:
     *     KBDEXT     - VK_NUMLOCK is an Extended key
     *     KBDMULTIVK - VK_NUMLOCK or VK_PAUSE (without or with CTRL)
     */
    T45 | KBDEXT | KBDMULTIVK,

    T46 | KBDMULTIVK,

    /*
     * Number Pad keys:
     *     KBDNUMPAD  - digits 0-9 and decimal point.
     *     KBDSPECIAL - require special processing by Windows
     */
    T47 | KBDNUMPAD | KBDSPECIAL,   // Numpad 7 (Home)
    T48 | KBDNUMPAD | KBDSPECIAL,   // Numpad 8 (Up),
    T49 | KBDNUMPAD | KBDSPECIAL,   // Numpad 9 (PgUp),
    T4A,
    T4B | KBDNUMPAD | KBDSPECIAL,   // Numpad 4 (Left),
    T4C | KBDNUMPAD | KBDSPECIAL,   // Numpad 5 (Clear),
    T4D | KBDNUMPAD | KBDSPECIAL,   // Numpad 6 (Right),
    T4E,
    T4F | KBDNUMPAD | KBDSPECIAL,   // Numpad 1 (End),
    T50 | KBDNUMPAD | KBDSPECIAL,   // Numpad 2 (Down),
    T51 | KBDNUMPAD | KBDSPECIAL,   // Numpad 3 (PgDn),
    T52 | KBDNUMPAD | KBDSPECIAL,   // Numpad 0 (Ins),
    T53 | KBDNUMPAD | KBDSPECIAL,   // Numpad . (Del),

    T54, T55, T56, T57, T58, T59, T5A, T5B,
    T5C, T5D, T5E, T5F, T60, T61, T62, T63,
    T64, T65, T66, T67, T68, T69, T6A, T6B,
    T6C, T6D, T6E, T6F, T70, T71, T72, T73,
    T74, T75, T76, T77, T78, T79, T7A, T7B,
    T7C, T7D, T7E

};

static VSC_VK aE0VscToVk[] = {
        { 0x1C, X1C | KBDEXT              },  // Numpad Entern
        { 0x1D, X1D | KBDEXT              },  // RControln
        { 0x35, X35 | KBDEXT              },  // Numpad Dividen
        { 0x37, X37 | KBDEXT              },  // Snapshotn
        { 0x38, X38 | KBDEXT              },  // RMenun
        { 0x46, X46 | KBDEXT              },  // Break (Ctrl + Pause)
        { 0x47, X47 | KBDEXT              },  // Home
        { 0x48, X48 | KBDEXT              },  // Up
        { 0x49, X49 | KBDEXT              },  // Prior
        { 0x4B, X4B | KBDEXT              },  // Left
        { 0x4D, X4D | KBDEXT              },  // Right
        { 0x4F, X4F | KBDEXT              },  // End
        { 0x50, X50 | KBDEXT              },  // Down
        { 0x51, X51 | KBDEXT              },  // Next
        { 0x52, X52 | KBDEXT              },  // Insert
        { 0x53, X53 | KBDEXT              },  // Delete
        { 0x5B, X5B | KBDEXT              },  // Left Win
        { 0x5C, X5C | KBDEXT              },  // Right Win
        { 0x5D, X5D | KBDEXT              },  // Application
        { 0,      0                       }
};

static VSC_VK aE1VscToVk[] = {
        { 0x1D, Y1D                       },  // Pause
        { 0   ,   0                       }
};

/***************************************************************************\
* aVkToBits[]  - map Virtual Keys to Modifier Bits
*
* See kbd.h for a full description.
*
* German_IBM Keyboard has only three shifter keys:
*     SHIFT (L & R) affects alphabnumeric keys,
*     CTRL  (L & R) is used to generate control characters
*     ALT   (L & R) used for generating characters by number with numpad
\***************************************************************************/
static VK_TO_BIT aVkToBits[] = {
    { VK_SHIFT,   KBDSHIFT },
    { VK_CONTROL, KBDCTRL  },
    { VK_MENU,    KBDALT   },
    { 0,          0        }
};

/***************************************************************************\
* aModification[]  - map character modifier bits to modification number
*
* See kbd.h for a full description.
*
\***************************************************************************/

static MODIFIERS CharModifiers = {
    &aVkToBits[0],
    7,
    {
    //  Modification# //  Keys Pressed
    //  ============= // =============
        0,            // 
        1,            // Shift
        4,            //   Ctrl
        5,            // S+Ctrl
        SHFT_INVALID, //       Alt
        SHFT_INVALID, // Shift+Alt
        2,            //   Ctl+Alt
        3             // S+Ctl+Alt
     }
};

/***************************************************************************\
*
* aVkToWch2[]  - Virtual Key to WCHAR translation for 2 shift states
* aVkToWch3[]  - Virtual Key to WCHAR translation for 3 shift states
* aVkToWch4[]  - Virtual Key to WCHAR translation for 4 shift states
* aVkToWch5[]  - Virtual Key to WCHAR translation for 5 shift states
* aVkToWch6[]  - Virtual Key to WCHAR translation for 6 shift states
*
* Table attributes: Unordered Scan, null-terminated
*
* Search this table for an entry with a matching Virtual Key to find the
* corresponding unshifted and shifted WCHAR characters.
*
* Special values for VirtualKey (column 1)
*     0xff          - dead chars for the previous entry
*     0             - terminate the list
*
* Special values for Attributes (column 2)
*     CAPLOK bit    - CAPS-LOCK affect this key like SHIFT
*
* Special values for wch[*] (column 3 & 4)
*     WCH_NONE      - No character
*     WCH_DEAD      - Dead Key (diaresis) or invalid (US keyboard has none)
*
\***************************************************************************/

static VK_TO_WCHARS2 aVkToWch2[] = {
//                      |         |  Shift  |
//                      |=========|=========|
  {'1'          ,0      ,'1'      ,'!'      },
  {'4'          ,0      ,'4'      ,'$'      },
  {'5'          ,0      ,'5'      ,'%'      },
  {VK_OEM_6     ,0      ,WCH_DEAD ,WCH_DEAD },
  {0xff         ,0      ,0x00b4   ,'`'      },
  {'W'          ,CAPLOK ,'w'      ,'W'      },
  {'E'          ,CAPLOK ,'e'      ,'E'      },
  {'R'          ,CAPLOK ,'r'      ,'R'      },
  {'T'          ,CAPLOK ,'t'      ,'T'      },
  {'Z'          ,CAPLOK ,'z'      ,'Z'      },
  {'U'          ,CAPLOK ,'u'      ,'U'      },
  {'I'          ,CAPLOK ,'i'      ,'I'      },
  {'O'          ,CAPLOK ,'o'      ,'O'      },
  {'P'          ,CAPLOK ,'p'      ,'P'      },
  {'A'          ,CAPLOK ,'a'      ,'A'      },
  {'S'          ,CAPLOK ,'s'      ,'S'      },
  {'D'          ,CAPLOK ,'d'      ,'D'      },
  {'F'          ,CAPLOK ,'f'      ,'F'      },
  {'G'          ,CAPLOK ,'g'      ,'G'      },
  {'H'          ,CAPLOK ,'h'      ,'H'      },
  {'J'          ,CAPLOK ,'j'      ,'J'      },
  {'K'          ,CAPLOK ,'k'      ,'K'      },
  {'L'          ,CAPLOK ,'l'      ,'L'      },
  {VK_OEM_3     ,CAPLOK ,0x00f6   ,0x00d6   },
  {VK_OEM_7     ,CAPLOK ,0x00e4   ,0x00c4   },
  {VK_OEM_5     ,0      ,WCH_DEAD ,0x00b0   },
  {0xff         ,0      ,'^'      ,WCH_NONE },
  {'Y'          ,CAPLOK ,'y'      ,'Y'      },
  {'X'          ,CAPLOK ,'x'      ,'X'      },
  {'C'          ,CAPLOK ,'c'      ,'C'      },
  {'V'          ,CAPLOK ,'v'      ,'V'      },
  {'B'          ,CAPLOK ,'b'      ,'B'      },
  {'N'          ,CAPLOK ,'n'      ,'N'      },
  {VK_OEM_COMMA ,0      ,','      ,';'      },
  {VK_OEM_PERIOD,0      ,'.'      ,':'      },
  {VK_DECIMAL   ,0      ,','      ,0x0008   },
  {VK_TAB       ,0      ,'\t'     ,'\t'     },
  {VK_ADD       ,0      ,'+'      ,'+'      },
  {VK_DIVIDE    ,0      ,'/'      ,'/'      },
  {VK_MULTIPLY  ,0      ,'*'      ,'*'      },
  {VK_SUBTRACT  ,0      ,'-'      ,'-'      },
  {0            ,0      ,0        ,0        }
};

static VK_TO_WCHARS3 aVkToWch3[] = {
//                      |         |  Shift  |  Ctl+Alt|
//                      |=========|=========|=========|
  {0            ,0      ,0        ,0        ,0        }
};

static VK_TO_WCHARS4 aVkToWch4[] = {
//                      |         |  Shift  |  Ctl+Alt|S+Ctl+Alt|
//                      |=========|=========|=========|=========|
  {'2'          ,0      ,'2'      ,'\"'     ,0x00b2   ,0x00b2   },
  {'3'          ,0      ,'3'      ,0x00a7   ,0x00b3   ,0x00b3   },
  {'7'          ,0      ,'7'      ,'/'      ,'{'      ,'{'      },
  {'8'          ,0      ,'8'      ,'('      ,'['      ,'['      },
  {'9'          ,0      ,'9'      ,')'      ,']'      ,']'      },
  {'0'          ,0      ,'0'      ,'='      ,'}'      ,'}'      },
  {VK_OEM_4     ,0      ,0x00df   ,'?'      ,'\\'     ,'\\'     },
  {'Q'          ,CAPLOK ,'q'      ,'Q'      ,'@'      ,'@'      },
  {'M'          ,CAPLOK ,'m'      ,'M'      ,0x00b5   ,0x00b5   },
  {VK_OEM_102   ,0      ,'<'      ,'>'      ,'|'      ,'|'      },
  {0            ,0      ,0        ,0        ,0        ,0        }
};

static VK_TO_WCHARS5 aVkToWch5[] = {
//                      |         |  Shift  |  Ctl+Alt|S+Ctl+Alt|  Ctrl   |
//                      |=========|=========|=========|=========|=========|
  {VK_OEM_1     ,CAPLOK ,0x00fc   ,0x00dc   ,WCH_NONE ,WCH_NONE ,0x001b   },
  {VK_OEM_PLUS  ,0      ,'+'      ,'*'      ,'~'      ,'~'      ,0x001d   },
  {VK_OEM_2     ,0      ,'#'      ,'\''     ,WCH_NONE ,WCH_NONE ,0x001c   },
  {VK_BACK      ,0      ,'\b'     ,'\b'     ,WCH_NONE ,WCH_NONE ,0x007f   },
  {VK_CANCEL    ,0      ,0x0003   ,0x0003   ,WCH_NONE ,WCH_NONE ,0x0003   },
  {VK_ESCAPE    ,0      ,0x001b   ,0x001b   ,WCH_NONE ,WCH_NONE ,0x001b   },
  {VK_RETURN    ,0      ,'\r'     ,'\r'     ,WCH_NONE ,WCH_NONE ,'\n'     },
  {VK_SPACE     ,0      ,' '      ,' '      ,WCH_NONE ,WCH_NONE ,' '      },
  {0            ,0      ,0        ,0        ,0        ,0        ,0        }
};

static VK_TO_WCHARS6 aVkToWch6[] = {
//                      |         |  Shift  |  Ctl+Alt|S+Ctl+Alt|  Ctrl   |S+Ctrl   |
//                      |=========|=========|=========|=========|=========|=========|
  {'6'          ,0      ,'6'      ,'&'      ,WCH_NONE ,WCH_NONE ,WCH_NONE ,0x001e   },
  {VK_OEM_MINUS ,0      ,'-'      ,'_'      ,WCH_NONE ,WCH_NONE ,WCH_NONE ,0x001f   },
  {0            ,0      ,0        ,0        ,0        ,0        ,0        ,0        }
};

// Put this last so that VkKeyScan interprets number characters
// as coming from the main section of the kbd (aVkToWch2 and
// aVkToWch5) before considering the numpad (aVkToWch1).

static VK_TO_WCHARS1 aVkToWch1[] = {
    { VK_NUMPAD0   , 0      ,  '0'   },
    { VK_NUMPAD1   , 0      ,  '1'   },
    { VK_NUMPAD2   , 0      ,  '2'   },
    { VK_NUMPAD3   , 0      ,  '3'   },
    { VK_NUMPAD4   , 0      ,  '4'   },
    { VK_NUMPAD5   , 0      ,  '5'   },
    { VK_NUMPAD6   , 0      ,  '6'   },
    { VK_NUMPAD7   , 0      ,  '7'   },
    { VK_NUMPAD8   , 0      ,  '8'   },
    { VK_NUMPAD9   , 0      ,  '9'   },
    { 0            , 0      ,  '\0'  }
};

static VK_TO_WCHAR_TABLE aVkToWcharTable[] = {
    {  (PVK_TO_WCHARS1)aVkToWch3, 3, sizeof(aVkToWch3[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch4, 4, sizeof(aVkToWch4[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch5, 5, sizeof(aVkToWch5[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch6, 6, sizeof(aVkToWch6[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch2, 2, sizeof(aVkToWch2[0]) },
    {  (PVK_TO_WCHARS1)aVkToWch1, 1, sizeof(aVkToWch1[0]) },
    {                       NULL, 0, 0                    },
};

/***************************************************************************\
* aKeyNames[], aKeyNamesExt[]  - Virtual Scancode to Key Name tables
*
* Table attributes: Ordered Scan (by scancode), null-terminated
*
* Only the names of Extended, NumPad, Dead and Non-Printable keys are here.
* (Keys producing printable characters are named by that character)
\***************************************************************************/

static VSC_LPWSTR aKeyNames[] = {
    0x01,    L"ESC",
    0x0e,    L"R\x00DC" L"CK",
    0x0f,    L"TABULATOR",
    0x1c,    L"EINGABE",
    0x1d,    L"STRG",
    0x2a,    L"UMSCHALT",
    0x36,    L"UMSCHALT RECHTS",
    0x37,    L" (ZEHNERTASTATUR)",
    0x38,    L"ALT",
    0x39,    L"LEER",
    0x3a,    L"FESTSTELL",
    0x3b,    L"F1",
    0x3c,    L"F2",
    0x3d,    L"F3",
    0x3e,    L"F4",
    0x3f,    L"F5",
    0x40,    L"F6",
    0x41,    L"F7",
    0x42,    L"F8",
    0x43,    L"F9",
    0x44,    L"F10",
    0x45,    L"PAUSE",
    0x46,    L"ROLLEN-FESTSTELL",
    0x47,    L"7 (ZEHNERTASTATUR)",
    0x48,    L"8 (ZEHNERTASTATUR)",
    0x49,    L"9 (ZEHNERTASTATUR)",
    0x4a,    L"- (ZEHNERTASTATUR)",
    0x4b,    L"4 (ZEHNERTASTATUR)",
    0x4c,    L"5 (ZEHNERTASTATUR)",
    0x4d,    L"6 (ZEHNERTASTATUR)",
    0x4e,    L"+ (ZEHNERTASTATUR)",
    0x4f,    L"1 (ZEHNERTASTATUR)",
    0x50,    L"2 (ZEHNERTASTATUR)",
    0x51,    L"3 (ZEHNERTASTATUR)",
    0x52,    L"0 (ZEHNERTASTATUR)",
    0x53,    L"KOMMA (ZEHNERTASTATUR)",
    0x57,    L"F11",
    0x58,    L"F12",
    0   ,    NULL
};

static VSC_LPWSTR aKeyNamesExt[] = {
    0x1c,    L"EINGABE (ZEHNERTASTATUR)",
    0x1d,    L"STRG-RECHTS",
    0x35,    L" (ZEHNERTASTATUR)",
    0x37,    L"DRUCK",
    0x38,    L"ALT GR",
    0x45,    L"NUM-FESTSTELL",
    0x46,    L"UNTBR",
    0x47,    L"POS1",
    0x48,    L"NACH-OBEN",
    0x49,    L"BILD-NACH-OBEN",
    0x4b,    L"NACH-LINKS",
    0x4d,    L"NACH-RECHTS",
    0x4f,    L"ENDE",
    0x50,    L"NACH-UNTEN",
    0x51,    L"BILD-NACH-UNTEN",
    0x52,    L"EINFG",
    0x53,    L"ENTF",
    0x54,    L"<00>",
    0x56,    L"HILFE",
    0x5b,    L"LINKE WINDOWS",
    0x5c,    L"RECHTE WINDOWS",
    0x5d,    L"ANWENDUNG",
    0   ,    NULL
};

static LPWSTR aKeyNamesDead[] = {
    L"\x00b4"	L"AKUT",
    L"`"	L"GRAVIS",
    L"^"	L"ZIRKUMFLEX",
    NULL
};

static DEADKEY aDeadKey[] = {
    DEADTRANS( L'a'   , L'`'   , 0x00e0 ),
    DEADTRANS( L'e'   , L'`'   , 0x00e8 ),
    DEADTRANS( L'i'   , L'`'   , 0x00ec ),
    DEADTRANS( L'o'   , L'`'   , 0x00f2 ),
    DEADTRANS( L'u'   , L'`'   , 0x00f9 ),
    DEADTRANS( L'A'   , L'`'   , 0x00c0 ),
    DEADTRANS( L'E'   , L'`'   , 0x00c8 ),
    DEADTRANS( L'I'   , L'`'   , 0x00cc ),
    DEADTRANS( L'O'   , L'`'   , 0x00d2 ),
    DEADTRANS( L'U'   , L'`'   , 0x00d9 ),
    DEADTRANS( L' '   , L'`'   , L'`'   ),

    DEADTRANS( L'a'   , 0x00b4 , 0x00e1 ),
    DEADTRANS( L'e'   , 0x00b4 , 0x00e9 ),
    DEADTRANS( L'i'   , 0x00b4 , 0x00ed ),
    DEADTRANS( L'o'   , 0x00b4 , 0x00f3 ),
    DEADTRANS( L'u'   , 0x00b4 , 0x00fa ),
    DEADTRANS( L'y'   , 0x00b4 , 0x00fd ),
    DEADTRANS( L'A'   , 0x00b4 , 0x00c1 ),
    DEADTRANS( L'E'   , 0x00b4 , 0x00c9 ),
    DEADTRANS( L'I'   , 0x00b4 , 0x00cd ),
    DEADTRANS( L'O'   , 0x00b4 , 0x00d3 ),
    DEADTRANS( L'U'   , 0x00b4 , 0x00da ),
    DEADTRANS( L'Y'   , 0x00b4 , 0x00dd ),
    DEADTRANS( L' '   , 0x00b4 , 0x00b4 ),

    DEADTRANS( L'a'   , L'^'   , 0x00e2 ),
    DEADTRANS( L'e'   , L'^'   , 0x00ea ),
    DEADTRANS( L'i'   , L'^'   , 0x00ee ),
    DEADTRANS( L'o'   , L'^'   , 0x00f4 ),
    DEADTRANS( L'u'   , L'^'   , 0x00fb ),
    DEADTRANS( L'A'   , L'^'   , 0x00c2 ),
    DEADTRANS( L'E'   , L'^'   , 0x00ca ),
    DEADTRANS( L'I'   , L'^'   , 0x00ce ),
    DEADTRANS( L'O'   , L'^'   , 0x00d4 ),
    DEADTRANS( L'U'   , L'^'   , 0x00db ),
    DEADTRANS( L' '   , L'^'   , L'^'   ),

    0, 0
};

static KBDTABLES KbdTables = {
    /*
     * Modifier keys
     */
    &CharModifiers,

    /*
     * Characters tables
     */
    aVkToWcharTable,

    /*
     * Diacritics
     */
    aDeadKey,

    /*
     * Names of Keys
     */
    aKeyNames,
    aKeyNamesExt,
    aKeyNamesDead,

    /*
     * Scan codes to Virtual Keys
     */
    ausVK,
    sizeof(ausVK) / sizeof(ausVK[0]),
    aE0VscToVk,
    aE1VscToVk,

    /*
     * Locale-specific special processing
     */
    KLLF_ALTGR
};

PKBDTABLES KbdLayerDescriptor(VOID)
{
    return &KbdTables;
}
