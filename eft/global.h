#pragma comment(linker, "/MERGE:.rdata=INIT")
#pragma comment(linker, "/MERGE:.pdata=INIT")
#pragma comment(linker,"/MERGE:.CRT=INIT")
#pragma warning(disable : 4244)
#pragma warning(disable : 5040)

extern "C" int _fltused = 0;
using namespace std;

#include <ntifs.h>
#include <ntddk.h>
#include <ntstrsafe.h>
#include <windef.h>
#include <intrin.h>
#include <initializer_list>
#include <stdint.h>
#include <type_traits>
#include <array>

namespace Win32 {
	PVOID ValidateHwnd;
	PVOID gpsi;
	PVOID gafAsyncKeyState;
	PVOID gafAsyncKeyStateRecentDown;
}

#include "ddk.h"
#include "crt.h"
#include "crypt.h"
#include "define.h"
#include "import.h"
#include "utils.h"
#include "usercallback.h"
#include "pageshift.h"

#include "math.h"
#include "font.h"
#include "render.h"
#include "directx.h"

#include "icons.h"
#include "gui.h"
#include "config.h"

#include "engine.h"