use std::io::Write;
use std::thread;
use std::time::Duration;

mod arduino;
mod linux;
mod windows;

fn main() {
    let arduino_port = match arduino::find() {
        Some(port) => port,
        _none => {
            eprintln!("Arduino not found!");
            return;
        }
    };

    let mut port = serialport::new(&arduino_port, 9600)
        .timeout(Duration::from_millis(3000))
        .open()
        .expect("Failed to open port");

    port.write_data_terminal_ready(false).ok();
    println!("Connected to Arduino on {}", arduino_port);
    thread::sleep(Duration::from_millis(500));

    let mut last_volume = 0;
    let mut last_muted = false;

    loop {
        match linux::get_volume() {
            Ok((current_volume, is_muted)) => {
                // Check if either volume or mute status changed
                if current_volume != last_volume || is_muted != last_muted {
                    // Send volume as 0 if muted, otherwise actual volume
                    let volume_to_send = if is_muted { 0 } else { current_volume };

                    match port.write_all(&[volume_to_send]) {
                        Ok(_) => {
                            port.flush().ok();
                            if is_muted {
                                println!("Muted (Volume: {}%)", current_volume);
                            } else {
                                println!("Volume changed: {}%", current_volume);
                            }
                            last_volume = current_volume;
                            last_muted = is_muted;
                        }
                        Err(e) => {
                            eprintln!("Failed to send: {}", e);
                            thread::sleep(Duration::from_millis(500));
                        }
                    }
                }
            }
            Err(e) => {
                eprintln!("Failed to get volume: {}", e);
            }
        }

        thread::sleep(Duration::from_millis(100));
    }
}
