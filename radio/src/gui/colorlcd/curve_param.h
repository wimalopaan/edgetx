/*
 * Copyright (C) EdgeTX
 *
 * Based on code named
 *   opentx - https://github.com/opentx/opentx
 *   th9x - http://code.google.com/p/th9x
 *   er9x - http://code.google.com/p/er9x
 *   gruvin9x - http://code.google.com/p/gruvin9x
 *
 * License GPLv2: http://www.gnu.org/licenses/gpl-2.0.html
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

#include "window.h"

struct CurveRef;

class CurveParam : public Window
{
  // Curve
  CurveRef* ref;

  // Controls
  Window* value_edit;
  Window* func_choice;
  Window* cust_choice;
  Window* act_field = nullptr;

  void update();
  static void value_changed(lv_event_t* e);
  static void LongPressHandler(void* data);

 public:
  CurveParam(Window* parent, const rect_t& rect, CurveRef* ref,
             std::function<void(int32_t)> setRefValue,
             std::function<void(void)> refreshView = nullptr);
};
