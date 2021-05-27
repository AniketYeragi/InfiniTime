#include "FallDetection.h"
#include "../DisplayApp.h"
#include "../LittleVgl.h"

using namespace Pinetime::Applications::Screens;

LV_FONT_DECLARE(lv_font_navi_80)

static void cancel_event_handler(lv_obj_t* obj, lv_event_t event) {
  // auto stopWatch = static_cast<StopWatch*>(obj->user_data);
  // stopWatch->playPauseBtnEventHandler(event);
}

FallDetection::FallDetection(Pinetime::Applications::DisplayApp* app) : Screen(app) {
  txtNarrative = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(txtNarrative, LV_LABEL_LONG_BREAK);
  lv_obj_set_width(txtNarrative, LV_HOR_RES);
  lv_obj_set_style_local_text_color(txtNarrative, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_RED);
  lv_label_set_text(txtNarrative, "Emergency button");
  lv_label_set_align(txtNarrative, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(txtNarrative, nullptr, LV_ALIGN_CENTER, 0, 10);

  btnCancel = lv_btn_create(lv_scr_act(), nullptr);
  btnCancel->user_data = this;
  lv_obj_set_event_cb(btnCancel, cancel_event_handler);
  lv_obj_align(btnCancel, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, -10);
  lv_obj_set_height(btnCancel, 40);
  txtCancel = lv_label_create(btnCancel, nullptr);
  lv_label_set_text(txtCancel, Symbols::phoneSlash);
}

FallDetection::~FallDetection() {
  lv_obj_clean(lv_scr_act());
}

bool FallDetection::Refresh() {
  return running;
}

