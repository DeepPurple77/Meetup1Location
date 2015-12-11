#include <pebble.h>
#include "Meetup.h"

static Window *window;
static TextLayer *s_locationLayer;

enum KEYS{
	READY,
	GET_LOCATION,
	COORDS
};

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
	
  s_locationLayer = text_layer_create(GRect(0,65,144,100));
  layer_add_child(window_layer, text_layer_get_layer(s_locationLayer));
  text_layer_set_text(s_locationLayer, "Obtaining...");
  text_layer_set_text_alignment(s_locationLayer, GTextAlignmentCenter);
  text_layer_set_font(s_locationLayer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_color(s_locationLayer, GColorBlue);	
}

static void window_unload(Window *window){
  text_layer_destroy(s_locationLayer);
}

static void get_location(){
	Tuplet location_tuplet = TupletInteger(GET_LOCATION,1);
	
	DictionaryIterator *iter;
	app_message_outbox_begin(&iter);
	
	if (iter == NULL) return;
	
	dict_write_tuplet(iter, &location_tuplet);
	dict_write_end(iter);
	
	app_message_outbox_send();
}

void in_received_handler(DictionaryIterator *received, void *context){
	Tuple *ready_tuple = dict_find(received,READY);
	Tuple *location_tuple = dict_find(received,COORDS);
	
	if (ready_tuple) get_location();
	
	if (location_tuple) text_layer_set_text(s_locationLayer,location_tuple->value->cstring);
}

static void out_sent_handler(DictionaryIterator *received, void *context){
	
}

void handle_init(void) {
	printf("Handle init");
	window = window_create();
	
	window_set_window_handlers(window, (WindowHandlers) {
    	.load = window_load,
    	.unload = window_unload
  	});
  
	window_stack_push(window, true /* Animated */);
	
	app_message_register_inbox_received(in_received_handler);
	app_message_register_outbox_sent(out_sent_handler);
	const uint32_t inbound_size=256;
	const uint32_t outbound_size=256;
	app_message_open(inbound_size,outbound_size);
	
}

void handle_deinit(void){
	printf("Handle deinit");
	app_message_deregister_callbacks();
	window_destroy(window);
}

int main() {
	handle_init();
	app_event_loop();
	handle_deinit();
}