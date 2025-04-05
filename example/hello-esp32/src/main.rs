use esp_idf_svc::hal::gpio::PinDriver;
use esp_idf_svc::hal::prelude::Peripherals;

fn main() {
    esp_idf_svc::sys::link_patches();
    esp_idf_svc::log::EspLogger::initialize_default();

    // Configure Led
    let peripherals = Peripherals::take().unwrap();
    let mut led = PinDriver::output(peripherals.pins.gpio2).unwrap();

    loop {
        log::info!("hello, esp32!");

        // HIGH
        match led.set_high() {
            Ok(_) => {},
            Err(_) => {
                log::error!("failed to high led");
            }
        };

        // Wait
        std::thread::sleep(std::time::Duration::from_millis(1000));

        // LOW
        match led.set_low() {
            Ok(_) => {},
            Err(_) => {
                log::error!("failed to low led");
            }
        };

        // Wait
        std::thread::sleep(std::time::Duration::from_millis(1000));
    }
}
