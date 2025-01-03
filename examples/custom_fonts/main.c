// This file is part of the Clay3DS project.
//
// (c) 2025 Tommaso Dimatore
//
// For the full copyright and license information, please view the LICENSE
// file that was distributed with this source code.

#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <clay3ds.h>

static s32 customFontId = Clay3DS_FONT_INVALID;

Clay_RenderCommandArray renderLayout(void)
{
  Clay_BeginLayout();
  // clang-format off

  CLAY(
    CLAY_LAYOUT({
      .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .childGap = 8
    })
  ) {
    CLAY(
      CLAY_ID("SYSTEM_TEXT"),
      CLAY_TEXT(
        CLAY_STRING("this is a text rendered with the system font"),
        CLAY_TEXT_CONFIG({
          .textColor = (Clay_Color){255, 255, 255, 255},
          .fontSize = 15
        })
      )
    ) {}

    CLAY(
      CLAY_ID("CUSTOM_TEXT"),
      CLAY_TEXT(
        CLAY_STRING("this is a text rendered with a custom font"),
        CLAY_TEXT_CONFIG({
          .textColor = (Clay_Color){255, 255, 255, 255},
          .fontSize = 15,
          .fontId = customFontId
        })
      )
    ) {}
  }

  // clang-format on
  return Clay_EndLayout();
}

void onClayError(Clay_ErrorData error)
{
  fprintf(stderr, "error: %s\n", error.errorText.chars);
}

int main(void)
{
  Clay_SetMaxElementCount(1024);
  u64 clayMemSize = Clay_MinMemorySize();
  Clay_Arena clayArena = Clay_CreateArenaWithCapacityAndMemory(clayMemSize, malloc(clayMemSize));
  Clay_SetMeasureTextFunction(Clay3DS_MeasureText);
  Clay_Dimensions dimensions = (Clay_Dimensions){320, 240};
  Clay_Initialize(clayArena, dimensions, (Clay_ErrorHandler){onClayError});

  romfsInit();
  cfguInit();

  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();

  consoleInit(GFX_TOP, NULL);
  C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

  // Load the custom font from romfs, and then register it in the Clay renderer.
  C2D_Font font = C2D_FontLoad("romfs:/galmuri.bcfnt");
  customFontId = Clay3DS_RegisterFont(font);
  if (customFontId == Clay3DS_FONT_INVALID)
  {
    printf("failed to register the created font :(");
  }
  else
  {
    printf("registered the custom font at id=%d", customFontId);
  }

  u32 clearColor = C2D_Color32(0, 0, 0, 255);

  while (aptMainLoop())
  {
    // ============================
    // Rendering
    // ============================

    C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
    C2D_TargetClear(bottom, clearColor);
    C2D_SceneBegin(bottom);

    Clay3DS_Render(bottom, dimensions, renderLayout());

    C3D_FrameEnd(0);
  }

  C2D_Fini();
  C3D_Fini();
  gfxExit();

  cfguExit();
  romfsExit();
  return 0;
}
