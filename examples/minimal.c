// This file is part of the Clay3DS project.
//
// (c) 2024 Tommaso Dimatore
//
// For the full copyright and license information, please view the LICENSE
// file that was distributed with this source code.

#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <clay3ds.h>

static bool isRounded = false;

void onButtonInteraction(Clay_ElementId element, Clay_PointerData pointer, intptr_t)
{
  if (pointer.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME)
  {
    isRounded = !isRounded;
  }
}

Clay_RenderCommandArray renderLayout(void)
{
  Clay_BeginLayout();
  // clang-format off

  CLAY(
    CLAY_LAYOUT({
      .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER},
      .childGap = 8
    })
  ) {
    // ============================
    // Simple Button
    // ============================
  
    CLAY(
      CLAY_ID("COOL_BUTTON"),
      Clay_OnHover(onButtonInteraction, 0),
      CLAY_RECTANGLE({
        .color = Clay_Hovered()
          ? (Clay_Color){50, 69, 103, 255}
          : (Clay_Color){33, 46, 69, 255},
        .cornerRadius = CLAY_CORNER_RADIUS(isRounded ? FLT_MAX : 0.f)
      }),
      CLAY_BORDER_OUTSIDE_RADIUS(
        1,
        ((Clay_Color){152, 171, 205, 255}),
        isRounded ? FLT_MAX : 0.f
      ),
      CLAY_LAYOUT({.padding = {.x = 12, .y = 6}})
    ) {
      CLAY(
        CLAY_TEXT(
          CLAY_STRING("are you brave enough to press me?"),
          CLAY_TEXT_CONFIG({
            .textColor = (Clay_Color){152, 171, 205, 255},
            .fontSize = 16
          })
        )
      ) {}
    }

    // ============================
    // Scrollable Color Palette Demo
    // ============================

    CLAY(
      CLAY_ID("PALETTE_VIEW"),
      CLAY_SCROLL({.horizontal = true}),
      CLAY_LAYOUT({
        .sizing = {.width = CLAY_SIZING_FIT(), .height = CLAY_SIZING_GROW()},
      })
    ) {
      for (u32 i = 1; i <= 10; ++i)
      {
        CLAY(
          CLAY_RECTANGLE({.color = (Clay_Color){i * 25, 0, 0, 255}}),
          CLAY_LAYOUT({
            .sizing = {.width = CLAY_SIZING_FIXED(80), .height = CLAY_SIZING_GROW()},
          })
        ) {}
      }
    }
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

  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();

  consoleInit(GFX_TOP, NULL);
  C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

  u32 clearColor = C2D_Color32(0, 0, 0, 255);
  u64 previousTime = osGetTime();

  printf("want to learn more?\n");
  printf("github.com/sonodima/clay3ds\n");

  while (aptMainLoop())
  {
    u64 currentTime = osGetTime();
    float deltaTime = (currentTime - previousTime) / 1000.f;
    previousTime = currentTime;

    // ============================
    // Update Input
    // ============================

    Clay_UpdateScrollContainers(true, (Clay_Vector2){0.f, 0.f}, deltaTime);

    hidScanInput();
    bool isTouching = hidKeysHeld() & KEY_TOUCH;
    touchPosition touch;
    hidTouchRead(&touch);

    if (isTouching)
    {
      Clay_SetPointerState((Clay_Vector2){touch.px, touch.py}, true);
    }
    else
    {
      Clay_SetPointerState((Clay_Vector2){-1, -1}, false);
    }

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
  return 0;
}
