#ifndef TDISPLAY_S3
#define TDISPLAY_S3

#include "TFT_eSPI.h"
#include "esphome.h"

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"

namespace esphome {
namespace tdisplays3 {

static const char *const TAG = "TDisplayS3";

class TDisplayS3 : public PollingComponent,
                   public display::DisplayBuffer
{
  public:
    void setup() override {
        tft.init();
    // Only execute this if we're exposing the TFT and SPR objects.  This is because the rotation between
    // the rotation between ESPHome and TFT_eSPI doesn't match.  There may be a better way to handle this        
#ifdef TDISPLAYS3_EXPOSE_TFT
        switch (this->rotation_) {
        case esphome::display::DISPLAY_ROTATION_0_DEGREES: 
            //this->rotation_ = esphome::display::DISPLAY_ROTATION_90_DEGREES;
            tft.setRotation(0);
            break;
        case esphome::display::DISPLAY_ROTATION_90_DEGREES: 
            this->rotation_ = esphome::display::DISPLAY_ROTATION_0_DEGREES;
            tft.setRotation(1);
            break;            
        case esphome::display::DISPLAY_ROTATION_180_DEGREES: 
            this->rotation_ = esphome::display::DISPLAY_ROTATION_0_DEGREES;
            tft.setRotation(2);
            break;            
        case esphome::display::DISPLAY_ROTATION_270_DEGREES: 
            this->rotation_ = esphome::display::DISPLAY_ROTATION_0_DEGREES;
            tft.setRotation(3);
            break;            
        //default:
            //id(global_display_rotation) = DISPLAY_ROTATION_0_DEGREES;
        }          
#endif   
#ifdef TDISPLAYS3_EXPOSE_TFT
        spr.createSprite(get_height_internal(), get_width_internal());
        tft.fillScreen(TFT_BLACK);
#else        
        spr.createSprite(get_width_internal(), get_height_internal());
        tft.fillScreen(TFT_BLACK);
#endif
    }

    void loop() override {
    }

    //////////
    // DisplayBuffer methods
    //////////
    void fill(Color color) override {
        spr.fillScreen(display::ColorUtil::color_to_565(color));
    }

    int get_width_internal() override {
        return tft.getViewportWidth();
    }

    int get_height_internal() override {
	return tft.getViewportHeight();
    }

    display::DisplayType get_display_type() override {
        return display::DisplayType::DISPLAY_TYPE_COLOR;
    }

    void draw_absolute_pixel_internal(int x, int y, Color color) override {
        spr.drawPixel(x, y, display::ColorUtil::color_to_565(color));
    }

    /////////////
    // PollingComponent Methods
    /////////////
    void update() override {
        this->do_update_();
        spr.pushSprite(0, 0);
    }
    // Unless the user specified expose_tft_objects: true in their config then the tft and spr objects will be private
#ifndef TDISPLAYS3_EXPOSE_TFT
    private:
#endif
    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite spr = TFT_eSprite(&tft);
};

}  // namespace tdisplays3
}  // namespace esphome

#endif
