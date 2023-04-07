// #include <memory>

#include <pico/stdlib.h>
#include <hardware/pwm.h>

#include <i2c_module.h>
#include <Pwm.h>
#include <mod_utils.h>
#include <tusb.h>

#define NUM_PWM_PINS 6

I2C_Module *i2c_module;
Pwm module_pwm(pwm_out, pwm_in, NUM_PWM_PINS);


static void module_worker_callback(i2c_inst_t *i2c, i2c_worker_event_t event) {
    if (event == I2C_WORKER_REQUEST) {
        //printf("\nPOLLED!\n");
        // printf("\nWriting buffer to hub: Bytes sent: ");
        // for(int i = 0; i < i2c_module->get_size(); i++) {
        //     printf("%d ", i2c_module->get_status()[i]);
        // }
      uint8_t request_type = i2c_module.get_request_type();

      if (request_type == 0) {
        i2c_write_raw_blocking(i2c, i2c_module->get_status(), i2c_module->get_size());
      }
      else {
	bool neighbor_found = false;
	uint8_t side = 6
	for (uint8_t ii = 0; ii < 6; ii++) {
	  if (module_pwm.isConnected(ii) &&
	      (module_pwm.read_PW(ii) & 0x00FF) == request_type) {
	    side = ii;
	    break;
	    
	  }
	}
	i2c_write_raw_blocking(i2c, &side, sizeof(side));	
      }
    }
    if (event == I2C_WORKER_RECEIVE) {
      uint8_t request_type;
      i2c_read_raw_blocking(i2c, &request_type, sizeof(request_type));
      i2c_module.set_request_type(request_type);
    }
}

int main() {
    stdio_init_all();

    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);

    // printf("waiting for usb host");
    // while (!tud_cdc_connected()) {
    //   //printf(".");
    //   sleep_ms(500);
    // }
    // printf("connected to usb\n\n");

    uint8_t addr = module::get_address();
    ModuleType type = parse_address(addr);

    //printf("Module's address: %d\n", addr);
    // switch (type) {
    // case kButton:
    //     printf("Module type: Button\n");
    //     break;
    // case kSwitch:
    //     printf("Module type: Switch\n");
    //     break;
    // case kSlider:
    //     printf("Module type: Slider\n");
    //     break;
    // case kPotentiometer:
    //     printf("Module type: Knob\n");
    //     break;
    // case kJoystick:
    //     printf("Module type: Joystick\n");
    //     break;
    // default:
    //     break;
    // }
    //printf("Expected buffer size: %d\n\n", hw_size_from_type(type));

    //printf("Initializing inputs..........");
    module::init_inputs();
    //printf("\rInitialized inputs!          ");
    
    uint pwm_in[6] = {
		   PWM_IN_SIDE_1,
		   PWM_IN_SIDE_2,
		   PWM_IN_SIDE_3,
		   PWM_IN_SIDE_4,
		   PWM_IN_SIDE_5,
		   PWM_IN_SIDE_6
    };
    uint pwm_out = PWM_OUT_PIN;

    //printf("\rInitializing PWM.............");
    //printf("\rInitialized PWM!             ");

    module_pwm.setPWMOut((uint16_t)addr);
    // sleep for some time to let neigbors initialize if necessary
    //printf("\nSleeping.....................");
    sleep_us(500000);
    //printf("\rI hath awoken from my slumber");

    uint8_t neighbor_address[6];
    // uint side = 0;
    for (int ii = 0; ii < 6; ii++) {
        if (module_pwm.isConnected(ii)) {
            //printf("\nFound connection on side %d", ii + 1);
            neighbor_address[ii] = module_pwm.read_PW(ii) & 0x00FF;
        }
        else {
            neighbor_address[ii] = 0x0000;
        }
    }
    //printf("\nNeighbor information: Side1 = 0x%X, Side2 = 0x%X, Side3 = 0x%X, Side4 = 0x%X, Side5 = 0x%X, Side6 = 0x%X", neighbor_address[0], neighbor_address[1], neighbor_address[2], neighbor_address[3], neighbor_address[4], neighbor_address[5]);
    //printf("\nGot my neighbors!            ");

    i2c_module = new I2C_Module(addr, neighbor_address,
                                PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN,
                                type, &module_worker_callback);
    //printf("\nReady to connect to the hub! ");
    i2c_module->setup();
    //printf("\rConnected to the hub!        ");

    uint8_t* buf = new uint8_t[hw_size_from_type(type)];
    uint8_t count;

    //printf("\nStarting input loop\n");

    while(1) {
        //printf("Retrieving input\r");
        count = module::get_input(buf);
        //printf("Verifying input \r");
        assert(count == hw_size_from_type(type));
        //printf("Updating I2C....\r");
        i2c_module->update_input(buf);
        //printf("Updated I2C!    \r");
        tight_loop_contents();
    }

    delete buf;
}
