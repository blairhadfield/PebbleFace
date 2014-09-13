/*

   The original source image is from:

      <http://openclipart.org/detail/26728/aiga-litter-disposal-by-anonymous>

   The source image was converted from an SVG into a RGB bitmap using
   Inkscape. It has no transparency and uses only black and white as
   colors.

*/

#include "pebble.h"
#define SCREEN_HEIGHT 168
#define SCREEN_WIDTH 144
#define STATUS_BAR_HEIGHT 0
#define TIME_HEIGHT 44
#define TIME_BUFFER_LENGTH 12
	
static Window *window;

static Layer *layer;

static TextLayer *tLayer;

static GBitmap *image;
char sTime[TIME_BUFFER_LENGTH];
	
static void layer_update_callback(Layer *me, GContext* ctx) {
  // We make sure the dimensions of the GRect to draw into
  // are equal to the size of the bitmap--otherwise the image
  // will automatically tile. Which might be what *you* want.

  GRect bounds = image->bounds;

  int coordX = (SCREEN_WIDTH - bounds.size.w) / 2;
  int coordY = (SCREEN_HEIGHT - STATUS_BAR_HEIGHT - bounds.size.h) / 2 + TIME_HEIGHT / 2;
  graphics_draw_bitmap_in_rect(ctx, image, (GRect) { .origin = { coordX, coordY }, .size = bounds.size });

  //graphics_draw_bitmap_in_rect(ctx, image, (GRect) { .origin = { 80, 60 }, .size = bounds.size });
}

void outputTime()
	{
	clock_copy_time_string(sTime, TIME_BUFFER_LENGTH);
	text_layer_set_text(tLayer,sTime );
	text_layer_set_text_alignment(tLayer, GTextAlignmentCenter);
	text_layer_set_font(tLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	outputTime();
}

int main(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);

  // Init the layer for display the image
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  GRect logoBounds = GRect(0, STATUS_BAR_HEIGHT + TIME_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT - STATUS_BAR_HEIGHT - TIME_HEIGHT);
  layer = layer_create(bounds);
  layer_set_update_proc(layer, layer_update_callback);

	GRect RTextBounds = GRect(0, 5, SCREEN_WIDTH, TIME_HEIGHT);
	tLayer = text_layer_create(RTextBounds);
	
  //image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NO_LITTER);
	image = gbitmap_create_with_resource(RESOURCE_ID_IMG_BHLOGO);

	outputTime();
	layer_add_child(window_layer,  text_layer_get_layer(tLayer));
  	layer_add_child(window_layer, layer);
	
	
	tick_timer_service_subscribe(MINUTE_UNIT, (TickHandler)tick_handler);	
	
  	app_event_loop();
	

  	gbitmap_destroy(image);

  	window_destroy(window);
  	layer_destroy(layer);
}

