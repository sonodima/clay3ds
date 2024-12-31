// This file is part of the Clay3DS project.
//
// (c) 2024 Tommaso Dimatore
//
// For the full copyright and license information, please view the LICENSE
// file that was distributed with this source code.

#ifndef __CC2D_RENDERER_H
#define __CC2D_RENDERER_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>

#include "clay.h"

#define CC2Di__CLAY_COLOR_TO_C2D(cc) C2D_Color32((u8)cc.r, (u8)cc.g, (u8)cc.b, (u8)cc.a)
#define CC2Di__DEG_TO_RAD(value) ((value) * (M_PI / 180.f))
#define CC2Di__GET_TEXT_SCALE(fs) ((float)(fs) / 30.f)

static void CC2Di__DrawArc(float cx, float cy, float radius, float angs, float ange, float thickness, u32 color)
{
  u32 segments = 8;
  float step = (ange - angs) / segments;

  float angle = CC2Di__DEG_TO_RAD(angs);
  float x1 = cx + radius * cos(angle);
  float y1 = cy + radius * sin(angle);

  for (u32 i = 0; i <= segments; ++i)
  {
    angle = CC2Di__DEG_TO_RAD(angs + i * step);
    float x2 = cx + radius * cos(angle);
    float y2 = cy + radius * sin(angle);

    C2D_DrawLine(x1, y1, color, x2, y2, color, thickness, 0.f);
    x1 = x2;
    y1 = y2;
  }
}

static void CC2Di__FillArc(float cx, float cy, float radius, float angs, float ange, u32 color)
{
  u32 segments = 8;
  float step = (ange - angs) / segments;

  float angle = CC2Di__DEG_TO_RAD(angs);
  float x1 = cx + radius * cos(angle);
  float y1 = cy + radius * sin(angle);

  for (u32 i = 0; i <= segments; ++i)
  {
    angle = CC2Di__DEG_TO_RAD(angs + i * step);
    float x2 = cx + radius * cos(angle);
    float y2 = cy + radius * sin(angle);

    C2D_DrawTriangle(cx, cy, color, x1, y1, color, x2, y2, color, 0.f);
    x1 = x2;
    y1 = y2;
  }
}

static Clay_Dimensions CC2D_MeasureText(Clay_String* string, Clay_TextElementConfig* config)
{
  float scale = CC2Di__GET_TEXT_SCALE(config->fontSize);

  // TODO: re-use the same buffer for performance.
  char* cloned = (char*)malloc(string->length + 1);
  memcpy(cloned, string->chars, string->length);
  cloned[string->length] = '\0';

  C2D_Text text;
  C2D_TextBuf buffer = C2D_TextBufNew(string->length + 1);
  C2D_TextParse(&text, buffer, cloned);
  C2D_TextOptimize(&text);

  Clay_Dimensions dimensions;
  C2D_TextGetDimensions(&text, scale, scale, &dimensions.width, &dimensions.height);

  C2D_TextBufDelete(buffer);
  free(cloned);
  return dimensions;
}

static void CC2D_Render(C3D_RenderTarget* renderTarget, Clay_Dimensions dimensions, Clay_RenderCommandArray renderCommands)
{
  for (u32 i = 0; i < renderCommands.length; i++)
  {
    Clay_RenderCommand* renderCommand = Clay_RenderCommandArray_Get(&renderCommands, i);
    Clay_BoundingBox box = renderCommand->boundingBox;

    switch (renderCommand->commandType)
    {
    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
      Clay_RectangleElementConfig* config = renderCommand->config.rectangleElementConfig;
      u32 color = CC2Di__CLAY_COLOR_TO_C2D(config->color);
      float tlr = config->cornerRadius.topLeft;
      float trr = config->cornerRadius.topRight;
      float brr = config->cornerRadius.bottomRight;
      float blr = config->cornerRadius.bottomLeft;

      if (tlr <= 0.f && trr <= 0.f && brr <= 0.f && blr <= 0.f)
      {
        // If no rounding is used, fall back to the faster, simpler, rectangle drawing.
        C2D_DrawRectSolid(box.x, box.y, 0.f, box.width, box.height, color);
      }
      else
      {
        // Make sure that the rounding is not bigger than half of any side.
        float max = fminf(box.width, box.height) / 2.f;
        tlr = fminf(tlr, max);
        trr = fminf(trr, max);
        brr = fminf(brr, max);
        blr = fminf(blr, max);

        // Holy Moly!
        // This is cryptic as fuck but it works.
        float lx = box.x;
        float ly1 = box.y + tlr;
        float ly2 = box.y + box.height - blr;
        float ty = box.y;
        float tx1 = box.x + tlr;
        float tx2 = box.x + box.width - trr;
        float rx = box.x + box.width;
        float ry1 = box.y + trr;
        float ry2 = box.y + box.height - brr;
        float by = box.y + box.height;
        float bx1 = box.x + blr;
        float bx2 = box.x + box.width - brr;

        C2D_DrawTriangle(lx, ly1, color, box.x + tlr, ly1, color, lx, ly2, color, 0.f);
        C2D_DrawTriangle(box.x + blr, ly2, color, box.x + tlr, ly1, color, lx, ly2, color, 0.f);
        C2D_DrawTriangle(tx1, ty, color, tx1, box.y + tlr, color, tx2, ty, color, 0.f);
        C2D_DrawTriangle(tx2, box.y + trr, color, tx1, box.y + tlr, color, tx2, ty, color, 0.f);
        C2D_DrawTriangle(rx, ry1, color, box.x + box.width - trr, ry1, color, rx, ry2, color, 0.f);
        C2D_DrawTriangle(box.x + box.width - brr, ry2, color, box.x + box.width - trr, ry1, color, rx, ry2, color, 0.f);
        C2D_DrawTriangle(bx1, by, color, bx1, box.y + box.height - blr, color, bx2, by, color, 0.f);
        C2D_DrawTriangle(bx2, box.y + box.height - brr, color, bx1, box.y + box.height - blr, color, bx2, by, color, 0.f);

        C2D_DrawTriangle(tx2, ry1, color, tx1, ly1, color, bx2, ry2, color, 0.f);
        C2D_DrawTriangle(bx1, ly2, color, tx1, ly1, color, bx2, ry2, color, 0.f);

        CC2Di__FillArc(box.x + tlr, box.y + tlr, tlr, 180.f, 270.f, color);
        CC2Di__FillArc(box.x + box.width - trr, box.y + trr, trr, 270.f, 360.f, color);
        CC2Di__FillArc(box.x + blr, box.y + box.height - blr, blr, 90.f, 180.f, color);
        CC2Di__FillArc(box.x + box.width - brr, box.y + box.height - brr, brr, 0.f, 90.f, color);
      }

      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_BORDER: {
      Clay_BorderElementConfig* config = renderCommand->config.borderElementConfig;
      u32 tc = CC2Di__CLAY_COLOR_TO_C2D(config->top.color);
      u32 lc = CC2Di__CLAY_COLOR_TO_C2D(config->left.color);
      u32 rc = CC2Di__CLAY_COLOR_TO_C2D(config->right.color);
      u32 bc = CC2Di__CLAY_COLOR_TO_C2D(config->bottom.color);
      float lw = config->left.width;
      float tw = config->top.width;
      float rw = config->right.width;
      float bw = config->bottom.width;
      // Make sure that the rounding is not bigger than half of any side.
      float max = fminf(box.width, box.height) / 2.f;
      float tlr = fminf(config->cornerRadius.topLeft, max);
      float trr = fminf(config->cornerRadius.topRight, max);
      float brr = fminf(config->cornerRadius.bottomRight, max);
      float blr = fminf(config->cornerRadius.bottomLeft, max);

      if (config->top.width > 0.f)
      {
        C2D_DrawRectSolid(box.x + tlr, box.y, 0.f, box.width - tlr - trr, tw, tc);
      }
      if (config->left.width > 0.f)
      {
        C2D_DrawRectSolid(box.x, box.y + tlr, 0.f, lw, box.height - tlr - blr, lc);
      }
      if (config->right.width > 0.f)
      {
        C2D_DrawRectSolid(box.x + box.width - rw, box.y + trr, 0.f, rw, box.height - trr - brr, rc);
      }
      if (config->bottom.width > 0.f)
      {
        C2D_DrawRectSolid(box.x + brr, box.y + box.height - bw, 0.f, box.width - blr - brr, bw, bc);
      }
      if (tlr > 0.f)
      {
        CC2Di__DrawArc(box.x + tlr, box.y + tlr, tlr - tw / 2.f, 180.f, 270.f, tw, tc);
      }
      if (trr > 0.f)
      {
        CC2Di__DrawArc(box.x + box.width - trr, box.y + trr, trr - tw / 2.f, 270.f, 360.f, tw, tc);
      }
      if (blr > 0.f)
      {
        CC2Di__DrawArc(box.x + blr, box.y + box.height - blr, blr - bw / 2.f, 90.f, 180.f, bw, bc);
      }
      if (brr > 0.f)
      {
        CC2Di__DrawArc(box.x + box.width - brr, box.y + box.height - brr, brr - bw / 2.f, 0.f, 90.f, bw, bc);
      }

      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_TEXT: {
      Clay_TextElementConfig* config = renderCommand->config.textElementConfig;
      float scale = CC2Di__GET_TEXT_SCALE(config->fontSize);
      u32 color = CC2Di__CLAY_COLOR_TO_C2D(config->textColor);

      Clay_String string = renderCommand->text;
      // TODO: re-use the same buffer for performance.
      char* cloned = (char*)malloc(string.length + 1);
      memcpy(cloned, string.chars, string.length);
      cloned[string.length] = '\0';

      C2D_Text text;
      // TODO: re-use the same buffer for performance.
      C2D_TextBuf buffer = C2D_TextBufNew(string.length + 1);
      C2D_TextParse(&text, buffer, cloned);
      C2D_TextOptimize(&text);
      C2D_DrawText(&text, C2D_WithColor, box.x, box.y, 0.f, scale, scale, color);

      C2D_TextBufDelete(buffer);
      free(cloned);
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
      C2D_SceneBegin(renderTarget);
      C3D_SetScissor(GPU_SCISSOR_NORMAL, dimensions.height - box.height - box.y, dimensions.width - box.width - box.x, box.height + box.y,
                     dimensions.width - box.x);
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
      C3D_SetScissor(GPU_SCISSOR_DISABLE, 0, 0, 0, 0);
      break;
    }
    default: {
      fprintf(stderr, "error: unhandled render command: %d\n", renderCommand->commandType);
      exit(1);
    }
    }
  }
}

static void CC2D_UpdateInput(float deltaTime)
{
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
}

#endif // __CC2D_RENDERER_H
