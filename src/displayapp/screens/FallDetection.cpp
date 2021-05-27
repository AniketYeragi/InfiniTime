#include "FallDetection.h"
#include "../DisplayApp.h"
#include "../LittleVgl.h"

using namespace Pinetime::Applications::Screens;

LV_FONT_DECLARE(lv_font_navi_80)

FallDetection::FallDetection(Pinetime::Applications::DisplayApp* app, Pinetime::Components::LittleVgl& lvgl) : Screen(app), lvgl {lvgl} {
  imgFlag = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(imgFlag, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &lv_font_navi_80);
  lv_obj_set_style_local_text_color(imgFlag, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_label_set_text(imgFlag, "Emergency activation!");
  lv_obj_align(imgFlag, nullptr, LV_ALIGN_CENTER, 0, -60);
}

FallDetection::~FallDetection() {
  lv_obj_clean(lv_scr_act());
}

bool FallDetection::Refresh() {
  return running;
}
