use esp_idf_svc::hal::prelude::Peripherals;

fn main() {
    esp_idf_svc::sys::link_patches();

    println!("Configuring output channel");

    let peripherals = Peripherals::take()?;

    loop {}
}
