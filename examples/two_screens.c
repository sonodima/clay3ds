// This file is part of the Clay3DS project.
//
// (c) 2025 Tommaso Dimatore
//
// For the full copyright and license information, please view the LICENSE
// file that was distributed with this source code.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <clay3ds.h>

// clang-format off
#define DISPLAY_TRANSFER_FLAGS GX_TRANSFER_FLIP_VERT(0) \
	| GX_TRANSFER_OUT_TILED(0) \
	| GX_TRANSFER_RAW_COPY(0) \
	| GX_TRANSFER_IN_FORMAT(GX_TRANSFER_FMT_RGBA8) \
	| GX_TRANSFER_OUT_FORMAT(GX_TRANSFER_FMT_RGB8) \
	| GX_TRANSFER_SCALING(GX_TRANSFER_SCALE_NO)
// clang-format on

void onButtonInteraction(Clay_ElementId element, Clay_PointerData pointer, intptr_t)
{
  if (pointer.state == CLAY_POINTER_DATA_PRESSED_THIS_FRAME)
  {
    exit(0);
  }
}

Clay_RenderCommandArray topLayout(void)
{
  Clay_BeginLayout();
  // clang-format off

  const Clay_LayoutConfig squareLayout =
  {
    .sizing = {.width = CLAY_SIZING_FIXED(60), .height = CLAY_SIZING_FIXED(60)}
  };

  CLAY(
    CLAY_LAYOUT({
      .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER},
      .childGap = 12
    })
  ) {
    CLAY(
      CLAY_ID("GEOMETRY_ROW"),
      CLAY_LAYOUT({
        .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER},
        .childGap = 20
      })
    ) {
      CLAY(
        CLAY_LAYOUT(squareLayout),
        CLAY_RECTANGLE({.color = (Clay_Color){255, 0, 0, 255}}),
        CLAY_BORDER_OUTSIDE(4, (Clay_Color){255, 255, 0, 255})
      ) {}

      CLAY(
        CLAY_LAYOUT(squareLayout),
        CLAY_RECTANGLE({
          .color = (Clay_Color){255, 0, 255, 255},
          .cornerRadius = CLAY_CORNER_RADIUS(16)
        })
      ) {}

      CLAY(
        CLAY_LAYOUT(squareLayout),
        CLAY_BORDER_OUTSIDE_RADIUS(4, ((Clay_Color){0, 255, 255, 255}), 16)
      ) {}

      CLAY(
        CLAY_TEXT(
          CLAY_STRING("BIG"),
          CLAY_TEXT_CONFIG({
            .textColor = (Clay_Color){0, 255, 0, 255},
            .fontSize = 48
          })
        )
      ) {}
    }

    CLAY(
      CLAY_TEXT(
        CLAY_STRING("clay is not for the weak"),
        CLAY_TEXT_CONFIG({
          .textColor = (Clay_Color){255, 255, 255, 255},
          .fontSize = 20
        })
      )
    ) {}
  }

  // clang-format on
  return Clay_EndLayout();
}

Clay_RenderCommandArray bottomLayout(void)
{
  Clay_BeginLayout();
  // clang-format off

  CLAY(
    CLAY_LAYOUT({
      .sizing = {.width = CLAY_SIZING_GROW(), .height = CLAY_SIZING_GROW()},
      .layoutDirection = CLAY_TOP_TO_BOTTOM,
      .childAlignment = {.x = CLAY_ALIGN_X_CENTER, .y = CLAY_ALIGN_Y_CENTER},
      .childGap = 12
    })
  ) {
    CLAY(
      CLAY_ID("EXIT_BUTTON"),
      Clay_OnHover(onButtonInteraction, 0),
      CLAY_RECTANGLE({
        .color = Clay_Hovered()
          ? (Clay_Color){50, 69, 103, 255}
          : (Clay_Color){33, 46, 69, 255},
      }),
      CLAY_BORDER_OUTSIDE(1, (Clay_Color){152, 171, 205, 255}),
      CLAY_LAYOUT({
        .sizing = {.width = CLAY_SIZING_FIXED(160), .height = CLAY_SIZING_FIT()},
        .padding = {.x = 12, .y = 6}
      })
    ) {
      CLAY(
        CLAY_TEXT(
          CLAY_STRING("interactivity is preserved\n\nguess what happens if you press this big button"),
          CLAY_TEXT_CONFIG({
            .textColor = (Clay_Color){152, 171, 205, 255},
            .fontSize = 16
          })
        )
      ) {}
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
  Clay_Initialize(clayArena, (Clay_Dimensions){0, 0}, (Clay_ErrorHandler){onClayError});

  gfxInitDefault();
  C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
  C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
  C2D_Prepare();

  C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  C3D_RenderTarget* bottom = C3D_RenderTargetCreate(240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
  C3D_RenderTargetSetOutput(top, GFX_TOP, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);
  C3D_RenderTargetSetOutput(bottom, GFX_BOTTOM, GFX_LEFT, DISPLAY_TRANSFER_FLAGS);

  u32 clearColor = C2D_Color32(0, 0, 0, 255);
  u64 previousTime = osGetTime();

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

    // ==================
    // Top Screen
    // ==================

    C2D_TargetClear(top, clearColor);
    C2D_SceneBegin(top);

    Clay_Dimensions dimensions = (Clay_Dimensions){400, 240};
    Clay_SetLayoutDimensions(dimensions);
    Clay3DS_Render(top, dimensions, topLayout());

    // ==================
    // Bottom Screen
    // ==================

    C2D_TargetClear(bottom, clearColor);
    C2D_SceneBegin(bottom);

    dimensions = (Clay_Dimensions){320, 240};
    Clay_SetLayoutDimensions(dimensions);
    Clay3DS_Render(bottom, dimensions, bottomLayout());

    C3D_FrameEnd(0);
  }

  C2D_Fini();
  C3D_Fini();
  gfxExit();
  return 0;
}
