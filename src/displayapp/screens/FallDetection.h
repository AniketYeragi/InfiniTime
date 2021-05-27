#pragma once

#include <lvgl/lvgl.h>
#include <lvgl/src/lv_core/lv_obj.h>
#include <cstdint>
#include "Screen.h"
#include "Symbols.h"

namespace Pinetime {
  namespace Components {
    class LittleVgl;
  }
  namespace Applications {
    namespace Screens {

      class FallDetection : public Screen {
      public:
        FallDetection(DisplayApp* app);

        ~FallDetection() override;

        bool Refresh() override;

      private:
        lv_obj_t* imgFlag;
        lv_obj_t* txtNarrative;
      };
    }
  }
}
